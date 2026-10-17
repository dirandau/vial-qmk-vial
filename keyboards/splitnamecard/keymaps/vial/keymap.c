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
//  1) 정지 상태(TB_IDLE_MS 이상 움직임 없음)에서 갑자기 TB_JUMP_DELTA 이상 튀는 리포트는 버림
//  2) 연속 움직임 중에는 통과시키되 TB_MAX_DELTA 로 클램프 (물리적으로 불가능한 점프만 차단)
//  TB_DEBUG 가 켜져 있으면 원시 값과 필터 판정을 콘솔로 출력
static inline int16_t tb_clamp(int16_t v) {
    if (v > TB_MAX_DELTA) return TB_MAX_DELTA;
    if (v < -TB_MAX_DELTA) return -TB_MAX_DELTA;
    return v;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    static uint32_t last_motion = 0;

    const int16_t raw_x = mouse_report.x;
    const int16_t raw_y = mouse_report.y;
    if (raw_x == 0 && raw_y == 0) return mouse_report;

    const uint32_t now      = timer_read32();
    const bool     was_idle = TIMER_DIFF_32(now, last_motion) > TB_IDLE_MS;
    const int16_t  mag      = (abs(raw_x) > abs(raw_y)) ? abs(raw_x) : abs(raw_y);

    if (was_idle && mag >= TB_JUMP_DELTA) {
        // 정지 상태에서의 급점프: 노이즈로 판단하고 버림. last_motion 은 갱신하지 않아
        // 다음 리포트도 같은 기준으로 검사됨
#ifdef TB_DEBUG
        uprintf("TB DROP raw=(%d,%d) idle=%lums\n", raw_x, raw_y, (unsigned long)TIMER_DIFF_32(now, last_motion));
#endif
        mouse_report.x = 0;
        mouse_report.y = 0;
        return mouse_report;
    }

    last_motion    = now;
    mouse_report.x = tb_clamp(raw_x);
    mouse_report.y = tb_clamp(raw_y);

#ifdef TB_DEBUG
    if (mouse_report.x != raw_x || mouse_report.y != raw_y) {
        uprintf("TB CLAMP raw=(%d,%d) -> (%d,%d)\n", raw_x, raw_y, mouse_report.x, mouse_report.y);
    } else {
        uprintf("TB raw=(%d,%d)\n", raw_x, raw_y);
    }
#endif
    return mouse_report;
}
