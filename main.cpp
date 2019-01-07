#include "mbed.h"
#include "FactoryTool.h"
#include "DoorStatus.h"
#include "PirSensor.h"
#include "HX8347.h"

#include "MPU6050.h"
#include "HTU21D.h"
#include "MAG3110.h"
#include "FXOS8700Q.h"
#include "MMA8652.h"
#include "FXLS8471Q.h"
#include "FXAS21000.h"
#include "MPL3115A2.h"
#include "VL6180x.h"
#include "STMPE1600.h"
#include "Display.h"
#include "Switch.h"
#include "MMA8491.h"
#include "IKS01A2.h"
#include "bh1790glc.h"

//#include "DmTftS6D0164.h"
#include <stdio.h>

//factory tool for TMPM3HQFDFG



#define TEST_LCD

#ifdef TEST_LCD_9314
LCDTFT9314 tft(LCD_RD_PIN,LCD_WR_PIN,LCD_RS_PIN,LCD_CS_PIN,LCD_RST_PIN);
#endif

#ifdef TEST_LCD
LCD tft(LCD_SPI_CS,LCD_SPI_DC, LCD_MOSI_PIN, LCD_MISO_PIN, LCD_SPI_CL_PIN,LCD_BL_PIN);
#endif

#if defined(TEST_PIR_SENSOR) | defined(TEST_DOOR_STATUS)
DigitalOut testLed(LED1);
#endif

static void doorStatusFunc()
{
#if defined(TEST_PIR_SENSOR) | defined(TEST_DOOR_STATUS)
    testLed = 1;
    wait_ms(5000);
    testLed = 0;
#endif
}

static void pirHandle()
{
#if defined(TEST_PIR_SENSOR) | defined(TEST_DOOR_STATUS)
    testLed = 1;
    wait_ms(5000);
    testLed = 0;
#endif
}



#ifdef TEST_VL6180
#define VL6180X_I2C_SDA   D14 
#define VL6180X_I2C_SCL   D15 

#define ALS_MODE  1
#define RANG_MODE 0
#endif

static char translateChar(char arg)
{
  if(arg > 9)
    return arg + 55;
  else
    return arg + 48;
}

static void translateBinaryToBCD(char *arg,char Id)
{
  char high = (Id >> 4) & 0x0f;
  char low = Id & 0x0f;
  arg[0] = '0';
  arg[1] = 'x';
  arg[2] = translateChar(high);
  arg[3] = translateChar(low);
}

void displayId(LCD &lcd,char *string,int length,int high,char id)
{
  char temp_buffer[5];
  temp_buffer[4] = '\0';
  lcd.drawString(0,high,string,RED);
  translateBinaryToBCD(temp_buffer,id);
  lcd.drawString(length * FONT_CHAR_WIDTH,high,(const char *)temp_buffer,BLACK);
}


#if __TMPM3HQ__
#define DISPLAY_TMPM_STRING "Welcome to Thundersoft TT_M3HQ"
#endif


