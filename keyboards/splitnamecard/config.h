/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

/* RP2040 SPI0 주변장치 지정 */
#ifndef SPI_DRIVER
#    define SPI_DRIVER SPID0
#endif

/* SPI & PMW3360 핀 매핑 (GP0 ~ GP3) */
#ifndef SPI_SCK_PIN
#    define SPI_SCK_PIN GP2   // Serial Clock
#endif
#ifndef SPI_MOSI_PIN
#    define SPI_MOSI_PIN GP3  // Master Out Slave In
#endif
#ifndef SPI_MISO_PIN
#    define SPI_MISO_PIN GP0  // Master In Slave Out
#endif
#ifndef PMW33XX_CS_PIN
#    define PMW33XX_CS_PIN GP1 // Chip Select (NCS)
#endif

/* 트랙볼 옵션 설정 */
#ifndef PMW33XX_CPI
#    define PMW33XX_CPI 400   // 마우스 감도 기본값
#endif
#ifndef MOUSE_EXTENDED_REPORT
#    define MOUSE_EXTENDED_REPORT
#endif

#ifndef POINTING_DEVICE_INVERT_Y
#    define POINTING_DEVICE_INVERT_Y
#endif
#ifndef PMW33XX_CLOCK_SPEED
#    define PMW33XX_CLOCK_SPEED 2000000
#endif

/* 리프트오프 거리(Lift-off Distance) 최소화하여 센서 튐 현상 방지 */
#ifndef PMW33XX_LIFTOFF_DISTANCE
#    define PMW33XX_LIFTOFF_DISTANCE 0x00
#endif
