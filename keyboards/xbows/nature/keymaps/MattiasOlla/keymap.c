#include QMK_KEYBOARD_H
#include "sendstring_swedish.h"

#define MODS_SHIFT_MASK (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT))

enum custom_keycodes {
    MO_GRV = SAFE_RANGE,
    MO_1,
    MO_2,
    MO_3,
    MO_4,
    MO_5,
    MO_6,
    MO_7,
    MO_8,
    MO_9,
    MO_0,
    MO_MINS,
    MO_EQL,
    MO_LBRC,
    MO_RBRC,
    MO_BSLS,
    MO_SCLN,
    MO_QUOT,
    MO_COMM,
    MO_DOT,
    MO_SLSH,
    ARROW,
    E_ACC
};

const char *codes[][2] = {
    /* MO_GRV */  {"` ", "~ "}, // Backtick and tilde are dead keys, send space to escape
    /* MO_1 */    {"1", "!"},
    /* MO_2 */    {"2", "@"},
    /* MO_3 */    {"3", "#"},
    /* MO_4 */    {"4", "$"},
    /* MO_5 */    {"5", "%"},
    /* MO_6 */    {"6", "^ "}, // Circumflex is dead key, send space to escape
    /* MO_7 */    {"7", "&"},
    /* MO_8 */    {"8", "*"},
    /* MO_9 */    {"9", "("},
    /* MO_0 */    {"0", ")"},
    /* MO_MINS */ {"-", "_"},
    /* MO_EQL */  {"=", "+"},
    /* MO_LBRC */ {"[", "{"},
    /* MO_RBRC */ {"]", "}"},
    /* MO_BSLS */ {"\\", "|"},
    /* MO_SCLN */ {";", ":"},
    /* MO_QUOT */ {"'", "\""},
    /* MO_COMM */ {",", "<"},
    /* MO_DOT */  {".", ">"},
    /* MO_SLSH */ {"/", "?"},
    /* ARROW  */  {"->", "->"}
};

uint8_t mods;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    mods = get_mods();
    switch (keycode) {
        case MO_GRV:
        case MO_1:
        case MO_2:
        case MO_3:
        case MO_4:
        case MO_5:
        case MO_6:
        case MO_7:
        case MO_8:
        case MO_9:
        case MO_0:
        case MO_MINS:
        case MO_EQL:
        case MO_LBRC:
        case MO_RBRC:
        case MO_BSLS:
        case MO_SCLN:
        case MO_QUOT:
        case MO_COMM:
        case MO_DOT:
        case MO_SLSH:
        case ARROW: {
            if (!record->event.pressed) return true;

            clear_mods();
            // Send code based on which key was pressed and whether Shift was held.
            uint16_t index = keycode - SAFE_RANGE;
            bool shift = (bool)(mods & MODS_SHIFT_MASK);

            send_string(codes[index][shift]);

            set_mods(mods);
            return false;
        }
        case E_ACC: {
            if (!record->event.pressed) return true;
            tap_code(SE_ACUT);
            tap_code(SE_E);
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
        KC_ESC,   KC_F1,   KC_F2,   KC_F3,    KC_F4,   KC_F5,   KC_F6,         KC_F7,     KC_F8,   KC_F9,   KC_F10,  KC_F11,     KC_F12,     KC_DEL,           KC_PSCR,
        MO_GRV,   MO_1,    MO_2,    MO_3,     MO_4,    MO_5,                              MO_6,    MO_7,    MO_8,    MO_9,       MO_0,       MO_MINS, MO_EQL,  KC_BSPC,
        KC_TAB,   KC_Q,    KC_W,    KC_E,     KC_R,    KC_T,                              KC_Y,    KC_U,    KC_I,    KC_O,       KC_P,       MO_LBRC, MO_RBRC, MO_BSLS, KC_PGUP,
        KC_CAPS,  KC_A,    KC_S,    KC_D,     KC_F,    KC_G,            KC_BSPC,          KC_H,    KC_J,    KC_K,    KC_L,       MO_SCLN,    MO_QUOT, KC_ENT,           KC_PGDN,
        KC_LSFT,  KC_Z,    KC_X,    KC_C,     KC_V,    KC_B,            KC_ENT,           KC_N,    KC_M,    MO_COMM, MO_DOT,     MO_SLSH,    KC_RSFT,          KC_UP,
        KC_LCTL,  MO(1),   LALT_T(KC_SPC),    KC_SPC,           CTL_T(KC_TAB), KC_LSFT,   KC_SPC,           RALT_T(KC_SPC),      KC_LGUI,    KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
    ),
    [1] = LAYOUT(
        RESET,    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,       KC_TRNS,   KC_TRNS, KC_CALC, KC_MYCM, KC_MSEL,    KC_MAIL,    NK_TOGG,          EEP_RST,
        KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,                           KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS,    KC_TRNS, KC_TRNS, KC_NLCK,
        RGB_TOG,  RGB_MOD, RGB_VAI, E_ACC,    KC_TRNS, KC_TRNS,                           SE_BSLS, SE_EQL,  KC_UP,   SE_PLUS,    SE_PIPE,    SE_ARNG, KC_TRNS, KC_TRNS, KC_MNXT,
        KC_TRNS,  RGB_SPD, RGB_VAD, RGB_SPI,  KC_TRNS, KC_TRNS,         KC_TRNS,          KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT,    SE_ODIA,    SE_ADIA, KC_TRNS,          KC_MPRV,
        KC_TRNS,  KC_TRNS, KC_TRNS, RGB_HUI,  KC_TRNS, KC_TRNS,         KC_TRNS,          KC_HOME, KC_END,  SE_MINS, SE_UNDS,    ARROW,      KC_MUTE,          KC_VOLU,
        KC_TRNS,  KC_TRNS, KC_TRNS,           KC_TRNS,          KC_TRNS,       KC_TRNS,   KC_TRNS,          KC_TRNS,             KC_TRNS,    KC_MPLY, KC_HOME, KC_VOLD, KC_END
    )
    // clang-format on
};