#if __TMPM4G9__
#define DISPLAY_TMPM_STRING "Welcome to Thundersoft TT_M4G9"
#endif
/*macros for ISK01A2 shiled board*/
#define DISPLAY_LSM6DSL_ID_STRING   "LSM6DSL ID:"
#define DISPLAY_LSM6DSL_ID_HIGHT    16
#define DISPLAY_LSM303ACC_ID_STRING   "LSMACC ID:"
#define DISPLAY_LSM303ACC_ID_HIGHT  32
#define DISPLAY_LSM303MAG_ID_STRING   "LSMMAG ID:"
#define DISPLAY_LSM303MAG_ID_HIGHT  48
#define DISPLAY_HTS221_ID_STRING   "HTS221 ID:"
#define DISPLAY_HTS221_ID_HIGHT  64
#define DISPLAY_LPS22_ID_STRING   "LPS22 ID:"
#define DISPLAY_LPS22_ID_HIGHT  80
#define DISPLAY_LSM6DSL_ACC_VALUE_STRING "LSM6DSL X Axes:"
#define DISPLAY_LSM6DSL_ACC_VALUE_STRING_HIGHT  96
#define DISPLAY_LSM6DSL_ACC_VALUE_HIGHT  (DISPLAY_LSM6DSL_ACC_VALUE_STRING_HIGHT + FONT_CHAR_HEIGHT)
#define DISPLAY_LSM6DSL_MAG_VALUE_STRING "LSM6DSL G Axes:"
#define DISPLAY_LSM6DSL_MAG_VALUE_STRING_HIGHT  128
#define DISPLAY_LSM6DSL_MAG_VALUE_HIGHT         (DISPLAY_LSM6DSL_MAG_VALUE_STRING_HIGHT + FONT_CHAR_HEIGHT)
#define DISPLAY_LSM303AGR_ACC_VALUE_STRING "LSM303AGR:"
#define DISPLAY_LSM303AGR_ACC_VALUE_STRING_HIGH 160
#define DISPLAY_LSM303AGR_ACC_VALUE_HIGH        (DISPLAY_LSM303AGR_ACC_VALUE_STRING_HIGH + FONT_CHAR_HEIGHT)
#define DISPLAY_LSM303MAG_VALUE_STRING       "LSM303MAG:"
#define DISPLAY_LSM303MAG_VALUE_STRING_HIGH     192
#define DISPLAY_LSM303MAG_VALUE_HIGH            (DISPLAY_LSM303MAG_VALUE_STRING_HIGH + FONT_CHAR_HEIGHT)
#define DISPLAY_HTS221_VALUE_STRING          "HTS221:"
#define DISPLAY_HTS221_VALUE_STRING_HIGH     (DISPLAY_LSM303MAG_VALUE_HIGH + FONT_CHAR_HEIGHT)
#define DISPLAY_HTS221_VALUE_HIGH            (DISPLAY_HTS221_VALUE_STRING_HIGH + FONT_CHAR_HEIGHT)
#define DISPLAY_LPS22HB_VALUE_STRING         "LPS22HB:"
#define DISPLAY_LPS22HB_VALUE_STRING_HIGH    (DISPLAY_HTS221_VALUE_HIGH + FONT_CHAR_HEIGHT)
#define DISPLAY_LPS22HB_VALUE_HIGH           (DISPLAY_LPS22HB_VALUE_STRING_HIGH + FONT_CHAR_HEIGHT)

/*macros for 6180XA1 board*/
#define DISPLAY_VL6180_ID_STRING "VL6180 ID:"
#define DISPLAY_VL6180_ID_HIGH   (16 * 1)
#define DISPLAY_STEMPE_STRING "STMPE ID:"
#define DISPLAY_STEMPE_ID_HIGH        (DISPLAY_VL6180_ID_HIGH * 2)
#define DISPLAY_VL6180_RANGE_VALUE_STRING "VL6180X RANG:"
#define DISPLAY_VL6180_RANGE_STRING_HIGH   (DISPLAY_VL6180_ID_HIGH * 3)
#define DISPLAY_VL6180_RANGE_VALUE_HIGH   (DISPLAY_VL6180_ID_HIGH * 4)
#define DISPLAY_VL6180_ALS_VALUE_STRING   "VL6180X ALS:"
#define DISPLAY_VL6180_ALS_STRING_HIGH    (DISPLAY_VL6180_ID_HIGH * 5)
#define DISPLAY_VL6180_ALS_VALUE_HIGH     (DISPLAY_VL6180_ID_HIGH * 6)

/*macros for A8491 board*/
#define DISPLAY_A8491_STRING "A8491 board:"
#define DISPLAY_A8491_STRING_HIGH 16
#define DISPLAY_A8491_VALUE_STRING  "A8491 value:"
#define DISPLAY_A8491_VALUE_STRING_HIGH  32
#define DISPLAY_A8491_VALUE_HIGH  48


/*macros for MPU6050 sensor*/
#define DISPLAY_MPU6050_ID_STRING "MPU6050 ID:"
#define DISPLAY_MPU6050_ID_STRING_HIGH 16
#define DISPLAY_MPU6050_ACC_STRING "MPU6050 ACC:"
#define DISPLAY_MPU6050_ACC_STRING_HIGH   32
#define DISPLAY_MPU6050_ACC_VALUE_HIGH   48
#define DISPLAY_MPU6050_MAG_STRING "MPU6050 MAG:"
#define DISPLAY_MPU6050_MAG_HIGH 64
#define DISPLAY_MPU6050_MAG_VALUE_HIGH 80


/*macros for HTU21D*/
#define DISPLAY_HTU21D_STRING "HTU21D Sensor:"
#define DISPLAY_HTU21D_STRING_HIGH 16
#define DISPLAY_HTU21D_TEM_VALUE_STRING  "Temperature value:"
#define DISPLAY_HTU21D_TEM_VALUE_STRING_HIGH  32
#define DISPLAY_HTU21D_TEM_VALUE_HIGH  48
#define DISPLAY_HTU21D_HUM_VALUE_STRING  "Humidity value:"
#define DISPLAY_HTU21D_HUM_VALUE_STRING_HIGH  64
#define DISPLAY_HTU21D_HUM_VALUE_HIGH  80


