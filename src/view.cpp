/*
    view.cpp
    Author: Scott Cashman
    Date: 6/4/2025
*/

#include "view.h"

namespace view
{

view_c::view_c()
: m_state(view_state_t::NONE)
, m_prev_state(view_state_t::NONE)
, m_current_selected_id(USHRT_MAX)
, m_current_selected_placement(UCHAR_MAX)
, m_update_macros(false)
, m_scroll(0)
{
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

    for (size_t i = 0; i < BTN_COUNT(m_macro_placement_options); i++)
    {
        m_macro_placement_options[i] = nullptr;
    }

    /// Load the active macros list
    for (size_t i = 0; i < MACRO_PLACE_OPTIONS; i++)
    {
        int val;
        EEPROM.get(i, val);
        m_active_macros_list[i] = (val & 0x00FF); // lower 8 bits
    }
}

view_c::~view_c()
{
    _deleteActiveMacros();
    _deleteMenuButtons();
    _deleteMacroSelectOptions();
    _deleteMacroPlacementOptions();
}

void view_c::clearScreen()
{
    display::tft_c::instance().fillScreen(INDIGO_DYE);
}

void view_c::displayMessage(char const *message)
{
    display::tft_c::instance().fillScreen(INDIGO_DYE);
    display::displayMessage(message);
}

void view_c::run()
{
    loadScreen();
    
    size_t constexpr num_active_macros_list = MACRO_PLACE_OPTIONS;
    String macro_names[num_active_macros_list];
    String macro_file_paths[num_active_macros_list];
    macro::macro_c macros[num_active_macros_list];

    size_t count = m_presenter->handleLoadMacros(
        m_active_macros_list, num_active_macros_list, macro_names, macro_file_paths, macros);
    createHomeScreenMacroButtons(macros, macro_names, macro_file_paths);

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

void view_c::loadScreen()
{
    m_state = view_state_t::LOADING;
    display::tft_c::instance().fillScreen(INDIGO_DYE);
    display::drawBmp("/bckgrnd.bmp", 0, 0, display::tft_c::instance().width(), display::tft_c::instance().height());
    display::drawTextInCanvas(0
        , display::tft_c::instance().height() / 2
        , display::tft_c::instance().width()
        , display::tft_c::instance().height() / 2
        , "Please wait Democratically"
        , ARYLIDE_YELLOW
        , 3);
    m_prev_state = m_state;
}

void view_c::homeScreen()
{
    m_state = view_state_t::HOME;
    if (m_prev_state == view_state_t::LOADING)
    {
        // Clear only the text section
        display::drawBmp("/bckgrnd.bmp"
            , 0
            , 0
            , display::tft_c::instance().width()
            , display::tft_c::instance().height()
            , false
            , display::tft_c::instance().height() / 2);
    }
    else if (m_prev_state == view_state_t::MAIN_MENU)
    {
        // Clear only up to the top of the menu buttons
        display::drawBmp("/bckgrnd.bmp"
            , 0
            , 0
            , display::tft_c::instance().width()
            , display::tft_c::instance().height()
            , false
            , display::tft_c::instance().height() - DEFAULT_MENU_BUTTON_HEIGHT);
    }
    else
    {
        display::drawBmp("/bckgrnd.bmp", 0, 0, display::tft_c::instance().width(), display::tft_c::instance().height());
    }
    
    _deleteMenuButtons();

    //////////////////////////
    if (m_update_macros)
    {
        _deleteActiveMacros();
        size_t constexpr num_active_macros_list = MACRO_PLACE_OPTIONS;
        String macro_names[num_active_macros_list];
        String macro_file_paths[num_active_macros_list];
        macro::macro_c macros[num_active_macros_list];
    
        size_t count = m_presenter->handleLoadMacros(
            m_active_macros_list, num_active_macros_list, macro_names, macro_file_paths, macros);

        createHomeScreenMacroButtons(macros, macro_names, macro_file_paths);
        m_update_macros = false;
    }

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

void view_c::mainMenu()
{
    m_state = view_state_t::MAIN_MENU;
    display::drawBmp("/bckgrnd.bmp", 0, 0, display::tft_c::instance().width(), display::tft_c::instance().height());
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
void view_c::macroSelect()
{
    m_state = view_state_t::MACRO_SELECT;
    if (m_prev_state == view_state_t::MAIN_MENU)
    {
        // Clear only up to the top of the menu buttons
        display::drawBmp("/bckgrnd.bmp"
            , 0
            , 0
            , display::tft_c::instance().width()
            , display::tft_c::instance().height()
            , false
            , display::tft_c::instance().height() - DEFAULT_MENU_BUTTON_HEIGHT);
    }
    else
    {
        display::drawBmp("/bckgrnd.bmp", 0, 0, display::tft_c::instance().width(), display::tft_c::instance().height());
    }

    if (m_prev_state == view_state_t::MACRO_PLACE) _deleteMacroPlacementOptions();

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

void view_c::macroPlace()
{
    m_state = view_state_t::MACRO_PLACE;
    display::drawBmp("/bckgrnd.bmp", 0, 0, display::tft_c::instance().width(), display::tft_c::instance().height());

    _deleteMenuButtons();
    _deleteMacroSelectOptions();
    gui::wf_home_screen_t wf;

    // create the placement button options
    for (int i = 0; i < BTN_COUNT(m_macro_placement_options); i++)
    {
        _generateButton(wf.macro_buttons[i]
            , m_macro_placement_options
            , String(i).c_str() // indicate the index
            , nullptr
            , nullptr
            , handleDrawButton
            , this
            , i);
        m_macro_placement_options[i]->fillColour(INDIGO_DYE);
        m_macro_placement_options[i]->textColour(ANTI_FLASH_WHITE);
        m_macro_placement_options[i]->borderColour(ANTI_FLASH_WHITE);
        m_macro_placement_options[i]->setPressedColours(UCLA_BLUE, ANTI_FLASH_WHITE, ANTI_FLASH_WHITE);
        m_macro_placement_options[i]->setDisabledColours(UCLA_BLUE, ANTI_FLASH_WHITE, ANTI_FLASH_WHITE);
        m_macro_placement_options[i]->id(i);
    }

    _generateButton(wf.menu_button
        , m_menu_buttons
        , "OK"
        , nullptr
        , nullptr
        , handleDrawButton
        , this
        , macro_select_done_place);

    if (m_current_selected_placement == UCHAR_MAX) // If no placement is selected
    {
        m_menu_buttons[macro_select_done_place]->active(false);
    }
    
    _drawButton(m_macro_placement_options, MACRO_PLACE_OPTIONS);
    m_menu_buttons[macro_select_done_place]->draw();
    m_prev_state = m_state;
}

void view_c::_drawButton(gui::button_base_c **button_array, size_t const count)
{
    for (size_t i = 0; i < count; i++)
    {
        if (button_array[i] != nullptr) button_array[i]->draw();
    }
}

void view_c::_drawButton(gui::button_base_c **button_array, size_t const *idx, size_t const count)
{
    for (size_t i = 0; i < count; i++)
    {
        if (button_array[idx[i]] != nullptr) button_array[idx[i]]->draw();
    }
}

void view_c::_generateButton(
    gui::wf_element_t const &element, 
    gui::button_base_c **button_array, 
    char const * text,
    buttonCallback callback,
    void * callback_ctx,
    drawButtonCallback draw_callback,
    void * draw_callback_ctx,
    int const idx
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

void view_c::createHomeScreenMacroButtons(macro::macro_c const *macros, String const *names, String const *file_paths)
{
    for (size_t i = 0; i < MACRO_PLACE_OPTIONS; i++)
    {
        if(m_active_macros_list[i] == USHRT_MAX) continue; // any non USHRT_MAX is a valid id
        _createHomeScreenMacroButton(&macros[i], &names[i], &file_paths[i], i);
    }
}

void view_c::_createHomeScreenMacroButton(
    macro::macro_c const *macro, 
    String const *name, 
    String const *file_path,
    size_t const idx
)
{
    gui::wf_home_screen_t wf;
    if (m_active_macros[idx] != nullptr) delete m_active_macros[idx];

    m_active_macros[idx] = new gui::macro_button_c(*macro, *name, *file_path);
    m_active_macros[idx]->setPos(wf.macro_buttons[idx].x, wf.macro_buttons[idx].y);
    m_active_macros[idx]->width(wf.macro_buttons[idx].width);
    m_active_macros[idx]->height(wf.macro_buttons[idx].height);
    m_active_macros[idx]->drawCallback(handleDrawBmpButton, this);
    m_active_macros[idx]->id(m_active_macros_list[idx]); // assume this for debugging
}

void view_c::_createMacroSelectMenuButton()
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
            , handleMacroPlace
            , this
            , handleDrawButton
            , this
            , macro_select_done_place);
    }
}

void view_c::_createMacroPlacementMenuButton()
{
    gui::wf_home_screen_t wf;
    _generateButton(wf.menu_button
        , m_menu_buttons
        , "OK"
        , handleUpdateActiveMacros
        , this
        , handleDrawButton
        , this
        , macro_select_done_place);

    bool active = (m_current_selected_placement == UCHAR_MAX) ? false : true;
    m_menu_buttons[macro_select_done_place]->active(active);
}

void view_c::_deleteActiveMacros()
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

void view_c::_deleteMenuButtons()
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

void view_c::_deleteMacroSelectOptions()
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

void view_c::_deleteMacroPlacementOptions()
{
    for (size_t i = 0; i <BTN_COUNT(m_macro_placement_options); i++)
    {
        if (m_macro_placement_options[i] != nullptr)
        {
            delete m_macro_placement_options[i];
            m_macro_placement_options[i] = nullptr;
        }
    }
}

void view_c::_saveActiveMacros()
{
    for (size_t i = 0; i < MACRO_PLACE_OPTIONS; i++)
    {
        int val = m_active_macros_list[i];
        EEPROM.update(i, val);
    }
}

void view_c::_drawButton(gui::button_base_c const & button)
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

void view_c::_drawButtonBmp(gui::button_base_c const & button)
{
    display::drawBmp(
        "/icons/" + button.imageFilePath(), button.minX(), button.minY(), button.width(), button.height(), true);
}

void view_c::_scrollUp()
{
    m_scroll = -1;
    macroSelect();
    m_scroll = 0;
}

void view_c::_scrollDown()
{
    m_scroll = 1;
    macroSelect();
    m_scroll = 0;
}

void view_c::_updateActiveMacros()
{
    if (m_current_selected_id != USHRT_MAX && m_current_selected_placement != UCHAR_MAX)
    {
        m_active_macros_list[m_current_selected_placement] = m_current_selected_id;
        m_current_selected_id = USHRT_MAX;
        m_current_selected_placement = UCHAR_MAX;
        m_update_macros = true;
    }
}

void view_c::_handleTouch(TSPoint const &tp)
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
    case view_state_t::MACRO_PLACE:
        _macroPlacementTouchHandler(tp);
        break;
    default:
        break;
    }
}

bool view_c::_homeScreenTouchHandler(TSPoint const &tp)
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

bool view_c::_menuTouchHandler(TSPoint const &tp)
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

bool view_c::_macroSelectTouchHandler(TSPoint const &tp)
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

bool view_c::_macroPlacementTouchHandler(TSPoint const &tp)
{
    if (_menuTouchHandler(tp)) return true; /// Handle the menu buttons first

    bool ret = false;
    for (size_t i = 0; i < BTN_COUNT(m_macro_placement_options); i++)
    {
        if (m_macro_placement_options[i] == nullptr) continue;

        if (_isPointInsideButton(tp, m_macro_placement_options[i]))
        {
            // Use the disabled button colours to indicate the currently selected option
            bool button_state = m_macro_placement_options[i]->active();
            m_macro_placement_options[i]->active(!button_state); // toggle the active state
            
            if (!button_state) m_current_selected_placement = UCHAR_MAX; // reset the selected id
            if (button_state) m_current_selected_placement = m_macro_placement_options[i]->id(); // else get the selected id
            
            m_macro_placement_options[i]->draw();
            ret = true;
        }
        else if (!m_macro_placement_options[i]->active())
        {
            m_macro_placement_options[i]->active(true);
            m_macro_placement_options[i]->draw();
        }
    }
    _createMacroPlacementMenuButton();
    m_menu_buttons[macro_select_done_place]->draw();
    return ret;
}

bool view_c::_isPointInsideButton(TSPoint const &tp, gui::button_base_c const *button)
{
    return (tp.x >= button->minX() && tp.x <= button->maxX() &&
            tp.y >= button->minY() && tp.y < button->maxY());
}

void view_c::setPresenter(presenter::presenter_abstract_c *presenter)
{
    m_presenter = presenter;
}

size_t view_c::_queryAllMacros(uint16_t *ids, String *names)
{
    size_t size = m_presenter->handleQueryMacros(ids, names);
    return size;
}

#if defined(DEBUG)
void view_c::printState()
{
    Serial.println("Debug: ");
    Serial.print("SRAM:");
    for (int i = 0; i < MACRO_PLACE_OPTIONS; i++)
        Serial.print(" " + String(m_active_macros_list[i]));
    Serial.println("\nEEPROM:");
    for (int i = 0; i < MACRO_PLACE_OPTIONS; i++)
        Serial.print(" " + String(EEPROM.read(i)));
    Serial.println("\nActive Macros:");
    for (size_t i = 0; i < MACRO_BTN_COUNT(m_active_macros); i++)
        Serial.print(" " + (m_active_macros[i] != nullptr) ? String(m_active_macros[i]->id()) : "NULL");
    Serial.println("\n----------------------------------------------------");
}
#endif
} // namespace view
