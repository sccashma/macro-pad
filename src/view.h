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

/// @brief Helper macro to get the number of macro button pointers in an array
/// @param x gui::macro_button_c*: The array
/// @return The number of macro buttons in the array
#define MACRO_BTN_COUNT(x) sizeof(m_active_macros) / sizeof(gui::macro_button_c*)

/// @brief Helper macro to get the number of menu button pointers in an array
/// @param x gui::button_base_c*: The array
/// @return The number of menu buttons in the array
#define MENU_BTN_COUNT(x) sizeof(m_menu_buttons) / sizeof(gui::button_base_c*)

typedef enum view_state_t
{
    NONE = 0,
    LOADING,
    HOME,
    MAIN_MENU,
    MACRO_SELECT,
    MACRO_PLACE,
    ERROR
} view_state_t;

using buttonCallback = void (*)(void*);

class view_c
{
private:
    view_state_t m_state;
    view_state_t m_prev_state;
    gui::button_base_c m_test_button; //< test button (for testing only)
    gui::macro_button_c *m_active_macros[MACRO_PLACE_OPTIONS];
    
    /// @brief Menu buttons and their indexes
    static size_t constexpr home_settings = 0;
    static size_t constexpr main_menu_load = 1;
    static size_t constexpr main_menu_back = 2;
    static size_t constexpr macro_select_left = 3;
    static size_t constexpr macro_select_right = 4;
    static size_t constexpr macro_select_done_place = 5;
    gui::button_base_c *m_menu_buttons[6];

public:
    /// @brief Constructor
    view_c()
    : m_state(view_state_t::NONE)
    , m_prev_state(view_state_t::NONE)
    {
        this->m_test_button = gui::button_base_c(
            0, 0, DEFAULT_MENU_BUTTON_WIDTH, DEFAULT_MENU_BUTTON_HEIGHT, "# Macros");
        this->m_test_button.drawCallback(handleDrawButton, this);

        for (size_t i = 0; i < MACRO_BTN_COUNT(m_active_macros); i++)
        {
            m_active_macros[i] = nullptr;
        }
        
        for (size_t i = 0; i < MENU_BTN_COUNT(m_menu_buttons); i++)
        {
            m_menu_buttons[i] = nullptr;
        }
    }

    /// @brief Destructor
    ~view_c()
    {
        _deleteActiveMacros();
        _deleteMenuButtons();
    }

    /// @brief Clear the screen
    /// @details This is used to clear the screen before drawing a new screen
    void clearScreen()
    {
        // display::tft->fillScreen(INDIGO_DYE);
        display::tft->fillScreen(0x0000);
    }

    /// @brief Display a message on the screen
    /// @param message The message to display
    void displayMessage(char const *message)
    {
        display::tft->fillScreen(INDIGO_DYE);
        display::displayMessage(message);
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
                _handleTouch(tp);
            }
        };
    }

    ///////////////////// Main Window Rendering /////////////////////
