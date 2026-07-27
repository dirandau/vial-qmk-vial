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

// pointing_device_task_user override removed to allow normal trackball motion