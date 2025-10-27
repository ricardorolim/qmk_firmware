#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _BASE 0
#define _NAV 1
#define _SYM 2
#define _FN     3
#define _NUMPAD 4

// Some basic macros
#define TASK   LCTL(LSFT(KC_ESC))
#define TAB_R  LCTL(KC_TAB)
#define TAB_L  LCTL(LSFT(KC_TAB))
#define TAB_RO LCTL(LSFT(KC_T))
#define LSFT_TMUX LSFT_T(TMUX)
#define RSFT_TMUX RSFT_T(TMUX)

enum custom_keycodes {
    TMUX = SAFE_RANGE,
    TMUX_CP,
};

void send_tmux(void) {
    register_code(KC_LALT);
    tap_code(KC_SPC);
    unregister_code(KC_LALT);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // shift + space -> underscore
    if (keycode == KC_SPC && record->event.pressed) {
        uint8_t mods = get_mods();
        if (mods & MOD_MASK_SHIFT) {
            // Temporarily clear shift so it doesn’t affect KC_MINUS itself
            del_mods(MOD_MASK_SHIFT);
            register_code(KC_LSFT);
            tap_code(KC_MINUS);
            unregister_code(KC_LSFT);
            set_mods(mods); // Restore mods
            return false;
        }
    }

    // tmux macro
    switch (keycode) {
        case LSFT_T(TMUX):
            if (record->tap.count && record->event.pressed) {
                send_tmux();
                return false;
            }
            break;

        case RSFT_T(TMUX):
            if (record->tap.count && record->event.pressed) {
                send_tmux();
                return false;
            }
            break;

        case TMUX_CP:
            if (record->event.pressed) {
                send_tmux();
                tap_code(KC_LBRC);
            }
            return false;
    }

    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_5x7(
        // left hand
        KC_MINS,                KC_1,    KC_2,      KC_3,   KC_4,   KC_5,   TT(_FN),
        KC_TAB,                 KC_Q,    KC_W,      KC_F,   KC_P,   KC_B,   KC_LBRC,
        MT(MOD_LCTL, KC_ESC),   KC_A,    KC_R,      KC_S,   KC_T,   KC_G,   XXXXXXX,
        LSFT_TMUX,              KC_Z,    KC_X,      KC_C,   KC_D,   KC_V,
        KC_CAPS,                KC_LGUI, CW_TOGG,   KC_LALT,
                                    KC_ESC, KC_BSPC,        // bottom left,   top left
                                    XXXXXXX, MO(_NAV),      // bottom center, top center
                                    XXXXXXX, MO(_SYM),      // bottom right,  top right
        // right hand
                          TT(_FN), KC_6,    KC_7,    KC_8,     KC_9,     KC_0,     KC_EQL,
                          KC_RBRC, KC_J,    KC_L,    KC_U,     KC_Y,     KC_SCLN,  KC_BSLS,
                      TG(_NUMPAD), KC_M,    KC_N,    KC_E,     KC_I,     KC_O,     RCTL_T(KC_QUOT),
                                   KC_K,    KC_H,    KC_COMM,  KC_DOT,   KC_SLSH,  RSFT_TMUX,
                                            KC_LBRC, KC_RBRC,    KC_RALT,  KC_RGUI,
             KC_ENT,   KC_RGUI,     // top right,   bottom right
             KC_SPC,   XXXXXXX,     // top center,  bottom center
             MO(_SYM), XXXXXXX      // top left,    bottom left
    ),

    [_NAV] = LAYOUT_5x7(
        // left hand
        _______,        _______,        _______,        _______,        _______,        _______,   _______,
        _______,        _______,        _______,        LCTL(KC_F),     _______,        _______,   _______,
        _______,        OSM(MOD_LGUI),  OSM(MOD_LALT),  OSM(MOD_LSFT),  OSM(MOD_LCTL),  _______,   _______,
        _______,        LCTL(KC_Z),     LCTL(KC_X),     LCTL(KC_C),     LCTL(KC_V),     _______,
        _______,        _______,        _______,        _______,
                                        _______,        _______,     // bottom left,   top left
                                        _______,        _______,     // bottom center, top center
                                        _______,        _______,     // bottom right,  top right
        // right hand
                      _______, _______, _______, _______,   _______,    _______,  _______,
                      _______, TMUX_CP, KC_PGUP, KC_UP,     KC_PGDN,    KC_SCLN,  _______,
                      _______, KC_BSPC, KC_LEFT, KC_DOWN,   KC_RGHT,    KC_DEL,   _______,
                               _______, KC_HOME, _______,   KC_END,     _______,  _______,
                                                 _______,   _______,    _______,  _______,
             _______, _______,   // bottom right,  top right
             _______, _______,   // bottom center, top center
             _______, _______    // bottom left,   top left
    ),


    [_SYM] = LAYOUT_5x7(
        // left hand
        _______,    _______,    _______,    _______,    _______,    _______,    _______,
        _______,    KC_QUOT,    KC_LT,      KC_GT,      KC_DQT,     KC_DOT,     _______,
        _______,    KC_EXLM,    KC_MINUS,   KC_PLUS,    KC_EQL,     KC_HASH,   _______,
        _______,    KC_CIRC,    KC_SLSH,    KC_ASTR,    KC_BSLS,    KC_GRV,
        _______,    _______,    _______,    _______,
                                    _______, _______,
                                    _______, _______,
                                    _______, _______,
        // right hand
                        _______, _______,    _______,    _______,   _______,    _______,    _______,
                        _______, KC_AMPR,    KC_TILD,    KC_LBRC,   KC_RBRC,    KC_PERC,    _______,
                        _______, KC_PIPE,    KC_COLN,    KC_LPRN,   KC_RPRN,    KC_QUES,    _______,
                                 KC_TILD,    KC_DLR,     KC_LCBR,   KC_RCBR,    KC_AT,      _______,
                                         _______, _______,    _______,  _______,
             _______, _______,
             _______, _______,
             _______, _______
    ),

    [_FN] = LAYOUT_5x7(
        // left hand
        _______,   KC_F1,     KC_F2,      KC_F3,    KC_F4,     KC_F5,    _______,
        _______,   _______,   _______,    KC_UP,    _______,   _______,  _______,
        _______,   _______,   KC_LEFT,    KC_DOWN,  KC_RGHT,   _______,  QK_BOOT,
        _______,   _______,   _______,   _______,   _______,   _______,
        KC_MSTP,   KC_MPLY,   KC_MPRV,   KC_MNXT,
                                    _______, _______,
                                    _______, _______,
                                    _______, _______,
        // right hand
                          _______,   KC_F6,     KC_F7,     KC_F8,     KC_F9,     KC_F10,    KC_F11,
                          _______,   _______,   _______,   _______,   _______,   _______,   KC_F12,
                          QK_BOOT,   _______,   _______,   _______,   _______,   _______,   _______,
                                     _______,   _______,   _______,   _______,   _______,   _______,
                                                           _______,   _______,   _______,   _______,
             KC_DEL, _______,
             _______, _______,
             _______, _______
    ),

    [_NUMPAD] = LAYOUT_5x7(
        // left hand
        _______,   _______,   _______,   _______,   _______,   _______,  _______,
        _______,   _______,   _______,   _______,   _______,   _______,  _______,
        _______,   _______,   _______,   _______,   _______,   _______,  _______,
        _______,   _______,   _______,   _______,   _______,   _______,
        _______,   _______,   _______,   _______,
                                    _______, _______,
                                    _______, _______,
                                    _______, _______,
        // right hand
                          _______,   _______,   KC_NUM,    _______,   KC_PMNS,   KC_PPLS,   _______,
                          _______,   _______,   KC_P7,     KC_P8,     KC_P9,     _______,   _______,
                          _______,   _______,   KC_P4,     KC_P5,     KC_P6,     KC_PAST,   _______,
                                     _______,   KC_P1,     KC_P2,     KC_P3,     KC_PSLS,   _______,
                                                           KC_P0,     KC_PDOT,   _______,   _______,
             _______, KC_PENT,
             _______, _______,
             _______, _______
    ),
};
