
#ifndef __KEY_MAP_H__
#define __KEY_MAP_H__

#include <Keyboard.h>
#include "hashtable.h"

namespace km
{

Hashtable<String, uint8_t> key_table;

/// @brief Initialise the key table with the key mappings
/// @details The key table is a hashtable that maps the string representation of the key to the key code
///          The key codes are defined in the Keyboard library: https://docs.arduino.cc/language-reference/en/functions/usb/Keyboard/keyboardModifiers/
void initTable()
{
    constexpr uint8_t NUM0 = 0X30;
    constexpr uint8_t NUM1 = 0X31;
    constexpr uint8_t NUM2 = 0X32;
    constexpr uint8_t NUM3 = 0X33;
    constexpr uint8_t NUM4 = 0X34;
    constexpr uint8_t NUM5 = 0X35;
    constexpr uint8_t NUM6 = 0X36;
    constexpr uint8_t NUM7 = 0X37;
    constexpr uint8_t NUM8 = 0X38;
    constexpr uint8_t NUM9 = 0X39;

    if (!key_table.isEmpty())
    {
        key_table.clear(); // Make sure the table is empty before we start
    }

    // Arrows
    key_table.put("UP", NUM2);
    key_table.put("LEFT", NUM4);
    key_table.put("DOWN", NUM5);
    key_table.put("RIGHT", NUM6);

    // Numbers
    key_table.put("0", NUM0);
    key_table.put("1", NUM1);
    key_table.put("2", NUM2);
    key_table.put("3", NUM3);
    key_table.put("4", NUM4);
    key_table.put("5", NUM5);
    key_table.put("6", NUM6);
    key_table.put("7", NUM7);
    key_table.put("8", NUM8);
    key_table.put("9", NUM9);

    // modifiers
    key_table.put("LCTRL", KEY_LEFT_CTRL);
    key_table.put("LSHIFT", KEY_LEFT_SHIFT);
    key_table.put("LALT", KEY_LEFT_ALT);
    key_table.put("LOPTION", KEY_LEFT_ALT);
    key_table.put("LWIN", KEY_LEFT_GUI);
    key_table.put("LCMD", KEY_LEFT_GUI);

    key_table.put("RCTRL", KEY_RIGHT_CTRL);
    key_table.put("RSHIFT", KEY_RIGHT_SHIFT);
    key_table.put("RALT", KEY_RIGHT_ALT);
    key_table.put("ROPTION", KEY_RIGHT_ALT);
    key_table.put("RWIN", KEY_RIGHT_GUI);
    key_table.put("RCMD", KEY_RIGHT_GUI);

    // Special keys
    key_table.put("TAB", KEY_TAB);
    key_table.put("CAPSLOCK", KEY_CAPS_LOCK);
    key_table.put("BACKSPACE", KEY_BACKSPACE);
    key_table.put("RETURN", KEY_RETURN);
    key_table.put("MENU", KEY_MENU);

    // Navigation keys
    key_table.put("INSERT", KEY_INSERT);
    key_table.put("DELETE", KEY_DELETE);
    key_table.put("HOME", KEY_HOME);
    key_table.put("END", KEY_END);
    key_table.put("PAGE_UP", KEY_PAGE_UP);
    key_table.put("PAGE_DOWN", KEY_PAGE_DOWN);
    key_table.put("UP_ARROW", KEY_UP_ARROW);
    key_table.put("DOWN_ARROW", KEY_DOWN_ARROW);
    key_table.put("LEFT_ARROW", KEY_LEFT_ARROW);
    key_table.put("RIGHT_ARROW", KEY_RIGHT_ARROW);

    // Numpad keys
    key_table.put("NUM_LOCK", KEY_NUM_LOCK);
    key_table.put("KP_SLASH", KEY_KP_SLASH);
    key_table.put("KP_ASTERISK", KEY_KP_ASTERISK);
    key_table.put("KP_MINUS", KEY_KP_MINUS);
    key_table.put("KP_PLUS", KEY_KP_PLUS);
    key_table.put("KP_ENTER", KEY_KP_ENTER);
    key_table.put("KP_0", KEY_KP_0);
    key_table.put("KP_1", KEY_KP_1);
    key_table.put("KP_2", KEY_KP_2);
    key_table.put("KP_3", KEY_KP_3);
    key_table.put("KP_4", KEY_KP_4);
    key_table.put("KP_5", KEY_KP_5);
    key_table.put("KP_6", KEY_KP_6);
    key_table.put("KP_7", KEY_KP_7);
    key_table.put("KP_8", KEY_KP_8);
    key_table.put("KP_9", KEY_KP_9);
    key_table.put("KP_DOT", KEY_KP_DOT);

    // Function keys
    key_table.put("ESC", KEY_ESC);
    key_table.put("F1", KEY_F1);
    key_table.put("F2", KEY_F2);
    key_table.put("F3", KEY_F3);
    key_table.put("F4", KEY_F4);
    key_table.put("F5", KEY_F5);
    key_table.put("F6", KEY_F6);
    key_table.put("F7", KEY_F7);
    key_table.put("F8", KEY_F8);
    key_table.put("F9", KEY_F9);
    key_table.put("F10", KEY_F10);
    key_table.put("F11", KEY_F11);
    key_table.put("F12", KEY_F12);
    key_table.put("F13", KEY_F13);
    key_table.put("F14", KEY_F14);
    key_table.put("F15", KEY_F15);
    key_table.put("F16", KEY_F16);
    key_table.put("F17", KEY_F17);
    key_table.put("F18", KEY_F18);
    key_table.put("F19", KEY_F19);
    key_table.put("F20", KEY_F20);
    key_table.put("F21", KEY_F21);
    key_table.put("F22", KEY_F22);
    key_table.put("F23", KEY_F23);
    key_table.put("F24", KEY_F24);

    // Function control keys
    key_table.put("PRINT_SCREEN", KEY_PRINT_SCREEN);
    key_table.put("SCROLL_LOCK", KEY_SCROLL_LOCK);
    key_table.put("PAUSE", KEY_PAUSE);

    // Space and punctuation/symbols
    key_table.put("SPACE", 0x20);
    key_table.put("!", 0x21);
    key_table.put("#", 0x23);
    key_table.put("$", 0x24);
    key_table.put("%", 0x25);
    key_table.put("&", 0x26);
    key_table.put("(", 0x28);
    key_table.put(")", 0x29);
    key_table.put("*", 0x2A);
    key_table.put("/", 0x2F);
    key_table.put(":", 0x3A);
    key_table.put(";", 0x3B);
    key_table.put("<", 0x3C);
    key_table.put("=", 0x3D);
    key_table.put(">", 0x3E);
    key_table.put("?", 0x3F);
    key_table.put("@", 0x40);
    key_table.put("[", 0x5B);
    key_table.put("]", 0x5D);
    key_table.put("^", 0x5E);
    key_table.put("_", 0x5F);
    key_table.put("{", 0x7B);
    key_table.put("|", 0x7C);
    key_table.put("}", 0x7D);
    key_table.put("~", 0x7E);

    // alphabetical keys
    key_table.put("A", 0x41);
    key_table.put("B", 0x42);
    key_table.put("C", 0x43);
    key_table.put("D", 0x44);
    key_table.put("E", 0x45);
    key_table.put("F", 0x46);
    key_table.put("G", 0x47);
    key_table.put("H", 0x48);
    key_table.put("I", 0x49);
    key_table.put("J", 0x4A);
    key_table.put("K", 0x4B);
    key_table.put("L", 0x4C);
    key_table.put("M", 0x4D);
    key_table.put("N", 0x4E);
    key_table.put("O", 0x4F);
    key_table.put("P", 0x50);
    key_table.put("Q", 0x51);
    key_table.put("R", 0x52);
    key_table.put("S", 0x53);
    key_table.put("T", 0x54);
    key_table.put("U", 0x55);
    key_table.put("V", 0x56);
    key_table.put("W", 0x57);
    key_table.put("X", 0x58);
    key_table.put("Y", 0x59);
    key_table.put("Z", 0x5A);
    key_table.put("a", 0x61);
    key_table.put("b", 0x62);
    key_table.put("c", 0x63);
    key_table.put("d", 0x64);
    key_table.put("e", 0x65);
    key_table.put("f", 0x66);
    key_table.put("g", 0x67);
    key_table.put("h", 0x68);
    key_table.put("i", 0x69);
    key_table.put("j", 0x6A);
    key_table.put("k", 0x6B);
    key_table.put("l", 0x6C);
    key_table.put("m", 0x6D);
    key_table.put("n", 0x6E);
    key_table.put("o", 0x6F);
    key_table.put("p", 0x70);
    key_table.put("q", 0x71);
    key_table.put("r", 0x72);
    key_table.put("s", 0x73);
    key_table.put("t", 0x74);
    key_table.put("u", 0x75);
    key_table.put("v", 0x76);
    key_table.put("w", 0x77);
    key_table.put("x", 0x78);
    key_table.put("y", 0x79);
    key_table.put("z", 0x7A);
}

/// @brief Get the key code for a given key
/// @param key The key to get the code for
/// @return uint8_t: The key code
uint8_t getKeyCode(String const& key)
{
    uint8_t code = NULL;

    if (key_table.containsKey(key))
    {
        code = key_table.getElement(key);
    }

    return code;
}

} // namespace km
#endif // __KEY_MAP_H__