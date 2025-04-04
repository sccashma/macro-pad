/*
    DISCLAIMER: bmp.h is from the Adafruit GFX library example "ImgViewerBmp".
    It is used to draw BMP images from the SD card to the screen. The code is not mine, but I have modified it to
    work with this project and provided some optimisations.
    Original works can be found here: https://github.com/moononournation/Arduino_GFX
*/

#ifndef __BMP_H__
#define __BMP_H__

#include <SD.h>

namespace bmp
{

typedef void(BMP_DRAW_CALLBACK)(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);

class BmpClass
{
public:
    void draw(
        File *f, BMP_DRAW_CALLBACK *bmpDrawCallback, bool useBigEndian,
        int16_t x, int16_t y, int16_t width, int16_t heightLimit, int16_t yend = 0)
    {
        _bmpDrawCallback = bmpDrawCallback;
        _useBigEndian = useBigEndian;
        _heightLimit = heightLimit;

        int16_t u, v;
        uint32_t xend;

        getbmpparms(f);

        //validate bitmap
        if ((bmtype == 19778) && (bmwidth > 0) && (bmheight > 0) && (bm_bits_per_pixel > 0))
        {
            u = x;
            v = y;
            xend = width;

            bmpRow = (uint16_t *)malloc(xend * 2);
            if (bmpRow)
            {
                drawbmtrue(f, u, v, xend, yend);
            }
            free(bmpRow);
        }
    }

private:
    /// @brief Draw the bitmap image to the screen
    /// @param f file pointer to the bitmap file
    /// @param u starting x coordinate
    /// @param v starting y coordinate
    /// @param xend width of the image to draw
    /// @param yend height of the image to draw
    /// @note This function will draw the image from the bottom up, so if using yend, it will stop after drawing yend
    /// lines (Default: 0, which means the whole image)
    void drawbmtrue(File *f, int16_t const u, int16_t const v, uint32_t const xend, int16_t yend = 0)
    {
        if (yend == 0)
        {
            yend = bmheight;
        }

        int16_t ystart;
        uint32_t y;
        bm_bytes_per_line = ((bm_bits_per_pixel * bmwidth + 31) / 32) * 4; // bytes per line, due to 32-bit chunks
        ystart = 0;
        if (bmheight > _heightLimit)
        {
            ystart = bmheight - _heightLimit; // don't draw if it's outside the screen
        }

        // Allocate a buffer for one line of pixel data
        uint8_t *lineBuffer = (uint8_t *)malloc(bm_bytes_per_line);
        if (!lineBuffer)
        {
            return; // Exit if memory allocation fails
        }

        for (y = ystart; y < yend; y++)
        {
            // Seek to the start of the current line
            f->seek(bmdataptr + y * bm_bytes_per_line);

            // Read the entire line into the buffer
            f->read(lineBuffer, bm_bytes_per_line);

            // Process the line and populate bmpRow
            for (uint32_t x = 0; x < xend; x++)
            {
                uint8_t b = lineBuffer[x * (bm_bits_per_pixel / 8)];
                uint8_t g = lineBuffer[x * (bm_bits_per_pixel / 8) + 1];
                uint8_t r = lineBuffer[x * (bm_bits_per_pixel / 8) + 2];

                bmpRow[x] = convertToRGB565(r, g, b); // Convert to RGB565 format
            }

            // Invoke the callback for the current line
            _bmpDrawCallback(u, v + bmheight - 1 - y, bmpRow, xend, 1);
        }

        // Free the allocated buffer
        free(lineBuffer);
    }

    uint16_t convertToRGB565(uint8_t r, uint8_t g, uint8_t b) {
        return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }

    void getbmpparms(File *f)
    {               //load into globals as ints-some parameters are 32 bit, but we can't handle this size anyway
        byte h[48]; //header is 54 bytes typically, but we don't need it all
        int16_t i;
        f->seek(0); //set start of file
        for (i = 0; i < 48; i++)
        {
            h[i] = f->read(); //read header
        }
        bmtype = h[0] + (h[1] << 8);      //offset 0 'BM'
        bmdataptr = h[10] + (h[11] << 8); //offset 0xA pointer to image data
        bmhdrsize = h[14] + (h[15] << 8); //dib header size (0x28 is usual)
        //files may vary here, if !=28, unsupported type, put default values
        bmwidth = 0;
        bmheight = 0;
        bm_bits_per_pixel = 0;
        bmpltsize = 0;
        if ((bmhdrsize == 0x28) || (bmhdrsize == 0x38))
        {
            bmwidth = h[18] + (h[19] << 8);   //width
            bmheight = h[22] + (h[23] << 8);  //height
            bm_bits_per_pixel = h[28] + (h[29] << 8);     //bits per pixel
            bmpltsize = h[46] + (h[47] << 8); //palette size
        }
    }

    BMP_DRAW_CALLBACK *_bmpDrawCallback;
    bool _useBigEndian;
    int16_t _heightLimit;

    uint16_t bmtype, bmdataptr;                              //from header
    uint32_t bmhdrsize, bmwidth, bmheight, bm_bits_per_pixel, bmpltsize; //from DIB Header
    uint16_t bm_bytes_per_line;                                          //bytes per line- derived
    uint16_t *bmplt;                                        //palette- stored encoded for LCD
    uint16_t *bmpRow;
};

} // namespace bmp
#endif //__BMP_H__
