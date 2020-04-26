#include QMK_KEYBOARD_H

enum my_keycodes {
	M_DELP = SAFE_RANGE,
	M_9,
	M_0,
	M_LPAR,
	M_RPAR
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// 9 and 0 shift into {}. () shift into []. Delete shifts into pipe.
//┌── ` ───┬── 1 ───┬── 2 ───┬── 3 ───┬── 4 ───┬── 5 ───┐ ┌── 6 ───┬── 7 ───┬── 8 ───┬── 9 ───┬── 0 ───┬── ( ───┐
//├─ tab ──┼───Q────┼───W────┼───E────┼───R────┼───T────┤ ├───Y────┼───U────┼───I────┼───O────┼───P────┼── ) ───┤
//├─ esc ──┼───A────┼───S────┼───D────┼───F────┼───G────┤ ├───H────┼───J────┼───K────┼───L────┼── ; ───┼── L1 ──┤
//├─ shft ─┼───Z────┼───X────┼───C────┼───V────┼───B────┤ ├───N────┼───M────┼── , ───┼── . ───┼── / ───┼─ shft ─┤
//└─ ctrl ─┴── L1 ──┴─ win ──┴─ alt ──┴─ bksp ─┴──space─┘ └─space──┴──enter─┴─ alt ──┴── ' ───┴─ del| ─┴─ ctrl ─┘

  [0] = LAYOUT(
		KC_GRV , KC_1   , KC_2   , KC_3   , KC_4   , KC_5   ,   KC_6   , KC_7   , KC_8   , M_9    , M_0    , M_LPAR ,
		KC_TAB , KC_Q   , KC_W   , KC_E   , KC_R   , KC_T   ,   KC_Y   , KC_U   , KC_I   , KC_O   , KC_P   , M_RPAR ,
		KC_ESC , KC_A   , KC_S   , KC_D   , KC_F   , KC_G   ,   KC_H   , KC_J   , KC_K   , KC_L   , KC_SCLN, MO(1)  ,
		KC_LSFT, KC_Z   , KC_X   , KC_C   , KC_V   , KC_B   ,   KC_N   , KC_M   , KC_COMM, KC_DOT , KC_SLSH, KC_RSFT,
		KC_LCTL, MO(1)  , KC_LWIN, KC_LALT, KC_BSPC, KC_SPC ,   KC_SPC , KC_ENT , KC_RALT, KC_QUOT, M_DELP , KC_RCTL
	),


//┌── F1 ──┬── F2 ──┬── F3 ──┬── F4 ──┬── F5 ──┬── F6 ──┐ ┌── F7 ──┬── F8 ──┬── F9 ──┬── F10 ─┬── F11 ─┬── F12 ─┐
//├────────┼─ home ─┼── ^ ───┼─ end ──┼─ pgup ─┼─ vol+ ─┤ ├─ brt- ─┼brt togg┼─ brt+ ─┼─ menu ─┼─ caps ─┼──reset─┤
//├────────┼── < ───┼── v ───┼── > ───┼─ pgdn ─┼─ mute ─┤ ├── < ───┼── v ───┼── ^ ───┼── > ───┼─insert─┼────────┤
//├────────┼─ mus> ─┼─ppause─┼─ mus< ─┼prt scrn┼─ vol+ ─┤ ├── + ───┼── = ───┼── - ───┼── _ ───┼── \ ───┼────────┤
//└────────┴────────┴────────┴────────┴────────┴────────┘ └────────┴────────┴────────┴────────┴────────┴────────┘
	[1] = LAYOUT(
		KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  , KC_F6  ,   KC_F7  , KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12 ,
		_______, KC_HOME, KC_UP  , KC_END , KC_PGUP, KC_VOLU,   BL_DEC , BL_TOGG, BL_INC , KC_APP , KC_CAPS, _______,
		_______, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN, KC_MUTE,   KC_LEFT, KC_DOWN, KC_UP  , KC_RGHT, KC_INS , _______,
		_______, KC_MPRV, KC_MPLY, KC_MNXT, KC_PSCR, KC_VOLD,   KC_PLUS, KC_EQL , KC_MINS, KC_UNDS, KC_BSLS, _______,
		_______, _______, _______, _______, _______, _______,   _______, _______, _______, XXXXXXX, KC_DEL , _______
	)
};


