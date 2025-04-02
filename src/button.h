#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>
#include "constants.h"

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
    /// @brief constructor for the button_base_c class with parameters
    /// @param x The x coordinate of the button in pixels
    /// @param y The y coordinate of the button in pixels
    /// @param width The width of the button in pixels
    /// @param height The height of the button in pixels
    button_base_c(int16_t x = 0, int16_t y = 0, int16_t width = 0, int16_t height = 0, const char* name = "")
    : m_active(true)
    , m_xmin(x)
    , m_ymin(y)
    , m_size_x(width)
    , m_size_y(height)
    , m_image_file("")
    , m_name(name)
    , m_callback_function(nullptr)
    , m_context(nullptr)
    , m_fill_colour(DEFAULT_FILL_COLOUR)
    , m_txt_colour(DEFAULT_TEXT_COLOUR)
    , m_border_colour(DEFAULT_BORDER_COLOUR)
    {
    }

    /// @brief copy constructor for the button_base_c class
    /// @param rhs The button to copy
    button_base_c(button_base_c const& rhs)
    : m_active(rhs.m_active)
    , m_xmin(rhs.m_xmin)
    , m_ymin(rhs.m_ymin)
    , m_size_x(rhs.m_size_x)
    , m_size_y(rhs.m_size_y)
    , m_image_file(rhs.m_image_file)
    , m_name(rhs.m_name)
    , m_callback_function(rhs.m_callback_function)
    , m_context(rhs.m_context)
    , m_fill_colour(rhs.m_fill_colour)
    , m_txt_colour(rhs.m_txt_colour)
    , m_border_colour(rhs.m_border_colour)
    {
    }

    /// @brief assignment operator for the button_base_c class
    /// @param rhs The button to assign
    /// @return button_base_c&: A reference to this button
    button_base_c& operator=(button_base_c const& rhs)
    {
        if (this != &rhs)
        {
            this->m_active = rhs.m_active;
            this->m_xmin = rhs.m_xmin;
            this->m_ymin = rhs.m_ymin;
            this->m_size_x = rhs.m_size_x;
            this->m_size_y = rhs.m_size_y;
            this->m_image_file = rhs.m_image_file;
            this->m_name = rhs.m_name;
            this->m_callback_function = rhs.m_callback_function;
            this->m_context = rhs.m_context;
            this->m_fill_colour = rhs.m_fill_colour;
            this->m_txt_colour = rhs.m_txt_colour;
            this->m_border_colour = rhs.m_border_colour;
        }
        return *this;
    }

    /// @brief Set the origin of the button in pixels
    /// @param x The x coordinate of the button in pixels
    /// @param y The y coordinate of the button in pixels
    void setPos(int16_t x, int16_t y)
    {
        this->m_xmin = x;
        this->m_ymin = y;
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
        if(m_callback_function && m_active)
        {
            m_callback_function(m_context);
        }
    }

    /// @brief Set the active state of the button
    /// @param active The active state of the button
    void active(bool active)
    {
        this->m_active = active;
    }

    /// @brief Get the active state of the button
    /// @return bool: The active state of the button
    bool active() const
    {
        return this->m_active;
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

    /// @brief Set the fill colour of the button
    /// @param colour The fill colour of the button in RGB565 format
    /// @note This determines the background colour of the button
    void fillColour(int colour)
    {
        this->m_fill_colour = colour;
    }

    /// @brief Get the fill colour of the button
    /// @return int: The fill colour of the button in RGB565 format
    /// @note This is the background colour of the button
    int fillColour() const
    {
        return this->m_fill_colour;
    }

    /// @brief Set the text colour of the button
    /// @param colour The text colour of the button in RGB565 format
    /// @note This determines the colour of the text displayed on the button
    void txtColour(int colour)
    {
        this->m_txt_colour = colour;
    }

    /// @brief Get the text colour of the button
    /// @return int: The text colour of the button in RGB565 format
    /// @note This is the colour of the text displayed on the button
    int txtColour() const
    {
        return this->m_txt_colour;
    }

    /// @brief Set the border colour of the button
    /// @param colour The border colour of the button in RGB565 format
    /// @note This determines the colour of the button's border
    void borderColour(int colour)
    {
        this->m_border_colour = colour;
    }

    /// @brief Get the border colour of the button
    /// @return int: The border colour of the button in RGB565 format
    /// @note This is the colour of the button's border
    int borderColour() const
    {
        return this->m_border_colour;
    }

private:
    static constexpr int DEFAULT_FILL_COLOUR = ARYLIDE_YELLOW;
    static constexpr int DEFAULT_TEXT_COLOUR = RICH_BLACK;
    static constexpr int DEFAULT_BORDER_COLOUR = RICH_BLACK;

    bool m_active;
    int m_fill_colour;
    int m_txt_colour;
    int m_border_colour;
    int16_t m_xmin;
    int16_t m_ymin;
    int16_t m_size_x;
    int16_t m_size_y;
    String m_image_file;
    String m_name;
};

} // namespace gui
#endif // __BUTTON_H__
