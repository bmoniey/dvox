/**
* This is for a KY-040 encoder I picke up on Amazon
* Pins
* 1. 3V3
* 2. GND
* 3. DT
* 4. SW
* 5. CLK
*
*/

#include "Encoder.h"
Encoder::Encoder(uint8_t dir_){
  encDir = dir_;  
}

Encoder::Encoder(uint8_t dir_, int16_t pmax_, int16_t pmin_){
  encDir = dir_; 
  pmax = pmax_;
  pmin = pmin_; 
}

Encoder::Encoder(uint8_t dir_, int16_t pmax_, int16_t pmin_,int16_t pos_){
  encDir = dir_; 
  pmax = pmax_;
  pmin = pmin_; 
  pos[0] = pos_;
  pos[1] = pos_;
}

void Encoder::setup(void){
	btnDnTmr=0;
  sw[0]=sw[1]   = digitalRead(ENC_SW_PIN);
  clk[0]=clk[1] = digitalRead(ENC_CLK_PIN);
}
/**
 * set which dirction is positive
 * use the ENC_DIR_CW_NEG and ENC_DIR_CW_POS definitions
 */
void Encoder::encoderSetDir(uint8_t dir_){
  encDir=dir_;
}

uint8_t  Encoder::encDatAvail(void){
  uint8_t retVal      = newDat;
          newDat = 0;
          return retVal;
}
/**
 * readDt()
 * This function reads the encoder and
 * detects the change on the clock pin
 * if the DT (data) pin is low the position is incremented
 * if the DT (data) pin is high the position is decremented
 * 
 * This function is meant to be polled
 */
void Encoder::readDT(void){
   pos[1]   = pos[0];
   clk[1]   = clk[0];
   clk[0]   = digitalRead(ENC_CLK_PIN);
   dir=0;
   if(!clk[0] && clk[1]){
      newDat = 1;
      //clk down
      if(!digitalRead(ENC_DT_PIN)){
        if(encDir == ENC_DIR_CW_POS){
          pos[0]++;
          dir = 1;
        }else{
          pos[0]--;
          dir = -1;
        }
      }else{
        if(encDir == ENC_DIR_CW_POS){
          pos[0]--;
          dir = -1;
        }else{
          pos[0]++;
          dir = 1;
        }
      }
      //check saturation
      if(pos[0] > pmax){pos[0] = pmax;}
      if(pos[0] < pmin){pos[0] = pmin;}
   }
}

/**
 * readSW
 * can be polled or called from interrupt
 * see isrReadSwPin() which call this function in the interrupt
 * Important that the interrupt is setup for CHANGE so that both the down
 * up events are recognized.
 */
void Encoder::readSW(void){
	sw[1]=sw[0];
	sw[0]=digitalRead(ENC_SW_PIN);
	btn = !sw[0];//save current state of button
	if(!sw[0] && sw[1]){
		//down event
		btnDn=1;
		btnDnTmr = millis();
	}

	if(sw[0] && !sw[1]){
		//up Event
		btnUp=1;
		//long press?
		if((millis() - btnDnTmr) > ENC_BTN_LONG_PRESS_MS){
			btnDnLong = 1;
		}
	}	
}

/**
 * Call this from your Swp
 */
void Encoder::isrReadSwPin(void){
  readSW();
}

void Encoder::resetPos(int16_t value)
{
  pos[0] = value;
  pos[1] = value;
  dir    = 0;    
}

/**
 * Call this from your Sw
 * Interrupt should be configured to trigger on falling edge
 */
void Encoder::isrReadClkPin(void){
      pos[1]   = pos[0];
      dir = 0;
      if(!digitalRead(ENC_CLK_PIN)){
        newDat=1;
        if(!digitalRead(ENC_DT_PIN)){
          if(encDir == ENC_DIR_CW_POS){
            pos[0]++;
            dir = 1;
          }else{
            pos[0]--;
            dir = -1;
          }
        }else{
          if(encDir == ENC_DIR_CW_POS){
            pos[0]--;
            dir = -1;
          }else{
            pos[0]++;
            dir = 1;
          }
        }
        //check saturation
        if(pos[0] > pmax){pos[0] = pmax;}
        if(pos[0] < pmin){pos[0] = pmin;}
      }
}

void Encoder::setPminPmax(int16_t pmax_,int16_t pmin_){
   pmax = pmax_;
   pmin = pmin_;
}

/**
*call read if you just want to poll the encoder
*/
void Encoder::read(void){
	readDT();
	readSW();
}

int16_t Encoder::getDir(void){
	return dir;
}
/**
*call read if you just want to poll the encoder
*/
int16_t Encoder::getPos(void){
	return pos[0];
}

/**
 * get the status of the button
 */
uint8_t  Encoder::getBtn(void){
    return btn;
  }
  
uint8_t  Encoder::getBtnDn(void){return btnDn;};
uint8_t  Encoder::getBtnUp(void){return btnUp;};
uint8_t  Encoder::getBtnDnLong(void){return btnDnLong;};
   void  Encoder::clrBtnDn(void){btnDn=0;};
   void  Encoder::clrBtnUp(void){btnUp=0;};
   void  Encoder::clrBtnDnLong(void){btnDnLong=0;};
   void  Encoder::clrBtnAll(void){btnDn=0;btnUp=0;btnDnLong=0;};
  
