
#ifndef __SD_UTILS_H__
#define __SD_UTILS_H__

#include "ILI9341_driver.h"
#include <SD.h>

namespace sd
{
    
/// @brief Attempt to initialise the sd card
/// \return bool: Did the SD card initialise correctly
inline bool initialiseSD()
{
    bool good = SD.begin();
    return good;
}

/// @brief Read a line from a given file on the SD card until a given delimiter is reached
/// @param file The file handle to read from
/// @param delim The delimiter to read until (default: ',')
/// @return String: The line
inline String readLineUntil(File *file, char delim = ',')
{
    char buffer[250] = {NULL};
    size_t idx = 0;
    while (file->peek() != delim && file->available())
    {
        buffer[idx++] = file->read();
        if (idx >= sizeof(buffer) - 1)
        {
            break;
        }
    }

    // Read the delimiting character
    if (file->peek() == delim)
    {
        (void) file->read();
    }

    buffer[idx] = '\0'; // Null-terminate the string
    String ret(buffer);
    return ret;
}

/// @brief Read the next line from a given file on the SD card
/// \param file The file handle to read from
/// \return String: The line
inline String readLine(File *file)
{
    return readLineUntil(file, '\n');
}

} // namespace sd
#endif // __SD_UTILS_H__