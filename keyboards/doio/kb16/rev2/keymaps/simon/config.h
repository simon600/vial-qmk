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
