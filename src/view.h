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

/// Store the current macro's
/// TODO: Store this to eeprom/SD and load it on startup.
uint16_t constexpr startup_macros[7] = {30, 8, 43, 47, 31, 32, 37};

/// @brief Helper macro to get the number of macro button pointers in an array
/// @param x gui::macro_button_c*: The array
/// @return The number of macro buttons in the array
#define MACRO_BTN_COUNT(x) sizeof(m_active_macros) / sizeof(gui::macro_button_c*)

/// @brief Helper macro to get the number of menu button pointers in an array
/// @param x gui::button_base_c*: The array
/// @return The number of menu buttons in the array
#define BTN_COUNT(x) sizeof(m_menu_buttons) / sizeof(gui::button_base_c*)

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
    gui::button_base_c m_test_button; //< test button (for testing only)

    uint16_t m_current_selected_id;
    uint8_t m_current_selected_placement;
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
    gui::macro_button_c *m_active_macros[MACRO_PLACE_OPTIONS];

public:
    /// @brief Constructor
    view_c()
    : m_state(view_state_t::NONE)
    , m_prev_state(view_state_t::NONE)
    , m_current_selected_id(USHRT_MAX)
    , m_current_selected_placement(UCHAR_MAX)
    , m_scroll(0)
    {
        this->m_test_button = gui::button_base_c(
            0, 0, DEFAULT_MENU_BUTTON_WIDTH, DEFAULT_MENU_BUTTON_HEIGHT, "# Macros");
        this->m_test_button.drawCallback(handleDrawButton, this);

        for (size_t i = 0; i < MACRO_BTN_COUNT(m_active_macros); i++)
        {
            m_active_macros[i] = nullptr;
        }
        
        for (size_t i = 0; i < BTN_COUNT(m_menu_buttons); i++)
        {
            m_menu_buttons[i] = nullptr;
        }

        for (size_t i = 0; i < BTN_COUNT(m_macro_select_options); i++)
        {
            m_macro_select_options[i] = nullptr;
        }
    }

    /// @brief Destructor
    ~view_c()
    {
        _deleteActiveMacros();
        _deleteMenuButtons();
        _deleteMacroSelectOptions();
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
        loadScreen();

        size_t num_startup_macros = MACRO_PLACE_OPTIONS;
        String macro_names[num_startup_macros];
        String macro_file_paths[num_startup_macros];
        macro::macro_c macros[num_startup_macros];

        size_t count = m_presenter->handleLoadMacros(
            startup_macros, num_startup_macros, macro_names, macro_file_paths, macros);
        
        createHomeScreenMacroButtons(count, macros, macro_names, macro_file_paths);
        homeScreen();

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
        _deleteMacroSelectOptions();
        gui::wf_macro_select_t wf;

        _createMacroSelectMenuButton();

        _generateButton(wf.scroll_left_button
            , m_menu_buttons
            , "<"
            , handleScrollUp
            , this
            , handleDrawButton
            , this
            , macro_select_left);
        _generateButton(wf.scroll_right_button
            , m_menu_buttons
            , ">"
            , handleScrollDown
            , this
            , handleDrawButton
            , this
            , macro_select_right);
            
        static uint16_t min_option = 0, max_option = 0;
        int16_t num_macros = m_presenter->handleGetMacroCount();
        
        // Get the macros to display
        uint16_t ids[MACRO_SELECT_OPTIONS];
        String names[MACRO_SELECT_OPTIONS];

        uint16_t start_search_id = 0;

        if (m_scroll > 0)
        {
            // scroll right/down
            start_search_id = max_option + 1;
        }
        else if (m_scroll < 0)
        {
            // scroll left/up
            start_search_id = min_option - MACRO_SELECT_OPTIONS;
        }
        size_t options = m_presenter->handleGetMacroOptions(MACRO_SELECT_OPTIONS, ids, names, start_search_id);
        min_option = ids[0];
        max_option = ids[options - 1];
        
        // figure out what scrolling is needed
        bool enable_scroll = options < num_macros; /// No need to scroll
        bool enable_scroll_left = true;
        bool enable_scroll_right = true;
        
        uint16_t min, max;
        m_presenter->handleMinMaxID(&min, &max);
        if (enable_scroll)
        {
            // if the min id is in the list, disable the scroll left button
            // if the max id is in the list, disable the scroll right button
            for (int i = 0; i < options; i++)
            {
                if (ids[i] == min) enable_scroll_left = false;
                if (ids[i] == max) enable_scroll_right = false;
            }
        }

        for (int i = 0; i < options; i++)
        {
            _generateButton(wf.macro_select_options[i]
                , m_macro_select_options
                , names[i].c_str()
                , nullptr
                , nullptr
                , handleDrawButton
                , this
                , i);
            m_macro_select_options[i]->fillColour(INDIGO_DYE);
            m_macro_select_options[i]->textColour(ANTI_FLASH_WHITE);
            m_macro_select_options[i]->borderColour(ANTI_FLASH_WHITE);
            m_macro_select_options[i]->setPressedColours(UCLA_BLUE, ANTI_FLASH_WHITE, ANTI_FLASH_WHITE);
            m_macro_select_options[i]->setDisabledColours(UCLA_BLUE, ANTI_FLASH_WHITE, ANTI_FLASH_WHITE);
            m_macro_select_options[i]->id(ids[i]);

            // Persist the currently selected macro visually
            bool active = true;
            if (m_current_selected_id == ids[i]) active = false;
            m_macro_select_options[i]->active(active);
        }
        
        if (!enable_scroll_left)
        {
            m_menu_buttons[macro_select_left]->active(false);
        }
        if (!enable_scroll_right)
        {
            m_menu_buttons[macro_select_right]->active(false);
        }
        
        size_t draw_ids[] = {macro_select_left, macro_select_right, macro_select_done_place};
        _drawButton(m_macro_select_options, MACRO_SELECT_OPTIONS);
        _drawButton(m_menu_buttons, draw_ids, 3);
        m_prev_state = m_state;
    }
    //////////////////// ~Main Window Rendering /////////////////////

    ///////////////////// Managing button creations /////////////////////
