#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>

namespace gui
{

/// @brief Base class implementation for all buttons
/// @note This class is not intended to be used directly, but as a base class for other button classes.
class button_base_c
{
protected:
    /// @brief type alias for function pointer
    /// @note This is used to define the function pointer type for the action function
    using buttonCallback = void (*)(void*);
    buttonCallback m_callback_function;

    /// @brief context in which to invoke the action function
    void* m_context;

public:
    /// @brief constructor for the button_base_c class
    button_base_c()
    : m_xmin(0)
    , m_ymin(0)
    , m_size_x(0)
    , m_size_y(0)
    , m_image_file("")
    , m_name("")
    , m_callback_function(nullptr)
    , m_context(nullptr)
    , m_colour(EERIE_BLACK)
    {
    }
    
    /// @brief Set the origin of the button in pixels
    /// @param x The x coordinate of the button in pixels
    /// @param y The y coordinate of the button in pixels
    void setPos(int16_t x, int16_t y)
    {
        m_xmin = x;
        m_ymin = y;
    }
    
    /// @brief Set the callback function to be called when the button is pressed
    /// @param action_function The function to be called when the button is pressed
    /// @param ctx The context in which to invoke the action function
    void callback(buttonCallback callback_function, void* ctx = nullptr)
    {
        this->m_callback_function = callback_function;
        this->m_context = ctx;
    }

    /// @brief Invoke the callback function with the context
    void press()
    {
        if(m_callback_function)
        {
            m_callback_function(m_context);
        }
    }

    /// @brief Set the file path name for the image to be displayed on the button
    /// @param file The file path name
    void imageFilePath(String const file)
    {
      this->m_image_file = file;
    }
  
    /// @brief Get the file path name for the image to be displayed on the button
    /// @return String: The file path name
    String imageFilePath() const
    {
      return this->m_image_file;
    }
  
    /// @brief Set the name of the button
    /// @param name The name of the button
    void name(String const name)
    {
      this->m_name = name;
    }
  
    /// @brief Get the name of the button
    /// @return String: The name of the button
    String name() const
    {
      return this->m_name;
    }

    /// @brief Get the macro associated with the button
    /// @return macro_t: The macro associated with the button
    macro::macro_t macro()
    {
        return this->m_macro;
    }

    /// @brief Set the macro associated with the button
    /// @param set The macro to be associated with the button
    void macro(macro::macro_t& set)
    {
        this->m_macro = set;
    }

    /// @brief Get the x coordinate of the button
    /// @return int16_t: The x coordinate of the button
    /// @note This is the top left corner of the button
    int16_t minX() const
    {
        return this->m_xmin;
    }

    /// @brief Set the x coordinate of the button
    /// @param x The x coordinate of the button
    /// @note This is the top left corner of the button
    void minX(int16_t const x)
    {
        this->m_xmin = x;
    }

    /// @brief Get the y coordinate of the button
    /// @return int16_t: The y coordinate of the button
    /// @note This is the top left corner of the button
    int16_t minY() const
    {
        return this->m_ymin;
    }

    /// @brief Set the y coordinate of the button
    /// @param y The y coordinate of the button
    /// @note This is the top left corner of the button
    void minY(int16_t const y)
    {
        this->m_ymin = y;
    }

    /// @brief Get the width of the button
    /// @return int16_t: The width of the button
    /// @note This is the width of the button in pixels
    int16_t sizeX() const
    {
        return this->m_size_x;
    }

    /// @brief Set the width of the button
    /// @param x The width of the button
    /// @note This is the width of the button in pixels
    void sizeX(int16_t const x)
    {
        this->m_size_x = x;
    }

    /// @brief Get the height of the button
    /// @return int16_t: The height of the button
    /// @note This is the height of the button in pixels
    int16_t sizeY() const
    {
        return this->m_size_y;
    }

    /// @brief Set the height of the button
    /// @param y The height of the button
    /// @note This is the height of the button in pixels
    void sizeY(int16_t const y)
    {
        this->m_size_y = y;
    }

    /// @brief Set the colour of the button
    /// @param colour The colour of the button
    /// @note This is the colour of the button in RGB565 format
    void setColour(int colour)
    {
        this->m_colour = colour;
    }

    /// @brief Get the colour of the button
    /// @return int: The colour of the button
    /// @note This is the colour of the button in RGB565 format
    int colour() const
    {
        return this->m_colour;
    }

protected:
    int m_colour;
    int16_t m_xmin;
    int16_t m_ymin;
    int16_t m_size_x;
    int16_t m_size_y;
    String m_image_file;
    String m_name;
    macro::macro_t m_macro;
};

} // namespace gui
#endif // __BUTTON_H__
