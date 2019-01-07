/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

//Tested on NUCLEO-F401RE, LPCXpresso11U68, LPCXpresso824-MAX platform.

#include "HX8347.h"



#define HX8347_DEVICE_ID 0x47

HX8347::HX8347(PinName cs, PinName dc, PinName mosi, PinName miso, PinName clk)
:GraphicsDisplay("HX8347"), spi(mosi, miso, clk)
{
  _cs = cs;
  _dc = dc;
}

HX8347::HX8347(PinName cs, PinName dc, PinName mosi, PinName miso, PinName clk,PinName bl)
:GraphicsDisplay("HX8347"), spi(mosi, miso, clk)
{
     _cs = cs;
     _dc = dc;
     _bl = bl;
}

HX8347::~HX8347() {
delete _pinCS;
delete _pinDC;

_pinCS = NULL;
_pinDC = NULL;
_pinBL = NULL;
}




uint8_t HX8347::read8BitData(uint8_t address)
{
  cbi(_pinDC, _bitmaskDC);
  spi.write(0x00);
  sbi(_pinDC, _bitmaskDC);
  return spi.write(address) & 0xff;
}

bool HX8347::isConnect()
{
  if(getDeviceID() == HX8347_DEVICE_ID)
    return true;
  return false;
}

uint8_t HX8347::getDeviceID()
{
  uint8_t temp;
  cbi(_pinCS, _bitmaskCS);
  temp = read8BitData(0x00);
  sbi(_pinCS, _bitmaskCS);
  return temp;
}

void HX8347::writeBus(uint8_t data) {
  spi.write(data);
}

void HX8347::sendCommand(uint8_t index) {
  cbi(_pinDC, _bitmaskDC);
  writeBus(index);
}

void HX8347::send8BitData(uint8_t data) {
  sbi(_pinDC, _bitmaskDC);
  writeBus(data);
}

void HX8347::sendData(uint16_t data) {
  uint8_t dh = data>>8;
  uint8_t dl = data&0xff;

  sbi(_pinDC, _bitmaskDC);
  writeBus(dh);
  writeBus(dl);
}

void HX8347::setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    wr_reg(0x03, x0 );
    wr_reg(0x02, (x0 >> 8));
    wr_reg(0x05, x0+x1-1 );
    wr_reg(0x04, ((x0+x1-1) >> 8));
    wr_reg(0x07,  y0 );
    wr_reg(0x06, ( y0 >> 8));
    wr_reg(0x09, ( y0+y1-1 ));
    wr_reg(0x08, ( (y0+y1-1) >> 8));
    sendCommand(0x22);
}

void HX8347::wr_reg(uint8_t address,uint8_t value)
{
  sendCommand(address);
  sendData(value);
}

int HX8347::width()
{
    return 240;
}

int HX8347::height()
{
    return 320;
}

void HX8347::drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint8_t size)
{
    cbi(_pinCS, _bitmaskCS);
      //   for (uint8_t i =0; i<5; i++ ) {
      //   //uint8_t line = pgm_read_byte(font_+(c*5)+i);
      //   for (uint8_t j = 0; j<8; j++) {
      //     if (line & 0x1) {
      //       pixel(x+i, y+j, color);
      //     }
      //     line >>= 1;
      //   }
      // }
    sbi(_pinCS, _bitmaskCS);
}

void HX8347::drawchar(int x,int y,int value,int color)
{
    cbi(_pinCS, _bitmaskCS);
    character_(x,y,value - 32,color);
    sbi(_pinCS, _bitmaskCS);
}

void HX8347::drawString(uint16_t x, uint16_t y, const char *p,int color)
{
  //cbi(_pinCS, _bitmaskCS);
  while(*p!='\0')
  {
    if(x > (width() - FONT_CHAR_WIDTH)) {
      x = 0;
      y += FONT_CHAR_HEIGHT;
    }
    if(y > (height() - FONT_CHAR_HEIGHT)) {
      y = x = 0;
    }
    //drawChar(x, y, *p, false);
    drawchar(x,y,*p,color);

    x += FONT_CHAR_WIDTH;
    //drawchar(x,y,'j');
    p++;
  }
  //sbi(_pinCS, _bitmaskCS);
}

void HX8347::pixel(int x, int y, int colour)
{
    wr_reg(0x03, (x >> 0));
    wr_reg(0x02, (x >> 8));
    wr_reg(0x07, (y >> 0));
    wr_reg(0x06, (y >> 8));
    sendCommand(0x22);
    sendData(colour);
}

void HX8347::clearScreenArea(int x,int y,int color)
{
    int pos,t;
    cbi(_pinCS, _bitmaskCS);
    for(x ;x < width();x+= FONT_CHAR_WIDTH)
    {
       for(pos=0;pos<FONT_CHAR_HEIGHT;pos++) {
        for(t=0;t<FONT_CHAR_WIDTH;t++) {
            pixel(x + t,y + pos,color);
        }
      }
  }
    sbi(_pinCS, _bitmaskCS);
}

void HX8347::window(unsigned int x,unsigned int y,unsigned int w,unsigned int h)
{
    setAddress(x,y,w,h);
}

