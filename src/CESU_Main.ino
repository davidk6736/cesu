
#include "DHT.h"                    //including library for humiture sensor
#include <SPI.h>                    //including SPI comunication library, SPI is adata protocol to send and receive data between arduino and SD card reader.
#include <SD.h>                     //including the SD library in the code in order to create a SD object to write and ead from SD card.
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>      //including library for I2C LCD Communication
#include <Adafruit_GPS.h>           //including Adafruit GPS library
#include <SoftwareSerial.h>         //including serial library to allow TX/RX communication
#include <SFE_BMP180.h>             //including library for barometer
SoftwareSerial gps_serial(8,7);     //creating an object of Software serial class with defining pin 8 as Rx and pin 7 as TX

#define         DHTPIN                        (4)       //define pin humiture is connected to
#define         DHTTYPE                       DHT11     //define sensor type
#define         RAIN_SENSOR                   (2)       //define pin for rain sensor
#define         FLAME_SENSOR                   (3)      //define pin for flame sensor
#define         CHIP_SELECT                   (10)      //define pin for datalogger

/************************Hardware Related Macros************************************/
#define         MQ_PIN                       (0)     //define which analog input channel you are going to use
#define         RL_VALUE                     (5)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (9.83)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
                                                     //which is derived from the chart in datasheet

/***********************Software Related Macros************************************/
#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
                                                     //cablibration phase
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
                                                     //normal operation

/**********************Application Related Macros**********************************/
#define         GAS_LPG                      (0)    //0=lpg reading
#define         GAS_CO                       (1)    //0=CO reading
#define         GAS_SMOKE                    (2)    //0=smoke reading

LiquidCrystal_I2C lcd(0x27,16,2);   //creates LCD datatype 
SFE_BMP180 pressure;                //creates BMP120 datatype
DHT dht(DHTPIN, DHTTYPE);           //creates dht datatype 
Adafruit_GPS GPS(&gps_serial);      //creating an object of Adafruit_GPS class which uses mySerial object as the serial port
File dataLogger;                    //creates a file called datalogger to log data onto
  
  unsigned char temp;
  unsigned char humidity;
  unsigned char pres;                       //Globals
  float lpg;
  float co;
  float smoke;
  bool flame;
  bool rain;
  char loopCount;
  float Ro;
 
  const float           LPGCurve[3]   =  {2.3,0.21,-0.47};
  const float           COCurve[3]    =  {2.3,0.72,-0.34};    //used to calibrate mq2
  const float           SmokeCurve[3] =  {2.3,0.53,-0.44};
void setup() 
{ 
  Serial.begin(9600);     //begin serial communication
  LCD_setup();            //lcd_setup
  LCDset1();
  lcd.print("working");   //lets us know lcd initialized
/*  if (barometer_setup())
  {
    lcd.clear();  
    LCDset1();
    lcd.print(F("BMP Success"));
    Serial.println(F("BMP Success"));
  }
  else
  {
    lcd.clear();
    LCDset1;
    lcd.print(F("BMP Fail"));
    Serial.println(F("BMP Fail"));
  }
  */
  delay(250);
  mq2_setup();                      //mq2_setup
  delay(250);
  flame_setup();                    //flame_setup
  rain_setup();                     //rain_setup
  gpsSetup();                       //GPS_setup
  delay(250);
 if (dataLogSetup())                //data logger setup returns a bool: if true, success is printed, else fail is printed
  {
    lcd.clear();
    LCDset1();
    lcd.print(F("dlog init"));
    Serial.println(F("Data logger initialization success"));
  }
  else
  {
    lcd.clear();
    LCDset1();
    lcd.print(F("dlog Fail"));
    Serial.println(F("Data logger initialization fail"));
  }
                                   // thing setup
                      
  loopCount=0;                     // counts loops that dictate which data to display on LCD
  
  temp=-1;
  humidity=-1;
  pres=-1;                        // variable initialization
  lpg=-1;
  co=-1;
  smoke=-1;
  Ro = 10;


}

void loop() 
{
  getData();              //gathers data
  displayData(loopCount);//uses loop count to determine which data to display
  delay(1000);        
  loopCount++;           //increments variable loop count
  if (loopCount==6)      //resets loop count to 0
  {
    loopCount=0;        
  }
  writeFile();          //store data onto sd chip
  // sendData();

  delay(1000);
}

void getData()      //delays decrease chance of errors due to port switching of arduino
{
  temp = get_temp();
  delay(20);
  humidity = get_humidity();
  delay(20);
//  pres = get_airPressure();
  lpg = get_LPG();
  delay(20);
  co = get_CO();
  delay(20);
  smoke = get_Smoke();
  delay(20);
 rain = get_rain();
delay(20);
  flame = get_flame();
  delay(20);
}

void displayData(int count)   //loopcount1->display temp and humidity
{                             //loopcount2->display pressure
  if (count==0)               //loopcount3->display LPG
  {                           //loopcount4->display CO
    lcd.clear();              //loopcount5->display Smoke
    LCDset1();                //loopcount6->rain and fire status
    lcd.print(temp);
    lcd.print(F(" C"));
    LCDset2();
    lcd.print(humidity);
    lcd.print(F("%"));
    delay(500);
  }
  else if (count==1)
  {
    lcd.clear();
    LCDset1();
    lcd.print("pressure: ");
    LCDset2();
    //lcd.print(pres);
    lcd.print(F(" mB"));
    delay(500);
  }
  else if (count==2)
  {
    lcd.clear();
    LCDset1();
    lcd.print(F("LPG: "));
    LCDset2();
    lcd.print(lpg);
    lcd.print(F("ppm"));
    delay(500);
  }
  else if (count==3)
  {
    lcd.clear();
    LCDset1();
    lcd.print(F("CO: "));
    LCDset2();
    lcd.print(co);
    lcd.print(F("ppm"));
    delay(500);
  }
  else if (count==4)
  {
    lcd.clear();
    LCDset1();
    lcd.print(F("Smoke: "));
    LCDset2();
    lcd.print(smoke);
    lcd.print(F("ppm"));
    delay(500);
  }
 else if(count==5 || (flame || rain)) 
  {
    lcd.clear();
    LCDset1();
    if(flame) 
    {
        lcd.print(F("There's FIRE!"));
    }
    else
    {
      lcd.print(F("No Fire"));
    }
    LCDset2();
    if (rain)
    {
      lcd.print(F("Its RAINING!"));
    }
    else
    {
      lcd.print(F("No Rain"));
    }
    delay(500);
  }
  
  else
  {
    lcd.clear();
    LCDset1();
    lcd.print(F("Error on loop"));
  }
  
}

