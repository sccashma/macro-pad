// This is a very light interface to the Arduino GFX library. The only board specifically
// targeted for support is the Duinotech 2.8 inch TFT shield.

#ifndef __ILI9341_DRIVER_H__
#define __ILI9341_DRIVER_H__


#include <Arduino_GFX_Library.h>

#define TFT_CS 9
#define TFT_DC 8
#define TFT_RST A4
#define GFX_BL 6

uint8_t Orientation = 3;    //LANDSCAPE

// 16-bit colour definitions
#define BLACK 0x0000


// Global scope instances of the tft
Arduino_DataBus *bus = new Arduino_UNOPAR8();
Arduino_GFX *tft = new Arduino_ILI9341(bus, TFT_RST, Orientation, false /* ips */);


void initialise_tft()
{
    tft->begin();
    tft->setRotation(Orientation);
    tft->setCursor(0, 0);
    tft->fillScreen(BLACK);
}

#endif // __ILI9341_DRIVER_H__