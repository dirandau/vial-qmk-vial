/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

/* RP2040 SPI0 주변장치 지정 */
#define SPI_DRIVER SPID0

/* SPI & PMW3360 핀 매핑 (GP0 ~ GP3) */
#define SPI_SCK_PIN GP2   // Serial Clock
#define SPI_MOSI_PIN GP3  // Master Out Slave In
#define SPI_MISO_PIN GP0  // Master In Slave Out
#define PMW33XX_CS_PIN GP1 // Chip Select (NCS)

/* 트랙볼 옵션 설정 */
#define PMW33XX_CPI 400   // 마우스 감도
#define MOUSE_EXTENDED_REPORT

#define POINTING_DEVICE_INVERT_Y
#define PMW33XX_CLOCK_SPEED 1000000