private:
    /// @brief Draw a set of buttons
    /// @param button_array Array of buttons to draw
    /// @param count Number of buttons to draw, starting from the beginning of the array
    /// @note This function will draw the buttons in the order specified
    void _drawButton(gui::button_base_c **button_array, size_t const count)
    {
        for (size_t i = 0; i < count; i++)
        {
            if (button_array[i] != nullptr) button_array[i]->draw();
        }
    }

    /// @brief Draw a set of buttons
    /// @param button_array Array of buttons to draw
    /// @param idx List of button_array indices to draw
    /// @param count Number of buttons to draw
    /// @note This function will draw the buttons in the order specified
    void _drawButton(gui::button_base_c **button_array, size_t const *idx, size_t const count)
    {
        for (size_t i = 0; i < count; i++)
        {
            if (button_array[idx[i]] != nullptr) button_array[idx[i]]->draw();
        }
    }

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
    )
    {
        if (idx >= 0)
        {
            if (button_array[idx] != nullptr) delete button_array[idx]; // delete the old button

            button_array[idx] = new gui::button_base_c(element.x
                , element.y
                , element.width
                , element.height
                , text);
            button_array[idx]->callback(callback, callback_ctx);
            button_array[idx]->drawCallback(draw_callback, draw_callback_ctx);
            return;
        }
        
        // Find the first available slot
        for (size_t i = 0; i < BTN_COUNT(button_array); i++)
        {
            if (button_array[i] != nullptr) continue;

            button_array[i] = new gui::button_base_c(element.x
                , element.y
                , element.width
                , element.height
                , text);
            button_array[i]->callback(callback, callback_ctx);
            button_array[i]->drawCallback(draw_callback, draw_callback_ctx);
            break;
        }
    }

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

    void _createMacroSelectMenuButton()
    {
        gui::wf_macro_select_t wf;
        if (m_current_selected_id == USHRT_MAX) // If no macro is selected
        {
            _generateButton(wf.confirm_button
                , m_menu_buttons
                , "Done"
                , handleHomeScreen
                , this
                , handleDrawButton
                , this
                , macro_select_done_place);
        }
        else
        {
            _generateButton(wf.confirm_button
                , m_menu_buttons
                , "Place"
                , nullptr
                , nullptr
                , handleDrawButton
                , this
                , macro_select_done_place);
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
        for (size_t i = 0; i < BTN_COUNT(m_menu_buttons); i++)
        {
            if (m_menu_buttons[i] != nullptr)
            {
                delete m_menu_buttons[i];
                m_menu_buttons[i] = nullptr;
            }
        }
    }

    /// @brief Delete the macro select options
    /// @details This is used to delete all macro select options when they are no longer needed
    void _deleteMacroSelectOptions()
    {
        for (size_t i = 0; i < BTN_COUNT(m_macro_select_options); i++)
        {
            if (m_macro_select_options[i] != nullptr)
            {
                delete m_macro_select_options[i];
                m_macro_select_options[i] = nullptr;
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

    /// @brief Scroll up the macro select options
    void _scrollUp()
    {
        m_scroll = -1;
        macroSelect();
        m_scroll = 0;
    }

    /// @brief Scroll down the macro select options
    void _scrollDown()
    {
        m_scroll = 1;
        macroSelect();
        m_scroll = 0;
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
            _macroSelectTouchHandler(tp);
            break;
        // Add cases for other wireframes as needed
        default:
            break;
        }
    }

    /// @brief Handle touch input for the home screen
    /// @param tp The touch point
    /// @return True if a button was pressed, false otherwise
    /// @details This function checks if the touch point intersects with any of the active macro buttons
    bool _homeScreenTouchHandler(TSPoint const &tp)
    {
        // Check if the touch point intersects with any active macro buttons
        for (size_t i = 0; i < MACRO_BTN_COUNT(m_active_macros); i++)
        {
            if (m_active_macros[i] == nullptr) continue;

            if (_isPointInsideButton(tp, m_active_macros[i]))
            {
                m_active_macros[i]->press();
                return true;
            }
        }

        // Check if the touch point intersects with the settings menu button
        if (_isPointInsideButton(tp, m_menu_buttons[home_settings]))
        {
            if (m_menu_buttons[home_settings]->press()) return true;
        }
        return false;
    }

    /// @brief Handle touch input for the menu screen
    /// @param tp The touch point
    /// @return True if a button was pressed, false otherwise
    /// @details This function checks if the touch point intersects with any of the menu buttons
    bool _menuTouchHandler(TSPoint const &tp)
    {
        for (size_t i = 0; i < BTN_COUNT(m_menu_buttons); i++)
        {
            if (m_menu_buttons[i] == nullptr) continue;

            if (_isPointInsideButton(tp, m_menu_buttons[i]))
            {
                m_menu_buttons[i]->press();
                return true;
            }
        }
        return false;
    }

    /// @brief Handle touch input for the macro select screen
    /// @param tp The touch point
    /// @return True if a button was pressed, false otherwise
    bool _macroSelectTouchHandler(TSPoint const &tp)
    {
        if (_menuTouchHandler(tp)) return true; /// Handle the menu buttons first

        bool ret = false;
        for (size_t i = 0; i < BTN_COUNT(m_macro_select_options); i++)
        {
            if (m_macro_select_options[i] == nullptr) continue;

            if (_isPointInsideButton(tp, m_macro_select_options[i]))
            {
                // Use the disabled button colours to indicate the currently selected option
                bool button_state = m_macro_select_options[i]->active();
                m_macro_select_options[i]->active(!button_state); // toggle the active state
                
                if (!button_state) m_current_selected_id = USHRT_MAX; // reset the selected id
                if (button_state) m_current_selected_id = m_macro_select_options[i]->id(); // else get the selected id
                
                m_macro_select_options[i]->draw();
                ret = true;
            }
            else if (!m_macro_select_options[i]->active())
            {
                m_macro_select_options[i]->active(true);
                m_macro_select_options[i]->draw();
            }
        }
        _createMacroSelectMenuButton();
        size_t idx[1] = {macro_select_done_place};
        _drawButton(m_menu_buttons, idx, 1);
        return ret;
    }

    /// @brief Check if a touch point is inside a button
    /// @param tp The touch point
    /// @param button The button to check
    /// @return True if the touch point is inside the button, false otherwise
    bool _isPointInsideButton(TSPoint const &tp, gui::button_base_c const *button)
    {
        return (tp.x >= button->minX() && tp.x <= button->maxX() &&
                tp.y >= button->minY() && tp.y < button->maxY());
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

    ///////////////////// PRESENTER CALLBACK HANDLERS /////////////////////
public:
    /// @brief Set the pointer to the presenter
    /// @param presenter The pointer to the presenter
    void setPresenter(presenter::presenter_abstract_c *presenter)
    {
        m_presenter = presenter;
    }

private:
    presenter::presenter_abstract_c *m_presenter;

    /// @brief Query the presenter for all available macros
    /// @return ids: The ids of the available macros
    /// @return names: The names of the available macros
    /// @return size: The number of available macros
    size_t _queryAllMacros(uint16_t *ids, String *names)
    {
        size_t size = m_presenter->handleQueryMacros(ids, names);
        return size;
    }

    //////////////////// ~PRESENTER CALLBACK HANDLERS /////////////////////

};

} // namespace view
#endif // _VIEW_H_
