#ifndef COMMON_DEFINE_H
#define COMMON_DEFINE_H


#include "mbed.h"


#define TwoWire I2C
#define delay(x)   wait_ms(x)


void i2cWrite(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite);

void i2cRead(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead);

void i2cWriteForVL6180X(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint16_t RegisterAddr, uint16_t NumByteToWrite);

void i2cReadForVL6180X(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint16_t RegisterAddr, uint16_t NumByteToRead);

void i2cReadForFXS_MUTIL(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint16_t RegisterAddr, uint16_t NumByteToRead);

void i2cWriteForFXS_MUTIL(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint16_t RegisterAddr, uint16_t NumByteToWrite);

#endif