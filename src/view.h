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

#include <Arduino.h>
#include <EEPROM.h>
#include "limits.h"

#include "presenter_abstract.h"
#include "view_abstract.h"
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
#define MACRO_BTN_COUNT(x) sizeof(x) / sizeof(gui::macro_button_c*)

/// @brief Helper macro to get the number of menu button pointers in an array
/// @param x gui::button_base_c*: The array
/// @return The number of menu buttons in the array
#define BTN_COUNT(x) sizeof(x) / sizeof(gui::button_base_c*)

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
using drawButtonCallback = void (*)(void*, void*);

class view_c : public view_abstract_c
{
private:
    view_state_t m_state;
    view_state_t m_prev_state;
    uint16_t m_current_selected_id;
    uint8_t m_current_selected_placement;
    bool m_update_macros;
    int m_scroll;
    
    /// @brief Buttons and their indexes
    static size_t constexpr home_settings = 0;
    static size_t constexpr main_menu_load = 1;
    static size_t constexpr main_menu_back = 2;
    static size_t constexpr macro_select_left = 3;
    static size_t constexpr macro_select_right = 4;
    static size_t constexpr macro_select_done_place = 5;
    gui::button_base_c *m_menu_buttons[6];
    gui::button_base_c *m_macro_select_options[MACRO_SELECT_OPTIONS];
    gui::button_base_c *m_macro_placement_options[MACRO_PLACE_OPTIONS];
    gui::macro_button_c *m_active_macros[MACRO_PLACE_OPTIONS];

    /// Store the current macro's
    uint16_t m_active_macros_list[MACRO_PLACE_OPTIONS];

public:
    /// @brief Constructor
    view_c();

    /// @brief Destructor
    ~view_c();

    /// @brief Clear the screen
    /// @details This is used to clear the screen before drawing a new screen
    void clearScreen();

    /// @brief Display a message on the screen
    /// @param message The message to display
    void displayMessage(char const *message);

    /// @brief Run the view
    /// @details This is the main loop to monitor for input
    void run();

    ///////////////////// Main Window Rendering /////////////////////
public:
    /// @brief Display the loading screen
    void loadScreen();

    /// @brief Display the home screen
    /// @details This is the main screen that displays the macros
    void homeScreen();
 
    /// @brief Display the main menu
    /// @details This is the main menu that allows the user to select options
    void mainMenu();

    /// @brief Display the macro select screen
    /// @details This screen allows the user to select a macro to place on the home screen
    void macroSelect();

    /// @brief Display the macro place screen
    /// @details This screen allows the user to place the selected macro on the home screen
    void macroPlace();
    //////////////////// ~Main Window Rendering /////////////////////

    ///////////////////// Managing button creations /////////////////////
private:
    /// @brief Draw a set of buttons
    /// @param button_array Array of buttons to draw
    /// @param count Number of buttons to draw, starting from the beginning of the array
    /// @note This function will draw the buttons in the order specified
    void _drawButton(gui::button_base_c **button_array, size_t const count);

    /// @brief Draw a set of buttons
    /// @param button_array Array of buttons to draw
    /// @param idx List of button_array indices to draw
    /// @param count Number of buttons to draw
    /// @note This function will draw the buttons in the order specified
    void _drawButton(gui::button_base_c **button_array, size_t const *idx, size_t const count);

    /// @brief Generate buttons
    /// @param element wireframe object specifying the buttons' layout
    /// @param button_array array of buttons to store the buttons in
    /// @param text Text to display on the button
    /// @param callback The function to be called when the button is pressed
    /// @param callback_ctx The context in which to invoke the action function
    /// @param draw_callback The function to draw the button
    /// @param draw_callback_ctx The context in which to invoke the draw function
    /// @param idx (optional) Specify the index of the array in which to create the button
    /// @note This function will create buttons for the macros in the first available slots
    void _generateButton(
        gui::wf_element_t const &element, 
        gui::button_base_c **button_array, 
        char const * text,
        buttonCallback callback,
        void * callback_ctx,
        drawButtonCallback draw_callback,
        void * draw_callback_ctx,
        int const idx = -1
    );

public:
    /// @brief Create a set of macro buttons for the home screen
    /// @param num_macros The number of macros being passed to the function
    /// @param macros The macros to create buttons for
    /// @param names The names of the macros
    /// @param file_paths The file paths of the images to display on the buttons
    /// @note This function will create buttons for the macros in the first available slots
    void createHomeScreenMacroButtons(
        size_t const num_macros, macro::macro_c const *macros, String const *names, String const *file_paths);

private:
    /// @brief Create a macro button in the first available slot
    /// @param macro The macro to send on button press
    /// @param name The names of the macro
    /// @param file_path The file path of the macro's bmp
    void _createHomeScreenMacroButton(macro::macro_c const *macro, String const *name, String const *file_path);

