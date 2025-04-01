// Helper functions to utilize the touch screen

#ifndef __TFT_TOUCH_H__
#define __TFT_TOUCH_H__

#include <TouchScreen.h>
#include "touchscreen_calibration.h"

// Pin interface definitions
#define XP 8
#define XM A2
#define YP A3
#define YM 9

/// @brief Touch screen calibration values
int constexpr MINPRESSURE = 200;
int constexpr MAXPRESSURE = 1000;

/// @brief Debounce parameters
unsigned long last_pressed_ms = millis();
unsigned long constexpr DEBOUNCE_THRESHOLD_MS = 300;

/// @brief Global instance of the touch screen
/// @note For more precise calibration (should not be necessary), follow the instructions provided in the Adafruit Touchscreen library
/// @note The touchscreen calibration process is quite involved and requires a multimeter.
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

/// @brief Debounce the button press
/// @return bool: True if the button was pressed, false otherwise
bool debounce()
{
    bool pressed = false;
    // software debounce
    unsigned long pressed_ms = millis();
    
    if ((pressed_ms - last_pressed_ms) > DEBOUNCE_THRESHOLD_MS)
    {      
            last_pressed_ms = pressed_ms;
            pressed = true;
    }
    
    return pressed;
}

/// @brief Checks if the screen was touched and updates the TouchPoint
/// @param tp If the screen was pressed, the x and y coordinates are updated to closely match the pixels
/// @return True if the screen was pressed, false otherwise
bool touched(TSPoint& tp)
{
    tp = ts.getPoint();  //tp.x, tp.y are raw ADC values

    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);

    if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE && debounce())
    {
#ifdef TOUCH_CALIBRATION_PROCESS
        calibrateTouchscreen(tp);
#else
        mapPoint(tp);
#endif
        return true;
    }

    return false;
}

#endif // __TFT_TOUCH_H__