/*
*/


//// NOTES ON HOW THE VIEW SHOULD WORK ////
/*
* The view should be a class that is used to display the macro pad.
* Should manage setting callbacks for buttons
* Should decide HOW to present the data/options
* IS the UI
* Recieves calls from the presenter
* Button callbacks should be a function of the presenter
* Main loop lives in the view as this is where the input is managed
*/

#ifndef _VIEW_H_
#define _VIEW_H_

#include "ILI9341_driver.h"
#include "tft_touch.h"
#include "button.h"
#include "constants.h"

namespace view
{

using buttonCallback = void (*)(void*);

class view_c
{
public:
    /// @brief Constructor
    view_c()
    {
        this->m_test_button = gui::button_base_c(0, 0, DEFAULT_MENU_BUTTON_WIDTH, DEFAULT_MENU_BUTTON_HEIGHT, "# Macros");
        this->m_test_button.drawCallback(handleDrawButton, this);
    }

    /// @brief Set the callback for a given button
    /// @param callback 
    /// @param ctx 
    void setCallback(buttonCallback callback, void* ctx = nullptr)
    {
        this->m_test_button.callback(callback, ctx);
    }

    /// @brief Display a message on the screen
    /// @param message The message to display
    void displayMessage(char const *message)
    {
        display::tft->fillScreen(INDIGO_DYE);
        display::displayMessage(message);
    }

    /// @brief Render the test screen
    /// @details This is used for testing only
    void renderTestScreen()
    {
        display::tft->fillScreen(INDIGO_DYE);
        m_test_button.draw();
    }

    /// @brief Run the view
    /// @details This is the main loop to monitor for input
    void run()
    {
        for (;;)
        {
            TSPoint tp;

            if (touched(&tp))
            {
                m_test_button.press();
                delay(1000);
                renderTestScreen();
            }
        };
    }

    static void handleDrawButton(void *obj, void *button)
    {
        if (obj) static_cast<view_c*>(obj)->_drawButton(*static_cast<gui::button_base_c*>(button));
    }
    
private:
    gui::button_base_c m_test_button; //< test button
    // gui::button_base_c m_
    
    void _drawButton(gui::button_base_c const & button)
    {
        display::drawButton(button.minX(), button.minY(), button.width(), button.height(), button.fillColour(), button.textColour(), button.borderColour(), button.name());
    }
};

} // namespace view
#endif // _VIEW_H_
