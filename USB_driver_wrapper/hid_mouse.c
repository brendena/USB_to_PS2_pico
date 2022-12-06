#include "hid_mouse.h"


//typedef struct TU_ATTR_PACKED
//{
//  uint8_t buttons; /**< buttons mask for currently pressed buttons in the mouse. */
//  int8_t  x;       /**< Current delta x movement of the mouse. */
//  int8_t  y;       /**< Current delta y movement on the mouse. */
//  int8_t  wheel;   /**< Current delta wheel movement on the mouse. */
//  int8_t  pan;     // using AC Pan
//} hid_mouse_report_t;
//*/

unsigned char mouseChanged;
hid_mouse_report_t prev_report = {0};

unsigned char checkAndResetChagnedMouseUsb(){
    unsigned char tmp = mouseChanged;
    mouseChanged = 0;
    return tmp;
}
const hid_mouse_report_t  getUSBMouseState(){
    return prev_report;
}

void process_mouse_report(hid_mouse_report_t const * report)
{
    memcpy(&prev_report, report, sizeof(hid_mouse_report_t));
    mouseChanged++;
}