    /// @brief Create a macro select menu button
    void _createMacroSelectMenuButton();

    /// @brief Create a macro placement menu button
    void _createMacroPlacementMenuButton();

    /// @brief Delete the active macros
    /// @details This is used to delete all active macros when they are no longer needed
    void _deleteActiveMacros();

    /// @brief Delete the menu buttons
    /// @details This is used to delete all menu buttons when they are no longer needed
    void _deleteMenuButtons();

    /// @brief Delete the macro select options
    /// @details This is used to delete all macro select options when they are no longer needed
    void _deleteMacroSelectOptions();

    /// @brief Delete the macro placement options
    /// @details This is used to delete all macro placement options when they are no longer needed
    void _deleteMacroPlacementOptions();

    /// @brief Save the active macros
    /// @details This is used to save the active macros list to persistent storage
    void _saveActiveMacros();
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

    /// @brief Handler for the macro placement button
    static void handleMacroPlace(void *obj)
    {
        if (obj) static_cast<view_c*>(obj)->macroPlace();
    }

    /// @brief Handler for scrolling up
    static void handleScrollUp(void *obj)
    {
        if (obj) static_cast<view_c*>(obj)->_scrollUp();
    }

    /// @brief Handler for scrolling down
    static void handleScrollDown(void *obj)
    {
        if (obj) static_cast<view_c*>(obj)->_scrollDown();
    }

    /// @brief Handler for updating the active macros
    static void handleUpdateActiveMacros(void *obj)
    {
        if (obj)
        {
            static_cast<view_c*>(obj)->_saveActiveMacros();
            static_cast<view_c*>(obj)->_updateActiveMacros();
            static_cast<view_c*>(obj)->macroSelect();
        }
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
    void _drawButton(gui::button_base_c const & button);


    /// @brief Draw a bmp button
    /// @param button The button to draw
    void _drawButtonBmp(gui::button_base_c const & button);


    /// @brief Scroll up the macro select options
    void _scrollUp();

    /// @brief Scroll down the macro select options
    void _scrollDown();

    /// @brief Update the active macros
    void _updateActiveMacros();
    //////////////////// ~BUTTON CALLBACK HANDLERS /////////////////////

    ///////////////////// TOUCH HANDLERS /////////////////////
private:
    /// @brief Handle touch input and determine which button was pressed
    /// @param tp The touch point
    void _handleTouch(TSPoint const &tp);

    /// @brief Handle touch input for the home screen
    /// @param tp The touch point
    /// @return True if a button was pressed, false otherwise
    /// @details This function checks if the touch point intersects with any of the active macro buttons
    bool _homeScreenTouchHandler(TSPoint const &tp);

    /// @brief Handle touch input for the menu screen
    /// @param tp The touch point
    /// @return True if a button was pressed, false otherwise
    /// @details This function checks if the touch point intersects with any of the menu buttons
    bool _menuTouchHandler(TSPoint const &tp);

    /// @brief Handle touch input for the macro select screen
    /// @param tp The touch point
    /// @return True if a button was pressed, false otherwise
    bool _macroSelectTouchHandler(TSPoint const &tp);

    /// @brief Handle touch input for the macro placement screen
    /// @param tp The touch point
    /// @return True if a button was pressed, false otherwise
    bool _macroPlacementTouchHandler(TSPoint const &tp);

    /// @brief Check if a touch point is inside a button
    /// @param tp The touch point
    /// @param button The button to check
    /// @return True if the touch point is inside the button, false otherwise
    bool _isPointInsideButton(TSPoint const &tp, gui::button_base_c const *button);
    //////////////////// ~TOUCH HANDLERS ///////////////////// 

    ///////////////////// PRESENTER CALLBACK HANDLERS /////////////////////
public:
    /// @brief Set the pointer to the presenter
    /// @param presenter The pointer to the presenter
    void setPresenter(presenter::presenter_abstract_c *presenter);


private:
    presenter::presenter_abstract_c *m_presenter;

    /// @brief Query the presenter for all available macros
    /// @return ids: The ids of the available macros
    /// @return names: The names of the available macros
    /// @return size: The number of available macros
    size_t _queryAllMacros(uint16_t *ids, String *names);
    //////////////////// ~PRESENTER CALLBACK HANDLERS /////////////////////
};

} // namespace view
#endif // _VIEW_H_
