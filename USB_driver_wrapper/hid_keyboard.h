#pragma once

#include "bsp/board.h"
#include "tusb.h"

/*
modifiing keys that hold there state
*/
typedef struct LockingKeys{
  unsigned char numLock : 1;
  unsigned char capsLock : 1;
  unsigned char scrollLock : 1;
  unsigned char compose : 1;
  unsigned char kana : 1;
} LockingKeys;

typedef struct USB_KeyboardState
{
  hid_keyboard_report_t input;
  LockingKeys lockedMod;
  unsigned char changed;
} USB_KeyboardState;


//externally
unsigned char checkAndResetChagnedKeyboardUsb();
const USB_KeyboardState  getUSBKeyboardState();
bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode);


extern uint8_t const keycode2ascii[128][2];