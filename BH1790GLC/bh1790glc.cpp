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


#include "bh1790glc.h"

BH1790GLC::BH1790GLC(RegisterWriter &i2c_obj, uint8_t sad, uint8_t wai) : i2c_rw(i2c_obj) {
    /* Note that bh1790glc writes need to be single write command, not two separate. */
    _sad = sad;
    _wai = wai;        
}

BH1790GLC::~BH1790GLC()
{
}

uint8_t BH1790GLC::getDeviceID()
{
    uint8_t device_id;
    i2c_rw.read_register(_sad, BH1790GLC_PART_ID, &device_id, 1);
    return device_id;
}

/* Check if sensor is found and setup default configuration for default measurements
 *
 * @return error true/false
 */
int BH1790GLC::set_default_on(void)
{
    int error;
    uint8_t id;
    uint8_t part_id;
    uint8_t setup[3];

    error = i2c_rw.read_register(_sad, BH1790GLC_PART_ID, &part_id, 1);
    error = error || i2c_rw.read_register(_sad, BH1790GLC_MANUFACTURER_ID, &id, 1);
    if (error) {
        DEBUG_printf("read error.\r\n");
        return (error);
    }

    if (part_id != BH1790GLC_PART_ID_WIA_ID) {
        DEBUG_printf("PartID [%02X] doesn't match BH1790GLC.\r\n", part_id);
    } else {
        DEBUG_printf("PartID [%02X] OK.\r\n", part_id);
    }

    if (id != BH1790GLC_MID_VAL) {
        DEBUG_printf("Manufacturer id [%02X] doesn't match!\r\n", id);
    } else {
        DEBUG_printf("Manufacturer id [%02X] OK.\r\n", id);
    }

    setup[0] = (BH1790GLC_MEAS_CONTROL1_RDY_ENABLE | BH1790GLC_MEAS_CONTROL1_LED_LIGHTING_FREQ_128HZ | BH1790GLC_MEAS_CONTROL1_RCYCLE_32HZ);
    setup[1] = (BH1790GLC_MEAS_CONTROL2_LED1_EN_PULSED | BH1790GLC_MEAS_CONTROL2_LED_ON_TIME_216T_OSC | BH1790GLC_MEAS_CONTROL2_LED_CURRENT_10MA);
    setup[2] = BH1790GLC_MEAS_START_MEAS_ST_START;
    error = i2c_rw.write_register(_sad, BH1790GLC_MEAS_CONTROL1, setup, (uint8_t)sizeof(setup));
    if (error) {
        DEBUG_printf("BH1790GLC_MEAS_CONTROL1 write failed.\r\n");
    }

    return (error);
}
/*
 * @return error true/false
 */
int BH1790GLC::getresults_raw(uint8_t *data)
{
    return i2c_rw.read_register(_sad, BH1790GLC_DATAOUT_LEDOFF_L, data, 4);
}

/*
 * @return error true/false
 */
int BH1790GLC::getresults(uint16_t *data)
{
    int error;
    uint8_t val[4];

    error = getresults_raw(val);
    if (error) {
        return error;
    }
    data[0] = ((unsigned short)val[1] << 8) | (val[0]);
    data[1] = ((unsigned short)val[3] << 8) | (val[2]);

    return false;
}

//int BH1790GLC::write(char memory_address, char *data, int size)
//{
//   int rc;
//    rc = i2c_rw.write_register(_sad, memory_address, data, size );
//    return !rc;
//}
//
//int BH1790GLC::read(uint8_t memory_address, uint8_t *data, uint8_t size)
//{
//    int rc;
//    rc = i2c_rw.read_register(_sad, memory_address, data, size);
//    
//    return !rc;
//}