#ifdef TEST_LCD

/*macro for BH1790GLC*/
#define DISPLAY_BH1790GLC_STRING_ID "BH1790GLC ID:"
#define DISPLAY_BH1790GLC_STRING_ID_HIGH  16
#define DISPLAY_BH1790GLC_VALUE_STRING "BH1790GLC value:"
#define DISPLAY_BH1790GLC_VALUE_STRING_HIGH 32
#define DISPLAY_BH1790GLC_VALUE_HIGH 48


static void displayForBH1790GLC(LCD &lcd,BH1790GLC &bh1790glc)
{
  lcd.drawString(0,0,DISPLAY_TMPM_STRING,RED);
  displayId(lcd,DISPLAY_BH1790GLC_STRING_ID,sizeof(DISPLAY_BH1790GLC_STRING_ID),DISPLAY_BH1790GLC_STRING_ID_HIGH,bh1790glc.getDeviceID());
  lcd.drawString(0,DISPLAY_BH1790GLC_VALUE_STRING_HIGH,DISPLAY_BH1790GLC_VALUE_STRING,RED);
}

static void handleLCDForBH1790GLC(LCD &lcd,BH1790GLC &bh1790glc)
{
  char temp_buffer[50];
  uint16_t data[2];
  while(1)
  {
    memset(temp_buffer,0,sizeof(temp_buffer));
    bh1790glc.getresults(data);
    sprintf(temp_buffer,"%d,%d",data[0],data[1]);
    printf("BH1790GLC data = %d,%d\r\n",data[0],data[1]);
    lcd.drawString(0,DISPLAY_BH1790GLC_VALUE_HIGH,temp_buffer,RED);
    wait_ms(1000);
    lcd.clearScreenArea(0,DISPLAY_BH1790GLC_VALUE_HIGH,WHITE);
  }
}

static void displayForHTU21D(LCD &lcd,HTU21D &htu)
{
  lcd.drawString(0,0,DISPLAY_TMPM_STRING,RED);
  lcd.drawString(0,DISPLAY_HTU21D_STRING_HIGH,DISPLAY_HTU21D_STRING,RED);
  lcd.drawString(0,DISPLAY_HTU21D_TEM_VALUE_STRING_HIGH,DISPLAY_HTU21D_TEM_VALUE_STRING,RED);
  lcd.drawString(0,DISPLAY_HTU21D_HUM_VALUE_STRING_HIGH,DISPLAY_HTU21D_HUM_VALUE_STRING,RED);
}

static void handleLCDForHTU21D(LCD &lcd,HTU21D &htu)
{
  char temp_buffer[50];
  int temperature,humidity;
  while(1)
  {
    temperature = htu.sample_ctemp();
    humidity = htu.sample_humid();
    memset(temp_buffer,0,sizeof(temp_buffer));
    sprintf(temp_buffer,"%d",temperature);
    printf("HTU21D temperature = %d \r\n",temperature);
    lcd.drawString(0,DISPLAY_HTU21D_TEM_VALUE_HIGH,temp_buffer,RED);
    memset(temp_buffer,0,sizeof(temp_buffer));
    sprintf(temp_buffer,"%d",humidity);
    printf("HTU21D humidity = %d \r\n",humidity);
    lcd.drawString(0,DISPLAY_HTU21D_HUM_VALUE_HIGH,temp_buffer,RED);
    wait_ms(1000);
    lcd.clearScreenArea(0,DISPLAY_HTU21D_TEM_VALUE_HIGH,WHITE);
    lcd.clearScreenArea(0,DISPLAY_HTU21D_HUM_VALUE_HIGH,WHITE);
  }
}


static void displayForMPU6050(LCD &lcd,MPU6050 &mpu6050)
{
  lcd.drawString(0,0,DISPLAY_TMPM_STRING,RED);
  displayId(lcd,DISPLAY_MPU6050_ID_STRING,sizeof(DISPLAY_MPU6050_ID_STRING),DISPLAY_MPU6050_ID_STRING_HIGH,mpu6050.getDeviceID());
  lcd.drawString(0,DISPLAY_MPU6050_ACC_STRING_HIGH,DISPLAY_MPU6050_ACC_STRING,RED);
  lcd.drawString(0,DISPLAY_MPU6050_MAG_HIGH,DISPLAY_MPU6050_MAG_STRING,RED);
}

