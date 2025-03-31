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
int constexpr X_o = 111, X_n = 923;
int constexpr Y_o = 74, Y_n = 900;

///////////////////////////////////////

// (0,0) ------------------- (320,0) //
//                                   //
//                                   //
// (0,0) ----------------- (320,240) //

// x_pixel = map(p.y, Y_n, Y_o, 0, 320)
// y_pixel = map(p.x, X_n, X_o, 0, 240)

///////////////////////////////////////

// LANDSCAPE CALIBRATION     320 x 240
void map_point(TSPoint& p)
{   
    uint16_t const screen_width = tft->width();
    uint16_t const screen_height = tft->height();

    int16_t tmp_x = p.x;
#if defined(DEBUG_TOUCH) || defined(TOUCH_CALIBRATION_PROCESS)
    Serial.print("raw- X: ");
    Serial.print(p.x);
    Serial.print(" Y: ");
    Serial.println(p.y);
#endif
    p.x = constrain(map(p.y, Y_n, Y_o, 0, screen_width), 0, screen_width);
    p.y = constrain(map(tmp_x, X_n, X_o, 0, screen_height), 0, screen_height);
#ifdef DEBUG_TOUCH
    Serial.print("map- X: ");
    Serial.print(p.x);
    Serial.print(" Y: ");
    Serial.println(p.y);
#endif
}

#endif // __TOUCHSCREEN_CALIBRATION_H_