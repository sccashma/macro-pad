
#include "src/ILI9341_driver.h"
#include "src/tft_touch.h"
#include "src/sd_utils.h"

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

    if (!sd::initialiseSD())
    {
        displayError("SD card error!");
        while(true) {}
    }

#ifdef TOUCH_CALIBRATION_PROCESS
    tft->setTextColor(ANTI_FLASH_WHITE);
    tft->setTextSize(3);
    tft->setTextWrap(true);
    tft->println("Touch screen to begin calibration process.");
    delay(2000);
#endif

    macro::initialiseKeyboard();

#if defined(TOUCH_CALIBRATION_PROCESS) || defined(DEBUG_TOUCH)
    for (;;)
    {
        TSPoint tp;
        (void) touched(tp);
    }
#else
    for (;;)
    {

    }
#endif

    // If the pragram reaches this point, then somehting went wrong and we should disconnect
    macro::closeKeyboard();
    tft->setTextColor(ANTI_FLASH_WHITE);
    tft->fillScreen(RICH_BLACK);
    tft->setCursor(0, 0);
    tft->setTextSize(1);
    tft->setTextWrap(true);
    tft->println("UNKNOWN ERROR");
}


void loop()
{
    // Program should never reach this point
}