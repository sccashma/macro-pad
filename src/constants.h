#ifndef __DEFAULTS_H__
#define __DEFAULTS_H__

////////////////////////////////////////////////////
// colour pallette
static int constexpr RICH_BLACK         = 0x0042;
static int constexpr INDIGO_DYE         = 0x21ea;
static int constexpr UCLA_BLUE          = 0x4393;
static int constexpr ARYLIDE_YELLOW     = 0xde6a;
static int constexpr BATTLESHIP_GREY    = 0x9cd3;
static int constexpr SILVER             = 0xc638;
static int constexpr ANTI_FLASH_WHITE   = 0xef7d;

////////////////////////////////////////////////////
// Menu states and state transitions
enum class menu_state_t
{
    NONE = 0,
    LOADING,
    MAIN_MENU,
    MACRO_SELECT,
    MACRO_PLACE,
    ERROR
};

////////////////////////////////////////////////////
// Default values for buttons
static int constexpr DEFAULT_BUTTON_CORNER_RADIUS = 12; // Corner radius of the buttons

static int constexpr HOME_SCREEN_BUTTONS = 8; // Number of macros on the home screen (SETTINGS BUTTON IS LAST BUTTON)
static int constexpr MAIN_MENU_BUTTONS = 2; // Number of buttons in the main menu
static int constexpr MACRO_SELECT_BUTTONS = 9; // Number of buttons in the macro select menu
static int constexpr MACRO_SELECT_OPTIONS = 6; // Number of options in the macro select menu
static int constexpr MACRO_PLACE_OPTIONS = 7; // Number of placement options in the macro place menu
static int constexpr MACRO_PLACE_BUTTONS = HOME_SCREEN_BUTTONS; // Number of buttons in the macro place menu (last button is save/return)

static int constexpr DEFAULT_MACRO_BUTTON_WIDTH = 80; // Width of the macro buttons on the home screen
static int constexpr DEFAULT_MACRO_BUTTON_HEIGHT = 80; // Height of the macro buttons on the home screen
static int constexpr DEFAULT_MENU_BUTTON_WIDTH = 106; // Width of the buttons in the main menu
static int constexpr DEFAULT_MENU_BUTTON_HEIGHT = 60; // Height of the buttons in the main menu

static int constexpr HOME_SCREEN_ROW_1_Y = 0; // Y coordinate of the first row of buttons on the home screen
static int constexpr HOME_SCREEN_ROW_2_Y = DEFAULT_MACRO_BUTTON_HEIGHT * 2; // Y coordinate of the second row of buttons on the home screen
static int constexpr HOME_SCREEN_COL_1_X = 0; // X coordinate of the first column of buttons on the home screen
static int constexpr HOME_SCREEN_COL_2_X = DEFAULT_MACRO_BUTTON_WIDTH * 2; // X coordinate of the second column of buttons on the home screen
static int constexpr HOME_SCREEN_COL_3_X = DEFAULT_MACRO_BUTTON_WIDTH * 3; // X coordinate of the third column of buttons on the home screen
static int constexpr HOME_SCREEN_COL_4_X = DEFAULT_MACRO_BUTTON_WIDTH * 4; // X coordinate of the fourth column of buttons on the home screen

static int constexpr MACRO_SELECT_OPTION_WIDTH = 320; // Width of the macro select options
static int constexpr MACRO_SELECT_OPTION_HEIGHT = 30; // Height of the macro select options
static int constexpr MACRO_SELECT_OPTION_1_Y = 0; // Y coordinate of the first macro select option
static int constexpr MACRO_SELECT_OPTION_2_Y = MACRO_SELECT_OPTION_HEIGHT; // Y coordinate of the second macro select option
static int constexpr MACRO_SELECT_OPTION_3_Y = MACRO_SELECT_OPTION_HEIGHT * 2; // Y coordinate of the third macro select option
static int constexpr MACRO_SELECT_OPTION_4_Y = MACRO_SELECT_OPTION_HEIGHT * 3; // Y coordinate of the fourth macro select option
static int constexpr MACRO_SELECT_OPTION_5_Y = MACRO_SELECT_OPTION_HEIGHT * 4; // Y coordinate of the fifth macro select option
static int constexpr MACRO_SELECT_OPTION_6_Y = MACRO_SELECT_OPTION_HEIGHT * 5; // Y coordinate of the sixth macro select option

static int constexpr MACRO_SELECT_SCROLL_LEFT_X = 1; // X coordinate of the scroll left button in the macro select menu
static int constexpr MACRO_SELECT_SCROLL_RIGHT_X = MACRO_SELECT_OPTION_WIDTH + 1; // X coordinate of the scroll right button in the macro select menu
static int constexpr MACRO_SELECT_SCROLL_Y = MACRO_SELECT_OPTIONS * MACRO_SELECT_OPTION_HEIGHT; // Y coordinate of the scroll buttons in the macro select menu


#endif // __DEFAULTS_H__
