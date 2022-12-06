#pragma once

#include "bsp/board.h"
#include "tusb.h"


unsigned char checkAndResetChagnedMouseUsb();
const hid_mouse_report_t  getUSBMouseState();
void process_mouse_report(hid_mouse_report_t const * report);