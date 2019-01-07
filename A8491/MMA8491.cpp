/*
 * Copyright (c) 2015 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MMA8491.h"
#include "mbed.h"




MMA8491::MMA8491(PinName sda, PinName scl, PinName en) : MMA8491_i2c(sda,scl),MMA8491_en(en)
 {
    MMA8491_en=1;
 }
    
 void MMA8491::acquire_MMA8491_data_g(float * a_data)
 {
  
   char data_bytes[7];
   char d[1];
   int status;
   MMA8491_en=1;
   wait(0.001);
   d[0]=MMA8491_STATUS;
   status  = MMA8491_i2c.write(MMA8491_I2C_ADDRESS,d,1);  

   MMA8491_i2c.read(MMA8491_I2C_ADDRESS,data_bytes,7);// Read the 6 data bytes - LSB and MSB for X, Y and Z Axes.
   MMA8491_en=0;
  
   
   // a_data[0]= ((float)((int16_t)data_bytes[1]));
   a_data[0] =  ((float)((int16_t)(((data_bytes[1]*256) + (data_bytes[2])))>> 2) * 0.0009765);
   a_data[1] =  ((float)((int16_t)(((data_bytes[3]*256) + (data_bytes[4])))>> 2) * 0.0009765);
   a_data[2] =  ((float)((int16_t)(((data_bytes[5]*256) + (data_bytes[6])))>> 2) * 0.0009765);
   
 }

     