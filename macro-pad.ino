
#include "src/ILI9341_driver.h"
#include "src/tft_touch.h"
#include "src/sd_utils.h"
#include "src/macro.h"

#include "src/model.h"
#include "src/view.h"
#include "src/presenter.h"

// #define DEBUG

void setup()
{
#if defined(DEBUG) || defined(DEBUG_TOUCH) || defined(TOUCH_CALIBRATION_PROCESS)
    Serial.begin(115200);
    while(!Serial) {
        delay(10);
    }
#endif

    display::initialiseTFT();

    if (!sd::initialiseSD())
    {
        display::displayError("SD card error!");
        while(true) {}
    }

#ifdef TOUCH_CALIBRATION_PROCESS
    display::tft->setTextColor(ANTI_FLASH_WHITE);
    display::tft->setTextSize(3);
    display::tft->setTextWrap(true);
    display::tft->println("Touch screen to begin calibration process.");
    delay(2000);
#endif

macro::initialiseKeyboard();

    model::model_c *model = new model::model_c();
    view::view_c *view = new view::view_c();
    presenter::presenter_c *presenter = new presenter::presenter_c(model, view);

#if defined(TOUCH_CALIBRATION_PROCESS) || defined(DEBUG_TOUCH)
    for (;;)
    {
        TSPoint tp;
        (void) touched(tp);
    }
#else
    presenter->run();
#endif

    delete presenter;
    delete view;
    delete model;

    // If the pragram reaches this point, then somehting went wrong and we should disconnect
    macro::closeKeyboard();
    display::tft->setTextColor(ANTI_FLASH_WHITE);
    display::tft->fillScreen(RICH_BLACK);
    display::tft->setCursor(0, 0);
    display::tft->setTextSize(1);
    display::tft->setTextWrap(true);
    display::tft->println("UNKNOWN ERROR");
}


void loop()
{
    // Program should never reach this point
}