/*
   DuinoVOX. Ver.3.1.1  Arduino based PTT control for radio interfaces. 
   Written Aug. 2016 by Kevin Loughin.  Originally for use
   in a clone of the Tigertronics Signalink design idea.
   
 */

//  Naming the pins I'm using for clarity.
const int audioInPin = A1;  // Audio sense
const int tailsetInPin = A3;  // delay time adjustment POT
const int sensitivity = A5; // threshold set POT
const int PTToutPin = 13; // output to keying transistor

//  declaring variables that we'll use
int delayvalue = 0; // amount of time in hundreths of a second before dropping PTT
int threshold = 0; // audio trigger level initial value
int ptt = 0;  // variable for holding current PTT delay countdown
int audio = 0;  // variable that will hold audio sense
int PTT_ON; // Flag to indicate current PTT status

void setup() {
 // Turn off PTT right away so we're not keying on startup

  pinMode (PTToutPin, OUTPUT);
 digitalWrite(PTToutPin, LOW);
 PTT_ON = 0; 
}

void loop() 
{
    // Here we go. First read the pots and set variables

 //delayvalue = analogRead(tailsetInPin) / 5; // yields 0 to 204
 delayvalue   = 500 / 5; // yields 0 to 204
 delayvalue   = delayvalue + 2; // adjusted to no less than 2, 1 after first pass through loop
 //threshold    = analogRead(sensitivity) * .8; // yields 0 to 819
 threshold      = 450 * .8; // yields 0 to 819
 threshold    = threshold + 70; // adjusted 70 to 889

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
 delay(10);  // wait 10ms before looping again.  loop 100 times/second
  
}
