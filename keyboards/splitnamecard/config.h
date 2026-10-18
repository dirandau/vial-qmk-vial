/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

/* 매트릭스 크기와 다이렉트 핀은 keyboard.json(matrix_pins.direct)에서 생성됨 */

/* 스위치가 눌렸을 때 GND(0)로 떨어지는 풀업 모드 강제 */
#undef MATRIX_INPUT_PRESSED_STATE
#define MATRIX_INPUT_PRESSED_STATE 0

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
/* SPI 클럭: RP2040에서는 PMW33XX_CLOCK_SPEED가 무시되고 분주비만 적용됨.
 * 드라이버 기본값 64는 125MHz/64 ≈ 1.95MHz로 PMW3360 한계(2MHz)에 붙어 있어
 * delta 값이 깨지는 현상이 있었음. 128(약 977kHz)에서도 최상위 비트가 깨져서
 * 최대 분주비 254(약 490kHz)로 낮춤 */
#ifndef PMW33XX_SPI_DIVISOR
#    define PMW33XX_SPI_DIVISOR 254
#endif

/* 리프트오프 거리(Lift-off Distance) 최소화하여 센서 튐 현상 방지 */
#ifndef PMW33XX_LIFTOFF_DISTANCE
#    define PMW33XX_LIFTOFF_DISTANCE 0x00
#endif
