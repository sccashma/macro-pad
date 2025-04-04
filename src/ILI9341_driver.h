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

/// @brief Global scope instances of the tft data bus
Arduino_DataBus *bus = new Arduino_UNOPAR8();

/// @brief Global scope instances of the tft display
Arduino_ILI9341 *tft = new Arduino_ILI9341(bus, TFT_RST, ORIENTATION, false /* ips */);

/// @brief Initialise the TFT display
void initialiseTFT()
{
   tft->begin();
   tft->setCursor(0, 0);
   tft->fillScreen(RICH_BLACK);
}

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
) 
{
    // Set text properties
    display::tft->setTextColor(textColor);
    display::tft->setTextSize(textSize);

    // Calculate the maximum number of characters per line
    int16_t charWidth = 6 * textSize; // Default character width in GFX library
    int16_t charHeight = 8 * textSize; // Default character height in GFX library
    int16_t maxCharsPerLine = width / charWidth;

    // Split the text into lines, wrapping only between words
    String wrappedText = "";
    String currentLine = "";
    String word = "";
    const char *ptr = text;

    while (*ptr) {
        if (*ptr == ' ' || *ptr == '\n') {
            // Add the current word to the line if it fits
            if (currentLine.length() + word.length() <= maxCharsPerLine) {
                currentLine += word + " ";
            } else {
                // Wrap the line
                wrappedText += currentLine + "\n";
                currentLine = word + " ";
            }
            word = ""; // Reset the word
            if (*ptr == '\n') {
                wrappedText += currentLine + "\n";
                currentLine = "";
            }
        } else {
            word += *ptr; // Build the current word
        }
        ptr++;
    }

    // Add the last word and line
    if (word.length() > 0) {
        if (currentLine.length() + word.length() <= maxCharsPerLine) {
            currentLine += word;
        } else {
            wrappedText += currentLine + "\n";
            currentLine = word;
        }
    }
    if (currentLine.length() > 0) {
        wrappedText += currentLine;
    }

    // Calculate the total height of the wrapped text
    int16_t totalLines = 0;
    for (int i = 0; i < wrappedText.length(); i++) {
        if (wrappedText[i] == '\n') {
            totalLines++;
        }
    }
    totalLines++; // Account for the last line
    int16_t totalTextHeight = totalLines * charHeight;

    // Calculate the starting y-coordinate to center the text
    int16_t startY = y + (height - totalTextHeight) / 2;

    // Draw the text line by line
    int16_t currentY = startY;
    String line = "";
    for (int i = 0; i < wrappedText.length(); i++) {
        if (wrappedText[i] == '\n') {
            // Center the line horizontally
            int16_t lineWidth = line.length() * charWidth;
            int16_t startX = x + (width - lineWidth) / 2;
            display::tft->setCursor(startX, currentY);
            display::tft->print(line);
            line = "";
            currentY += charHeight;
        } else {
            line += wrappedText[i];
        }
    }
    // Draw the last line
    if (line.length() > 0) {
        int16_t lineWidth = line.length() * charWidth;
        int16_t startX = x + (width - lineWidth) / 2;
        display::tft->setCursor(startX, currentY);
        display::tft->print(line);
    }
}

/// @brief Display an error message on the screen
/// @param msg The message to display
void displayError(char const *msg)
{
   tft->fillScreen(RICH_BLACK);
   tft->setTextColor(ANTI_FLASH_WHITE);
   tft->setCursor(0, 0);
   tft->setTextSize(3);
   tft->println(msg);
}

/// @brief Display message on the screen
/// @param msg The message to display
void displayMessage(char const *msg)
{
   tft->fillScreen(RICH_BLACK);
   tft->setTextColor(ANTI_FLASH_WHITE);
   tft->setCursor(0, 0);
   tft->setTextSize(3);
   tft->println(msg);
}

/// @brief Calculates the "text size" for use in the GFX library.
///        Text size returned is based on the height of the button. Up to 1/2 the button height.
/// @param button_height The height of the button in pixels
/// @return uint8_t The text size to use in the GFX library
uint8_t calculateTextSize(int button_height) {
    int half_height = button_height / 2;

    if (half_height >= 64) return 4;
    if (half_height >= 32) return 3;
    if (half_height >= 16) return 2;
    return 1;
}

/// @brief Convert the text size to a pixel height for use in the GFX library
/// @param text_size The text size as used in the GFX library
/// @return uint8_t The pixel height of the text
uint8_t textPixelHeight(uint8_t const text_size)
{
    switch(text_size)
    {
        case 1: return 8;
        case 2: return 16;
        case 3: return 32;
        case 4: return 64;
        default: return 8;
    }
}

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
)
{
    tft->fillRect(x, y, x_, y_, fill_colour);
    tft->drawRect(x, y, x_, y_, border_colour);

    if (text != "")
    {   
        uint8_t scaled_font_size = text_size;
        if (text_size == 0) scaled_font_size = calculateTextSize(y_);
        tft->setTextSize(scaled_font_size);
        drawTextInCanvas(x, y, x_, y_, text.c_str(), text_colour, scaled_font_size);
    }
}

/// @brief Callback function to send to the bmp class draw function
/// @note See bmp.h for usage
static void bmpDrawCallback(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
    tft->draw16bitRGBBitmap(x, y, bitmap, w, h);
}

/// @brief Draw a bitmap on the screen
/// @param x The x coordinate of the bitmap
/// @param y The y coordinate of the bitmap
/// @param bitmap The bitmap to draw
/// @param w The width of the bitmap
/// @param h The height of the bitmap
/// @param border If true, draw a border around the bitmap (default: false)
/// @param yend The height to stop drawing at (default: -1, which means the whole image)
/// @note This function will draw the image from the bottom up, so if using yend, it will stop after drawing yend lines
static void drawBmp(
    String const file_path, 
    int16_t x, 
    int16_t y, 
    int16_t w, 
    int16_t h, 
    bool border = false, 
    int16_t yend = -1
)
{
    static bmp::BmpClass bmp;
    File image_file = SD.open(file_path);

    if(!image_file)
    {
        // Couldn't open the file, display a generic button instead
        drawButton(x, y, w, h, BATTLESHIP_GREY, SILVER, SILVER, "?");
        return;
    }

    if (yend == -1)
    {
        yend = h; // default to the bottom of the screen
    }

    bmp.draw(&image_file, bmpDrawCallback, false, x, y, w, h, yend);

    if (border)
    {
        tft->drawRect(x, y, w, h, ANTI_FLASH_WHITE);
    }

    image_file.close();
}

} // namespace display
#endif // __ILI9341_DRIVER_H__