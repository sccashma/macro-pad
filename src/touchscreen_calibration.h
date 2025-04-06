// Touchscreen calibration for ILI9341 TFT display
// This file contains the calibration values for the touch screen and the function to map the raw touch values to screen coordinates
// The calibration values are determined by touching the four corners of the screen and entering the values in the X_o, X_n, Y_o and Y_n variables below

#include <TouchScreen.h>

#ifndef __TOUCHSCREEN_CALIBRATION_H__
#define __TOUCHSCREEN_CALIBRATION_H__

#include "ILI9341_driver.h"

/* Each device will need to calibrate its own touch screen, to do this, simply define TOUCH_CALIBRATION_PROCESS below.
 * Then using a stylus, touch the four corners of the screen. The values will be printed to the serial monitor.
 * Enter those values in the X_o, X_n, Y_o and Y_n variables below.
*/
// #define TOUCH_CALIBRATION_PROCESS

// To debug the touch screen, define DEBUG_TOUCH
// #define DEBUG_TOUCH

/// EXAMPLE TOUCHSCREEN CALIBRATION ///
///////////////////////////////////////
// ------------- X_n --------------- //
// |                               | //
// |                               | //
// Y_n                           Y_o //
// |                               | //
// |                               | //
// ------------- X_o --------------- //

// X_o and Y_o are minimum values from the touch screen
// X_n and Y_n are maximum values from the touch screen

// measured values for the screeen
int constexpr X_o = 151, X_n = 891;
int constexpr Y_o = 111, Y_n = 893;

///////////////////////////////////////

// (0,0) ------------------- (320,0) //
//                                   //
//                                   //
// (0,0) ----------------- (320,240) //

// x_pixel = map(p.y, Y_n, Y_o, 0, 320)
// y_pixel = map(p.x, X_n, X_o, 0, 240)

///////////////////////////////////////

// LANDSCAPE CALIBRATION     320 x 240

#ifdef TOUCH_CALIBRATION_PROCESS
/// @brief Calibration process for the touch screen
/// @param p The touch screen point to calibrate
inline void calibrateTouchscreen(TSPoint& p)
{
    uint16_t const screen_width = display::tft_c::instance().width() - 1;
    uint16_t const screen_height = display::tft_c::instance().height() - 1;

    uint16_t const text_y_offset = screen_height / 3;

    display::tft_c::instance().drawRect(0, 0, 2, 2, ANTI_FLASH_WHITE); // Draw a pixel at the top left corner of the screen
    display::tft_c::instance().drawRect(screen_width - 2, 0, 2, 2, ANTI_FLASH_WHITE); // Draw a pixel at the top right corner of the screen
    display::tft_c::instance().drawRect(0, screen_height - 2, 2, 2, ANTI_FLASH_WHITE); // Draw a pixel at the bottom left corner of the screen
    display::tft_c::instance().drawRect(screen_width - 2, screen_height - 2, 2, 2, ANTI_FLASH_WHITE); // Draw a pixel at the bottom right corner of the screen
    display::tft_c::instance().setTextColor(ANTI_FLASH_WHITE);
    display::tft_c::instance().setTextSize(3);
    display::tft_c::instance().setTextWrap(true);
    display::tft_c::instance().setCursor(0, text_y_offset); // Center the text vertically
    display::tft_c::instance().println("Calibration mode");
    display::tft_c::instance().println("Touch corners and read values from serial monitor");

    // Print the raw values to the serial monitor for calibration
    Serial.print("X: ");
    Serial.print(p.x);
    Serial.print(" Y: ");
    Serial.println(p.y);
}
#endif

/// @brief Map the raw touch screen values to the screen coordinates
/// @param p The touch screen point to map
inline void mapPoint(TSPoint *p)
{   
#ifdef DEBUG_TOUCH
    uint16_t raw_x = p->x;
    uint16_t raw_y = p->y;
#endif

    uint16_t const screen_width = display::tft_c::instance().width() - 1;
    uint16_t const screen_height = display::tft_c::instance().height() - 1;
    int16_t tmp_x = p->x;
    p->x = constrain(map(p->y, Y_n, Y_o, 0, screen_width), 0, screen_width);
    p->y = constrain(map(tmp_x, X_n, X_o, 0, screen_height), 0, screen_height);

#ifdef DEBUG_TOUCH
    uint16_t const text_y_offset1 = (screen_height / 2) - (textPixelHeight(3) + 2);
    uint16_t const text_y_offset2 = (screen_height / 2) + 2;

    display::tft_c::instance().fillScreen(RICH_BLACK); // Clear the screen
    display::tft_c::instance().setTextColor(ANTI_FLASH_WHITE);
    display::tft_c::instance().setTextSize(3);
    display::tft_c::instance().setTextWrap(true);

    display::tft_c::instance().setCursor(10, text_y_offset1);
    display::tft_c::instance().print("X: ");
    display::tft_c::instance().print(raw_x);
    display::tft_c::instance().print(" -> ");
    display::tft_c::instance().print(p->x);
    
    display::tft_c::instance().setCursor(10, text_y_offset2);
    display::tft_c::instance().print("Y: ");
    display::tft_c::instance().print(raw_y);
    display::tft_c::instance().print(" -> ");
    display::tft_c::instance().print(p->y);
#endif
}

#endif // __TOUCHSCREEN_CALIBRATION_H__