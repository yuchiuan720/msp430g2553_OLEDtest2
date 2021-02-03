/*
Example sketch to display Strings and float values on the OLED
1602 or 2004 display from Uninus. 

The I2C Address is set to 0x3C in OLedI2C.cpp
Uninus 2014 www.uninus.idv.tw  04/18/2014
標準的Arduino：
SDA -- Pin A4
SCL -- Pin A5

MSP430G2553:
SDA -- Pin 10
SCL -- Pin 9

LCD Interface4(IF4)
VSS   -- PIN 1
VDD   -- PIN 2
NC    -- PIN 3
REST# -- PIN 4
RS    -- PIN 5
SCL   -- PIN 6
SDA   -- PIN 7
NC    -- PIN 8
*/

//#include <I2C.h>
#include "Wire.h"
#include "OLedI2C_Wire.h"
OLedI2C LCD(0x3c, 16, 2);
//OLedI2C LCD(0x3c, 20, 4);

char msg[] = "This is an OLED display test. You can see how beautiful it is!";
unsigned char contrast = 0xff;

void setup()
{
  //Serial.begin(9600);
  Wire.begin();
  //I2c.begin();
  //I2c.scan();
  //Serial.println("Scan finished!");
  //LCD.powerDownLcd();
  LCD.init();
}

void loop()
{  
  LCD.clear();
 
  
  LCD.scrollString((char*) msg, 0, 100);
  LCD.scrollString((char*) msg, 1, 100);
  //LCD.noDoubleHeightFont();
  //LCD.FULL_ON_TEST();
  //LCD.clear();
  //LCD.scrollString((char*) msg, 2, 100);
  delay(1000);
  LCD.cursor();
  LCD.blink();
  LCD.sendString("<Flashing test>",0,0);
  delay(1000);
  LCD.clear();
  LCD.cursor();
  LCD.blink();
  LCD.sendString("I Love Money",0,0);
  delay(3000);
  LCD.noCursor();
  LCD.noBlink();

}
