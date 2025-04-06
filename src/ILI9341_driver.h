// This is a very light interface to the Arduino GFX library. The only board specifically
// targeted for support is the Duinotech 2.8 inch TFT shield.

#ifndef __ILI9341_DRIVER_H__
#define __ILI9341_DRIVER_H__

#include <Arduino_GFX_Library.h>
#include "bmp.h"
#include "constants.h"
#include "sd_utils.h"

namespace display
{

// Pin interface definitions
#define TFT_RST A4

// Orientation of the display
uint8_t constexpr ORIENTATION = 3;

class tft_c
{
public:
    static Arduino_ILI9341& instance();

private:
    tft_c(); // Private constructor to prevent instantiation
    tft_c& operator=(tft_c const&); // Private copy assignment operator
    tft_c(tft_c const&); // Private copy constructor
};


/// @brief Initialise the TFT display
void initialiseTFT();

/// @brief Draws a virtual canvas and writes centered, wrapped text within it.
/// @param x The x-coordinate of the top-left corner of the canvas.
/// @param y The y-coordinate of the top-left corner of the canvas.
/// @param width The width of the canvas.
/// @param height The height of the canvas.
/// @param text The text to display.
/// @param textColor The color of the text.
/// @param textSize The size of the text.
void drawTextInCanvas(
    int16_t x, 
    int16_t y, 
    int16_t width, 
    int16_t height, 
    const char *text, 
    uint16_t textColor, 
    uint8_t textSize = 1
);

/// @brief Display an error message on the screen
/// @param msg The message to display
void displayError(char const *msg);

/// @brief Display message on the screen
/// @param msg The message to display
void displayMessage(char const *msg);

/// @brief Calculates the "text size" for use in the GFX library.
///        Text size returned is based on the height of the button. Up to 1/2 the button height.
/// @param button_height The height of the button in pixels
/// @return uint8_t The text size to use in the GFX library
uint8_t calculateTextSize(int button_height);

/// @brief Convert the text size to a pixel height for use in the GFX library
/// @param text_size The text size as used in the GFX library
/// @return uint8_t The pixel height of the text
uint8_t textPixelHeight(uint8_t const text_size);

/// @brief Draw a button on the screen
/// @param x The x coordinate of the button
/// @param y The y coordinate of the button
/// @param x_ The width of the button
/// @param y_ The height of the button
/// @param colour The colour of the button
/// @param text The text to display on the button, if 0, or not specified, the text will be half the button height
/// @param text_size The size of the text to display on the button
void drawButton(
    int16_t const x, 
    int16_t const y, 
    int16_t x_, 
    int16_t y_, 
    uint16_t fill_colour, 
    uint16_t text_colour, 
    uint16_t border_colour, 
    String text = "", 
    uint8_t text_size = 0
);

/// @brief Callback function to send to the bmp class draw function
/// @note See bmp.h for usage
static void bmpDrawCallback(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);

/// @brief Draw a bitmap on the screen
/// @param x The x coordinate of the bitmap
/// @param y The y coordinate of the bitmap
/// @param bitmap The bitmap to draw
/// @param w The width of the bitmap
/// @param h The height of the bitmap
/// @param border If true, draw a border around the bitmap (default: false)
/// @param yend The height to stop drawing at (default: -1, which means the whole image)
/// @note This function will draw the image from the bottom up, so if using yend, it will stop after drawing yend lines
void drawBmp(
    String const file_path, 
    int16_t x, 
    int16_t y, 
    int16_t w, 
    int16_t h, 
    bool border = false, 
    int16_t yend = -1
);

} // namespace display
#endif // __ILI9341_DRIVER_H__