static void handleLCDForMPU6050(LCD &lcd,MPU6050 &mpu6050)
{
  uint16_t acc[3];
  uint16_t mag[3];
  char temp_buffer[50];
  while(1)
  {
    
    acc[0] = mpu6050.getX();
    acc[1] = mpu6050.getY();
    acc[2] = mpu6050.getZ();
    mag[0] = mpu6050.getGX();
    mag[1] = mpu6050.getGY();
    mag[2] = mpu6050.getGZ();
    memset(temp_buffer,0,sizeof(temp_buffer));
    sprintf(temp_buffer,"%d,%d,%d",acc[0],acc[1],acc[2]);
    lcd.drawString(0,DISPLAY_MPU6050_ACC_VALUE_HIGH,temp_buffer,RED);
    printf("MPU6050 ACC data = %d,%d,%d \r\n",acc[0],acc[1],acc[2]);
    memset(temp_buffer,0,sizeof(temp_buffer));
    sprintf(temp_buffer,"%d,%d,%d",mag[0],mag[1],mag[2]);
    lcd.drawString(0,DISPLAY_MPU6050_MAG_VALUE_HIGH,temp_buffer,RED);
    printf("MPU6050 MAG data = %d,%d,%d \r\n",mag[0],mag[1],mag[2]);
    wait_ms(1000);
    lcd.clearScreenArea(0,DISPLAY_MPU6050_ACC_VALUE_HIGH,WHITE);
    lcd.clearScreenArea(0,DISPLAY_MPU6050_MAG_VALUE_HIGH,WHITE);
  }
}

static void displayForA8491(LCD &lcd)
{
  lcd.drawString(0,0,DISPLAY_TMPM_STRING,RED);
  lcd.drawString(0,DISPLAY_A8491_STRING_HIGH,DISPLAY_A8491_STRING,RED);
  lcd.drawString(0,DISPLAY_A8491_VALUE_STRING_HIGH,DISPLAY_A8491_VALUE_STRING,RED);
}

static void handleLCDForA8491(LCD &lcd,MMA8491 &accel)
{
  float accel_data[3]; float accel_rms=0.0;
  char temp_buffer[50];
  while(1)
  {
    accel.acquire_MMA8491_data_g(accel_data);
    sprintf(temp_buffer,"%4.2f,%4.2f,%4.2f",accel_data[0],accel_data[1],accel_data[2]);
    lcd.drawString(0,DISPLAY_A8491_VALUE_HIGH,temp_buffer,RED);
    printf("%4.2f,%4.2f,%4.2f,\r\n",accel_data[0],accel_data[1],accel_data[2]);
    wait_ms(1000);
    lcd.clearScreenArea(0,DISPLAY_A8491_VALUE_HIGH,WHITE);
  }
}

static void displayForVL6180X(LCD &lcd,VL6180X &vl6180x,STMPE1600 &stmpe1600)
{
  char temp_buffer[5];
  temp_buffer[4] = '\0';
  lcd.drawString(0,0,DISPLAY_TMPM_STRING,RED);

  lcd.drawString(0,DISPLAY_VL6180_ID_HIGH,DISPLAY_VL6180_ID_STRING,RED);
  translateBinaryToBCD(temp_buffer,vl6180x.readID());
  lcd.drawString((sizeof(DISPLAY_VL6180_ID_STRING) - 1) * FONT_CHAR_WIDTH,DISPLAY_VL6180_ID_HIGH,(const char *)temp_buffer,BLACK);

  displayId(lcd,DISPLAY_STEMPE_STRING,sizeof(DISPLAY_STEMPE_STRING),DISPLAY_STEMPE_ID_HIGH,stmpe1600.readID());

  /*Set 6180XA1 for LCD string*/
  lcd.drawString(0,DISPLAY_VL6180_RANGE_STRING_HIGH,DISPLAY_VL6180_RANGE_VALUE_STRING,RED);
  lcd.drawString(0,DISPLAY_VL6180_ALS_STRING_HIGH,DISPLAY_VL6180_ALS_VALUE_STRING,RED);
}

static void handleLCDForVL6180X(LCD &lcd,VL6180X &vl6180x)
{
  char temp_buffer[50];
  while(1)
  {
    /*data for range */
    memset(temp_buffer,0,sizeof(temp_buffer));
    sprintf(temp_buffer,"%d",vl6180x.readRangeSingle());
    lcd.drawString(0,DISPLAY_VL6180_RANGE_VALUE_HIGH,temp_buffer,RED);
    printf("Range = %d \r\n",vl6180x.readRangeSingle());
    /*data for als*/
    memset(temp_buffer,0,sizeof(temp_buffer));
    sprintf(temp_buffer,"%d",vl6180x.readAmbientSingle());
    lcd.drawString(0,DISPLAY_VL6180_ALS_VALUE_HIGH,temp_buffer,RED);
    printf("ALS = %d \r\n",vl6180x.readAmbientSingle());
    wait_ms(1000);
    lcd.clearScreenArea(0,DISPLAY_VL6180_RANGE_VALUE_HIGH,WHITE);
    lcd.clearScreenArea(0,DISPLAY_VL6180_ALS_VALUE_HIGH,WHITE);
  }
}

