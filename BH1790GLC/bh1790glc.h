/*
The MIT License (MIT)
Copyright (c) 2017 Rohm Semiconductor

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _BH1790GLC_H_
#define _BH1790GLC_H_


#include "bh1790glc_registers.h"
#define BH1790GLC_DEFAULT_SLAVE_ADDRESS (0x5B)
#define BH1790GLC_MID_VAL               (0xE0)

#include "mbed.h"
#include "RegisterWriter.h"

/** BH1790GLC class
 *
 *  BH1790GLC: an I2C bus optical sensor for heart rate monitor library
 *
 *  BH1790GLC is optical sensor for heart rate monitor IC in
 *  which LED driver and green light detection photo-diode
 *  are incorporated. This device drives LED and provides
 *  the intensity of light reflected from body.
 *
 *  For more information about BH1790GLC:
 *    http://www.rohm.com/web/global/datasheet/BH1790GLC/bh1790glc-e
 *    
 *    http://rohmfs.rohm.com/en/products/databook/applinote/ic/sensor/optical_sensor_for_heart_rate_monitor/bh1790glc-evk-001-manual-e.pdf
 *    http://rohmfs.rohm.com/en/products/databook/applinote/ic/sensor/optical_sensor_for_heart_rate_monitor/bh1790glc-software-manual-e.pdf
 *
 *  This was ported from Arduino library by H.Tanaka and rewritten to match other Rohm mbed drivers by M.Koivunen.
 */

class BH1790GLC
{
public:
    /** Create a BH1790GLC instance connected to specified I2C object with specified address
    *
    * @param i2c_obj RegisterWriter object (instance)
    * @param sad I2C-bus address
    * @param wai Who Am I -value
    */
    BH1790GLC(RegisterWriter &i2c_obj, uint8_t sad = BH1790GLC_DEFAULT_SLAVE_ADDRESS, uint8_t wai = BH1790GLC_PART_ID_WIA_ID);

    /** Destructor of BH1790GLC
     */
    ~BH1790GLC();

    void init(void);
    int set_default_on(void);
    int getresults_raw(uint8_t *data);
    int getresults(uint16_t *data);

    uint8_t getDeviceID();
    //int write(uint8_t memory_address,uint8_t *data,uint8_t size);
    //int read(uint8_t memory_address,uint8_t *data, uint8_t size);
private:
    RegisterWriter i2c_rw;
    uint8_t _sad;
    uint8_t _wai;
};
#endif