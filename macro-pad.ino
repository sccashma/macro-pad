
#include "ILI9341_driver.h"
#include "tft_touch.h"
#include "sd_utils.h"

// #define DEBUG
void setup()
{
#if defined(DEBUG) || defined(DEBUG_TOUCH) || defined(TOUCH_CALIBRATION_PROCESS)
    Serial.begin(115200);
    while(!Serial) {
        delay(10);
    }
#endif

    initialiseTFT();

    if (!initialiseSD())
    {
        displayError("SD card error!");
        while(true) {}
    }

#ifdef TOUCH_CALIBRATION_PROCESS
    tft->setTextColor(SNOW);
    tft->setTextSize(3);
    tft->setTextWrap(true);
    tft->println("Touch screen to begin calibration process.");
    delay(2000);
#endif
}

#if defined(TOUCH_CALIBRATION_PROCESS) || defined(DEBUG_TOUCH)
void loop()
{
    TSPoint tp;
    (void) touched(tp);
}
#else
void loop()
{

}
#endif