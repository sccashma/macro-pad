
#include "ILI9341_driver.h"
#include "tft_touch.h"

void setup()
{
#if defined(DEBUG_TOUCH) || defined(TOUCH_CALIBRATION_PROCESS)
    Serial.begin(115200);
    while(!Serial) {
        delay(10);
    }
#endif
    initialise_tft();
}

void loop()
{

}