// This is a very light interface to the Arduino GFX library. The only board specifically
// targeted for support is the Duinotech 2.8 inch TFT shield.

#ifndef __ILI9341_DRIVER_H__
#define __ILI9341_DRIVER_H__

#include <Arduino_GFX_Library.h>

// Pin interface definitions
#define TFT_RST A4

// 16-bit colour definitions
// These use the define preprocessor directive due to limitations in the GFX library
#define BLACK 0x0000

// Orientation of the display
uint8_t constexpr ORIENTATION = 3;

/// @brief Global scope instances of the tft data bus
Arduino_DataBus *bus = new Arduino_UNOPAR8();

/// @brief Global scope instances of the tft display
Arduino_GFX *tft = new Arduino_ILI9341(bus, TFT_RST, ORIENTATION, false /* ips */);

/// @brief Initialise the TFT display
void initialise_tft()
{
    tft->begin();
    tft->setCursor(0, 0);
    tft->fillScreen(BLACK);
    tft->println("Hello World!");
}

#endif // __ILI9341_DRIVER_H__