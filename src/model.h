/*
    Model.h
    Author: Scott Cashman
    Created: 2/4/2025
    Description: This class is designed to be used in the context of a MVP architecture.
    It is a model class that is used to store the data for the macro pad.
*/


//// Some notes on how the model should work ////
/*
* The model should be a class that is used to handle the data for the macro pad.
* For example, on a call to get a list of all the available macros, the model should return a list.
*/

#ifndef __MODEL_H__
#define __MODEL_H__

#include "sd_utils.h"

namespace model
{

/// @brief Model class for the macro pad
class model_c
{
public:
    model_c()
    {
        m_macro_count = _availableMacros();
    }

    ~model_c() = default;

    /// @brief Get the number of available macros
    /// @return int16_t: The number of available macros
    int16_t availableMacros()
    {
        return m_macro_count;
    }

private:
    int m_macro_count; ///< The number of macros in the macro file

    /// @brief Count the number of macros in the macro file
    /// @return int16_t: The number of macros
    uint16_t _availableMacros()
    {
        // Count entries in the macro file
        File file = SD.open("macros.csv");
        while(!file)
        {
            delay(500);
        }

        uint16_t count = 0;
        sd::readLine(&file); // read header

        while(file.available())
        {
            sd::readLine(&file);
            count++;
        }

        file.close();
        return count;
    }
};

} // namespace model
#endif // __MODEL_H__