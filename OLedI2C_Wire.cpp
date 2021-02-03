/*
This is my first Library to make writing to the OLED 1602
Display much easier, the display is based on the SSD1311.
The display has 16 characters and 2 lines.
The library is far from complete and may be prone to errors.
Feedback welcome, visit www.gadjet.co.uk
Phil Grant 2013
Scrolling contributed by Nathan Chantrell http://nathan.chantrell.net/

Updated 06/11/2013 to include the cursPos data in the sendString function
sendString("string", col, row)

*/

#include "OLedI2C_Wire.h"

#include "Wire.h"

#define OLED_Address 0x3c
//#define OLED_Address 0x78
//#define OLED_Command_Mode 0x80
#define OLED_Command_Mode 0x00   // modify by ivan 2014/4/11
#define OLED_Data_Mode 0x40

// CGRAM has up to 8 characters of 5 x 8 dots, selectable by OPR0 and OPR1 pins
unsigned char CGRAM1[40] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, //00H
                                 0x1F,0x11,0x11,0x11,0x11,0x11,0x11,0x1F, // 01H
                                 0x0A,0x15,0x0A,0x15,0x0A,0x15,0x0A,0x15, // 02H
                                 0x15,0x0A,0x15,0x0A,0x15,0x0A,0x15,0x0A,}; // 03H
                                 
                                
