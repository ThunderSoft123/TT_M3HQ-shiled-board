#ifndef MPU6050_H
#define MPU6050_H

#include "mbed.h"

class MPU6050 : public I2C {


public:
    MPU6050();
    MPU6050(PinName sda,PinName scl);
    /**
     * @brief Get MPU6050 device's ID for test the device wheter connect.
     * @return [0x68] [0xFF] fail.
     */
    uint8_t getDeviceID();

    uint16_t getX();

    uint16_t getY();

    uint16_t getZ();

    uint16_t getGX();

    uint16_t getGY();

    uint16_t getGZ();
private:
    /**
     * @brief  Read value from MPU6050 register address
     * @param  register_address 
     * @return                  [description]
     */
    uint8_t readByte(uint8_t register_address);

    /**
     * @brief  Write value to MPU6050's register address.
     * @param  register_address [description]
     * @param  value            [description]
     * @return [0] success.
     */
    uint8_t writeByte(uint8_t register_address,uint8_t value);

  
    /**
     * @brief Init MPU6050 sensor.
     */
    void init_sensor();

    /**
     * @brief Set clock source.
     * @param value [description]
     */
    void setClockSource(uint8_t value);

    /**
     * @brief Set gyroscopes’ full scale range.
     * @param value 
     */
    void setFullScaleGyroRange(uint8_t value);

    /**
     * @brief Set configure the accelerometer full scale range.
     * @param value [description]
     */
    void setFullScaleAccelRange(uint8_t value);

};


#endif