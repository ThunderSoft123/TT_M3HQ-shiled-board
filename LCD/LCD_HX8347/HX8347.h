/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef HX8347_h
#define HX8347_h

#include "GraphicsDisplay.h"
#include "dm_platform.h"


//Other Colors
#define CYAN        0x07ff
#define BRIGHT_RED  0xf810
#define GRAY1       0x8410
#define GRAY2       0x4208

class HX8347 : public GraphicsDisplay
{
public:
  HX8347(PinName cs, PinName dc, PinName mosi, PinName miso, PinName clk,PinName bl);
  HX8347(PinName cs, PinName dc, PinName mosi, PinName miso, PinName clk);
  virtual ~HX8347();
  virtual void init(void);

  /**
   * @brief Analize the device whether connect.
   * @return [true] connect [false] disconnect 
   */
  bool isConnect();

  void drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint8_t size);

  void clearScreen(uint16_t color = BLACK);

  void drawchar(int x,int y,int value,int color = WHITE);

  void drawString(uint16_t x, uint16_t y, const char *p,int color = WHITE);

  void clearScreenArea(int x,int y,int color);

  /**
   * @brief Get device's id.
   * @return [device id]
   */
  uint8_t getDeviceID();
private:
  void send8BitData(uint8_t data);
  void writeBus(uint8_t data);

  virtual void pixel(int x, int y, int colour);
  virtual int width();
  virtual int height();
  virtual void window(unsigned int x,unsigned int y,unsigned int w,unsigned int h);


  virtual void setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
  virtual void sendCommand(uint8_t index);
  virtual void sendData(uint16_t data);

  /**
   * @brief Read register's address value.
   * @param  address [the register value]
   * @return         [register's address value]
   */
  uint8_t read8BitData(uint8_t address);

  void wr_reg(uint8_t address,uint8_t value);

  static const uint16_t _width;
  static const uint16_t _height;

  PinName _cs, _dc,_bl;
  DigitalOut *_pinDC;
  DigitalOut* _pinCS;
  DigitalOut *_pinBL;
  uint8_t _bitmaskCS;
  SPI spi;

};

typedef HX8347 DmTftIli9341;
typedef HX8347 LCD;

#endif



