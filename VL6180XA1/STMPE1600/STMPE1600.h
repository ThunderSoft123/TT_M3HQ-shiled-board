#ifndef STMPE1600_H
#define STMPE1600_H

#include "mbed.h"

#define STMPE1600_DEF_DEVICE_ADDRESS  (uint8_t)0x42*2   
#define STMPE1600_DEF_DIGIOUT_LVL      1

/**  STMPE1600 registr map **/
#define ChipID_0_7      (uint8_t)0x00
#define ChipID_8_15     (uint8_t)0x01
#define VersionId       (uint8_t)0x02
#define SYS_CTRL        (uint8_t)0x03
#define IEGPIOR_0_7     (uint8_t)0x08
#define IEGPIOR_8_15      (uint8_t)0x09
#define ISGPIOR_0_7     (uint8_t)0x0A
#define ISGPIOR_8_15      (uint8_t)0x0B
#define GPMR_0_7          (uint8_t)0x10
#define GPMR_8_15         (uint8_t)0x11
#define GPSR_0_7          (uint8_t)0x12
#define GPSR_8_15         (uint8_t)0x13
#define GPDR_0_7          (uint8_t)0x14
#define GPDR_8_15         (uint8_t)0x15
#define GPIR_0_7          (uint8_t)0x16
#define GPIR_8_15         (uint8_t)0x17

#define SOFT_RESET          (uint8_t)0x80


#define STMPE1600_DEFAULT_ID 0x01

typedef enum {
    // GPIO Expander pin names
    GPIO_0=  0,
    GPIO_1 ,
    GPIO_2 ,
    GPIO_3 ,            
    GPIO_4 ,
    GPIO_5 ,
    GPIO_6 ,
    GPIO_7 ,            
    GPIO_8 ,
    GPIO_9 ,
    GPIO_10 ,
    GPIO_11 ,            
    GPIO_12 ,
    GPIO_13 ,
    GPIO_14 ,
    GPIO_15 ,
    NOT_CON
} exp_gpio_pin_name;   

typedef enum {
    INPUT = 0,
    OUTPUT,
    NOT_CONNECTED
} exp_gpio_pin_direction;


class STMPE1600 {
public: 
    /** Constructor
    * @param[in] &i2c device I2C to be used for communication
    * @param[in] DevAddr the stmpe1600 I2C device addres (deft STMPE1600_DEF_DEVICE_ADDRESS)
    */ 
    STMPE1600 (I2C *i2c, uint8_t DevAddr=STMPE1600_DEF_DEVICE_ADDRESS );

    /**
     * @brief       Write the SYS_CTRL register 
     * @param[in]   Data to be written (bit fields)
     */     
    void write_sys_ctrl (uint8_t data)     // data = SOFT_RESET reset the device
    {
        // _dev_i2c->i2c_write((uint8_t*)SYS_CTRL, _expdevaddr, data, 1);
        write(&data,SYS_CTRL,1);
    }
      
    /**
     * @brief       Set the out pin
     * @param[in]   The pin name
     * @return      0 on Success
     */         
    bool set_gpio (exp_gpio_pin_name pin_name) 
    {
          if (pin_name == NOT_CON) return true;         
          _gpsro_15 = _gpsro_15 | ((uint16_t)0x0001<<pin_name);
          write_16_bit_reg (GPSR_0_7 , &_gpsro_15);
          return false;
    }
    
    /**
     * @brief       Clear the out pin
     * @param[in]   The pin name
     * @return      0 on Success
     */         
    bool clr_gpio (exp_gpio_pin_name pin_name) 
    {
          if (pin_name == NOT_CON) return true;         
          _gpsro_15 = _gpsro_15 & (~((uint16_t)0x0001<<pin_name));
          write_16_bit_reg (GPSR_0_7 , &_gpsro_15);
          return false;
    }       
    
    /**
     * @brief       Read the input pin
     * @param[in]   The pin name
     * @return      The logical pin level
     */                 
    bool rd_gpio (exp_gpio_pin_name pin_name) 
    {               
          uint16_t gpmr0_15;
          if (pin_name == NOT_CON) return true;
          read_16_bit_reg (GPMR_0_7, &gpmr0_15);
          gpmr0_15 = gpmr0_15 & ((uint16_t)0x0001<<pin_name);
          if (gpmr0_15) {
            return true;
        }
          return false;         
    }
    
    /**
     * @brief       Set the pin direction
     * @param[in]   The pin name
     * @param[in]   The pin direction    
     * @return      0 on success
     */
    bool set_gpio_dir (exp_gpio_pin_name pin_name, exp_gpio_pin_direction pin_dir) 
    {
        if (pin_name == NOT_CON || pin_dir == NOT_CONNECTED) {
            return true;
        }
        _gpdro_15 = _gpdro_15 & (~((uint16_t)0x0001<<pin_name));  // clear the Pin
        _gpdro_15 = _gpdro_15 | ((uint16_t)pin_dir<<pin_name);
        write_16_bit_reg (GPDR_0_7 , &_gpdro_15);
        return false;
    }

    /**
     * @brief       Read a 16 bits register
     * @param[in]   The register address
     * @param[in]   The pointer to the read data
     */ 
    void read_16_bit_reg (uint8_t reg_16_addr, uint16_t *reg_16_data)
    {
        uint8_t data[2];
        read(data,reg_16_addr,2);
        *reg_16_data = data[1] << 8 | data[0];
    }               

    /**
     * @brief       Write a 16 bits register
     * @param[in]   The register address
     * @param[in]   The pointer to the data to be written
     */                         
    void write_16_bit_reg (uint8_t reg_16_addr, uint16_t *reg_16_data)
    {
        uint8_t data[2];
        data[1] = (*reg_16_data >> 8) & 0xff;
        data[0] = *reg_16_data & 0xff; 
        write(data,reg_16_addr,2);
    }   
    
    /**
     * @brief  Read STMPE1600 device id.
     * @return device ID 0x01
     */
    uint8_t readID();

    /**
     * @brief Test the STMPE1600 device whether connect.
     * @return [true] connect [false] disconnect. 
     */
    bool testConnect();

protected:

    /**
     * @brief Init device.
     */
    void init();

    /**
     * @brief       Read data from STMPE1600.
     * @param[out]  The pointer to the read data.
     * @param[in]   The register address.
     * @param[in]   The pointer to the data size.
     */                 
    void read(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToRead);        

    /**
     * @brief       Write data to STMPE1600.
     * @param[in]   The data pointer of write. 
     * @param[in]   The register address.
     * @param[in]   The pointer to the data size.
     */                         
    void write(uint8_t* pBuffer, uint8_t RegisterAddr, uint16_t NumByteToWrite);

private:                    
    I2C *_dev_i2c;    
    uint16_t _gpdro_15;   // local copy of bit direction reg 
    uint16_t _gpsro_15;   // local copy of bit status reg 
    uint8_t address; // expander device i2c addr
};




#endif