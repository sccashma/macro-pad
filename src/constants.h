/*
 * macro.h
 *
 * Created: 2/4/2025
 * Author: Scott Cashman
*/

#ifndef __DEFAULTS_H__
#define __DEFAULTS_H__


/// @brief The maximum number of key codes in a macro
/// @note This is arbitrary, but should be large enough to hold the largest macro. 32x8bit = 256 bits = 32 bytes = 32 characters.
uint8_t constexpr KEY_CODES_MAX = 32;

/// @brief Delay time between key presses.
/// @note Some applications are sensitive to the time between key presses. You may need to adjust this value to suit your application.
int constexpr KEYBOARD_ENTRY_DELAY_MS = 50;

////////////////////////////////////////////////////
// colour pallette
int constexpr RICH_BLACK         = 0x0042;
int constexpr INDIGO_DYE         = 0x21ea;
int constexpr UCLA_BLUE          = 0x4393;
int constexpr ARYLIDE_YELLOW     = 0xde6a;
int constexpr BATTLESHIP_GREY    = 0x9cd3;
int constexpr SILVER             = 0xc638;
int constexpr ANTI_FLASH_WHITE   = 0xef7d;

////////////////////////////////////////////////////
// Menu states and state transitions
enum class menu_state_t
{
    NONE = 0,
    LOADING,
    HOME,
    MAIN_MENU,
    MACRO_SELECT,
    MACRO_PLACE,
    ERROR
};

////////////////////////////////////////////////////
// Numbers of buttons
uint16_t constexpr HOME_SCREEN_BUTTONS = 8; // Number of macros on the home screen (SETTINGS BUTTON IS LAST BUTTON)
uint16_t constexpr MAIN_MENU_BUTTONS = 2; // Number of buttons in the main menu
uint16_t constexpr MACRO_SELECT_BUTTONS = 3; // Number of buttons in the macro select menu
uint16_t constexpr MACRO_SELECT_OPTIONS = 6; // Number of options in the macro select menu
uint16_t constexpr MACRO_PLACE_OPTIONS = 7; // Number of placement options in the macro place menu
uint16_t constexpr MACRO_PLACE_BUTTONS = HOME_SCREEN_BUTTONS; // Number of buttons in the macro place menu (last button is save/return)

// Button sizes/drawing parameters
uint16_t constexpr DEFAULT_BUTTON_CORNER_RADIUS = 12; // Corner radius of the buttons
uint16_t constexpr DEFAULT_MACRO_BUTTON_WIDTH = 80; // Width of the macro buttons on the home screen
uint16_t constexpr DEFAULT_MACRO_BUTTON_HEIGHT = 80; // Height of the macro buttons on the home screen
uint16_t constexpr DEFAULT_MENU_BUTTON_WIDTH = 106; // Width of the buttons in the main menu
uint16_t constexpr DEFAULT_MENU_BUTTON_HEIGHT = 60; // Height of the buttons in the main menu
uint16_t constexpr MACRO_SELECT_OPTION_WIDTH = 320; // Width of the macro select options
uint16_t constexpr MACRO_SELECT_OPTION_HEIGHT = 30; // Height of the macro select options

// Home screen Button locations
uint16_t constexpr HOME_SCREEN_ROW_1_Y = 0; // Y coordinate of the first row of buttons on the home screen
uint16_t constexpr HOME_SCREEN_ROW_2_Y = DEFAULT_MACRO_BUTTON_HEIGHT * 2; // Y coordinate of the second row of buttons on the home screen
uint16_t constexpr HOME_SCREEN_COL_1_X = 0; // X coordinate of the first column of buttons on the home screen
uint16_t constexpr HOME_SCREEN_COL_2_X = DEFAULT_MACRO_BUTTON_WIDTH * 1; // X coordinate of the second column of buttons on the home screen
uint16_t constexpr HOME_SCREEN_COL_3_X = DEFAULT_MACRO_BUTTON_WIDTH * 2; // X coordinate of the third column of buttons on the home screen
uint16_t constexpr HOME_SCREEN_COL_4_X = DEFAULT_MACRO_BUTTON_WIDTH * 3; // X coordinate of the fourth column of buttons on the home screen

// Main menu button locations
uint16_t constexpr MAIN_MENU_ROW_Y = 90; // Y coordinate of the first row of buttons in the main menu
uint16_t constexpr MAIN_MENU_COL_1_X = 36; // X coordinate of the first column of buttons in the main menu
uint16_t constexpr MAIN_MENU_COL_2_X = DEFAULT_MENU_BUTTON_WIDTH + (2*36); // X coordinate of the second column of buttons in the main menu

// Macro selection menu button locations
uint16_t constexpr MACRO_SELECT_OPTION_X = 0; // X coordinate of the options
uint16_t constexpr MACRO_SELECT_OPTION_1_Y = 0; // Y coordinate of the first macro select option
uint16_t constexpr MACRO_SELECT_OPTION_2_Y = MACRO_SELECT_OPTION_HEIGHT; // Y coordinate of the second macro select option
uint16_t constexpr MACRO_SELECT_OPTION_3_Y = MACRO_SELECT_OPTION_HEIGHT * 2; // Y coordinate of the third macro select option
uint16_t constexpr MACRO_SELECT_OPTION_4_Y = MACRO_SELECT_OPTION_HEIGHT * 3; // Y coordinate of the fourth macro select option
uint16_t constexpr MACRO_SELECT_OPTION_5_Y = MACRO_SELECT_OPTION_HEIGHT * 4; // Y coordinate of the fifth macro select option
uint16_t constexpr MACRO_SELECT_OPTION_6_Y = MACRO_SELECT_OPTION_HEIGHT * 5; // Y coordinate of the sixth macro select option
uint16_t constexpr MACRO_SELECT_SCROLL_LEFT_X = 0; // X coordinate of the scroll left button in the macro select menu
uint16_t constexpr MACRO_SELECT_SCROLL_RIGHT_X = DEFAULT_MENU_BUTTON_WIDTH + 1; // X coordinate of the scroll right button in the macro select menu
uint16_t constexpr MACRO_SELECT_BACK_X = MACRO_SELECT_SCROLL_RIGHT_X + DEFAULT_MENU_BUTTON_WIDTH + 1; // X coordinate of the back button in the macro select menu
uint16_t constexpr MACRO_SELECT_SCROLL_Y = MACRO_SELECT_OPTIONS * MACRO_SELECT_OPTION_HEIGHT; // Y coordinate of the scroll buttons in the macro select menu

#endif // __DEFAULTS_H__
