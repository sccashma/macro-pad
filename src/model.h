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
#include "macro.h"

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

    /// @brief Load the macros from the SD card
    /// @param ids The ids of the macros to load
    /// @param size The number of macros to load
    /// @param names The names of the loaded macros
    /// @param file_paths The file paths of the loaded macros
    /// @param macros The loaded macros
    /// @return size_t: The number of macros loaded
    /// @note This function will load the macros from the SD card and store them in the provided arrays
    size_t loadMacros(uint16_t const *ids, size_t const size, String *names, String *file_paths, macro::macro_c *macros)
    {
        return _loadMacros(ids, size, names, file_paths, macros);
    }

    size_t queryMacros(uint16_t *ids, String *names)
    {
        return _queryMacros(ids, names);
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

    /// @brief load the macro's in the active_macro's array
    size_t _loadMacros(uint16_t const *ids, size_t const size, String *names, String *file_paths, macro::macro_c *macros)
    {   
        size_t count = 0; // count how many macros are loaded
        File file = SD.open("macros.csv");
        
        while(!file)
        {
            delay(500);
            file = SD.open("macros.csv"); // try again
        }
        
        sd::readLine(&file); // read header
        
        size_t idx = 0; // index for the data arrays
        while(file.available())
        {
            String line = sd::readLine(&file);

            String id;
            String entries[64];
            csv::parseCSVLine(line, entries, 64);
            id = entries[0];
            
            for (int i = 0; i < size; i++)
            {
                if (id == String(ids[i])) // Only load the ones requested
                {
                    names[i] = entries[1];
                    file_paths[i] = entries[2];
                    
                    macro::macro_c macro;
                    String code_string = csv::parseCodeEntry(line);
                    macro.initialiseCodes(code_string);
                    macros[i] = macro; // assign the macro to the array
                    count++;
                    idx++;
                    break; // no need to check the rest of the ids
                }
            }
        }
        file.close();

        return count; // return the number of macros loaded
    }

        /// @brief load the macro's in the active_macro's array
        size_t _queryMacros(uint16_t *ids, String *names)
        {   
            size_t count = 0; // count how many macros are loaded
            File file = SD.open("macros.csv");
            
            while(!file)
            {
                delay(500);
                file = SD.open("macros.csv"); // try again
            }
            
            sd::readLine(&file); // read header
            
            size_t idx = 0; // index for the data arrays
            while(file.available())
            {
                String line = sd::readLine(&file);
                String entries[64];
                csv::parseCSVLine(line, entries, 64);
                ids[idx] = static_cast<uint16_t>(entries[0].toInt());
                names[idx] = entries[2];
            }
            file.close();
    
            return count; // return the number of macros loaded
        }
};

} // namespace model
#endif // __MODEL_H__