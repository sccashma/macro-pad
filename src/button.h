#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>
#include "constants.h"
#include "limits.h"

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
    void *m_callback_context;

    /// @brief type alias for function pointer to draw the button
    /// @note First void* is the thing drawing the button, the second void* is the button (this)
    using drawButtonCallback = void (*)(void*, void*);
    drawButtonCallback m_draw_function;

    /// @brief context in which to invoke the draw function
    void *m_draw_context;

public:
    /// @brief constructor for the button_base_c class with parameters
    /// @param x The x coordinate of the button in pixels
    /// @param y The y coordinate of the button in pixels
    /// @param width The width of the button in pixels
    /// @param height The height of the button in pixels
    button_base_c(int16_t x = 0, int16_t y = 0, int16_t width = 0, int16_t height = 0, const char* name = "")
    : m_id(UINT_MAX)
    , m_active(true)
    , m_pressed(false)
    , m_x(x)
    , m_y(y)
    , m_width(width)
    , m_height(height)
    , m_image_file("")
    , m_name(name)
    , m_callback_function(nullptr)
    , m_callback_context(nullptr)
    , m_fill_colour(DEFAULT_FILL_COLOUR)
    , m_txt_colour(DEFAULT_TEXT_COLOUR)
    , m_border_colour(DEFAULT_BORDER_COLOUR)
    , m_fill_colour_pressed(DEFAULT_FILL_COLOUR_PRESSED)
    , m_txt_colour_pressed(DEFAULT_TEXT_COLOUR_PRESSED)
    , m_border_colour_pressed(DEFAULT_BORDER_COLOUR_PRESSED)
    , m_fill_colour_disabled(DEFAULT_FILL_COLOUR_DISABLED)
    , m_txt_colour_disabled(DEFAULT_TEXT_COLOUR_DISABLED)
    , m_border_colour_disabled(DEFAULT_BORDER_COLOUR_DISABLED)
    {
    }

    /// @brief copy constructor for the button_base_c class
    /// @param rhs The button to copy
    button_base_c(button_base_c const& rhs)
    : m_id(rhs.m_id)
    , m_active(rhs.m_active)
    , m_pressed(rhs.m_pressed)
    , m_x(rhs.m_x)
    , m_y(rhs.m_y)
    , m_width(rhs.m_width)
    , m_height(rhs.m_height)
    , m_image_file(rhs.m_image_file)
    , m_name(rhs.m_name)
    , m_callback_function(rhs.m_callback_function)
    , m_callback_context(rhs.m_callback_context)
    , m_fill_colour(rhs.m_fill_colour)
    , m_txt_colour(rhs.m_txt_colour)
    , m_border_colour(rhs.m_border_colour)
    , m_fill_colour_pressed(rhs.m_fill_colour_pressed)
    , m_txt_colour_pressed(rhs.m_txt_colour_pressed)
    , m_border_colour_pressed(rhs.m_border_colour_pressed)
    , m_fill_colour_disabled(rhs.m_fill_colour_disabled)
    , m_txt_colour_disabled(rhs.m_txt_colour_disabled)
    , m_border_colour_disabled(rhs.m_border_colour_disabled)
    {
    }

    /// @brief assignment operator for the button_base_c class
    /// @param rhs The button to assign
    /// @return button_base_c&: A reference to this button
    button_base_c& operator=(button_base_c const& rhs)
    {
        if (this != &rhs)
        {
            this->m_id = rhs.m_id;
            this->m_active = rhs.m_active;
            this->m_pressed = rhs.m_pressed;
            this->m_x = rhs.m_x;
            this->m_y = rhs.m_y;
            this->m_width = rhs.m_width;
            this->m_height = rhs.m_height;
            this->m_image_file = rhs.m_image_file;
            this->m_name = rhs.m_name;
            this->m_callback_function = rhs.m_callback_function;
            this->m_callback_context = rhs.m_callback_context;
            this->m_fill_colour = rhs.m_fill_colour;
            this->m_txt_colour = rhs.m_txt_colour;
            this->m_border_colour = rhs.m_border_colour;
            this->m_fill_colour_pressed = rhs.m_fill_colour_pressed;
            this->m_txt_colour_pressed = rhs.m_txt_colour_pressed;
            this->m_border_colour_pressed = rhs.m_border_colour_pressed;
            this->m_fill_colour_disabled = rhs.m_fill_colour_disabled;
            this->m_txt_colour_disabled = rhs.m_txt_colour_disabled;
            this->m_border_colour_disabled = rhs.m_border_colour_disabled;
        }
        return *this;
    }

    /// @brief Set the origin of the button in pixels
    /// @param x The x coordinate of the button in pixels
    /// @param y The y coordinate of the button in pixels
    void setPos(int16_t x, int16_t y)
    {
        this->m_x = x;
        this->m_y = y;
    }
    
    /// @brief Set the callback function to be called when the button is pressed
    /// @param action_function The function to be called when the button is pressed
    /// @param ctx The context in which to invoke the action function
    void callback(buttonCallback function, void *ctx = nullptr)
    {
        this->m_callback_function = function;
        this->m_callback_context = ctx;
    }

    /// @brief Invoke the callback function with the context
    /// @return bool: True if the callback function was invoked, false otherwise
    bool press()
    {
        bool ret = false;
        if (m_callback_function && m_active)
        {
            m_pressed = true;
            draw();
            ret = true;
            m_callback_function(m_callback_context);
            m_pressed = false;
        }
        return ret;
    }

    /// @brief Set the draw function to be called that draws the button
    /// @param function The function to draw the button
    /// @param ctx The context in which to invoke the draw function
    void drawCallback(drawButtonCallback function, void *ctx = nullptr)
    {
        this->m_draw_function = function;
        this->m_draw_context = ctx;
    }

    /// @brief Invoke the draw function with the context
    void draw()
    {
        if(m_draw_function)
        {
            m_draw_function(m_draw_context, this);
        }
    }

    /// @brief Set the id of the button
    /// @param id The id of the button
    void id(int16_t id) { this->m_id = id; }

    /// @brief Get the id of the button
    /// @return int16_t: The id of the button
    uint16_t id() const { return this->m_id; }
    
    /// @brief Set the active state of the button
    /// @param active The active state of the button
    void active(bool active) { this->m_active = active; }
    
    /// @brief Get the active state of the button
    /// @return bool: The active state of the button
    bool active() const { return this->m_active; }
    
    /// @brief Set the file path name for the image to be displayed on the button
    /// @param file The file path name
    void imageFilePath(String const file) { this->m_image_file = file; }
    
    /// @brief Get the file path name for the image to be displayed on the button
    /// @return String: The file path name
    String imageFilePath() const { return this->m_image_file; }
    
    /// @brief Set the name of the button
    /// @param name The name of the button
    void name(String const name) { this->m_name = name; }
    
    /// @brief Get the name of the button
    /// @return String: The name of the button
    String name() const { return this->m_name; }
    
    /// @brief Get the x coordinate of the button
    /// @return int16_t: The x coordinate of the button
    /// @note This is the top left corner of the button
    int16_t minX() const { return this->m_x; }
    
    /// @brief Set the x coordinate of the button
    /// @param x The x coordinate of the button
    /// @note This is the top left corner of the button
    void minX(int16_t const x) { this->m_x = x; }
    
    /// @brief Get the y coordinate of the button
    /// @return int16_t: The y coordinate of the button
    /// @note This is the top left corner of the button
    int16_t minY() const { return this->m_y; }
    
    /// @brief Set the y coordinate of the button
    /// @param y The y coordinate of the button
    /// @note This is the top left corner of the button
    void minY(int16_t const y) { this->m_y = y; }

    /// @brief Get the maximum x coordinate of the button
    /// @return int16_t: The maximum x coordinate of the button
    /// @note This is the bottom right corner of the button
    int16_t maxX() const { return this->m_x + this->m_width; }

    /// @brief Get the maximum y coordinate of the button
    /// @return int16_t: The maximum y coordinate of the button
    /// @note This is the bottom right corner of the button
    int16_t maxY() const { return this->m_y + this->m_height; }
    
    /// @brief Get the width of the button
    /// @return int16_t: The width of the button
    /// @note This is the width of the button in pixels
    int16_t width() const { return this->m_width; }
    
    /// @brief Set the width of the button
    /// @param x The width of the button
    /// @note This is the width of the button in pixels
    void width(int16_t const x) { this->m_width = x; }

    /// @brief Get the height of the button
    /// @return int16_t: The height of the button
    /// @note This is the height of the button in pixels
    int16_t height() const { return this->m_height; }

    /// @brief Set the height of the button
    /// @param y The height of the button
    /// @note This is the height of the button in pixels
    void height(int16_t const y) { this->m_height = y; }

    /// @brief Set the fill colour of the button
    /// @param colour The fill colour of the button in RGB565 format
    /// @note This determines the background colour of the button
    void fillColour(int colour) { this->m_fill_colour = colour; }

    /// @brief Get the fill colour of the button
    /// @return int: The fill colour of the button in RGB565 format
    /// @note This is the background colour of the button
    int fillColour() const
    {
        if (this->m_pressed) return this->m_fill_colour_pressed;
        if (!this->m_active) return this->m_fill_colour_disabled;
        return this->m_fill_colour;
    }

    /// @brief Set the text colour of the button
    /// @param colour The text colour of the button in RGB565 format
    /// @note This determines the colour of the text displayed on the button
    void textColour(int colour) { this->m_txt_colour = colour; }

    /// @brief Get the text colour of the button
    /// @return int: The text colour of the button in RGB565 format
    /// @note This is the colour of the text displayed on the button
    int textColour() const 
    { 
        if (this->m_pressed) return this->m_txt_colour_pressed;
        if (!this->m_active) return this->m_txt_colour_disabled;
        return this->m_txt_colour; 
    }

    /// @brief Set the border colour of the button
    /// @param colour The border colour of the button in RGB565 format
    /// @note This determines the colour of the button's border
    void borderColour(int colour) { this->m_border_colour = colour; }

    /// @brief Get the border colour of the button
    /// @return int: The border colour of the button in RGB565 format
    /// @note This is the colour of the button's border
    int borderColour() const 
    { 
        if (this->m_pressed) return this->m_border_colour_pressed;
        if (!this->m_active) return this->m_border_colour_disabled;
        return this->m_border_colour; 
    }

    /// @brief Set the colours for when the button is pressed
    /// @param fill The fill colour
    /// @param txt The text colour
    /// @param border The border colour
    /// @note This sets the colours for when the button is pressed, colours are in RGB565 format
    void setPressedColours(int fill, int txt, int border)
    {
        this->m_fill_colour_pressed = fill;
        this->m_txt_colour_pressed = txt;
        this->m_border_colour_pressed = border;
    }

    /// @brief Set the colours for when the button is disabled
    /// @param fill The fill colour
    /// @param txt The text colour
    /// @param border The border colour
    /// @note This sets the colours for when the button is disabled, colours are in RGB565 format
    void setDisabledColours(int fill, int txt, int border)
    {
        this->m_fill_colour_disabled = fill;
        this->m_txt_colour_disabled = txt;
        this->m_border_colour_disabled = border;
    }

