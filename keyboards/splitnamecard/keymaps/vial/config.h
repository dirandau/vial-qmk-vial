/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

/* VIAL 설정 */
#define VIAL_KEYBOARD_UID {0x04, 0x1F, 0x18, 0x4F, 0x36, 0x7D, 0x41, 0xB6}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 1 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 1 }

/* 트랙볼 옵션 설정 */
#undef PMW33XX_CPI
#define PMW33XX_CPI 1200  // 마우스 감도 (100~12000 설정 가능)

/* 트랙볼 노이즈 필터 (keymap.c의 pointing_device_task_user 참고)
 * - TB_SPIKE_DELTA : 물리적으로 불가능한 값(SPI 데이터 깨짐). 넘으면 리포트 전체를 버림
 * - TB_MAX_DELTA   : 1회 리포트당 허용 최대 이동량. 넘으면 이 값으로 클램프
 * - TB_JUMP_DELTA  : 정지 상태에서 갑자기 이만큼 이상 튀면 그 리포트 하나만 노이즈로 보고 버림
 * - TB_IDLE_MS     : 마지막 움직임 후 이 시간이 지나면 정지 상태로 간주
 * 모두 CPI에 비례하므로 CPI만 바꿔도 체감 동작이 유지됨 */
#define TB_SPIKE_DELTA (PMW33XX_CPI / 4)   // 1200 CPI 기준 300. 이보다 크면 SPI 오류 값으로 보고 리포트 전체를 버림
#define TB_MAX_DELTA  (PMW33XX_CPI / 8)    // 1200 CPI 기준 150. 노이즈는 복원 필터가 처리하므로 빠른 회전을 자르지 않게 넉넉히
#define TB_JUMP_DELTA (PMW33XX_CPI / 32)   // 1200 CPI 기준 37
#define TB_IDLE_MS    30

/* 원시 x/y 값과 필터 동작을 콘솔로 출력 (qmk console 로 확인). 끄려면 주석 처리 */
// #define TB_DEBUG
