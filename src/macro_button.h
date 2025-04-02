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
    macro_button_c() = delete; // We don't want a default constructor

    macro_button_c(macro::macro_t macro, String const name, String const file_name)
    : button_base_c(0, 0, DEFAULT_MACRO_BUTTON_WIDTH, DEFAULT_MACRO_BUTTON_HEIGHT, name.c_str())
    {
        this->m_macro = macro;
        this->callback(macro_button_c::m_call_send_macro, this);
    }

    macro_button_c(macro_button_c const& rhs)
    : button_base_c(rhs)
    {
        this->callback(macro_button_c::m_call_send_macro, this);
    }

    macro_button_c& operator=(macro_button_c& rhs)
    {
        if (this != &rhs)
        {
            this->m_macro = rhs.m_macro;
        }
        return *this;
    }

    void m_send_macro()
    {
        this->m_macro.play();
    }

    static void m_call_send_macro(void* obj)
    {
        Serial.println("macro_button_c::m_call_send_macro");
        if (obj) static_cast<macro_button_c*>(obj)->m_send_macro();
    }

private:
    macro::macro_t m_macro; ///< The macro associated with the button
};

} // namespace gui
#endif // _MACRO_BUTTON_H_