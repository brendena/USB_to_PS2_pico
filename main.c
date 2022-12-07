#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bsp/board.h"
#include "tusb.h"
#include "USB_driver_wrapper/hid_keyboard.h"
#include "USB_driver_wrapper/hid_mouse.h"
#include "atkPico/atkPico.h"
#include "PS2_keyboard_device.h"
#include "PS2_mouse_device.h"
#include "USB_to_ps2_arrays.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+
void led_blinking_task(void);

extern void hid_keyboard_app_task(void);
extern void hid_mouse_app_task(void);

/*------------- MAIN -------------*/
int main(void)
{
  board_init();

  printf("\n\n\n\n[USB to PS2]\r\n");
  ps2_keyboard_device_init(0,16);
  ps2_mouse_device_init(0,14);

  tusb_init();

  while (1)
  {
    // tinyusb host task
    tuh_task();
    led_blinking_task();
    hid_keyboard_app_task();
    hid_mouse_app_task();
  }
  //*/

  return 0;
}



//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

//--------------------------------------------------------------------+
// Blinking Task
//--------------------------------------------------------------------+

void led_blinking_task(void)
{
  const uint32_t interval_ms = 1000;
  static uint32_t start_ms = 0;

  static bool led_state = false;

  // Blink every interval ms
  if ( board_millis() - start_ms < interval_ms) return; // not enough time
  start_ms += interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}

void hid_mouse_app_task(void)
{
  if(checkAndResetChagnedMouseUsb())
  {
    /*convert usb into ps2*/
    hid_mouse_report_t mouseState = getUSBMouseState();
    PS2_mouse_data mousePs2Data = { 0 };
    mousePs2Data.btnAndSigns.keys.leftBtn =   (mouseState.buttons & MOUSE_BUTTON_LEFT )  != 0;
    mousePs2Data.btnAndSigns.keys.rightBtn =  (mouseState.buttons & MOUSE_BUTTON_RIGHT ) != 0;
    mousePs2Data.btnAndSigns.keys.middleBtn = (mouseState.buttons & MOUSE_BUTTON_MIDDLE) != 0;
    mousePs2Data.btnAndSigns.keys.xSignBit = (mouseState.x < 0) ;
    mousePs2Data.btnAndSigns.keys.ySignBit = (mouseState.y  > 0) ;
    mousePs2Data.btnAndSigns.keys.xOverflow = 0;
    mousePs2Data.btnAndSigns.keys.yOverflow = 0;
    
    mousePs2Data.x = mouseState.x; //remove sign
    mousePs2Data.y = ~mouseState.y; //remove sign

    mousePs2Data.zAndBtn.keys.btn4 = (mouseState.buttons & MOUSE_BUTTON_BACKWARD) != 0;
    mousePs2Data.zAndBtn.keys.btn5 = (mouseState.buttons & MOUSE_BUTTON_FORWARD) != 0;

    printf("[mouse] %d\n",mouseState.x);
    printf("mouse %x\n",mousePs2Data.btnAndSigns.keys.xSignBit);

    ps2_tx_mouse(&mousePs2Data);
  }
}

void hid_keyboard_app_task(void)
{
  static hid_keyboard_report_t prev_report = { 0, 0, {0} }; // previous report to check key released

  if(checkAndResetChagnedKeyboardUsb()){
    //probably should be using the 
    //changed property usbKeyboardState.changed
    const USB_KeyboardState keyboardState = getUSBKeyboardState();
    for(uint8_t i=0; i<6; i++)
    {
      if ( keyboardState.input.keycode[i] )
      {
        //Still there
        if ( find_key_in_report(&prev_report, keyboardState.input.keycode[i]) )
        {
        }
        //ADDED
        else
        {
          unsigned char ps2Key    = USB_to_ps2[keyboardState.input.keycode[i]];
          unsigned char ps2KeyExt = USB_to_ps2_ext[keyboardState.input.keycode[i]];
          printf("ps2Key %d\n",ps2Key);
          if(ps2Key != 0)
          {
              ps2_tx_blocking(ps2Key, ps2KeyExt, 0);
          }
        }
      }
      if(prev_report.keycode[i]){
        //REMOVED
        if ( find_key_in_report(&keyboardState.input, prev_report.keycode[i]) ==0)
        {
          printf("removed the key\n");
          unsigned char ps2Key    = USB_to_ps2[prev_report.keycode[i]];
          unsigned char ps2KeyExt = USB_to_ps2_ext[prev_report.keycode[i]];
          if(ps2Key != 0)
          {
              ps2_tx_blocking(ps2Key, ps2KeyExt, 1);
          }
        }
      }
    }
    //handle modifiers
    if(keyboardState.input.modifier != prev_report.modifier)
    {
      const usb_2_ps2_modifiers[8]     = {0x14,0x12,0x11,0x1f,0x14,0x59,0x11,0x27};
      const usb_2_ps2_ext_modifiers[8] = {0x0 , 0x0, 0x0, 0x1, 0x1, 0x0, 0x1,0x1 };  

      for(unsigned char i =0; i < 8; i++)
      {
        unsigned char pos = 1 << i;
        unsigned char input     = keyboardState.input.modifier & pos;
        unsigned char prevInput = prev_report.modifier & pos;
        //added
        if(input && !prevInput){
          ps2_tx_blocking(usb_2_ps2_modifiers[i], usb_2_ps2_ext_modifiers[i], 0);
        }
        //removed
        else if(prevInput && !input)
        {
          ps2_tx_blocking(usb_2_ps2_modifiers[i], usb_2_ps2_ext_modifiers[i], 1);
        }
      }
    }
    

    prev_report = keyboardState.input;
  }
}