static void displayForIKS01A2(LCD &lcd,IKS01A2 &iks102)
{
  char temp_buffer[5];
  temp_buffer[4] = '\0';
  lcd.drawString(0,0,DISPLAY_TMPM_STRING,RED);

  lcd.drawString(0,DISPLAY_LSM6DSL_ID_HIGHT,DISPLAY_LSM6DSL_ID_STRING,RED);
  translateBinaryToBCD(temp_buffer,iks102.getIdLSM6DSL());
  lcd.drawString((sizeof(DISPLAY_LSM6DSL_ID_STRING) - 1) * FONT_CHAR_WIDTH,DISPLAY_LSM6DSL_ID_HIGHT,(const char *)temp_buffer,BLACK);

  lcd.drawString(0,DISPLAY_LSM303ACC_ID_HIGHT,DISPLAY_LSM303ACC_ID_STRING,RED);
  translateBinaryToBCD(temp_buffer,iks102.getIdLSM303AGR());
  lcd.drawString((sizeof(DISPLAY_LSM303ACC_ID_STRING) - 1) * FONT_CHAR_WIDTH,DISPLAY_LSM303ACC_ID_HIGHT,(const char *)temp_buffer,BLACK);

  lcd.drawString(0,DISPLAY_LSM303MAG_ID_HIGHT,DISPLAY_LSM303MAG_ID_STRING,RED);
  translateBinaryToBCD(temp_buffer,iks102.getIdLSM303MAG());
  lcd.drawString((sizeof(DISPLAY_LSM303MAG_ID_STRING) - 1) * FONT_CHAR_WIDTH,DISPLAY_LSM303MAG_ID_HIGHT,(const char *)temp_buffer,BLACK);

  lcd.drawString(0,DISPLAY_HTS221_ID_HIGHT,DISPLAY_HTS221_ID_STRING,RED);
  translateBinaryToBCD(temp_buffer,iks102.getIdHTS221());
  lcd.drawString((sizeof(DISPLAY_HTS221_ID_STRING) - 1) * FONT_CHAR_WIDTH,DISPLAY_HTS221_ID_HIGHT,(const char *)temp_buffer,BLACK);

  lcd.drawString(0,DISPLAY_LPS22_ID_HIGHT,DISPLAY_LPS22_ID_STRING,RED);
  translateBinaryToBCD(temp_buffer,iks102.getIdLPS22HB());
  lcd.drawString((sizeof(DISPLAY_LPS22_ID_STRING) - 1) * FONT_CHAR_WIDTH,DISPLAY_LPS22_ID_HIGHT,(const char *)temp_buffer,BLACK);
  /*Set LSM6DSL for LCD string*/
  lcd.drawString(0,DISPLAY_LSM6DSL_ACC_VALUE_STRING_HIGHT,DISPLAY_LSM6DSL_ACC_VALUE_STRING,RED);
  lcd.drawString(0,DISPLAY_LSM6DSL_MAG_VALUE_STRING_HIGHT,DISPLAY_LSM6DSL_MAG_VALUE_STRING,RED);

  lcd.drawString(0,DISPLAY_LSM303AGR_ACC_VALUE_STRING_HIGH,DISPLAY_LSM303AGR_ACC_VALUE_STRING,RED);
  lcd.drawString(0,DISPLAY_LSM303MAG_VALUE_STRING_HIGH,DISPLAY_LSM303MAG_VALUE_STRING,RED);
  lcd.drawString(0,DISPLAY_HTS221_VALUE_STRING_HIGH,DISPLAY_HTS221_VALUE_STRING,RED);
  lcd.drawString(0,DISPLAY_LPS22HB_VALUE_STRING_HIGH,DISPLAY_LPS22HB_VALUE_STRING,RED);
}