public:
    /// @brief Display the loading screen
    void loadScreen()
    {
        m_state = view_state_t::LOADING;
        display::tft->fillScreen(INDIGO_DYE);
        display::drawBmp("/bckgrnd.bmp", 0, 0, display::tft->width(), display::tft->height());
        display::drawTextInCanvas(0
            , display::tft->height() / 2
            , display::tft->width()
            , display::tft->height() / 2
            , "Please wait Democratically"
            , ARYLIDE_YELLOW
            , 3);
        m_prev_state = m_state;
    }

    /// @brief Display the home screen
    /// @details This is the main screen that displays the macros
    void homeScreen()
    {
        m_state = view_state_t::HOME;
        if (m_prev_state == view_state_t::LOADING)
        {
            // Clear only the text section
            display::drawBmp("/bckgrnd.bmp"
                , 0
                , 0
                , display::tft->width()
                , display::tft->height()
                , false
                , display::tft->height() / 2);
        }
        else if (m_prev_state == view_state_t::MAIN_MENU)
        {
            // Clear only up to the top of the menu buttons
            display::drawBmp("/bckgrnd.bmp"
                , 0
                , 0
                , display::tft->width()
                , display::tft->height()
                , false
                , display::tft->height() - DEFAULT_MENU_BUTTON_HEIGHT);
        }
        else
        {
            display::drawBmp("/bckgrnd.bmp", 0, 0, display::tft->width(), display::tft->height());
        }
        
        _deleteMenuButtons();

        for (int i = 0; i < MACRO_BTN_COUNT(m_active_macros); i++)
        {
            if (m_active_macros[i] == nullptr) continue;
            m_active_macros[i]->draw();
        }
        gui::wf_home_screen_t wf;

        m_menu_buttons[home_settings] = new gui::button_base_c(wf.menu_button.x
            , wf.menu_button.y
            , DEFAULT_MACRO_BUTTON_WIDTH
            , DEFAULT_MACRO_BUTTON_HEIGHT
            , "Settings");
        m_menu_buttons[home_settings]->imageFilePath("menu.bmp");
        m_menu_buttons[home_settings]->drawCallback(handleDrawBmpButton, this);
        m_menu_buttons[home_settings]->callback(handleMainMenu, this);
        m_menu_buttons[home_settings]->draw();
        m_prev_state = m_state;
    }

    /// @brief Display the main menu
    /// @details This is the main menu that allows the user to select options
    void mainMenu()
    {
        m_state = view_state_t::MAIN_MENU;
        display::drawBmp("/bckgrnd.bmp", 0, 0, display::tft->width(), display::tft->height());
        _deleteMenuButtons();
        
        gui::wf_main_menu_t wf;
        m_menu_buttons[main_menu_load] = new gui::button_base_c(wf.load_button.x
            , wf.load_button.y
            , DEFAULT_MENU_BUTTON_WIDTH
            , DEFAULT_MENU_BUTTON_HEIGHT
            , "Load");
        m_menu_buttons[main_menu_back] = new gui::button_base_c(wf.back_button.x
            , wf.back_button.y
            , DEFAULT_MENU_BUTTON_WIDTH
            , DEFAULT_MENU_BUTTON_HEIGHT
            , "Back");
        
        m_menu_buttons[main_menu_load]->drawCallback(handleDrawButton, this);
        m_menu_buttons[main_menu_load]->callback(handleMacroSelect, this);
        m_menu_buttons[main_menu_load]->draw();

        m_menu_buttons[main_menu_back]->drawCallback(handleDrawButton, this);
        m_menu_buttons[main_menu_back]->callback(handleHomeScreen, this);
        m_menu_buttons[main_menu_back]->draw();
        m_prev_state = m_state;
    }

    /// @brief Display the macro select screen
    /// @details This screen allows the user to select a macro to place on the home screen
    void macroSelect()
    {
        m_state = view_state_t::MACRO_SELECT;
        if (m_prev_state == view_state_t::MAIN_MENU)
        {
            // Clear only up to the top of the menu buttons
            display::drawBmp("/bckgrnd.bmp"
                , 0
                , 0
                , display::tft->width()
                , display::tft->height()
                , false
                , display::tft->height() - DEFAULT_MENU_BUTTON_HEIGHT);
        }
        else
        {
            display::drawBmp("/bckgrnd.bmp", 0, 0, display::tft->width(), display::tft->height());
        }
        _deleteMenuButtons();

        gui::wf_macro_select_t wf;

        // scroll left button
        m_menu_buttons[macro_select_left] = new gui::button_base_c(wf.scroll_left_button.x
            , wf.scroll_left_button.y
            , wf.scroll_left_button.width
            , wf.scroll_left_button.height
            , "<");
        m_menu_buttons[macro_select_left]->active(false);
        m_menu_buttons[macro_select_left]->drawCallback(handleDrawButton, this);
        /// @todo Add callback to scroll left
        m_menu_buttons[macro_select_left]->draw();

        // scroll right button
        m_menu_buttons[macro_select_right] = new gui::button_base_c(wf.scroll_right_button.x
            , wf.scroll_right_button.y
            , wf.scroll_right_button.width
            , wf.scroll_right_button.height
            , ">");
        m_menu_buttons[macro_select_right]->drawCallback(handleDrawButton, this);
        /// @todo Add callback to scroll right
        m_menu_buttons[macro_select_right]->draw();

        // done button
        m_menu_buttons[macro_select_done_place] = new gui::button_base_c(wf.confirm_button.x
            , wf.confirm_button.y
            , wf.confirm_button.width
            , wf.confirm_button.height
            , "Done");
        m_menu_buttons[macro_select_done_place]->drawCallback(handleDrawButton, this);
        m_menu_buttons[macro_select_done_place]->callback(handleHomeScreen, this);
        m_menu_buttons[macro_select_done_place]->draw();

        m_prev_state = m_state;
    }
    //////////////////// ~Main Window Rendering /////////////////////

    ///////////////////// Managing button creations /////////////////////
public:
    /// @brief Create a set of macro buttons for the home screen
    /// @param num_macros The number of macros being passed to the function
    /// @param macros The macros to create buttons for
    /// @param names The names of the macros
    /// @param file_paths The file paths of the images to display on the buttons
    /// @note This function will create buttons for the macros in the first available slots
    void createHomeScreenMacroButtons(
        size_t const num_macros, macro::macro_c const *macros, String const *names, String const *file_paths)
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
        for (size_t i = 0; i < MACRO_BTN_COUNT(m_active_macros); i++)
        {
            if (m_active_macros[i] != nullptr) continue;

            m_active_macros[i] = new gui::macro_button_c(*macro, *name, *file_path);
            m_active_macros[i]->setPos(wf.macro_buttons[i].x, wf.macro_buttons[i].y);
            m_active_macros[i]->width(wf.macro_buttons[i].width);
            m_active_macros[i]->height(wf.macro_buttons[i].height);
            m_active_macros[i]->drawCallback(handleDrawBmpButton, this);
            break;
        }
    }

    /// @brief Delete the active macros
    /// @details This is used to delete all active macros when they are no longer needed
    void _deleteActiveMacros()
    {
        for (size_t i = 0; i < MACRO_BTN_COUNT(m_active_macros); i++)
        {
            if (m_active_macros[i] != nullptr)
            {
                delete m_active_macros[i];
                m_active_macros[i] = nullptr;
            }
        }
    }

    /// @brief Delete the menu buttons
    /// @details This is used to delete all menu buttons when they are no longer needed
    void _deleteMenuButtons()
    {
        for (size_t i = 0; i < MENU_BTN_COUNT(m_menu_buttons); i++)
        {
            if (m_menu_buttons[i] != nullptr)
            {
                delete m_menu_buttons[i];
                m_menu_buttons[i] = nullptr;
            }
        }
    }
    //////////////////// ~Managing button creations /////////////////////

    ///////////////////// BUTTON CALLBACK HANDLERS /////////////////////
