#ifndef DM_PLATFORM_h
#define DM_PLATFORM_h

#include "mbed.h"


#define DM_TOOLCHAIN_MBED



#define LCD_MOSI_PIN       D11
#define LCD_MISO_PIN       D12 
#define LCD_SPI_CL_PIN     D13
#define LCD_SPI_CS         D10
#define LCD_SPI_DC         D7
#define LCD_BL_PIN         D9



#ifdef TEST_LCD_9314
#define LCD_RD_PIN   A0
#define LCD_WR_PIN   A1
#define LCD_RS_PIN   A2
#define LCD_CS_PIN   A3
#define LCD_RST_PIN  A4
#endif


// Clear bit, Set bit, High pulse, Low pulse, Boundary limits and Delay macros
#define sbi(reg, _bitmask) (*(reg) = 1)
#define cbi(reg, _bitmask) (*(reg) = 0)
#define pulse_high(reg, _bitmask) do { *(reg) = 1; *(reg) = 0; } while(0)
#define pulse_low(reg, _bitmask) do { *(reg) = 0;  *(reg) = 1; } while(0)
#define constrain(amt,low,high) ((amt)<=(low)?(low):((amt)>(high)?(high):(amt)))
#define delay(ms) wait_ms(ms)

#endif /* DM_PLATFORM_h */

