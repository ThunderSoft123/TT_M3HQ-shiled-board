#include "STMPE1600.h"
#include "common_define.h"



 STMPE1600::STMPE1600 (I2C *i2c, uint8_t DevAddr) : _dev_i2c(i2c) , address(DevAddr)
 {                         
    write_sys_ctrl (SOFT_RESET);    
    _gpdro_15 = (uint16_t)0;    // gpio dir all IN
    write_16_bit_reg (GPDR_0_7, &_gpdro_15);            
    _gpsro_15 = (uint16_t)0x0ffff;  // gpio status all 1
    write_16_bit_reg (GPSR_0_7, &_gpsro_15);
 }


void  STMPE1600::read(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead)
{
    i2cRead(_dev_i2c,address,pBuffer,RegisterAddr,NumByteToRead);
}               
                       
void  STMPE1600::write(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite)
{
    i2cWrite(_dev_i2c,address,pBuffer,RegisterAddr,NumByteToWrite);
}  

uint8_t STMPE1600::readID()
{
    uint8_t device_id;
    read(&device_id,VersionId,1);
    return device_id;
}

