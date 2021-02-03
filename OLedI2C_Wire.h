/*
	OLedI2C.h
	OLED 1602 Library for the I2C
	OLED display from WIDE.HK
	Also my first attempt at a Library
	Phil Grant Sept 2013
	www.gadjet.co.uk
	Scrolling contributed by Nathan Chantrell http://nathan.chantrell.net/
	Updated 06/11/2013 to include the cursPos data within the sendString function.
*/
#ifndef OLedI2C_h
#define OLedI2C_h	

#include "Arduino.h"
#include "Wire.h"

// fundamental commands; RE =0
// ------------------------------------
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode  (_displaymode)
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display control    (_displaycontrol)
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for function set , RE=0    (_displayfunction)
#define LCD_2OR4LINE 0x08     //N=1 (POR)
#define LCD_1OR3LINE 0x00     //N=0
#define LCD_EXTENSIONREGISTERON 0x02      // RE: extension register for extension command
#define LCD_DOUBLEHEIGHTON 0x04  //DH=1
#define LCD_DOUBLEHEIGHTOFF 0x00  //DH=0 (POR)

// ------------------------------------

// entended command
#define LCD_EXTENDEDFUNCTIONSET 0x08

// flags for extended function set      (_extenddisplayfunction)
#define LCD_6DOTS 0x04
#define LCD_5DOTS 0x00
#define LCD_INVERTCURSORON  0x02
#define LCD_3OR4LINE 0x01 //NW = 1
#define LCD_1OR2LINE 0x00 //NW = 0

// flags for function set , RE=1         (_displayfunctionRE)
#define LCD_REVERSEDISPLAYON 0x01     //REV=1
#define LCD_REVERSEDISPLAYOFF 0x00     //REV=1

#define LCD_OLEDCHARACTERIZATION 0x78
#define LCD_OLEDCOMMANDSET 0x01

class OLedI2C {
public:
		OLedI2C(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows);
		~OLedI2C();
		void init();
    void powerDownLcd();
		void sendCommand(uint8_t command);
    void sendData(uint8_t data);
		void sendString(const char *String, uint8_t col, uint8_t row);
    void print(const char *String);
		void sendFloat(float digit, uint8_t dec, uint8_t nad, uint8_t col, uint8_t row);
		void clear();
    void home();
		void setCursor(uint8_t, uint8_t); // cloumn, row
		void scrollString(char* message, byte row, unsigned int time); //written by Nathan Chantrell http://nathan.chantrell.net/
		void lcdOff();
		void lcdOn();
    void display();
    void noDisplay();
    void noBlink();
    void blink();
    void noCursor();
    void cursor(); 
    void backlight();
    void noBacklight();
    void autoscroll();
    void noAutoscroll(); 
    void cursorInvert();
    void noCursorInvert();
    void doubleHeightFont();
    void noDoubleHeightFont();
    void reverseDisplay();
		void noReverseDisplay();
    void fadeOut(unsigned char step);
    void noFadeOut();
    void setContrast(unsigned char contrast); // contrast should be the hex value between 0x00 and 0xFF
    void enterSleepMode(void);
    void exitSleepMode(void);
    void FULL_ON_TEST(void);
    
private:
    uint8_t _row_offsets[4];
    uint8_t _Addr;
    uint8_t _displayfunction;           // RE = 0
    uint8_t _displayfunctionRE;         // RE = 1
    uint8_t _extenddisplayfunction;     // RE = 1 
    uint8_t _displaycontrol;
    uint8_t _displaymode;
    uint8_t _numlines;
    uint8_t _cols;
    uint8_t _rows;
  
    void CGRAM(void);
    void WriteSerialData(unsigned char count,unsigned char * MSG);
    void setRE();
    void clearRE();
    
};
#endif
