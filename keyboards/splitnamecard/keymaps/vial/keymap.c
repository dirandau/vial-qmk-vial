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

// 마우스 커서 순간 튐(노이즈) 클램핑 필터
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    // 1회 리포트당 최대 이동 거리를 ±25 픽셀로 제한하여 화면 벽으로 순간 튕기는 현상 방지
    if (mouse_report.x > 25) mouse_report.x = 25;
    else if (mouse_report.x < -25) mouse_report.x = -25;

    if (mouse_report.y > 25) mouse_report.y = 25;
    else if (mouse_report.y < -25) mouse_report.y = -25;

    return mouse_report;
}