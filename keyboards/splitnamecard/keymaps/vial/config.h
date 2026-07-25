/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#define VIAL_KEYBOARD_UID {0x0F, 0xB0, 0xD3, 0xDF, 0x7B, 0x86, 0xA6, 0x00}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 1 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 1 }

/* SPI & PMW3360 핀 매핑 (GP0 ~ GP3) */
#define SPI_SCK_PIN GP2   // Serial Clock
#define SPI_MOSI_PIN GP3  // Master Out Slave In
#define SPI_MISO_PIN GP0  // Master In Slave Out
#define PMW33XX_CS_PIN GP1 // Chip Select (NCS)

/* 트랙볼 옵션 설정 */
#define PMW33XX_CPI 1600   // 마우스 감도 (기본 1600, 100~12000 범위 설정 가능)
#define MOUSE_EXTENDED_REPORT // 부드러운 트래킹을 위한 확장 리포트 사용

// 센서 장착 방향에 따라 x, y 축 반전이나 회전이 필요할 경우 아래 옵션을 켭니다.
// #define POINTING_DEVICE_INVERT_X
// #define POINTING_DEVICE_INVERT_Y
// #define ROTATIONAL_TRANSFORM_ANGLE 90 // 90, 180, 270 등 회전 각도 조정