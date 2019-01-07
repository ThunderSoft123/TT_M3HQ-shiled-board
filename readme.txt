#define TEST_MPU6050
#define TEST_HTU21D
#define TEST_DOOR_STATUS
#define TEST_PIR_SENSOR
#define TEST_LCD
#define TEST_FRDM_FXS_MULTI
#define TEST_VL6180
#define TEST_A8491
#define TEST_IKS01A2
#define TEST_BH1790GLC
#define TEST_LCD_9314

//Test for shiled board and sensor.
mbed compile -t GCC_ARM -m TMPM3HQ/TMPM4G9 -D macro
//Test for 6180XA1
mbed compile -t GCC_ARM -m TMPM3HQ/TMPM4G9 -D TEST_VL6180
//Test for A8491
mbed compile -t GCC_ARM -m TMPM3HQ/TMPM4G9 -D TEST_A8491
//Test for IKS01A2
mbed compile -t GCC_ARM -m TMPM3HQ/TMPM4G9 -D TEST_IKS01A2
//Test for BH1790GLC
mbed compile -t GCC_ARM -m TMPM3HQ/TMPM4G9 -D TEST_BH1790GLC
//Test for PIR SENSOR
mbed compile -t GCC_ARM -m TMPM3HQ/TMPM4G9 -D TEST_PIR_SENSOR
//Test for DOOR
mbed compile -t GCC_ARM -m TMPM3HQ/TMPM4G9 -D TEST_DOOR_STATUS
//Test for MPU6050
mbed compile -t GCC_ARM -m TMPM3HQ/TMPM4G9 -D TEST_MPU6050
//Test for FRDM FXS MULTI
mbed compile -t GCC_ARM -m TMPM3HQ/TMPM4G9 -D TEST_FRDM_FXS_MULTI
//Test for HTU21D
mbed compile -t GCC_ARM -m TMPM3HQ/TMPM4G9 -D TEST_HTU21D