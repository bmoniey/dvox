/**
* This is for a KY-040 encoder I picke up on Amazon
* Pins
* 1. 3V3
* 2. GND
* 3. DT   added 100nF cap
* 4. SW   added 100nF cap
* 5. CLK  added 100nF cap
*
*/
#ifndef ENCODER_H
#define ENCODER_H
#include "Arduino.h"
#define  ENC_DT_PIN  3
#define  ENC_CLK_PIN 2
#define  ENC_SW_PIN  4
#define  ENC_DT_BIT   0x01
#define  ENC_CLK_BIT  0x02
#define  ENC_SW_BIT   0x04
#define  ENC_BTN_LONG_PRESS_MS 500
#define  ENC_DIR_CW_POS 0
#define  ENC_DIR_CW_NEG 1
#define  ENC_PMAX    ((int16_t)32767)
#define  ENC_PMIN    ((int16_t)-32767)

class Encoder{
	public:
  Encoder(){};
  Encoder(uint8_t dir_);
  Encoder(uint8_t dir_, int16_t pmax_, int16_t pmin_);
  Encoder(uint8_t dir_, int16_t pmax_, int16_t pmin_,int16_t pos_);
	void setup(void);
	void read(void);//call this to poll the both the switch and encoder
	void  isrReadSwPin(void);//call this from the switch pin interrupt
  void  isrReadClkPin(void);//call this from the Clock pin interrupt 
	int16_t  getPos(void);//returns current position
	int16_t  getDir(void);//returns direction 1=ccw,0=stop,-1=ccw
  uint8_t  getBtn(void);
  uint8_t  getBtnDn(void);
  uint8_t  getBtnUp(void);
  uint8_t  getBtnDnLong(void);
  void  clrBtnDn(void);
  void  clrBtnUp(void);
  void  clrBtnDnLong(void);
  void  clrBtnAll(void);  
  void     readSW(void);
  void  encoderSetDir(uint8_t dir);
  uint8_t encDatAvail(void);//Use this to check if there is new encoder position available
                            //calling this resets the status so will fail on second call if no new data is available
  void resetPos(int16_t value);
  void setPminPmax(int16_t pmax_,int16_t pmin_);
  
	private:
  uint8_t newDat;
  uint8_t encDir; //holds setting for direction output
  void readDT();
	uint8_t  sw[2];
  uint8_t  clk[2];
	uint8_t  enc[2];//holds last reads of the DT and CLK pin and sw pin
	int16_t  pos[2];//holds the last 2 position values
  int16_t  pmin = ENC_PMIN;
  int16_t  pmax = ENC_PMAX;
	int16_t  dir;//
	uint8_t  btn;//current button state
	uint8_t  btnDn;//button down event
	uint8_t  btnUp;//button up event
	uint8_t  btnDnLong;//button Long Press
	uint32_t btnDnTmr;//holds the millis() when the button was pressed down
	
}; 
#endif