static void handleLCDForIKS102(LCD &lcd,IKS01A2 &iks102)
{
  int32_t accelerometer[3];
  char temp_buffer[50];
  float humidity,temperature,pressure;
  //display for IKS102
  //display Img
  while(1)
  {
     /*Display LSM6DSL ACC*/
     memset(temp_buffer,0,sizeof(temp_buffer));
     iks102.getXAxesLSM6DSL(accelerometer);
     sprintf(temp_buffer,"%d,%d,%d",accelerometer[0],accelerometer[1],accelerometer[2]);
     printf("LSM6DSL X Axes = %d , %d , %d\r\n",accelerometer[0],accelerometer[1],accelerometer[2]);
     lcd.drawString(0,DISPLAY_LSM6DSL_ACC_VALUE_HIGHT,temp_buffer,RED);
     
      /*Display LSM6DSL MAG*/
     memset(temp_buffer,0,sizeof(temp_buffer));
     iks102.getGAxesLSM6DSL(accelerometer);
     sprintf(temp_buffer,"%d,%d,%d",accelerometer[0],accelerometer[1],accelerometer[2]);
     printf("LSM6DSL G Axes = %d , %d , %d\r\n",accelerometer[0],accelerometer[1],accelerometer[2]);
     lcd.drawString(0,DISPLAY_LSM6DSL_MAG_VALUE_HIGHT,temp_buffer,RED);
   
     /*Display LSM303AGR_ACC*/
     memset(temp_buffer,0,sizeof(temp_buffer));
     iks102.getDataLSM303AGR(accelerometer);
     sprintf(temp_buffer,"%d,%d,%d",accelerometer[0],accelerometer[1],accelerometer[2]);
     printf("LSM303AGR [acc/mg] = %d , %d , %d\r\n",accelerometer[0],accelerometer[1],accelerometer[2]);
     lcd.drawString(0,DISPLAY_LSM303AGR_ACC_VALUE_HIGH,temp_buffer,RED);
    
     /*Display LSM303AGR_MAG*/
     memset(temp_buffer,0,sizeof(temp_buffer));
     iks102.getDataLSM303MAG(accelerometer);
     sprintf(temp_buffer,"%d,%d,%d",accelerometer[0],accelerometer[1],accelerometer[2]);
     printf("LSM303MAG [mag/mgauss] = %d , %d , %d\r\n",accelerometer[0],accelerometer[1],accelerometer[2]);
     lcd.drawString(0,DISPLAY_LSM303MAG_VALUE_HIGH,temp_buffer,RED);
     
     /*Display HTS221*/
     memset(temp_buffer,0,sizeof(temp_buffer));
     iks102.GetHumidity(&humidity);
     sprintf(temp_buffer,"humidity %d,",(int)humidity);
     iks102.GetTemperatureHTS221(&temperature);
     sprintf(temp_buffer + strlen(temp_buffer),"temperature %d",(int)temperature);
     printf("HTS221Sensor humidity = %f,temperature = %f \r\n",humidity,temperature);
     lcd.drawString(0,DISPLAY_HTS221_VALUE_HIGH,temp_buffer,RED);
     
     /*Display LPS22HB*/
     memset(temp_buffer,0,sizeof(temp_buffer));
     iks102.GetPressure(&humidity);
     sprintf(temp_buffer,"pressure %d,",(int)humidity);
     iks102.GetTemperatureLPS22HB(&temperature);
     sprintf(temp_buffer + strlen(temp_buffer),"temperature %d",(int)temperature);
     printf("LPS22HBSensor pressure = %f, temperature = %f \r\n",pressure,temperature);
     lcd.drawString(0,DISPLAY_LPS22HB_VALUE_HIGH,temp_buffer,RED);
     //add log
     wait_ms(1000);
     lcd.clearScreenArea(0,DISPLAY_LSM6DSL_ACC_VALUE_HIGHT,WHITE);
     lcd.clearScreenArea(0,DISPLAY_LSM6DSL_MAG_VALUE_HIGHT,WHITE);
     lcd.clearScreenArea(0,DISPLAY_LSM303AGR_ACC_VALUE_HIGH,WHITE);
     lcd.clearScreenArea(0,DISPLAY_LSM303MAG_VALUE_HIGH,WHITE);
     lcd.clearScreenArea(0,DISPLAY_HTS221_VALUE_HIGH,WHITE);
     lcd.clearScreenArea(0,DISPLAY_LPS22HB_VALUE_HIGH,WHITE);
  }
}

#endif