// underglow/layer binding
void keyboard_post_init_user(void) { rgblight_sethsv_noeeprom(0, 0, 0); }
uint32_t layer_state_set_user(uint32_t state) {
	// if current layer == 0, we are swithing to the second layer, so make white
	// similarly if current layer == 0, we are swithing to base, so turn off
	     if (layer_state == 0) { rgblight_sethsv_noeeprom(0, 0, 255); }
	else if (layer_state == 2) { rgblight_sethsv_noeeprom(0, 0, 0); }
	return state;
}


// custom shift keys
// https://github.com/jeherve/qmk_firmware/blob/06206a9d5bfcbf96d65394b8bed495dc7b0ddf70/keyboards/redox/keymaps/jeherve/keymap.c#L64-L109

static bool lsft_prsd = false;
static bool rsft_prsd = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	//rgblight_sethsv(0,0,0); // to configure EEPROM

	uint16_t kc = keycode;
	switch (keycode) {
		case KC_LSFT: lsft_prsd = record->event.pressed; return true;
		case KC_RSFT: rsft_prsd = record->event.pressed; return true;

		// this is the template for a key that shifts into another shifted key
		// e.g. normally delete, becomes | (pipe) when shifted
		case M_DELP:
			kc = (lsft_prsd || rsft_prsd) ? KC_BSLS : KC_DEL;
			if (record->event.pressed)
				register_code(kc);
			else
				unregister_code(kc);
			return false;

		case M_9:
			kc = (lsft_prsd || rsft_prsd) ? KC_LBRC : KC_9;
			if (record->event.pressed)
				register_code(kc);
			else
				unregister_code(kc);
			return false;

		case M_0:
			kc = (lsft_prsd || rsft_prsd) ? KC_RBRC : KC_0;
			if (record->event.pressed)
				register_code(kc);
			else
				unregister_code(kc);
			return false;

	// this is the template for a key that is a shifted key normally and a normal
	//   key when shifted
	// e.g. normally (, becomes [ when shifted
	case M_LPAR:
			kc = (lsft_prsd || rsft_prsd) ? KC_LBRC : KC_9;
			if (lsft_prsd || rsft_prsd) {
				if (lsft_prsd) unregister_code(KC_LSFT);
				if (rsft_prsd) unregister_code(KC_RSFT);
				if (record->event.pressed)
					register_code(kc);
				else
					unregister_code(kc);
				if (lsft_prsd) register_code(KC_LSFT);
				if (rsft_prsd) register_code(KC_RSFT);
			} else {
				register_code(KC_LSFT);
				if (record->event.pressed)
					register_code(kc);
				else
					unregister_code(kc);
				unregister_code(KC_LSFT);
			}
			return false;

		case M_RPAR:
			kc = (lsft_prsd || rsft_prsd) ? KC_RBRC : KC_0;
			if (lsft_prsd || rsft_prsd) {
				if (lsft_prsd) unregister_code(KC_LSFT);
				if (rsft_prsd) unregister_code(KC_RSFT);
				if (record->event.pressed)
					register_code(kc);
				else
					unregister_code(kc);
				if (lsft_prsd) register_code(KC_LSFT);
				if (rsft_prsd) register_code(KC_RSFT);
			} else {
				register_code(KC_LSFT);
				if (record->event.pressed)
					register_code(kc);
				else
					unregister_code(kc);
				unregister_code(KC_LSFT);
			}
			return false;

		// this is the template for a key that shifts into a normal key
		// e.g. normally A, becomes 3 when shfited
		/*case M_ASDF:
			kc = (lsft_prsd || rsft_prsd) ? KC_3 : KC_A;
			if (record->event.pressed) {
				if (lsft_prsd) unregister_code(KC_LSFT);
				if (rsft_prsd) unregister_code(KC_RSFT);
				register_code(kc);
				if (lsft_prsd) register_code(KC_LSFT);
				if (rsft_prsd) register_code(KC_RSFT);
			} else {
				if (lsft_prsd) unregister_code(KC_LSFT);
				if (rsft_prsd) unregister_code(KC_RSFT);
				unregister_code(kc);
				if (lsft_prsd) register_code(KC_LSFT);
				if (rsft_prsd) register_code(KC_RSFT);
			}
			return false;*/
	}
	return true;
}
