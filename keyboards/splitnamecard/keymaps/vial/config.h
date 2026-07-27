/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

/* VIAL 설정 */
#define VIAL_KEYBOARD_UID {0x0F, 0xB0, 0xD3, 0xDF, 0x7B, 0x86, 0xA6, 0x77}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 1 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 1 }

/* 트랙볼 옵션 설정 */
#undef PMW33XX_CPI
#define PMW33XX_CPI 200   // 마우스 감도 (100~12000 설정 가능)