int main()   
{

#if defined(TEST_HTU21D)
  HTU21D htu;
  printf("%s\r\n",DISPLAY_TMPM_STRING);
  tft.init();
  displayForHTU21D(tft,htu);
  handleLCDForHTU21D(tft,htu);
  while(1)
  {
  }
#endif

#ifdef TEST_PIR_SENSOR
    printf("%s\r\n",DISPLAY_TMPM_STRING);
    PirSensor pir;
    testLed = 1;
    pir.handleCallback(&pirHandle);
    while(1)
    {

    }  
#endif

#ifdef TEST_DOOR_STATUS
    DoorStatus door;
    door.handleCallback(&doorStatusFunc);

    while(1)
    {

    }
#endif


#ifdef TEST_FACTORY
#define TEST_FACTORY_DELAY_MS 10
    FactoryTool *factorytool = new FactoryTool();
    while(1)
    {
        //I/O pull up
        factorytool->pinWaveTest(TEST_FACTORY_DELAY_MS);
        //factorytool->pullOn();
        //wait_ms(TEST_FACTORY_DELAY_MS);
        //I/O pull down
        //factorytool->pullOff();
        //wait_ms(TEST_FACTORY_DELAY_MS);
    }
#endif

#ifdef TEST_MPU6050
  MPU6050 mp;
  
  printf("%s\r\n",DISPLAY_TMPM_STRING);
  printf("MPU6050 ID = 0x%x \r\n",mp.getDeviceID());
  tft.init();
  displayForMPU6050(tft,mp);
  handleLCDForMPU6050(tft,mp);
  while(1)
  {
  }
#endif

#ifdef TEST_FRDM_FXS_MULTI
    FXLS8471Q acc1(D11, D12, D13, D10);
    MMA8652   acc2(D14, D15);
    FXOS8700Q_acc combo_acc(D14, D15, FXOS8700CQ_SLAVE_ADDR0);
    FXOS8700Q_mag combo_mag(D14, D15, FXOS8700CQ_SLAVE_ADDR0);
    MAG3110   mag2(D14, D15);
    FXAS21000 gyro(D14, D15);
    MPL3115A2 wigo_sensor1(D14, D15, MPL3115A2_DEFAULT_ADDRESS,(PinName)0,(PinName)0);
    float acc_data[3], gyro_data[3],sensor_data[2];
    MotionSensorDataUnits adata;
    MotionSensorDataUnits mdata;
    int16_t acc_raw[3];
    printf("%s\r\n",DISPLAY_TMPM_STRING);
    combo_acc.enable();
    combo_mag.enable();
    mag2.enable();
    wigo_sensor1.Barometric_Mode();
    wigo_sensor1.Oversample_Ratio( OVERSAMPLE_RATIO_32);
     printf("FXLS8471 Acc   = 0x%x\r\n", acc1.getWhoAmI());
     printf("MMA8652 Acc    = 0x%x\r\n", acc2.getWhoAmI());
     printf("FXOS8700 Combo = 0x%x\r\n", combo_acc.whoAmI());
     printf("MAG3110 Mag    = 0x%x\r\n", mag2.whoAmI());
     printf("FXAS21000 Gyro = 0x%x\r\n", gyro.getWhoAmI());
     printf("MPL3115A2 = 0x%x \r\n",wigo_sensor1.getDeviceID());    
     while(1) {
        acc1.ReadXYZ(acc_data);
        acc1.ReadXYZraw(acc_raw);
        printf("FXLS8471 Acc:   X:%6.3f Y:%6.3f Z:%6.3f (Raw X:%4d Y:%4d Z:%4d)\r\n", acc_data[0], acc_data[1], acc_data[2], acc_raw[0], acc_raw[1], acc_raw[2]);

        acc2.ReadXYZ(acc_data);
        acc2.ReadXYZraw(acc_raw);
        printf("MMA8652 Acc:    X:%6.3f Y:%6.3f Z:%6.3f (Raw X:%4d Y:%4d Z:%4d)\r\n", acc_data[0], acc_data[1], acc_data[2], acc_raw[0], acc_raw[1], acc_raw[2]);

        combo_acc.getAxis(adata);
        printf("FXOS8700 Acc:   X:%6.3f Y:%6.3f Z:%6.3f\r\n", adata.x, adata.y, adata.z);
        
        combo_mag.getAxis(mdata);
        printf("FXOS8700 Mag:   X:%6.2f Y:%6.2f Z:%6.2f\r\n", mdata.x, mdata.y, mdata.z);

        mag2.getAxis(mdata);
        printf("MAG3110 Mag:    X:%6.2f Y:%6.2f Z:%6.2f\r\n", mdata.x, mdata.y, mdata.z);

        gyro.ReadXYZ(gyro_data);
        printf("FXAS21000 Gyro: X:%6.2f Y:%6.2f Z:%6.2f\r\n", gyro_data[0], gyro_data[1], gyro_data[2]);
        
        
        if ( wigo_sensor1.isDataAvailable()) 
        {
            if(wigo_sensor1.getMode() == BAROMETRIC_MODE)
            {
              wigo_sensor1.getAllData( &sensor_data[0]);
              printf("Pressure: %f Temperature: %f\r\n", sensor_data[0], sensor_data[1]);
              wigo_sensor1.Altimeter_Mode();
            }
            else
            {
              wigo_sensor1.getAllData( &sensor_data[0]);
              printf("Altitude: %f Temperature: %f\r\n", sensor_data[0], sensor_data[1]);
              wigo_sensor1.Barometric_Mode();
            }
        }
        printf("\r\n");
        wait_ms(1000);
     }
#endif

#ifdef TEST_VL6180
    printf("%s\r\n",DISPLAY_TMPM_STRING);
    I2C *i2c =new I2C(D14,D15);
    VL6180X *vl6180x = new VL6180X(i2c);
    STMPE1600 *stmpe1600 = new STMPE1600(i2c);
    Display display(*stmpe1600);
    Switch  _switch(*stmpe1600);
    //MeasureData_t measure_data;
    vl6180x->init();
    vl6180x->configureDefault();
     //vl6180x->startAmbientContinuous();
    printf("Vl6180X ID = 0x%x \r\n",vl6180x->readID());
    printf("STMPE1600 ID = 0x%x \r\n",stmpe1600->readID());
    tft.init();
    displayForVL6180X(tft,*vl6180x,*stmpe1600);
    handleLCDForVL6180X(tft,*vl6180x);
    while(1)
    {
    }
#endif

#ifdef TEST_A8491
    MMA8491 accel(D14,D15,A1);
    float accel_data[3]; float accel_rms=0.0;
    tft.init();    
    displayForA8491(tft);
    printf("%s\r\n",DISPLAY_TMPM_STRING);
    handleLCDForA8491(tft,accel);
    
    while(1)
    {       
    }
#endif


#ifdef TEST_IKS01A2
     int32_t accelerometer[3];
     float humidity,temperature;
     float pressure;
     printf("%s\r\n",DISPLAY_TMPM_STRING);
     I2C *i2c =new I2C(D14,D15);
     LSM6DSLSensor *lsm = new LSM6DSLSensor(i2c,LSM6DSL_ACC_GYRO_I2C_ADDRESS_HIGH);
     LSM303AGR_ACC_Sensor *Acc = new LSM303AGR_ACC_Sensor(i2c);
     LSM303AGR_MAG_Sensor *Mag = new LSM303AGR_MAG_Sensor(i2c);

     HTS221Sensor *HumTemp = new HTS221Sensor (i2c);  
     LPS22HBSensor *PressTemp = new LPS22HBSensor(i2c);  
     uint8_t temp_id;
     lsm->ReadID(&temp_id);
     // 
     lsm->Enable_X();  
     lsm->Enable_G(); 
     //
     Acc->Enable();
     Mag->Enable(); 

     HumTemp->Enable();
     PressTemp->Enable();
     printf("LSM6DSLSensor ID = 0x%x \r\n",temp_id);

     Acc->ReadID(&temp_id);
     printf("LSM303AGR_ACC_Sensor ID = 0x%x \r\n",temp_id);
     Mag->ReadID(&temp_id);
     printf("LSM303AGR_MAG_Sensor ID = 0x%x \r\n",temp_id);
     HumTemp->ReadID(&temp_id);
     printf("HTS221Sensor ID = 0x%x \r\n",temp_id);
     PressTemp->ReadID(&temp_id);
     printf("LPS22HBSensor ID = 0x%x \r\n",temp_id);
    
     IKS01A2 iks102(i2c);
     tft.init();
     displayForIKS01A2(tft,iks102);
     handleLCDForIKS102(tft,iks102);
     while(1)
     {
     }
#endif


#ifdef TEST_BH1790GLC
    uint16_t data[2];
    I2C i2c(I2C_SDA, I2C_SCL);
    RegisterWriter i2c_rw(i2c);
    BH1790GLC bh1790glc(i2c_rw);
    printf("%s\r\n",DISPLAY_TMPM_STRING);
    printf("BH1790GLC Decide Id = 0x%x \r\n",bh1790glc.getDeviceID());
    bh1790glc.set_default_on();
    tft.init();
    displayForBH1790GLC(tft,bh1790glc);
    handleLCDForBH1790GLC(tft,bh1790glc);
    while(1)
    {
    }
#endif


#ifdef TEST_LCD
  tft.init();
  tft.drawString(0,0,DISPLAY_TMPM_STRING,RED);
  while(1) {

  }
#endif


#ifdef TEST_LCD_9314
    printf("%s\r\n",DISPLAY_TMPM_STRING);
    tft.init();
    while(1)
    {

    }
#endif

}


