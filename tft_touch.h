// Helper functions to utilize the touch screen

#ifndef __TFT_TOUCH_H__
#define __TFT_TOUCH_H__

#include <TouchScreen.h>
#include "touchscreen_calibration.h"

// Touch screen calibration values
int constexpr MINPRESSURE = 200;
int constexpr MAXPRESSURE = 1000;

// Pin interface definitions
#define XP 8
#define XM A2
#define YP A3
#define YM 9

// Global instance of the touch screen
// For more precise calibration (should not be necessary), follow the instructions provided in the
//   Adafruit Touchscreen library
//   Note: The touchscreen calibration process is quite involved and requires a multimeter.
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

/// @brief Checks if the screen was touched and updates the TouchPoint
/// @param tp If the screen was pressed, the x and y coordinates are updated to closely match the pixels
/// @return True if the screen was pressed, false otherwise
/// TODO: Add debounce to the touch screen
bool touched(TSPoint& tp)
{
    tp = ts.getPoint();  //tp.x, tp.y are raw ADC values

    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE)
    {
      map_point(tp);
      return true;
    }

    return false;
}

#endif // __TFT_TOUCH_H__