public:
    /// @brief Handler for the main menu button
    static void handleMainMenu(void *obj)
    {
        if (obj) static_cast<view_c*>(obj)->mainMenu();
    }

    /// @brief Handler for the home screen button
    static void handleHomeScreen(void *obj)
    {
        if (obj) static_cast<view_c*>(obj)->homeScreen();
    }

    /// @brief Handler for the macro select button
    static void handleMacroSelect(void *obj)
    {
        if (obj) static_cast<view_c*>(obj)->macroSelect();
    }

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
        display::drawButton(button.minX()
        , button.minY()
        , button.width()
        , button.height()
        , button.fillColour()
        , button.textColour()
        , button.borderColour()
        , button.name());
    }

    /// @brief Draw a bmp button
    /// @param button The button to draw
    void _drawButtonBmp(gui::button_base_c const & button)
    {
        display::drawBmp(
            "/icons/" + button.imageFilePath(), button.minX(), button.minY(), button.width(), button.height(), true);
    }
    //////////////////// ~BUTTON CALLBACK HANDLERS /////////////////////

    ///////////////////// TOUCH HANDLERS /////////////////////
private:
    /// @brief Handle touch input and determine which button was pressed
    /// @param tp The touch point
    void _handleTouch(TSPoint const &tp)
    {
        switch (m_state)
        {
        case view_state_t::HOME:
            _homeScreenTouchHandler(tp);
            break;
        case view_state_t::MAIN_MENU:
            _menuTouchHandler(tp);
            break;
        case view_state_t::MACRO_SELECT:
            _menuTouchHandler(tp);
            break;
        // Add cases for other wireframes as needed
        default:
            break;
        }
    }

    /// @brief Handle touch input for the home screen
    /// @param tp The touch point
    /// @details This function checks if the touch point intersects with any of the active macro buttons
    void _homeScreenTouchHandler(TSPoint const &tp)
    {
        // Check if the touch point intersects with any active macro buttons
        for (size_t i = 0; i < MACRO_BTN_COUNT(m_active_macros); i++)
        {
            if (m_active_macros[i] == nullptr) continue;

            if (_isPointInsideButton(tp, m_active_macros[i]))
            {
                if (m_active_macros[i]->press()) return;
            }
        }

        // Check if the touch point intersects with the settings menu button
        if (_isPointInsideButton(tp, m_menu_buttons[home_settings]))
        {
            m_menu_buttons[home_settings]->press();
        }
    }

    /// @brief Handle touch input for the menu screen
    /// @param tp The touch point
    /// @details This function checks if the touch point intersects with any of the menu buttons
    void _menuTouchHandler(TSPoint const &tp)
    {
        for (size_t i = 0; i < MENU_BTN_COUNT(m_menu_buttons); i++)
        {
            if (m_menu_buttons[i] == nullptr) continue;

            if (_isPointInsideButton(tp, m_menu_buttons[i]))
            {
                if (m_menu_buttons[i]->press()) return;
            }
        }
    }

    /// @brief Check if a touch point is inside a button
    /// @param tp The touch point
    /// @param button The button to check
    /// @return True if the touch point is inside the button, false otherwise
    bool _isPointInsideButton(TSPoint const &tp, gui::button_base_c const *button)
    {
        return (tp.x >= button->minX() && tp.x <= button->maxX() &&
                tp.y >= button->minY() && tp.y <= button->maxY());
    }
    //////////////////// ~TOUCH HANDLERS ///////////////////// 

    ////////////// TESTING FUNCTIONS /////////////////////
public:
    /// @brief Set the callback for a given button
    /// @param callback 
    /// @param ctx 
    /// @note This is used for testing only
    void setCallback(buttonCallback callback, void* ctx = nullptr)
    {
        this->m_test_button.callback(callback, ctx);
    }

    /// @brief Render the test screen
    /// @details This is used for testing only
    void renderTestScreen()
    {
        display::tft->fillScreen(INDIGO_DYE);
        // m_test_button.draw();
        m_active_macros[0]->draw();
    }
    ///////////////// ~TESTING FUNCTIONS /////////////////////
};

} // namespace view
#endif // _VIEW_H_
