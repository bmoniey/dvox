#include "Arduino.h"
#include "Encoder.h"
char cstr[255];
Encoder enc((uint8_t)ENC_DIR_CW_POS,(int16_t)100,(int16_t)0,(int16_t)50);
void encoderClkISR(void);
void encoderSwISR(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Serial.println("Begining Encoder test\n");
  pinMode(ENC_DT_PIN, INPUT_PULLUP);
  pinMode(ENC_CLK_PIN, INPUT_PULLUP);
  pinMode(ENC_SW_PIN, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(ENC_CLK_PIN), encoderClkISR, FALLING );//attach interrupts if using interrupts
  attachInterrupt(digitalPinToInterrupt(ENC_SW_PIN), encoderSwISR, CHANGE );//attach interrupts if using interrupts
  
  enc.setup();  
}


void loop() {
  // put your main code here, to run repeatedly:
  //enc.read();//if polling
  enc.isrReadSwPin();
  
    if(enc.encDatAvail()){
      sprintf(cstr,"Encoder::Pos:%3d Dir:%2d\n",enc.getPos(),enc.getDir());
      Serial.print(cstr);
    }
    
    if(enc.getBtnDn()){
       enc.clrBtnDn();
      sprintf(cstr,"Encoder::Button Down");
      Serial.println(cstr);
    }
  
    if(enc.getBtnUp()){
      enc.clrBtnUp();
      sprintf(cstr,"Encoder::Button up");
      Serial.println(cstr);
    }
  
    if(enc.getBtnDnLong()){
      enc.clrBtnDnLong();
      sprintf(cstr,"Encoder::Button Down long");
      Serial.println(cstr);

      enc.resetPos(0);
      sprintf(cstr,"Reseting position to zero!");
      Serial.println(cstr);
    }
}

void encoderClkISR(void){
  enc.isrReadClkPin();
  //Serial.print(".");
}

void encoderSwISR(void){
  enc.isrReadSwPin();
  //Serial.print("-");
}

