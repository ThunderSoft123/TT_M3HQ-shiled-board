#include "common_define.h"




void i2cWrite(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite)
{
    uint8_t temp_buffer[NumByteToWrite + 1];
    temp_buffer[0] = RegisterAddr;
    memcpy(temp_buffer + 1,pBuffer,NumByteToWrite);
    dev_i2c->write((int)address,(const char *)temp_buffer,NumByteToWrite + 1);
}


void i2cRead(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead)
{
    dev_i2c->write((int)address,(const char *)&RegisterAddr,1);
    dev_i2c->read((int)address,(char *)pBuffer,NumByteToRead);
}

void i2cWriteForVL6180X(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint16_t RegisterAddr, uint16_t NumByteToWrite)
{   
    uint8_t temp_buffer[NumByteToWrite + 2];
    temp_buffer[0] = (RegisterAddr >> 8) & 0xff;
    temp_buffer[1] = (RegisterAddr) & 0xff;
    memcpy(temp_buffer + 2,pBuffer,NumByteToWrite);
    dev_i2c->write((int)address,(const char *)temp_buffer,NumByteToWrite + 2);
}

void i2cReadForVL6180X(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint16_t RegisterAddr, uint16_t NumByteToRead)
{
    uint8_t temp_buffer[2];
    temp_buffer[0] = (RegisterAddr >> 8) & 0xff;
    temp_buffer[1] = (RegisterAddr) & 0xff;
    dev_i2c->write((int)address,(const char *)temp_buffer,2);
    dev_i2c->read((int)address,(char *)pBuffer,NumByteToRead);
}

void i2cReadForFXS_MUTIL(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint16_t RegisterAddr, uint16_t NumByteToRead)
{
    dev_i2c->write((int)address,(const char *)&RegisterAddr,1,true);
    dev_i2c->read((int)address,(char *)pBuffer,NumByteToRead);
}

void i2cWriteForFXS_MUTIL(I2C * dev_i2c,uint8_t address,uint8_t* pBuffer, uint16_t RegisterAddr, uint16_t NumByteToWrite)
{
    uint8_t temp_buffer[NumByteToWrite + 1];
    temp_buffer[0] = RegisterAddr;
    memcpy(temp_buffer + 1,pBuffer,NumByteToWrite);
    dev_i2c->write((int)address,(const char *)temp_buffer,NumByteToWrite + 1);
}