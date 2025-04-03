/*
 * macro.h
 *
 * Created: 1/3/2025
 * Author: Scott Cashman
*/

#ifndef __MACRO_H__
#define __MACRO_H__

#include <Keyboard.h>
#include "key_map.h"
#include "csv_parser.h"
#include "sd_utils.h"
#include "constants.h"

namespace macro
{

size_t parseKeyCodes(String const &, uint8_t *, size_t const );

/// @brief Data structure defining a macro
/// @note The macro is defined as a sequence of keys. The key codes are defined in the key_map.h file.
class macro_c
{
public:
    /// @brief constructor for the macro_c type
    macro_c()
    {
        this->id = NULL;
        this->name = "";
        for (size_t i = 0; i < this->codes_size; i++)
        {
            this->codes[i] = 0;
        }
    }

    /// @brief operator overload for the assignment operator
    /// @param other The macro to assign to this macro
    /// @return macro_c&: A reference to this macro
    macro_c& operator=(macro_c const &other)
    {
        this->id = other.id;
        this->name = other.name;
        for (size_t i = 0; i < this->codes_size; i++)
        {
            this->codes[i] = other.codes[i];
        }
        return *this;
    }

    /// @brief setup the macro's code
    /// @param keys The key sequence to generate a macro from
    /// @return size_t: The number of key codes generated
    size_t initialiseCodes(String const &keys)
    {
        size_t idx = parseKeyCodes(keys, this->codes, this->codes_size);
        this->codes[idx] = 0; // Null terminate the array
        return idx;
    }

    /// @brief play the macro
    /// @note This function will send the key codes to the keyboard in the order they are defined in the macro
    /// @todo Add functionality to send multiple keys at once (e.g. CTRL + C)
    void play() const
    {
        for (size_t i = 0; i < this->codes_size; i++)
        {
            if (this->codes[i] == 0) break; // Stop if we reach the end of the macro
            Keyboard.write(this->codes[i]);
            delay(KEYBOARD_ENTRY_DELAY_MS); // Delay between key presses
        }
    }

    uint8_t id;                                 ///< The id of the macro
    String name;                                ///< The name of the macro
    size_t const codes_size = KEY_CODES_MAX;    ///< The size of the key codes array
    uint8_t codes[KEY_CODES_MAX];               ///< The key codes for the macro
};

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