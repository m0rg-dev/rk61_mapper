#include "USBHost_t36.h"

uint8_t keyboard_modifiers = 0;

USBHost usb;
USBHub hub1(usb);
KeyboardController keyboard1(usb);
USBHIDParser hid1(usb);

uint8_t remap_normal[256] = {0};
uint8_t remap_fn[256] = {0};

uint8_t is_remapping = 0;

void setup() {
	remap_normal[KEY_CAPS_LOCK & 0xFF] = KEY_ESC   & 0xFF;
	remap_normal[KEY_ESC       & 0xFF] = KEY_TILDE & 0xFF;

	// Since we're going to ape left GUI as our function key, let's remap the menu key as RGUI.
	remap_normal[KEY_MENU & 0xFF] = 110;

	remap_fn[KEY_H & 0xFF] = KEY_LEFT & 0xFF;
	remap_fn[KEY_J & 0xFF] = KEY_DOWN & 0xFF;
	remap_fn[KEY_K & 0xFF] = KEY_UP & 0xFF;
	remap_fn[KEY_L & 0xFF] = KEY_RIGHT & 0xFF;
	
	// I don't mind the default layout of the nav block, it just needs to not be in the way of the vim arrows.
	// Let's arbitrarily shift it over to ERTDFGCVB.
	remap_fn[KEY_E & 0xFF] = KEY_PRINTSCREEN & 0xFF;
	remap_fn[KEY_R & 0xFF] = KEY_SCROLL_LOCK & 0xFF;
	remap_fn[KEY_T & 0xFF] = KEY_PAUSE       & 0xFF;
	remap_fn[KEY_D & 0xFF] = KEY_INSERT      & 0xFF;
	remap_fn[KEY_F & 0xFF] = KEY_HOME        & 0xFF;
	remap_fn[KEY_G & 0xFF] = KEY_PAGE_UP     & 0xFF;
	remap_fn[KEY_C & 0xFF] = KEY_DELETE      & 0xFF;
	remap_fn[KEY_V & 0xFF] = KEY_END         & 0xFF;
	remap_fn[KEY_B & 0xFF] = KEY_PAGE_DOWN   & 0xFF;

	// And, of course, we'll need the function keys.
	for(int i = 0; i < 10; i++) {
		remap_fn[(KEY_1 & 0xFF) + i] = (KEY_F1 & 0xFF) + i;
	}

	remap_fn[KEY_MINUS & 0xFF] = KEY_F11 & 0xFF;
	remap_fn[KEY_EQUAL & 0xFF] = KEY_F12 & 0xFF;

	remap_fn[KEY_A & 0xFF] = KEY_B & 0xFF;



	usb.begin();
	keyboard1.attachRawPress(onRawPress);
	keyboard1.attachRawRelease(onRawRelease);
}

void loop() {
	usb.Task();
}

void onRawPress(uint8_t keycode) {
	if(is_remapping && remap_fn[keycode])
		keycode = remap_fn[keycode];
	
	if(remap_normal[keycode])
		keycode = remap_normal[keycode];

	if (keycode == 106) {
		is_remapping = 1;
	} else if (keycode >= 103 && keycode < 111) {
    uint8_t keybit = 1 << (keycode - 103);
    keyboard_modifiers |= keybit;
    Keyboard.set_modifier(keyboard_modifiers);
		Keyboard.send_now();
  } else {
    /* if (keyboard1.getModifiers() != keyboard_modifiers) {
      keyboard_modifiers = keyboard1.getModifiers();
      Keyboard.set_modifier(keyboard_modifiers);
    } */
    Keyboard.press(0XF000 | keycode);
  }
}

void onRawRelease(uint8_t keycode) {
	if(is_remapping && remap_fn[keycode])
		keycode = remap_fn[keycode];
	
	if(remap_normal[keycode])
		keycode = remap_normal[keycode];

  if (keycode == 106) {
	  is_remapping = 0;
		for(int i = 0; i < 256; i++) {
			if(remap_fn[i])
				Keyboard.release(0xF000 | remap_fn[i]);
		}
	} else if (keycode >= 103 && keycode < 111) {
    uint8_t keybit = 1 << (keycode - 103);
    keyboard_modifiers &= ~keybit;
    Keyboard.set_modifier(keyboard_modifiers);
		Keyboard.send_now();
  } else {
    Keyboard.release(0XF000 | keycode);
  }
}
