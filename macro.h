/*
 * macro.h
 *
 * Created: 3/10/2020 10:51:52 AM
 * Author: Scott Cashman
*/

#ifndef __MACRO_H__
#define __MACRO_H__

#include <Keyboard.h>
#include "key_map.h"
#include "csv_parser.h"
#include "sd_utils.h"

namespace macro
{

/// @brief The maximum number of key codes in a macro
/// @note This is arbitrary, but should be large enough to hold the largest macro. 32x8bit = 256 bits = 32 bytes = 32 characters.
uint8_t constexpr KEY_CODES_MAX = 32;

/// @brief Data structure defining a macro
/// @note The macro is defined as a sequence of keys. The key codes are defined in the key_map.h file.
typedef struct macro_t
{
    uint8_t id;                                 ///< The id of the macro
    String name;                                ///< The name of the macro
    size_t const codes_size = KEY_CODES_MAX;    ///< The size of the key codes array
    uint8_t codes[KEY_CODES_MAX];               ///< The key codes for the macro
} macro_t;


/// @brief initialise the keyboard
void initialiseKeyboard()
{
    Keyboard.begin();
    km::initTable();
}

/// @brief close the keyboard
void closeKeyboard()
{
    Keyboard.end();
}

/// @brief Generate a macro from a given key sequence
/// @param keys The key sequence to generate a macro from
/// @param codes The array to store the key codes in
/// @param codes_size The size of the array to store the key codes in
/// @return size_t: The number of key codes generated
size_t parseKeyCodes(String const &keys, uint8_t *codes, size_t const codes_size)
{
    String components[codes_size];

    csv::parseCSVLine(keys, components, codes_size);

    size_t idx = 0;
    while (components[idx].length() != 0)
    {
        codes[idx] = km::getKeyCode(components[idx]);
        idx++;
    }

    return idx;
}


} // namespace macro
#endif // __MACRO_H__