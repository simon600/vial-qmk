/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#define VIAL_KEYBOARD_UID {0x65, 0x9A, 0x6E, 0x3C, 0x47, 0x63, 0xB9, 0x9B}

#define VIAL_UNLOCK_COMBO_ROWS { 3, 0 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 3 }

#define OPENRGB_DEVICE_NAME "DOIO KB16"
#define OPENRGB_DEVICE_VENDOR "DOIO"

// LEDs off while USB is suspended (the PC asleep), like the Q5 HE: openrgb-
// daemon leaves the keyboards out of its sleep blackout
#define RGB_MATRIX_SLEEP

// The lighting's default, the Q5 HE's: solid white at full brightness. Only
// read on an EEPROM reset; the Default key (A pressed on LIGHTING) sets it too.
// DOIO's info.json sets cycle_up_down.
#undef RGB_MATRIX_DEFAULT_MODE
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_SOLID_COLOR
#define RGB_MATRIX_DEFAULT_HUE 0
#define RGB_MATRIX_DEFAULT_SAT 0
#define RGB_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