private:
    static constexpr int DEFAULT_FILL_COLOUR = ARYLIDE_YELLOW;
    static constexpr int DEFAULT_TEXT_COLOUR = RICH_BLACK;
    static constexpr int DEFAULT_BORDER_COLOUR = RICH_BLACK;
    static constexpr int DEFAULT_FILL_COLOUR_PRESSED = BRIGHT_YELLOW;
    static constexpr int DEFAULT_TEXT_COLOUR_PRESSED = RICH_BLACK;
    static constexpr int DEFAULT_BORDER_COLOUR_PRESSED = RICH_BLACK;
    static constexpr int DEFAULT_FILL_COLOUR_DISABLED = BATTLESHIP_GREY;
    static constexpr int DEFAULT_TEXT_COLOUR_DISABLED = SILVER;
    static constexpr int DEFAULT_BORDER_COLOUR_DISABLED = SILVER;

    uint16_t m_id;
    bool m_active;
    bool m_pressed;
    int m_fill_colour;
    int m_txt_colour;
    int m_border_colour;
    int16_t m_x;
    int16_t m_y;
    int16_t m_width;
    int16_t m_height;
    String m_image_file;
    String m_name;

    int m_fill_colour_pressed;
    int m_txt_colour_pressed;
    int m_border_colour_pressed;
    int m_fill_colour_disabled;
    int m_txt_colour_disabled;
    int m_border_colour_disabled;
};

} // namespace gui
#endif // __BUTTON_H__
