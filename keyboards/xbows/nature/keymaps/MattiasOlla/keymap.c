#include QMK_KEYBOARD_H

#define MODS_SHIFT_MASK (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))

#define US_ADIA ALGR(KC_Q)  // Ä
#define US_ARNG ALGR(KC_W)  // Å
#define US_ODIA ALGR(KC_P)  // Ö
#define US_EACU ALGR(KC_E)  // É

enum custom_keycodes {
    PLACEHOLDER = SAFE_RANGE,
    ARROW,
    MO_GRV,
    MO_CIRC,
    MO_QUOT
};

const char *codes[][2] = {
    /* ARROW */   {"->", "->"},
    /* MO_GRV */  {"` ", "~ "},
    /* MO_CIRC */ {"6", "^ "},
    /* MO_QUOT */ {"' ", "\" "}
};

uint8_t mods;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    mods = get_mods();

    switch (keycode) {
        case ARROW:
        case MO_GRV:
        case MO_CIRC:
        case MO_QUOT: {
            if (!record->event.pressed) return true;

            clear_mods();
            // Send code based on which key was pressed and whether Shift was held.
            uint16_t index = keycode - PLACEHOLDER - 1;
            bool shift = (bool)(mods & MODS_SHIFT_MASK);

            send_string(codes[index][shift]);

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
        KC_ESC,   KC_F1,   KC_F2,   KC_F3,    KC_F4,   KC_F5,   KC_F6,          KC_F7,     KC_F8,   KC_F9,   KC_F10,  KC_F11,     KC_F12,     KC_DEL,           KC_PSCR,
        MO_GRV,   KC_1,    KC_2,    KC_3,     KC_4,    KC_5,                               MO_CIRC, KC_7,    KC_8,    KC_9,       KC_0,       KC_MINS, KC_EQL,  KC_BSPC,
        KC_TAB,   KC_Q,    KC_W,    KC_E,     KC_R,    KC_T,                               KC_Y,    KC_U,    KC_I,    KC_O,       KC_P,       KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
        KC_CAPS,  KC_A,    KC_S,    KC_D,     KC_F,    KC_G,             KC_BSPC,          KC_H,    KC_J,    KC_K,    KC_L,       KC_SCLN,    MO_QUOT, KC_ENT,           KC_PGDN,
        KC_LSFT,  KC_Z,    KC_X,    KC_C,     KC_V,    KC_B,             KC_ENT,           KC_N,    KC_M,    KC_COMM, KC_DOT,     KC_SLSH,    KC_RSFT,          KC_UP,
        KC_LCTL,  MO(1),   KC_LALT,           KC_SPC,           LCTL_T(KC_TAB), KC_LSFT,   KC_SPC,           KC_RALT,             KC_LGUI,    KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT(
        RESET,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,        KC_TRNS,   KC_TRNS, KC_CALC, KC_MYCM, KC_MSEL,    KC_MAIL,    NK_TOGG,          EEP_RST,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,                            KC_TRNS, KC_ESC,  KC_TRNS, KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS, KC_NLCK,
        RGB_TOG,  RGB_MOD, RGB_VAI, US_EACU,  KC_TRNS, KC_TRNS,                            KC_BSLS, KC_EQL,  KC_UP,   S(KC_EQL),  S(KC_BSLS), US_ARNG, KC_TRNS, KC_TRNS, KC_MNXT,
        KC_TRNS,  RGB_SPD, RGB_VAD, RGB_SPI,  KC_TRNS, KC_TRNS,          KC_TRNS,          KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT,    US_ODIA,    US_ADIA, KC_TRNS,          KC_MPRV,
        KC_TRNS,  KC_TRNS, KC_TRNS, RGB_HUI,  KC_TRNS, KC_TRNS,          KC_TRNS,          KC_HOME, KC_END,  KC_MINS, S(KC_MINS), ARROW,      KC_MUTE,          KC_VOLU,
        KC_TRNS,  KC_TRNS, KC_TRNS,           KC_TRNS,          KC_TRNS,        KC_TRNS,   KC_TRNS,          KC_TRNS,             KC_TRNS,    KC_MPLY, KC_HOME, KC_VOLD, KC_END
    )
    // clang-format on
};
