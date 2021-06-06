#include QMK_KEYBOARD_H

#define MODS_SHIFT_MASK (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))

enum custom_keycodes {
    SE_AA = SAFE_RANGE,
    SE_AE,
    SE_OE,
};

const char *swedish_codes[][2] = {
    {
        SS_RALT(SS_TAP(X_KP_1) SS_TAP(X_KP_3) SS_TAP(X_KP_4)),  // Alt 134 -> å
        SS_RALT(SS_TAP(X_KP_1) SS_TAP(X_KP_4) SS_TAP(X_KP_3)),  // Alt 143 -> Å
    },
    {
        SS_RALT(SS_TAP(X_KP_1) SS_TAP(X_KP_3) SS_TAP(X_KP_2)),  // Alt 132 -> ä
        SS_RALT(SS_TAP(X_KP_1) SS_TAP(X_KP_4) SS_TAP(X_KP_2)),  // Alt 142 -> Ä
    },
    {
        SS_RALT(SS_TAP(X_KP_1) SS_TAP(X_KP_4) SS_TAP(X_KP_8)),  // Alt 148 -> ö
        SS_RALT(SS_TAP(X_KP_1) SS_TAP(X_KP_5) SS_TAP(X_KP_3)),  // Alt 153 -> Ö
    },
};

uint8_t mods;
bool    process_record_user(uint16_t keycode, keyrecord_t *record) {
    mods = get_mods();
    switch (keycode) {
        case SE_AA:
        case SE_AE:
        case SE_OE: {
            if (!record->event.pressed) return true;

            clear_mods();
            // Send code based on which key was pressed and whether Shift was held.
            uint16_t index = keycode - SE_AA;
            uint8_t  shift = mods & MODS_SHIFT_MASK;

            send_string(swedish_codes[index][(bool)shift]);

            set_mods(mods);
            return false;
        }
        case KC_BSPC: {
            // Initialize a boolean variable that keeps track
            // of the delete key status: registered or not?
            static bool delkey_registered;
            if (record->event.pressed) {
                // Detect the activation of either shift keys
                if (mods & MOD_MASK_SHIFT) {
                    // First temporarily canceling both shifts so that
                    // shift isn't applied to the KC_DEL keycode
                    del_mods(MOD_MASK_SHIFT);
                    register_code(KC_DEL);
                    // Update the boolean variable to reflect the status of KC_DEL
                    delkey_registered = true;
                    // Reapplying modifier state so that the held shift key(s)
                    // still work even after having tapped the Backspace/Delete key.
                    set_mods(mods);
                    return false;
                }
            } else {  // on release of KC_BSPC
                // In case KC_DEL is still being sent even after the release of KC_BSPC
                if (delkey_registered) {
                    unregister_code(KC_DEL);
                    delkey_registered = false;
                    return false;
                }
            }
            // Let QMK process the KC_BSPC keycode as usual outside of shift
            return true;
        }
        default:
            return true;
    }
}

const uint16_t WS_LEFT = SGUI(KC_LEFT);
const uint16_t WS_RGHT = SGUI(KC_RGHT);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Keymap VANILLA: (Base Layer) Default Layer
     *
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * | Esc |  F1  |  F2  |  F3  |  F4  |      F5  |  F6  |  F7  |  F8  |      F9  |  F10 |  F11 |  F12 |   Delete   |    Prtsc    |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * |  ~  |     1   |   2   |   3   |   4   |    5      |       6    |    7    |    8   |   9  |   0  |   -  |  =  |  Backspace  |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * | Tab |   Q    |    W   |   E  |   R  |   T  |            |    Y   |    U   |    I  |   O  |   P  |   [  |  ]  |   \  | PgUp |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * | Ctl |   A   |   S   |   D  |   F  |   G  |      Bksp      |    H  |    J   |   K  |   L  |   ;  |  '"  |    Enter   | PgDn |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * |Shift|   Z  |   X  |   C  |   V  |   B  |       Enter       |    N  |    M   |  ,  |   .  |  /?  | Shift|      |  Up |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     * |Ctrl | GUI |     Alter   |    Space   |   Ctrl   |   Shift   |     Space     |    Alter   |  FN  | Ctrl | Lft  |  Dn |  Rig |
     * |---------------------------------------------------------------------------------------------------------------------------------|
     */
    // clang-format off
    [0] = LAYOUT(
        KC_ESC,        KC_F1,   KC_F2,      KC_F3,    KC_F4,   KC_F5,   KC_F6,   KC_F7,     KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,           KC_PSCR,
        KC_GRV,        KC_1,    KC_2,       KC_3,     KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,        KC_Q,    KC_W,       KC_E,     KC_R,    KC_T,                        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
        KC_CAPS,       KC_A,    KC_S,       KC_D,     KC_F,    KC_G,         KC_BSPC,       KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,           KC_PGDN,
        KC_LSFT,       KC_Z,    KC_X,       KC_C,     KC_V,    KC_B,         KC_ENT,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,
        KC_LCTL,       MO(1),   LALT_T(KC_SPC),       KC_SPC,           KC_LCTL, KC_SFTENT, KC_SPC,           RALT_T(KC_SPC),   KC_LGUI, KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT(
        RESET,         KC_TRNS, KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_CALC, KC_MYCM, KC_MSEL, KC_MAIL, NK_TOGG,          EEP_RST,
        KC_TRNS,       KC_TRNS, KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS,                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_NLCK,
        RGB_TOG,       RGB_MOD, RGB_VAI,    RGB_HUI,  KC_TRNS, KC_TRNS,                     KC_TRNS, KC_TRNS, KC_UP,   KC_TRNS, KC_TRNS, SE_AA,   KC_TRNS, KC_TRNS, KC_MNXT,
        KC_TRNS,       RGB_SPD, RGB_VAD,    RGB_SPI,  KC_TRNS, KC_TRNS,      KC_TRNS,       KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, SE_OE,   SE_AE,   KC_TRNS,          KC_MPRV,
        KC_TRNS,       KC_TRNS, KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS,      KC_TRNS,       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE,          KC_VOLU,
        MO(2),         KC_TRNS, KC_TRNS,              KC_TRNS,          KC_TRNS, KC_TRNS,   KC_TRNS,          KC_TRNS,          KC_TRNS, KC_MPLY, KC_HOME, KC_VOLD, KC_END
    ),
    [2] = LAYOUT(
        KC_TRNS,       KC_TRNS, KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
        KC_TRNS,       KC_TRNS, KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS,                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,       KC_TRNS, KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS,                     KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS,       KC_TRNS, SGUI(KC_S), KC_TRNS,  KC_TRNS, KC_TRNS,      KC_TRNS,       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
        KC_TRNS,       KC_TRNS, KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS,      KC_TRNS,       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,
        KC_TRNS,       KC_TRNS, KC_TRNS,              KC_TRNS,          KC_TRNS, KC_TRNS,   KC_TRNS,          KC_TRNS,          KC_TRNS, KC_TRNS, WS_LEFT, KC_TRNS, WS_RGHT
    )
    // clang-format on
};
