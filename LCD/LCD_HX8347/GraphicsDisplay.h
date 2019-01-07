/* mbed GraphicsDisplay Display Library Base Class
 * Copyright (c) 2007-2009 sford
 * Released under the MIT License: http://mbed.org/license/mit
 *
 * A library for providing a common base class for Graphics displays
 * To port a new display, derive from this class and implement
 * the constructor (setup the display), pixel (put a pixel
 * at a location), width and height functions. Everything else
 * (locate, printf, putc, cls, window, putp, fill, blit, blitbit) 
 * will come for free. You can also provide a specialised implementation
 * of window and putp to speed up the results
 */

#ifndef MBED_GRAPHICSDISPLAY_H
#define MBED_GRAPHICSDISPLAY_H

#include "TextDisplay.h"


#define RED     0xf800
#define GREEN   0x07e0
#define BLUE    0x001f
#define BLACK   0x0000
#define YELLOW  0xffe0
#define WHITE   0xffff

#define FONT_CHAR_WIDTH    8
#define FONT_CHAR_HEIGHT  16

class GraphicsDisplay : public TextDisplay {

public:         
          
    GraphicsDisplay(const char* name);
     
    virtual void pixel(int x, int y, int colour) = 0;
    virtual int width() = 0;
    virtual int height() = 0;
        
    virtual void window(unsigned int x,unsigned int y,unsigned int w,unsigned int h);
    virtual void putp(int colour);
    
    virtual void cls();
    virtual void fill(int x, int y, int w, int h, int colour);
    virtual void blit(int x, int y, int w, int h, const int *colour);    
    virtual void blitbit(int x, int y, int w, int h, const char* colour);
    
    virtual void character_(int x, int y, char value,int colour = WHITE);

    virtual void character(int column, int row, int value);
    virtual int columns();
    virtual int rows();

    virtual void sendData(uint16_t data) = 0;
    virtual void setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) = 0;
    
    virtual void lcd_display_char(uint16_t hwXpos, //specify x position.
                         uint16_t hwYpos, //specify y position.
                         uint8_t chChr,   //a char is display.
                         uint8_t chSize,  //specify the size of the char
                         uint16_t hwColor);
protected:

    // pixel location
    short _x;
    short _y;
    
    // window location
    short _x1;
    short _x2;
    short _y1;
    short _y2;

};

#endif
