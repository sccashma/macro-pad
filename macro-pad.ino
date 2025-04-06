
// #define DEBUG

#include "src/ILI9341_driver.h"
#include "src/tft_touch.h"
#include "src/sd_utils.h"
#include "src/macro.h"

#include "src/model.h"
#include "src/view.h"
#include "src/presenter.h"


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

    macro::initialiseKeyboard();

#ifdef TOUCH_CALIBRATION_PROCESS
    display::tft_c::instance().setTextColor(ANTI_FLASH_WHITE);
    display::tft_c::instance().setTextSize(3);
    display::tft_c::instance().setTextWrap(true);
    display::tft_c::instance().println("Touch screen to begin calibration process.");
    delay(2000);
#endif

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
    display::tft_c::instance().setTextColor(ANTI_FLASH_WHITE);
    display::tft_c::instance().fillScreen(RICH_BLACK);
    display::tft_c::instance().setCursor(0, 0);
    display::tft_c::instance().setTextSize(1);
    display::tft_c::instance().setTextWrap(true);
    display::tft_c::instance().println("UNKNOWN ERROR");
}


void loop()
{
    // Program should never reach this point
}