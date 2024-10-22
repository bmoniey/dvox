/*
   DuinoVOX. Ver.3.1.1  Arduino based PTT control for radio interfaces. 
   Written Aug. 2016 by Kevin Loughin.  Originally for use
   in a clone of the Tigertronics Signalink design idea.
   
 */
#include "Arduino.h" 
#include "Encoder.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
char cstr[255];
Encoder enc((uint8_t)ENC_DIR_CW_POS,(int16_t)100,(int16_t)0,(int16_t)50);
void encoderClkISR(void);
void encoderSwISR(void);
void updateDisplay(void);
void doEncoder(void);

//  Naming the pins I'm using for clarity.
const int audioInPin = A1;  // Audio sense
const int tailsetInPin = A3;  // delay time adjustment POT
const int sensitivity = A5; // threshold set POT
const int PTToutPin = 13; // output to keying transistor

//  declaring variables that we'll use
int16_t delayvalue = 0; // amount of time in hundreths of a second before dropping PTT
int16_t threshold = 0; // audio trigger level initial value
int ptt = 0;  // variable for holding current PTT delay countdown
uint16_t audio = 0;  // variable that will hold audio sense
int PTT_ON; // Flag to indicate current PTT status
int sel_idx,sel;//used to keep track of the menu
int dspCntr;

void setup() {
  Serial.begin(9600);
  Serial.println("Begining DVOX1\n");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(1000);
  
  pinMode(ENC_DT_PIN, INPUT_PULLUP);
  pinMode(ENC_CLK_PIN, INPUT_PULLUP);
  pinMode(ENC_SW_PIN, INPUT_PULLUP);
  pinMode (PTToutPin, OUTPUT);
  digitalWrite(PTToutPin, LOW); 
  attachInterrupt(digitalPinToInterrupt(ENC_CLK_PIN), encoderClkISR, FALLING );//attach interrupts if using interrupts
  //attachInterrupt(digitalPinToInterrupt(ENC_SW_PIN), encoderSwISR, CHANGE );//attach interrupts if using interrupts
  
  enc.setup(); 
  // Turn off PTT right away so we're not keying on startup
  PTT_ON = 0;
  delayvalue = 50;
  threshold  = 400;
  display.clearDisplay();

  //sel,sel_item
  sel=0;//0 not selected, selected
  sel_idx = 0;//0=vox 1=delay
  dspCntr = 0;//initialy the display update counter    
}

void loop() 
{
    // Here we go. First read the pots and set variables
    doEncoder();

    // Check for audio. Set delay if present
 audio = analogRead(audioInPin);
 if ( audio > threshold ) // if audio in exceeds sesitivity threshold.
 {
   ptt = delayvalue; // set ptt to current delay in hundredths of a second
 }

  // PTT control check
 if ( ptt > 0 ) // if PTT is positive (loop is counting down)
 { 
   if ( PTT_ON == 0 ) // check that we havn't already turned it on
   {
     digitalWrite(PTToutPin, HIGH); // turn on PTT if it's off
     PTT_ON = 1; // so we don't waste time writing on next pass
   }
 
  ptt = ptt - 1; // count down in hundredths of a second
  }
 else // the counter reached zero.
 {
   if ( PTT_ON == 1 )  // check if we already turned it off
   {
            digitalWrite(PTToutPin, LOW); // turn off PTT
           PTT_ON = 0; // Remeber that we've turned it off
   } 
 }

 //update the display every 10 loops
 dspCntr++;
 if(dspCntr>9){
  dspCntr=0;
  updateDisplay();
 }
   
 
 delay(10);  // wait 10ms before looping again.  loop 100 times/second 
}

void encoderClkISR(void){
  enc.isrReadClkPin();
  //Serial.print(".");
}

void encoderSwISR(void){
  enc.isrReadSwPin();
  //Serial.print("-");
}

void updateDisplay(){
  int w1,t1,w2,t2,b1;
  b1 = 5;
  w1 = audio / 24;
  t1 = 10;
  w2 = threshold / 24;
  t2 = t1*2;
  display.clearDisplay();
  //DVOX VER
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(15,0);            // Start at top-left corner
  display.println(F("DVOX 1.0"));
  
  //dot
  display.drawCircle(5, 25, 5, WHITE);
  //VOX
  display.setCursor(15,17);
  display.println(F("VOX:"));
  //VOX Bar
  display.drawRect(60, 17,    w2, 10, WHITE);//threshold
  display.fillRect(60, 19,    w1, 6, WHITE);//audio
  //dot
  display.drawCircle(5, 40, 5, WHITE);
  //DLY:
  display.setCursor(15,32);
  sprintf(cstr,"DLY:%4d",delayvalue);
  display.println(cstr);
  
  //square
  //PTT
  if(PTT_ON){
    display.fillRect(30,48,128-30-30,64-48, WHITE);
    display.setTextColor(BLACK);
    display.setCursor(30+15,49);
    display.println(F("PTT"));
  }else{
    display.drawRect(30,48,128-30-30,64-48, WHITE);
    display.setCursor(30+15,49);
    display.println(F("PTT"));
  }
  //select dot
  if(sel_idx){
    //delay
    if(sel){
      display.fillCircle(5, 40, 3, WHITE);
    }else
    {
      display.drawCircle(5, 40, 3, WHITE);
    }
  }else
  {
    //vox
    if(sel){
      display.fillCircle(5, 25, 3, WHITE);
    }else{
      display.drawCircle(5, 25, 3, WHITE);
    }
  }
  display.display();
}


void doEncoder(){
     enc.isrReadSwPin();//check the switch on the encoder
    if(enc.getBtnDn()){
       enc.clrBtnDn();
       sel = sel ? 0 : 1;
      //sprintf(cstr,"Encoder::Button Down:%d,%d",sel,sel_idx);
      //Serial.println(cstr);
    }
    
    if(enc.encDatAvail()){
        if(sel){
          if(sel_idx == 0){
            //adjust vox
            threshold += enc.getDir()*10;
            if(threshold > 1000){ threshold = 1000;}
            if(threshold < 10 ){ threshold = 1;} 
          }else if(sel_idx == 1){
            //adjust delay
            delayvalue += enc.getDir();
            if(delayvalue > 100){delayvalue = 100;}
            if(delayvalue < 1){delayvalue   = 1;}
          }
        }else{
          sel_idx++;
          if(sel_idx > 1){
            sel_idx = 0;
          }
        }
             
        //sprintf(cstr,"Encoder::Pos:%3d Dir:%2d threshold:%3d delay:%3d\n",enc.getPos(),enc.getDir(),threshold,delayvalue);
        //Serial.print(cstr);
    }
}

