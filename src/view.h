/*
    view.h
    Author: Scott Cashman
    Date: 3/4/2025
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

#include "button.h"
#include "constants.h"
#include "ILI9341_driver.h"
#include "macro_button.h"
#include "tft_touch.h"
#include "wireframe.h"

namespace view
{

enum class view_state_t
{
    NONE = 0,
    LOADING,
    HOME,
    MAIN_MENU,
    MACRO_SELECT,
    MACRO_PLACE,
    ERROR
};

} // namespace view
using buttonCallback = void (*)(void*);

class view_c
{
private:
    view_state_t m_state;
    gui::button_base_c m_test_button; //< test button (for testing only)
    gui::macro_button_c* m_active_macros[MACRO_PLACE_OPTIONS];

public:
    /// @brief Constructor
    view_c()
    : m_state(view_state_t::NONE)
    {
        this->m_test_button = gui::button_base_c(0, 0, DEFAULT_MENU_BUTTON_WIDTH, DEFAULT_MENU_BUTTON_HEIGHT, "# Macros");
        this->m_test_button.drawCallback(handleDrawButton, this);

        for (size_t i = 0; i < MACRO_PLACE_OPTIONS; i++)
        {
            m_active_macros[i] = nullptr;
        }
    }

    /// @brief Destructor
    ~view_c()
    {
        _deleteActiveMacros();
    }

    /// @brief Clear the screen
    /// @details This is used to clear the screen before drawing a new screen
    void clearScreen()
    {
        // display::tft->fillScreen(INDIGO_DYE);
        display::tft->fillScreen(0x0000);
    }

    ///////////////////// Main Window Rendering /////////////////////

    /// @brief Display the loading screen
    void loadScreen()
    {
        m_state = view_state_t::LOADING;
        display::tft->fillScreen(INDIGO_DYE);
        display::drawBmp("/bckgrnd.bmp", 0, 0, display::tft->width(), display::tft->height());
    }

    void homeScreen()
    {
        m_state = view_state_t::HOME;
        display::drawBmp("/bckgrnd.bmp", 0, 0, display::tft->width(), display::tft->height());

        for (int i = 0; i < sizeof(m_active_macros) / sizeof(gui::macro_button_c*); i++)
        {
            if (m_active_macros[i] == nullptr) continue;
            m_active_macros[i]->draw();
        }
    }
    //////////////////// ~Main Window Rendering /////////////////////

    /// @brief Set the callback for a given button
    /// @param callback 
    /// @param ctx 
    /// @note This is used for testing only
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
        // m_test_button.draw();
        m_active_macros[0]->draw();
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
                // m_test_button.press();
                // delay(1000);
                // renderTestScreen();
            }
        };
    }

    ///////////////////// Managing button creations /////////////////////
public:
    /// @brief Create a set of macro buttons for the home screen
    /// @param num_macros The number of macros being passed to the function
    /// @param macros The macros to create buttons for
    /// @param names The names of the macros
    /// @param file_paths The file paths of the images to display on the buttons
    /// @note This function will create buttons for the macros in the first available slots
    void createHomeScreenMacroButtons(size_t const num_macros, macro::macro_c const *macros, String const *names, String const *file_paths)
    {
        for (size_t i = 0; i < num_macros; i++)
        {
            _createHomeScreenMacroButton(&macros[i], &names[i], &file_paths[i]);
        }
    }

private:
    /// @brief Create a macro button in the first available slot
    /// @param macro The macro to send on button press
    /// @param name The names of the macro
    /// @param file_path The file path of the macro's bmp
    void _createHomeScreenMacroButton(macro::macro_c const *macro, String const *name, String const *file_path)
    {
        gui::wf_home_screen_t wf;

        // Find the first available slot
        for (size_t i = 0; i < sizeof(m_active_macros) / sizeof(gui::macro_button_c*); i++)
        {
            if (m_active_macros[i] != nullptr) continue;

            m_active_macros[i] = new gui::macro_button_c(*macro, *name, *file_path);

            m_active_macros[i]->setPos(wf.macro_buttons[i].x, wf.macro_buttons[i].y);
            m_active_macros[i]->width(wf.macro_buttons[i].width);
            m_active_macros[i]->height(wf.macro_buttons[i].height);

            m_active_macros[i]->drawCallback(handleDrawBmpButton, this);
            break; // no need to continue
        }
    }

    /// @brief Delete the active macros
    /// @details This is used to delete all active macros when they are no longer needed
    void _deleteActiveMacros()
    {
        for (size_t i = 0; i < sizeof(m_active_macros) / sizeof(gui::macro_button_c*); i++)
        {
            if (m_active_macros[i] != nullptr)
            {
                delete m_active_macros[i];
                m_active_macros[i] = nullptr;
            }
        }
    }
    //////////////////// ~Managing button creations /////////////////////

    ///////////////////// BUTTON CALLBACK HANDLERS /////////////////////
public:
    /// @brief Handler for drawing standard buttons
    static void handleDrawButton(void *obj, void *button)
    {
        if (obj) static_cast<view_c*>(obj)->_drawButton(*static_cast<gui::button_base_c*>(button));
    }

    /// @brief Handler for drawing bmp buttons
    static void handleDrawBmpButton(void *obj, void *button)
    {
        if (obj) static_cast<view_c*>(obj)->_drawButtonBmp(*static_cast<gui::button_base_c*>(button));
    }
private:
    /// @brief Draw a button
    /// @param button The button to draw
    void _drawButton(gui::button_base_c const & button)
    {
        display::drawButton(button.minX(), button.minY(), button.width(), button.height(), button.fillColour(), button.textColour(), button.borderColour(), button.name());
    }

    /// @brief Draw a bmp button
    /// @param button The button to draw
    void _drawButtonBmp(gui::button_base_c const & button)
    {
        display::drawBmp("/icons/" + button.imageFilePath(), button.minX(), button.minY(), button.width(), button.height(), true);
    }
    //////////////////// ~BUTTON CALLBACK HANDLERS /////////////////////
};

} // namespace view
#endif // _VIEW_H_