void HX8347::clearScreen(uint16_t color)
{
  cbi(_pinCS, _bitmaskCS);
  setAddress(0,0,width(), height());
  for(uint16_t i=0; i<height(); i++) {
    for(uint16_t j=0; j<width(); j++) {
        sendData(color);
    }
  }
  sbi(_pinCS, _bitmaskCS);
}

void HX8347::init(void) {
  //setTextColor(BLACK, WHITE);
  //
  _pinBL = new DigitalOut(_bl);
  _pinCS = new DigitalOut(_cs);
  _pinDC = new DigitalOut(_dc);

  sbi(_pinCS, _bitmaskCS);
  //light on
  sbi(_pinBL,_bitmaskCS);
  spi.format(8,0);
  spi.frequency(8000000); // Max SPI speed for display is 10 and for 17 for LPC15xx  

  cbi(_pinCS, _bitmaskCS);
  delay(135); // This much delay needed??

  // ILI9341 init
    wr_reg(0xEA, 0x00);                 /* Reset Power Control 1                */
    wr_reg(0xEB, 0x20);                 /* Power Control 2                      */
    wr_reg(0xEC, 0x0C);                 /* Power Control 3                      */
    wr_reg(0xED, 0xC4);                 /* Power Control 4                      */
    wr_reg(0xE8, 0x40);                 /* Source OPON_N                        */
    wr_reg(0xE9, 0x38);                 /* Source OPON_I                        */
    wr_reg(0xF1, 0x01);                 /*                                      */
    wr_reg(0xF2, 0x10);                 /*                                      */
    wr_reg(0x27, 0xA3);                 /* Display Control 2                    */

    /* Power On sequence ---------------------------------------------------------*/
    wr_reg(0x1B, 0x1B);                 /* Power Control 2                      */
    wr_reg(0x1A, 0x01);                 /* Power Control 1                      */
    wr_reg(0x24, 0x2F);                 /* Vcom Control 2                       */
    wr_reg(0x25, 0x57);                 /* Vcom Control 3                       */
    wr_reg(0x23, 0x8D);                 /* Vcom Control 1                       */

    /* Gamma settings  -----------------------------------------------------------*/
    wr_reg(0x40,0x00);   //   default setup
    wr_reg(0x41,0x00);   //
    wr_reg(0x42,0x01);   //
    wr_reg(0x43,0x13);   //
    wr_reg(0x44,0x10);   //
    wr_reg(0x45,0x26);   //
    wr_reg(0x46,0x08);   //
    wr_reg(0x47,0x51);   //
    wr_reg(0x48,0x02);   //
    wr_reg(0x49,0x12);   //
    wr_reg(0x4A,0x18);   //
    wr_reg(0x4B,0x19);   //
    wr_reg(0x4C,0x14);   //
    wr_reg(0x50,0x19);   //
    wr_reg(0x51,0x2F);   //
    wr_reg(0x52,0x2C);   //
    wr_reg(0x53,0x3E);   //
    wr_reg(0x54,0x3F);   //
    wr_reg(0x55,0x3F);   //
    wr_reg(0x56,0x2E);   //
    wr_reg(0x57,0x77);   //
    wr_reg(0x58,0x0B);   //
    wr_reg(0x59,0x06);   //
    wr_reg(0x5A,0x07);   //
    wr_reg(0x5B,0x0D);   //
    wr_reg(0x5C,0x1D);   //
    wr_reg(0x5D,0xCC);   //

    /* Power + Osc ---------------------------------------------------------------*/
    wr_reg(0x18, 0x36);                 /* OSC Control 1                        */
    wr_reg(0x19, 0x01);                 /* OSC Control 2                        */
    wr_reg(0x01, 0x00);                 /* Display Mode Control                 */
    wr_reg(0x1F, 0x88);                 /* Power Control 6                      */
    wait_ms(5);                           /* Delay 5 ms                           */
    wr_reg(0x1F, 0x80);                 /* Power Control 6                      */
    wait_ms(5);                         /* Delay 5 ms                           */
    wr_reg(0x1F, 0x90);                 /* Power Control 6                      */
    wait_ms(5);                           /* Delay 5 ms                           */
    wr_reg(0x1F, 0xD0);                 /* Power Control 6                      */
    wait_ms(5);                           /* Delay 5 ms                           */

    wr_reg(0x17, 0x05);                 /* Colmod 16Bit/Pixel                   */

    wr_reg(0x36, 0x00);                 /* Panel Characteristic                 */
    wr_reg(0x28, 0x38);                 /* Display Control 3                    */
    wait_ms(40);
    wr_reg(0x28, 0x3C);                 /* Display Control 3     */
    delay(50);

    /*Set Display area*/
    wr_reg(0x02,0x00);
    wr_reg(0x03,0x00); //Column Start
    wr_reg(0x04,0x00);
    wr_reg(0x05,0xEF); //Column End
    wr_reg(0x06,0x00);
    wr_reg(0x07,0x00); //Row Start
    wr_reg(0x08,0x01);
    wr_reg(0x09,0x3F); //Row End

    sbi(_pinCS, _bitmaskCS);
    clearScreen(WHITE);
}




/*********************************************************************************************************
  END FILE
*********************************************************************************************************/


