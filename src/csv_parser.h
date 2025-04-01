#ifndef __CSV_PARSER_H__
#define __CSV_PARSER_H__

namespace csv {

const char DELIMETER = ',';

/// @brief Parse a string by delimeters and break it up into an array
/// @param input The string to parse
/// @param components The array to store the components in
/// @param components_size The size of the array to store the components in
/// @return size_t: The number of components parsed
size_t parseCSVLine(String input, String components[], size_t const components_size)
{
    size_t read_entries = 0;
    
    if (input.length() == 0 && input[0] != DELIMETER)
    {
        return read_entries; // nothing in the input, do nothing.
    }

    size_t num_of_delims = 0;
    {
        size_t idx = 0;
        // count delimeters
        while (idx != -1)
        {
            idx = input.indexOf(DELIMETER, idx + 1); // search for the delimeter after the current index.
            if (idx != -1) num_of_delims++;
        }
    }

    size_t num_of_entries = num_of_delims + 1;


    if (num_of_delims == 0) // no delimeters
    {
        components[0] = input; // there's only 1 component
        return num_of_entries;
    }
    
    size_t start_pos = 0;
    for (size_t i = 0; i < num_of_entries; i++) // for each entry
    {
        size_t idx = input.indexOf(DELIMETER, start_pos);
        components[i] = input.substring(start_pos, idx);
        read_entries++; // increment number of entries read.
        start_pos = idx + 1; // move to next position
    }

    for (size_t i = read_entries; i < components_size; i++)
    {
        components[i] = ""; // empty all the remaining components
    }

    return read_entries;
}

/// @brief Create a substring of the text between the two outer most quotes.
/// @param entry The entry to parse
/// @return String: The parsed entry
String parseCodeEntry(String entry)
{
    size_t opening = entry.indexOf('"');
    size_t closing = entry.lastIndexOf('"');

    String result = entry.substring(opening + 1, closing);
    return result;
}

} // end csv
#endif // __CSV_PARSER_H__