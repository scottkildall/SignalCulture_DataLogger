/*
  SignalCulture_DataLogger
  by Scott Kildall
  Adapted from the SD Data Logger from TomIgoe
  This will log analog port 0
  
  Red LED will be on while we are not logging
  A momentary switch will begin logging
  Pressing the momentary switch will stop logging
  
  We log to RAM and then flush when the buffer is filled.
  
  Currently, we are not logging timecode.
  
 */

#include <SD.h>

//-- we are using the AdaFruit Data Logging shiueld, so we need pin 10
const int chipSelect = 10;

//-- flag indicates whether we are currently logging
boolean bLogging = false;
boolean bError = false;

//-- flag for swiitch presses, good for interrupts
boolean bSwitchPressed = false;  

//-- pins in use
int greenLED = 8;
int redLED = 9;
int switchPin = 2;
int analogPin = 0;

File dataFile;

void setup()
{
  //-- specify output pins
  pinMode(chipSelect, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  
  
  //-- both pins on while we initialize
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, HIGH);
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("Initializing SD card or not present");
  
  //-- we have a voltage divider, so we can have a max of about 1.1V output
  analogReference(EXTERNAL);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  
  // red on
  bSwitchPressed = false;
  bLogging = false;
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
  
  //-- INT0  is pin2
  attachInterrupt(0, switchPressed, RISING);
}

//-- main logging loop, with attempts to be as efficient as possible
void loop()
{  
   // this just looks for an interrupt flag, if tripped then we begin/end logging
   if( bSwitchPressed ) {
     // debounce and then wait for switch to be let go
     delay(200);
     while( digitalRead(switchPin) )
      ;
    
    // turn logging on/off  
    bLogging = !bLogging;
    
    //-- begin/end logging
    if( bLogging )
      beginLogging();
    else  
      endLogging();
    
    bSwitchPressed = false;
  }
  if( bLogging )
    dataFile.println(analogRead(analogPin));

}

//-- set LED pins, add sequential filename
void beginLogging() {
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);
      
  char filename[64];
  int fileNum = 1;
  
  // The SD Card neesd an all caps filename
   while( true ) {
     sprintf(filename, "DL%d.txt", fileNum);
     Serial.println(filename);
     if( SD.exists(filename) == false ) {
       dataFile = SD.open(filename, FILE_WRITE);
       break;
     }
  
     fileNum++;
  }
  
  if (dataFile) {
    Serial.println("successfully opened data log");
     bLogging = true;
  }  
   else {
    Serial.println("error opening data log");
    bLogging = false;
    bError = true;
  }
}

void endLogging() {
  dataFile.close();
  bLogging = false;
  digitalWrite(greenLED, LOW);
   digitalWrite(redLED, HIGH);
}





//-- interrupt routine, set flag
void switchPressed() {
  bSwitchPressed = true;
}