OLedI2C::OLedI2C(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
{
  _Addr = lcd_Addr;
  _cols = lcd_cols;
  _rows = lcd_rows;
  _numlines = _rows;
  
  _row_offsets[0] = 0x00;
  if(_rows == 2)
    _row_offsets[1] = 0x40;
  else if(_rows == 3)
  {
    _row_offsets[1] = 0x20;
    _row_offsets[2] = 0x40;
	}
  else if(_rows == 4)
  {
    _row_offsets[1] = 0x20;
    _row_offsets[2] = 0x40;
    _row_offsets[3] = 0x60;
	}
}

OLedI2C::~OLedI2C()
{}

/*
void OLedI2C::init() {
   // *** I2C initial *** //
 delay(100);
 sendCommand(0x2A); // **** Set "RE"=1  00101010B
 sendCommand(0x71);
 sendCommand(0x5C);
 sendCommand(0x28);

 sendCommand(0x08); // **** Set Sleep Mode On
 sendCommand(0x2A); // **** Set "RE"=1  00101010B
 sendCommand(0x79); // **** Set "SD"=1  01111001B

 sendCommand(0xD5);
 sendCommand(0x70);
 sendCommand(0x78); // **** Set "SD"=0  01111000B

 sendCommand(0x08); // **** Set 5-dot, 3 or 4 line(0x09), 1 or 2 line(0x08)

 sendCommand(0x06); // **** Set Com31-->Com0  Seg0-->Seg99

 // **** Set OLED Characterization *** //
 sendCommand(0x2A);     // **** Set "RE"=1 
 sendCommand(0x79);     // **** Set "SD"=1

 // **** CGROM/CGRAM Management *** //
 sendCommand(0x72);     // **** Set ROM
 sendCommand(0x00);     // **** Set ROM A and 8 CGRAM


 sendCommand(0xDA);     // **** Set Seg Pins HW Config
 sendCommand(0x10);   

 sendCommand(0x81);     // **** Set Contrast
 sendCommand(0xFF); 

 sendCommand(0xDB);     // **** Set VCOM deselect level
 sendCommand(0x30);     // **** VCC x 0.83

 sendCommand(0xDC);     // **** Set gpio - turn EN for 15V generator on.
 sendCommand(0x03);

 sendCommand(0x78);     // **** Exiting Set OLED Characterization
 sendCommand(0x28); 
 sendCommand(0x2A); 
 //sendCommand(0x05);   // **** Set Entry Mode
 sendCommand(0x06);     // **** Set Entry Mode
 sendCommand(0x08);  
 sendCommand(0x28);     // **** Set "IS"=0 , "RE" =0 //28
 sendCommand(0x01); 
 sendCommand(0x80);     // **** Set DDRAM Address to 0x80 (line 1 start)

 delay(100);
 sendCommand(0x0C);     // **** Turn on Display
 }
*/

// modify by Ivan Lee
//
void OLedI2C::init() {

 // *** I2C initial *** //
 delay(100);
 
 // Enable Internal Regulator
 sendCommand(0x2A);	// **** Set "RE"=1	00101010B
 //sendCommand(0x08); 
 sendCommand(0x71); // Enable Internal Regulator
 sendData(0x5C);    // ivan 
 sendCommand(0x28); // clear RE

 // set display off
 sendCommand(0x08);	
 
 // Set display clock divide ratio oscillator frequency
 sendCommand(0x2A);	// **** Set "RE"=1	00101010B
 sendCommand(0x79);	// **** Set "SD"=1	01111001B
 sendCommand(0xD5);
 sendCommand(0x70);
 sendCommand(0x78);	// **** Set "SD"=0  01111000B

 // set display mode
 if(_rows >= 3)
  sendCommand(0x09);	// **** Set 5-dot, 3 or 4 line(0x09), 1 or 2 line(0x08)
 else 
  sendCommand(0x08);

 // set re-map
 sendCommand(0x06);	// **** Set Com31-->Com0  Seg0-->Seg99

 // **** CGROM/CGRAM Management *** //
 sendCommand(0x72);  	// **** Set ROM
 //sendData(0x00);  	// **** Set ROM A and 8 CGRAM
 sendData(0x05);  	// **** Set ROM B and 8 CGRAM

 // Set OLED Characterization
 sendCommand(0x79);  
 
 // set seg pins hardware configuration
 sendCommand(0xDA); 	// **** Set Seg Pins HW Config
 sendCommand(0x10);   

 //sendCommand(0x81);  	// **** Set Contrast
 //sendCommand(0xFF); 

 //sendCommand(0xDB);  	// **** Set VCOM deselect level
 //sendCommand(0x30);  	// **** VCC x 0.83

 // set segment low voltage&GPIO
 sendCommand(0xDC);  	// **** Set gpio - turn EN for 15V generator on.
 sendCommand(0x03);

 // Vcc power stabilized
 delay(100);
 
 sendCommand(0x81);  	// **** Set Contrast
 sendCommand(0xFF); 
 
 // set pre-charge period
 sendCommand(0xD9);  	
 sendCommand(0xF1); 
 
 // set VCOMH disslect level
 sendCommand(0xDB);  
 sendCommand(0x40); 
 
 //
 sendCommand(0x78);  	// **** Exiting Set OLED Characterization
 sendCommand(0x28);   // **** Set "IS"=0 , "RE" =0 //28
 
 //sendCommand(0x05); 	// **** Set Entry Mode
 //sendCommand(0x06); 	// **** Set Entry Mode
 
 // set display off
 sendCommand(0x08);
  
 // CGRAM() 
 //CGRAM();
 
 // clear display
 sendCommand(0x01);
 delay(500);
 
 // set DDRAM address 
 sendCommand(0x80); 	// **** Set DDRAM Address to 0x80 (line 1 start)

 delay(100);
 // set display on
 sendCommand(0x0C);  	// **** Turn on Display
 
 	// turn the display on with no cursor or blinking default
	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  
	// Initialize to default text direction (for roman languages)
	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	
  _displayfunction = LCD_2OR4LINE | LCD_DOUBLEHEIGHTOFF; 
  
  _extenddisplayfunction = LCD_5DOTS;           //FW=0
  if(_rows <= 2)
    _extenddisplayfunction |= LCD_1OR2LINE; 
  else
    _extenddisplayfunction |= LCD_3OR4LINE;     //NW=1
  
  _displayfunctionRE = _displayfunction & 0xF8; //clear last 3 bits
	// set the entry mode
	//sendCommand(LCD_ENTRYMODESET | _displaymode);
	
	//home();  
}

void OLedI2C::clear()
{
  sendCommand(LCD_CLEARDISPLAY);// clear display, set cursor position to zero
  //sendCommand(0x01);
}

void OLedI2C::home()
{
	sendCommand(LCD_RETURNHOME);  // set cursor position to zero
  //sendCommand(0x02);
}

void OLedI2C::lcdOn()
{
	_displaycontrol |= LCD_DISPLAYON;
	sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
  //sendCommand(0x0C);  	// **** Turn on 
}

void OLedI2C::lcdOff()
{
	_displaycontrol &= ~LCD_DISPLAYON;
	sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
  //sendCommand(0x08);  	// **** Turn Off
}

void OLedI2C::display() 
{
  lcdOn();
}

void OLedI2C::noDisplay() 
{
  lcdOff();
}

// Turns the underline cursor on/off
void OLedI2C::cursor() 
{
	_displaycontrol |= LCD_CURSORON;
	sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

void OLedI2C::noCursor() 
{
	_displaycontrol &= ~LCD_CURSORON;
	sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void OLedI2C::noBlink() 
{
	_displaycontrol &= ~LCD_BLINKON;
	sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}
void OLedI2C::blink() 
{
	_displaycontrol |= LCD_BLINKON;
	sendCommand(LCD_DISPLAYCONTROL | _displaycontrol);
}

void OLedI2C::backlight()
{
}

void OLedI2C::noBacklight()
{
}

// This will 'right justify' text from the cursor
void OLedI2C::autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	sendCommand(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void OLedI2C::noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	sendCommand(LCD_ENTRYMODESET | _displaymode);
}

// -------------------------------------------------- 
void OLedI2C::setCursor(uint8_t col, uint8_t row)
{ 
  if ( row > _numlines ) {
		row = _numlines-1;    // we count rows starting w/0
	}  
  sendCommand(0x80 | (col + _row_offsets[row]));
}

void OLedI2C::setContrast(unsigned char contrast) // contrast as 0x00 to 0xFF
{
  //Set OLED Command set
	setRE();
  //sendCommand(0x2A);    // set RE to 1
	sendCommand(0x79);    // set SD to 1; enter OLED command set
	
	sendCommand(0x81);  	// Set Contrast
	sendCommand(contrast);	// send contrast value
	
  sendCommand(0x78);  	// clear SD; Exiting Set OLED Command set
  clearRE();
  //sendCommand(0x28);    // clear RE
	
}

void OLedI2C::fadeOut(unsigned char step) // step as 0x00 to 0xF
{
  //Set OLED Command set
	setRE();
  //sendCommand(0x2A);    // set RE to 1
	sendCommand(0x79);    // set SD to 1; enter OLED command set
	
	sendCommand(0x23);  	// Set 
	sendCommand(0x30 | step);	// send 
	
  sendCommand(0x78);  	// clear SD; Exiting Set OLED Command set
  clearRE();
  //sendCommand(0x28);    // clear RE
	
}

void OLedI2C::noFadeOut() // step as 0x00 to 0xF
{
  //Set OLED Command set
	setRE();
  //sendCommand(0x2A);    // set RE to 1
	sendCommand(0x79);    // set SD to 1; enter OLED command set
	
	sendCommand(0x23);  	// 
	sendCommand(0x00);	// 
	
  sendCommand(0x78);  	// clear SD; Exiting Set OLED Command set
  clearRE();
  //sendCommand(0x28);    // clear RE
}

void OLedI2C::cursorInvert() // black/white inverting of cursor
{
  //Set OLED Command set
  //sendCommand(0x2A);
  setRE();
    _extenddisplayfunction |= LCD_INVERTCURSORON;
    sendCommand(LCD_EXTENDEDFUNCTIONSET | _extenddisplayfunction);
  clearRE();
  //sendCommand(0x28);
}

void OLedI2C::noCursorInvert() // black/white inverting of cursor
{
  //Set OLED Command set
  //sendCommand(0x2A);
	setRE();
    _extenddisplayfunction &= ~LCD_INVERTCURSORON;
    sendCommand(LCD_EXTENDEDFUNCTIONSET | _extenddisplayfunction);
  clearRE();
  //sendCommand(0x28);
}

void OLedI2C::doubleHeightFont() //
{
  _displayfunction |= LCD_DOUBLEHEIGHTON;
	sendCommand(LCD_FUNCTIONSET | _displayfunction);   //2A
	//sendCommand(0x2C);  	// enable double height font
}

void OLedI2C::noDoubleHeightFont() //
{
  _displayfunction &= ~LCD_DOUBLEHEIGHTON;
	sendCommand(LCD_FUNCTIONSET | _displayfunction);
	//sendCommand(0x28);  	// disable double height font
}

void OLedI2C::reverseDisplay() // 
{
  //sendCommand(0x2A);    // set RE to 1
  //Set OLED Command set
  setRE();
	//sendCommand(0x2B);    // set RE to 1, REV to 1
  _displayfunctionRE = (_displayfunction & 0xFA) | LCD_REVERSEDISPLAYON;
	sendCommand(LCD_FUNCTIONSET | _displayfunctionRE);
  clearRE();
  //sendCommand(0x28);    // clear RE
	
}

void OLedI2C::noReverseDisplay() // 
{
  //Set OLED Command set
  setRE();
	//sendCommand(0x2A);    // set RE to 1
  //sendCommand(0x2A);    // set RE to 1
  _displayfunctionRE = (_displayfunction & 0xFA);
	sendCommand(LCD_FUNCTIONSET | _displayfunctionRE);
  clearRE();
  //sendCommand(0x28);    // clear RE
	
}

void OLedI2C::setRE() // set entension register (RE) bit
{
  //Set OLED Command set
  _displayfunction |= LCD_EXTENSIONREGISTERON;
  sendCommand(LCD_FUNCTIONSET | _displayfunction);
  //                0x20      |        0x02 
	//sendCommand(0x2A);    // set RE to 1
}

void OLedI2C::clearRE() // set entension register (RE) bit
{
  _displayfunction &= ~LCD_EXTENSIONREGISTERON;
	sendCommand(LCD_FUNCTIONSET | _displayfunction);   //2A
  //sendCommand(0x28);    // clear RE
}

void OLedI2C::sendFloat(float digit, uint8_t dec, uint8_t nad, uint8_t col, uint8_t row)
{
  char line[10];//Ten characters, I hope that's enough
  dtostrf(digit,dec,nad,line);//Convert the float value to a string
  sendString(line, col, row);
}

void OLedI2C::sendString(const char *String, uint8_t col, uint8_t row)
{
  setCursor(col, row);
  unsigned char i=0;
  while(String[i])
  {
    sendData(String[i]);      // *** Show String to OLED
    i++;
  }
}

void OLedI2C::print(const char *String)
{
  unsigned char i=0;
  while(String[i])
  {
    sendData(String[i]);      // *** Show String to OLED
    i++;
  }
}

void OLedI2C::scrollString(char* message, byte row, unsigned int time)//written by Nathan Chantrell http://nathan.chantrell.net/
{ 
  char buffer[_cols+1];
  buffer[_cols] = '\0';
  CGRAM();
  
  for (byte i=0; i < strlen(message)+_cols; i++) 
  {
    byte pos = i + 1;
    for (byte j=0; j<_cols; j++) 
    {
      if ((pos < _cols) || (pos > strlen(message)+_cols-1)) 
      { // pad and trail with blank spaces
        buffer[j]=' ';
      }
      else 
        buffer[j]=message[pos-_cols];
      
      pos++;
    }
    //cursPos(0,row); removed by PG
    sendString(buffer, 0, row); //Edited by PG tho include the cursor pos within the sendString command
    delay(time);
  }
}

//=================================================
void OLedI2C::FULL_ON_TEST(void)
{
   unsigned char i;
   unsigned char tchar[8] = {0x1F, 0x01, 0x3C, 0x3E, 0x02, 0x03, 0x93, 0x83};

   for(int j = 0; j< sizeof(tchar) ; j++)
   {

   sendCommand(0x80); //Set display data RAM addr=00H
   for(i=20;i>0;i--)
   {
      sendData(tchar[j]);  
   }
   sendCommand(0xC0); //Set display data RAM addr=40H
   for(i=20;i>0;i--)
   {
      sendData(tchar[j]);
   }

    delay(2000);
   }
}

//=======Entering Sleep Mode=============
void OLedI2C::enterSleepMode(void)
{
    sendCommand(0x2A);    /*  RE : 1  ; IS : 0  ; SD : 0 */
    sendCommand(0x79);    /*  RE : 1  ; IS : 0  ; SD : 1 */
    
    sendCommand(0xDC);    //Set VSL & GPIO (Vcc)
    sendCommand(0x02);    // Vcc OFF

    sendCommand(0x78);    /*  RE : 1  ; IS : 0  ; SD : 0 */
    sendCommand(0x28);    //
    
    sendCommand(0x08);    //Set display off

    sendCommand(0x2A);    /*  RE : 1  ; IS : 0  ; SD : 0 */
    //sendCommand(0x71);    //  
    //sendData(0x00);//     // disable internal Vdd regulator
    sendCommand(0x28);    //

}

//=======Exiting Sleep Mode=============
void OLedI2C::exitSleepMode(void)
{
     sendCommand(0x2A);   /*  RE : 1  ; IS : 0  ; SD : 0 */
     sendCommand(0x79);   //* RE : 1  ; IS : 0  ; SD : 1 */

     //sendCommand(0x71);   //Set Regulator
     //sendData(0x5C);    //Enable internal Vdd regulator at 5V I/O application mode

     sendCommand(0xDC);   //Set VSL & GPIO (Vcc)On
     sendCommand(0x03);    //

     //delay(100);

     sendCommand(0x78);   /*  RE : 1  ; IS : 0  ; SD : 0 */
     sendCommand(0x28);   /*  RE : 0  ; IS : 0  ; SD : 0 */
     sendCommand(0x0C);    //Display ON
}

void OLedI2C::powerDownLcd()
{
 // Set OLED Characterization
 sendCommand(0x2A); 
 sendCommand(0x79); 
 sendCommand(0xDC); 
 sendCommand(0x02); 
 
 sendCommand(0x78); 
 sendCommand(0x28); 
 sendCommand(0x08); 
 
 delay(100);
}

//void OLedI2C::sendCommand(unsigned char command)
void OLedI2C::sendCommand(uint8_t command)
{
    Wire.beginTransmission(OLED_Address); 	 // **** Start I2C 
    Wire.write(OLED_Command_Mode);     		 // **** Set OLED Command mode
    Wire.write(command);
    Wire.endTransmission();                 	 // **** End I2C 
    //delay(10);
}

void OLedI2C::sendData(uint8_t data)
{
  Wire.beginTransmission(OLED_Address);  	// **** Start I2C 
  Wire.write(OLED_Data_Mode);     		// **** Set OLED Data mode
  Wire.write(data);
  Wire.endTransmission();                     // **** End I2C 
}

void OLedI2C::CGRAM(void)
{
    sendCommand(0x40);
    WriteSerialData(40,CGRAM1);
}

void OLedI2C::WriteSerialData(unsigned char count,unsigned char * MSG)
{
    delay(200);

    for(int i = 0; i<count; i++)
    {
        sendData(MSG[i]);
    }
}
