#include "ILI9341_driver.h"

namespace display
{

Arduino_ILI9341& tft_c::instance()
{
    /// @brief Global scope instances of the tft data bus
    static Arduino_DataBus *bus = new Arduino_UNOPAR8();
    
    /// @brief Global scope instances of the tft display
    static Arduino_ILI9341 *tft = new Arduino_ILI9341(bus, TFT_RST, ORIENTATION, false /* ips */);
    
    return *tft;
}

void initialiseTFT()
{
   tft_c::instance().begin();
   tft_c::instance().setCursor(0, 0);
   tft_c::instance().fillScreen(RICH_BLACK);
}

void drawTextInCanvas(
    int16_t x, 
    int16_t y, 
    int16_t width, 
    int16_t height, 
    const char *text, 
    uint16_t textColor, 
    uint8_t textSize
) 
{
    // Set text properties
    display::tft_c::instance().setTextColor(textColor);
    display::tft_c::instance().setTextSize(textSize);

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
            display::tft_c::instance().setCursor(startX, currentY);
            display::tft_c::instance().print(line);
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
        display::tft_c::instance().setCursor(startX, currentY);
        display::tft_c::instance().print(line);
    }
}

void displayError(char const *msg)
{
   tft_c::instance().fillScreen(RICH_BLACK);
   tft_c::instance().setTextColor(ANTI_FLASH_WHITE);
   tft_c::instance().setCursor(0, 0);
   tft_c::instance().setTextSize(3);
   tft_c::instance().println(msg);
}

void displayMessage(char const *msg)
{
   tft_c::instance().fillScreen(RICH_BLACK);
   tft_c::instance().setTextColor(ANTI_FLASH_WHITE);
   tft_c::instance().setCursor(0, 0);
   tft_c::instance().setTextSize(3);
   tft_c::instance().println(msg);
}

uint8_t calculateTextSize(int button_height) {
    int half_height = button_height / 2;

    if (half_height >= 64) return 4;
    if (half_height >= 32) return 3;
    if (half_height >= 16) return 2;
    return 1;
}

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

void drawButton(
    int16_t const x, 
    int16_t const y, 
    int16_t x_, 
    int16_t y_, 
    uint16_t fill_colour, 
    uint16_t text_colour, 
    uint16_t border_colour, 
    String text, 
    uint8_t text_size
)
{
    tft_c::instance().fillRect(x, y, x_, y_, fill_colour);
    tft_c::instance().drawRect(x, y, x_, y_, border_colour);

    if (text != "")
    {   
        uint8_t scaled_font_size = text_size;
        if (text_size == 0) scaled_font_size = calculateTextSize(y_) + 1;
        tft_c::instance().setTextSize(scaled_font_size);
        drawTextInCanvas(x, y, x_, y_, text.c_str(), text_colour, scaled_font_size);
    }
}

void bmpDrawCallback(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h)
{
    tft_c::instance().draw16bitRGBBitmap(x, y, bitmap, w, h);
}

void drawBmp(
    String const file_path, 
    int16_t x, 
    int16_t y, 
    int16_t w, 
    int16_t h, 
    bool border, 
    int16_t yend
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
        tft_c::instance().drawRect(x, y, w, h, ANTI_FLASH_WHITE);
    }

    image_file.close();
}

} // namespace display
