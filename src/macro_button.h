/*
    macro_button.h - Macro Button Header File
    Created: 2/4/2025
    Author: Scott Cashman
    Description: This file contains the definition of the macro button class, which is used to create buttons for the macro pad.
*/

#ifndef _MACRO_BUTTON_H_
#define _MACRO_BUTTON_H_

#include <Arduino.h>
#include "button.h"
#include "constants.h"
#include "macro.h"

namespace gui
{

class macro_button_c : public button_base_c
{
public:
    /// @brief Default constructor
    macro_button_c()
    {
    }

    /// @brief Constructor
    /// @param macro The macro to send on button press
    /// @param name The name of the macro
    /// @param file_path The file path of the macro's bmp
    macro_button_c(macro::macro_c macro, String const name, String const file_path)
    : button_base_c(0, 0, DEFAULT_MACRO_BUTTON_WIDTH, DEFAULT_MACRO_BUTTON_HEIGHT, name.c_str())
    , m_macro(macro)
    {
        this->imageFilePath(file_path);
        this->callback(macro_button_c::handleSendMacro, this);
    }

    /// @brief Copy constructor
    /// @param rhs The macro button to copy
    macro_button_c(macro_button_c const& rhs)
    : button_base_c(rhs)
    , m_macro(rhs.m_macro)
    {
        this->callback(macro_button_c::handleSendMacro, this);
    }

    /// @brief Assignment operator
    /// @param rhs The macro button to assign 
    /// @return macro_button_c& Reference to this
    macro_button_c& operator=(macro_button_c& rhs)
    {
        if (this != &rhs)
        {
            this->m_macro = rhs.m_macro;
        }
        return *this;
    }

private:
    macro::macro_c m_macro; ///< The macro associated with the button

    /// @brief Play the macro
    void _sendMacro()
    {
        this->m_macro.play();
    }

    /// @brief Handler to send the macro
    static void handleSendMacro(void* obj)
    {
        if (obj) static_cast<macro_button_c*>(obj)->_sendMacro();
    }
};

} // namespace gui
#endif // _MACRO_BUTTON_H_