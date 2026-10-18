/* SPDX-License-Identifier: GPL-2.0-or-later */
#include <stdlib.h>
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_1, KC_2, KC_3, 
        KC_4, KC_5, KC_6,
        KC_7, KC_8, KC_9
    ),

    [1] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, 
        KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS
    )
};

// 트랙볼 노이즈 필터
//
// 관찰된 SPI 오류: 센서 바이트의 최상위 비트(1)가 0으로 읽힘. 직전 비트가 0일 때 주로 발생
// (MISO 상승이 느림). 그래서 작은 음수만 깨짐: -1(0xFFFF) -> -129(0xFF7F), -2(0xFFFE) -> 32638(0x7F7E).
// 양수 작은 값은 최상위 비트가 원래 0이라 영향이 없어서, 위로만 빠르고 튀는 비대칭이 생겼음.
//
//  0) 복원: 상위 바이트가 0xFF 또는 0x7F면 음수로 보고, 깨졌을 수 있는 비트를 채운 후보 중
//     직전 값에 가장 가까운 값을 고름 (-129 -> -1). 여러 비트가 동시에 깨져도 복원됨.
//     센서 좌표계에서 해야 하므로 드라이버/INVERT 설정의 부호를 되돌린 뒤 적용
//  1) TB_SPIKE_DELTA 를 넘는 값은 리포트 전체를 버림
//  2) 정지 상태(TB_IDLE_MS)에서 갑자기 TB_JUMP_DELTA 이상 튀면 그 리포트 하나만 버림
//  3) 통과한 값은 TB_MAX_DELTA 로 클램프
//  TB_DEBUG 가 켜져 있으면 원시 값과 필터 판정을 콘솔로 출력

// 리포트 값 = 부호 x 센서 값. 드라이버가 x, y 모두 -1을 곱하고 POINTING_DEVICE_INVERT_Y 가 y에 -1을 다시 곱함
#define TB_SENSOR_SIGN_X (-1)
#define TB_SENSOR_SIGN_Y (1)

// 오류는 1 -> 0 방향으로만 생기므로, 실제 값은 "읽힌 값의 1비트를 모두 포함하는 값" 중 하나.
// 음수(상위 바이트 0xFF/0x7F)면 최상위 비트만 채운 값을 먼저 쓰고, 그 값이 직전 값과 동떨어지면
// 하위 바이트에서 0인 비트를 채운 후보들 중 직전 값에 가장 가까운 것을 고름.
// 예: 0xFF0F(-241)는 -113, -97, ... -17, -1 이 후보이고 직전 값이 -1 근처면 -1로 복원
static int16_t tb_repair_sensor(int16_t v, int16_t prev) {
    const uint16_t u  = (uint16_t)v;
    const uint8_t  hi = u >> 8;
    if (hi != 0xFF && hi != 0x7F) return v;
    // 직전 값과 자연스럽게 이어지면 깨지지 않은 값. 빠르게 돌릴 때 나오는 -150 같은 실제 큰 음수를 보호
    if (abs(v - prev) < TB_JUMP_DELTA) return v;

    const uint8_t base = (u & 0xFF) | 0x80;
    const uint8_t free = (uint8_t)~base & 0x7F;
    int16_t       best = (int16_t)(0xFF00 | base);
    // 가장 흔한 경우는 최상위 비트 하나만 깨진 것. 그 값이 직전 값과 자연스럽게 이어지면 그대로 사용
    if (abs(best - prev) < TB_JUMP_DELTA) return best;
    // free 비트의 모든 부분집합을 순회 (최대 128개)
    uint8_t sub = free;
    while (true) {
        const int16_t cand = (int16_t)(0xFF00 | base | sub);
        const int16_t d    = abs(cand - prev);
        const int16_t db   = abs(best - prev);
        if (d < db || (d == db && abs(cand) < abs(best))) best = cand;
        if (sub == 0) break;
        sub = (sub - 1) & free;
    }
    return best;
}

// sign: 리포트 값 = sign x 센서 값
static inline int16_t tb_repair(int16_t v, int8_t sign, int16_t prev_report) {
    return sign * tb_repair_sensor(sign * v, sign * prev_report);
}

static inline int16_t tb_clamp(int16_t v) {
    if (v > TB_MAX_DELTA) return TB_MAX_DELTA;
    if (v < -TB_MAX_DELTA) return -TB_MAX_DELTA;
    return v;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    static uint32_t last_motion = 0;
    static int16_t  prev_x = 0, prev_y = 0; // 직전에 통과한 값. 정지 상태가 되면 0

    const int16_t raw_x = mouse_report.x;
    const int16_t raw_y = mouse_report.y;
    if (raw_x == 0 && raw_y == 0) return mouse_report;

    const uint32_t now = timer_read32();
    if (TIMER_DIFF_32(now, last_motion) > TB_IDLE_MS) {
        prev_x = 0;
        prev_y = 0;
    }

    // 0) 깨진 비트 복원
    const int16_t x = tb_repair(raw_x, TB_SENSOR_SIGN_X, prev_x);
    const int16_t y = tb_repair(raw_y, TB_SENSOR_SIGN_Y, prev_y);
#ifdef TB_DEBUG
    if (x != raw_x || y != raw_y) uprintf("TB FIX raw=(%d,%d) -> (%d,%d)\n", raw_x, raw_y, x, y);
#endif
    mouse_report.x = 0;
    mouse_report.y = 0;

    // 1) 물리적으로 불가능한 값
    if (abs(x) > TB_SPIKE_DELTA || abs(y) > TB_SPIKE_DELTA) {
#ifdef TB_DEBUG
        uprintf("TB SPIKE (%d,%d)\n", x, y);
#endif
        return mouse_report;
    }

    // 2) 정지 상태에서의 급점프는 첫 리포트 하나만 버림.
    //    last_motion 을 갱신해서 다음 리포트부터는 통과시킴. 갱신하지 않으면 빠르게 돌리기
    //    시작했을 때 큰 값이 계속 "정지 중 급점프"로 판정되어 휙 돌린 움직임이 통째로 무시됨
    const int16_t  mag = (abs(x) > abs(y)) ? abs(x) : abs(y);
    if (TIMER_DIFF_32(now, last_motion) > TB_IDLE_MS && mag >= TB_JUMP_DELTA) {
#ifdef TB_DEBUG
        uprintf("TB DROP (%d,%d) idle\n", x, y);
#endif
        last_motion = now;
        return mouse_report;
    }

    // 3) 클램프
    last_motion    = now;
    prev_x         = x;
    prev_y         = y;
    mouse_report.x = tb_clamp(x);
    mouse_report.y = tb_clamp(y);

#ifdef TB_DEBUG
    if (mouse_report.x != x || mouse_report.y != y) {
        uprintf("TB CLAMP (%d,%d) -> (%d,%d)\n", x, y, mouse_report.x, mouse_report.y);
    } else if (x == raw_x && y == raw_y) {
        uprintf("TB raw=(%d,%d)\n", raw_x, raw_y);
    }
#endif
    return mouse_report;
}
