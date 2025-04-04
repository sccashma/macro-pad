/*
    DISCLAIMER: bmp.h is from the Adafruit GFX library example "ImgViewerBmp".
    It is used to draw BMP images from the SD card to the screen. The code is not mine, but I have modified it to
    work with this project.
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
    // draw true colour bitmap at (u,v) handles 24/32 not 16bpp yet
    void drawbmtrue(File *f, int16_t const u, int16_t const v, uint32_t const xend, int16_t yend = 0)
    {
        if (yend == 0)
        {
            yend = bmheight;
        }
        
        int16_t i, ystart;
        uint32_t x, y;
        byte r, g, b;
        bm_bytes_per_line = ((bm_bits_per_pixel * bmwidth + 31) / 32) * 4; //bytes per line, due to 32bit chunks
        ystart = 0;
        if (bmheight > _heightLimit)
        {
            ystart = bmheight - _heightLimit; //don't draw if it's outside screen
        }
        for (y = ystart; y < yend; y++)
        {                                   //invert in calculation (y=0 is bottom)
            f->seek(bmdataptr + y * bm_bytes_per_line); //seek at start of line
            for (x = 0; x < xend; x++)
            {
                b = f->read();
                g = f->read();
                r = f->read();
                if (bm_bits_per_pixel == 32)
                {
                    f->read(); //dummy byte for 32bit
                }
                bmpRow[x] = (_useBigEndian) ? ((r & 0xf8) | (g >> 5) | ((g & 0x1c) << 11) | ((b & 0xf8) << 5)) : (((r & 0xf8) << 8) | ((g & 0xfc) << 3) | (b >> 3));
            }
            _bmpDrawCallback(u, v + bmheight - 1 - y, bmpRow, xend, 1);
        }
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
