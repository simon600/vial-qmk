/* Copyright 2022 DOIO
 * Copyright 2022 HorrorTroll <https://github.com/HorrorTroll>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "simon.h"
#ifdef HOST_PROTOCOL_ENABLE
#    include "host_protocol.h"
#endif

// Base layers: the layer key (top left) steps through all four, and kbd-daemon
// switches to GAMING while a game has focus ("gaming mode", host protocol).
// Holding the layer key lights the layer's keys that do something specific
// (the fn-layer highlight; the F-keys and Enter dim with the empty ones) and
// tells the host, whose overlay shows what they do.
enum layers {
    DESKTOP,
    GAMING,
    HOME,
    LIGHTING,
};

// Tap: next layer; hold: highlight this layer's keys. An LT for its tap / hold
// decision only (the layer is never switched on), kept as it was so the
// keymap Vial saved still has it
#define K_LAYER LT(LIGHTING, KC_NO)
// Held 1 s with the layer key held: the bootloader (bottom right)
#define BOOT_ROW 3
#define BOOT_COL 3

// Each layer's F-keys carry their own modifiers, so Hyprland's binds never eat
// a game's: SUPER + F13–F23 desktop (Hyprland), plain F13–F24 gaming (the
// game), Hyper + F13–F22 Home Assistant (Quickshell), Meh + F13–F15 the mic.
// With the us layout F13–F18 / F20–F23 arrive as XF86 keysyms (keybinds.lua).
#define MIC_DN MEH(KC_F13)
#define MIC_UP MEH(KC_F14)
#define MIC_MUT MEH(KC_F15)
#define HA_BRGT HYPR(KC_F13)
#define HA_DIM HYPR(KC_F14)
#define HA_GAME HYPR(KC_F15)
#define HA_DESK HYPR(KC_F16)
#define HA_FLOR HYPR(KC_F17)
#define HA_DRWR HYPR(KC_F18)
#define HA_DSK_D HYPR(KC_F19)
#define HA_DSK_U HYPR(KC_F20)
#define HA_FLR_D HYPR(KC_F21)
#define HA_FLR_U HYPR(KC_F22)
// Hyprland's own Hyper + volume binds: every light
#define HA_ALL_D HYPR(KC_VOLD)
#define HA_ALL_U HYPR(KC_VOLU)
#define HA_ALL HYPR(KC_MUTE)
// Hyprland's ALT + volume binds: the mouse's DPI, ALT + mute stores it in
// the onboard profile
#define DPI_DN A(KC_VOLD)
#define DPI_UP A(KC_VOLU)
#define DPI_SAV A(KC_MUTE)

// Vial names custom keycodes from QK_KB_0 (vial.json customKeycodes); the
// userspace's start at SAFE_RANGE (QK_USER_0). The first ones stand for these,
// in the userspace's order, and are translated before it sees them.
enum kb_keycodes {
    // The lighting's default: solid white (the userspace's UG_ANIM1)
    KB_LIGHT_DEF = QK_KB_0 + (UG_ANIM1 - SAFE_RANGE),
    KB_IND_BR_U = QK_KB_0 + (IND_BR_U - SAFE_RANGE),
    KB_IND_BR_D = QK_KB_0 + (IND_BR_D - SAFE_RANGE),
    KB_IND_MODE = QK_KB_0 + (IND_MODE - SAFE_RANGE),
    // The keyboard's own: bootloader once held for BOOT_HOLD_MS (boot_hold())
    KB_BOOT = QK_KB_0 + (SAFE_RANGE_SHARED - SAFE_RANGE),
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │Lyr│S13│S14│S15│   │ A │ │ B │      A workspaces · overview
       ├───┼───┼───┼───┤   └───┘ └───┘      B seek · mic mute
       │S16│S17│S18│S19│                    C volume · mute
       ├───┼───┼───┼───┤
       │S20│S21│S22│S23│      ┌───┐         S = SUPER + F
       ├───┼───┼───┼───┤      │ C │
       │Prv│Ply│Nxt│Ent│      └───┘
       └───┴───┴───┴───┘
*/
    [DESKTOP] = LAYOUT(
        K_LAYER,   G(KC_F13), G(KC_F14), G(KC_F15), G(KC_TAB),
        G(KC_F16), G(KC_F17), G(KC_F18), G(KC_F19), MIC_MUT,
        G(KC_F20), G(KC_F21), G(KC_F22), G(KC_F23), KC_MUTE,
        KC_MPRV,   KC_MPLY,   KC_MNXT,   KC_ENT
    ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │Lyr│F13│F14│F15│   │ A │ │ B │      A mouse DPI · save to profile
       ├───┼───┼───┼───┤   └───┘ └───┘      B mic volume · mic mute
       │F16│F17│F18│F19│                    C volume · mute
       ├───┼───┼───┼───┤
       │F20│F21│F22│F23│      ┌───┐         Shot: the screenshot submap
       ├───┼───┼───┼───┤      │ C │
       │F24│Esc│Sht│Ent│      └───┘
       └───┴───┴───┴───┘
*/
    [GAMING] = LAYOUT(
        K_LAYER, KC_F13, KC_F14,  KC_F15, DPI_SAV,
        KC_F16,  KC_F17, KC_F18,  KC_F19, MIC_MUT,
        KC_F20,  KC_F21, KC_F22,  KC_F23, KC_MUTE,
        KC_F24,  KC_ESC, SGUI(KC_S), KC_ENT
    ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │Lyr│Brt│Dim│Gam│   │ A │ │ B │      A desk lamp · toggle
       ├───┼───┼───┼───┤   └───┘ └───┘      B floor lamp · toggle
       │Dsk│Flr│All│Drw│                    C all lights · toggle
       ├───┼───┼───┼───┤
       │   │   │   │   │      ┌───┐         Brt / Dim / Gam: scenes
       ├───┼───┼───┼───┤      │ C │         Drw: the HA drawer
       │   │   │   │   │      └───┘
       └───┴───┴───┴───┘
*/
    [HOME] = LAYOUT(
        K_LAYER, HA_BRGT, HA_DIM, HA_GAME, HA_DESK,
        HA_DESK, HA_FLOR, HA_ALL, HA_DRWR, HA_FLOR,
        KC_NO,   KC_NO,   KC_NO,  KC_NO,   HA_ALL,
        KC_NO,   KC_NO,   KC_NO,  KC_NO
    ),

/*
       ┌───┬───┬───┬───┐   ┌───┐ ┌───┐
       │Lyr│Pr-│Pr+│Tog│   │ A │ │ B │      A profile -/+ · default
       ├───┼───┼───┼───┤   └───┘ └───┘      B indicator brightness
       │Hu-│Hu+│Sa-│Sa+│                    C brightness · on/off
       ├───┼───┼───┼───┤                    Driven by openrgb-daemon while it
       │Sp-│Sp+│In-│In+│      ┌───┐         listens, the keyboard's own
       ├───┼───┼───┼───┤      │ C │         lighting otherwise. In± indicator
       │IMd│   │   │Bt*│      └───┘         brightness; *Boot: held 1 s
       └───┴───┴───┴───┘
*/
    [LIGHTING] = LAYOUT(
        K_LAYER,     UG_PREV, UG_NEXT,     UG_TOGG,     KB_LIGHT_DEF,
        UG_HUED,     UG_HUEU, UG_SATD,     UG_SATU,     KC_NO,
        UG_SPDD,     UG_SPDU, KB_IND_BR_D, KB_IND_BR_U, UG_TOGG,
        KB_IND_MODE, _______, _______,     KB_BOOT
    ),
};

#ifdef ENCODER_MAP_ENABLE
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    //                A (top left)                       B (top right)                       C (big)
    [DESKTOP]  = { ENCODER_CCW_CW(G(MS_WHLU), G(MS_WHLD)), ENCODER_CCW_CW(KC_MRWD, KC_MFFD),   ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [GAMING]   = { ENCODER_CCW_CW(DPI_DN, DPI_UP),        ENCODER_CCW_CW(MIC_DN, MIC_UP),     ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [HOME]     = { ENCODER_CCW_CW(HA_DSK_D, HA_DSK_U),    ENCODER_CCW_CW(HA_FLR_D, HA_FLR_U), ENCODER_CCW_CW(HA_ALL_D, HA_ALL_U) },
    [LIGHTING] = { ENCODER_CCW_CW(UG_PREV, UG_NEXT),      ENCODER_CCW_CW(KB_IND_BR_D, KB_IND_BR_U), ENCODER_CCW_CW(UG_VALD, UG_VALU) },
};
#endif
// clang-format on

// --- BASE LAYERS ---

// The layer stepped to
static uint8_t base = DESKTOP;
// The layer key is held (past a tap): its keys light up
static bool layer_key_held = false;
// The bottom right key went down while the layer key was held: the bootloader
static bool boot_combo = false;
// GAMING because kbd-daemon asked (a game has focus), and the layer before it
static bool gaming_auto = false;
#ifdef HOST_PROTOCOL_ENABLE
static uint8_t before_gaming = DESKTOP;

static uint8_t base_layer(void) {
    return base;
}
#endif

static void move_to(uint8_t layer) {
    base = layer;
    layer_move(layer);
}

static void next_base_layer(void) {
    gaming_auto = false;
    move_to(base == LIGHTING ? DESKTOP : base + 1);
}

#ifdef HOST_PROTOCOL_ENABLE
// kbd-daemon's "gaming mode" is the GAMING layer here: on while a game has
// focus. A layer picked by hand reaches it as a state change on the keyboard,
// which pauses its switching until focus moves on.
uint8_t host_gaming_state_user(void) {
    return base_layer() == GAMING ? 1 : 0;
}

bool host_gaming_set_user(uint8_t state) {
    if (state > 2) return false;
    if (state != 0 && base != GAMING) {
        before_gaming = base;
        gaming_auto   = true;
        move_to(GAMING);
    } else if (state == 0 && base == GAMING) {
        gaming_auto = false;
        move_to(before_gaming);
    }
    return true;
}

// The layer showing, for the host's keyboard widget, which may also pick
// one: like the layer key, by hand
uint8_t host_layer_user(void) {
    return get_highest_layer(layer_state);
}

// While the layer key is held: the host's overlay shows that layer's keys
uint8_t host_held_layer_user(void) {
    return layer_key_held ? base : HOST_STATE_UNSUPPORTED;
}

bool host_layer_set_user(uint8_t layer) {
    if (layer > LIGHTING) return false;
    gaming_auto = false;
    move_to(layer);
    return true;
}
#endif

// --- OLED: the layer in large letters, and what the knobs turn ---
//
// The bezel hides the top ~4 pixel rows and a few columns on the right, so the
// top page stays empty and lines keep to 19 characters (114 px).

#ifdef OLED_ENABLE
// The driver's font (OLED_FONT_H)
extern const unsigned char font[] PROGMEM;

// One character at column x of `page`; double size takes two pages. Returns the
// next column.
static uint8_t draw_char(uint8_t x, uint8_t page, char c, bool big) {
    const unsigned char *glyph = font + (uint8_t)(c - OLED_FONT_START) * OLED_FONT_WIDTH;
    for (uint8_t col = 0; col < OLED_FONT_WIDTH; col++) {
        uint8_t bits = pgm_read_byte(glyph + col);
        if (!big) {
            if (x < OLED_DISPLAY_WIDTH) oled_write_raw_byte(bits, page * OLED_DISPLAY_WIDTH + x);
            x++;
            continue;
        }
        // Every pixel twice as tall and twice as wide
        uint16_t tall = 0;
        for (uint8_t b = 0; b < 8; b++) {
            if (bits & (1 << b)) tall |= 3u << (b * 2);
        }
        for (uint8_t w = 0; w < 2; w++, x++) {
            if (x >= OLED_DISPLAY_WIDTH) continue;
            oled_write_raw_byte(tall & 0xFF, page * OLED_DISPLAY_WIDTH + x);
            oled_write_raw_byte(tall >> 8, (page + 1) * OLED_DISPLAY_WIDTH + x);
        }
    }
    return x;
}

static uint8_t draw_text(uint8_t x, uint8_t page, const char *text, bool big) {
    while (*text) x = draw_char(x, page, *text++, big);
    return x;
}

typedef struct {
    const char *name;
    // What the knobs (A, B, C) turn, at most 19 characters
    const char *knobs;
} layer_legend_t;

static const layer_legend_t legends[] = {
    [DESKTOP]  = {"DESKTOP", "A wksp B seek C vol"},
    [GAMING]   = {"GAMING", "A dpi  B mic  C vol"},
    [HOME]     = {"HOME", "A desk B flr  C all"},
    [LIGHTING] = {"LIGHTING", "A prof B ind C brt"},
};

bool oled_task_user(void) {
    uint8_t layer = get_highest_layer(layer_state);
    if (layer >= ARRAY_SIZE(legends)) layer = DESKTOP;
    const char *flag = indicator_library[INDICATOR_MACRO_REC].active ? "REC" : (layer == GAMING && gaming_auto) ? "auto" : "";

    // Redrawn only when something on it changes
    static uint8_t     shown_layer = 0xFF;
    static const char *shown_flag  = NULL;
    if (layer == shown_layer && flag == shown_flag) return false;
    shown_layer = layer;
    shown_flag  = flag;

    oled_clear();
    uint8_t x = draw_text(0, 1, legends[layer].name, true);
    draw_text(x + 4, 1, flag, false);
    draw_text(0, 3, legends[layer].knobs, false);
    return false;
}
#endif

// --- QMK CALLBACK WRAPPERS ---

void keyboard_post_init_user(void) {
    keyboard_post_init_shared();
}

void matrix_scan_user(void) {
    matrix_scan_shared();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case K_LAYER:
            if (record->tap.count) {
                if (record->event.pressed) next_base_layer();
                return false;
            }
            // Held: this layer's keys light up, no layer switch
            layer_key_held = record->event.pressed;
            set_highlight_layer(get_highest_layer(layer_state), layer_key_held);
            return false;
        case KB_BOOT:
            boot_hold(record->event.pressed);
            return false;
    }
    // Layer key + bottom right, held 1 s: the bootloader on every layer. Its
    // release goes the same way, whenever the layer key is let go
    if (record->event.key.row == BOOT_ROW && record->event.key.col == BOOT_COL) {
        if (record->event.pressed) boot_combo = layer_key_held;
        if (boot_combo) {
            boot_hold(record->event.pressed);
            if (!record->event.pressed) boot_combo = false;
            return false;
        }
    }
    // Vial's custom keycodes -> the userspace's
    if (keycode >= QK_KB_0 && keycode < QK_KB_0 + (SAFE_RANGE_SHARED - SAFE_RANGE)) {
        keycode = keycode - QK_KB_0 + SAFE_RANGE;
    }
    return process_record_shared(keycode, record);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    // The host may switch the layer while the layer key is held
    set_highlight_layer(get_highest_layer(state), layer_key_held);
    return layer_state_set_shared(state);
}

// Lit while the layer key is held: what does something specific. The F-keys you program
// (plain on GAMING, SUPER + on DESKTOP) and Enter dim with the empty keys;
// Home Assistant's Hyper + F-keys light.
bool fn_layer_key_lit_user(uint16_t keycode) {
    uint16_t basic = keycode;
    uint8_t  mods  = 0;
    if (IS_QK_MODS(keycode)) {
        basic = QK_MODS_GET_BASIC_KEYCODE(keycode);
        mods  = QK_MODS_GET_MODS(keycode);
    }
    if (keycode == KC_NO || basic == KC_ENT) return false;
    if (basic >= KC_F13 && basic <= KC_F24 && (mods == 0 || mods == MOD_LGUI)) return false;
    return true;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    return rgb_matrix_indicators_advanced_shared(led_min, led_max);
}

// No get_permissive_hold wrapper: Vial's QMK Settings own permissive hold

void leader_start_user(void) {
    leader_start_shared();
}

void leader_end_user(void) {
    leader_end_shared();
}

bool dynamic_macro_record_start_user(int8_t direction) {
    return dynamic_macro_record_start_shared(direction);
}

bool dynamic_macro_record_end_user(int8_t direction) {
    return dynamic_macro_record_end_shared(direction);
}

void suspend_power_down_user(void) {
    suspend_power_down_shared();
}

void suspend_wakeup_init_user(void) {
    suspend_wakeup_init_shared();
}
