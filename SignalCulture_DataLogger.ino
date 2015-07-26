/*
  SignalCulture_DataLogger
  by Scott Kildall
  Adapted from the SD Data Logger from TomIgoe
  This will log analog port 0
  
  Red LED will be on while we are not logging
  A momentary switch will begin logging
  Pressing the momentary switch will stop logging
  
  Ideally, we would log to RAM and then flush when the buffer is filled.
  
  We are logging to microseconds, 1000 micros = 1 millis
  
  Other options would be to overclock, which might give us 2X the data-logging rate
  However, this is complicated.
  
  Also, simply running this with a fan on it, too cool it down might make the CPU faster
  
  max # of characters
  - signal value 0-1023 = 4 charcters
  - command (,) = 1 character
  - max unsigned long = 18446744073709551615 = 20
  - newline = 1 chacter
  - termination string = 1 char
  - total = 27 characters, add 1 for safekeeping to make 28
  
  sample time on a is about 280 microseconds
  a faster SD card will have marginal improvements since we are using a RAM buffer and flushing it every 20 lines or so
  
  Sample rate ~3500-3600 H
  i.e. 3500-3600 samples/second
  Pretty good!!
  
  ~ 1.7mb file for 60 seconds
  60 minutes = 102mb
  
  The next issue is that we run out of micros(), so have an overflow problem after about a minute
  We could fix this in code (best solution)
  
  Could also log for a specified amount of time, using a millis() timer
  
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

unsigned long startMicros = 0;    // subtract from micros() when logging

#define MAX_LINE_SIZE (28)
#define BUFFER_SIZE (400)        // 400 is safe, 560 will cause error with opening datalog

int bufferIndex = 0;
char *buffer;

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
  Serial.print("buffer size = " );
  Serial.println(BUFFER_SIZE);
  
  // we allocate from SRAM (I think)
  buffer = new char[BUFFER_SIZE];
  
  if( buffer == 0L ) {
    Serial.println("new allocator failed" );
    return;
  }
  
  Serial.print("buffer address = " );
  Serial.println( (unsigned long)buffer );
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
  if( bLogging ) {
    // we will want to optimize this to write to a buffer and then flush the buffer when we fill it up
    sprintf(buffer + bufferIndex, "%d,%lu\n", analogRead(analogPin),micros() - startMicros);
    bufferIndex += strlen(buffer+bufferIndex);
    
    // check to see if we are about to overflow the vuffer
    if( bufferIndex > BUFFER_SIZE - MAX_LINE_SIZE ) {
      bufferIndex = 0;
 
      // buffer of data
      dataFile.print(buffer+bufferIndex);  // see if the entire thing prints out
    }
  }
}

//-- set LED pins, add sequential filename
void beginLogging() {
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);
      
  char filename[64];
  int fileNum = 1;
  
  // The SD Card needs an all caps filename
   while( true ) {
     sprintf(filename, "DATA_%d.CSV", fileNum);
     Serial.println(filename);
     if( SD.exists(filename) == false ) {
       dataFile = SD.open(filename, FILE_WRITE);
       break;
     }
  
     fileNum++;
  }
  
  if (dataFile) {
    Serial.println("successfully opened data log");
    
    // write header
    dataFile.println("value, ms");
    
     bLogging = true;
     bufferIndex = 0;
     
     startMicros = micros();
  }  
   else {
    Serial.println("error opening data log");
    bLogging = false;
    bError = true;
  }
}

void endLogging() {
  // log the remaining buffer
  bufferIndex = 0;
  dataFile.print(buffer+bufferIndex);  // see if the entire thing prints out
      
  dataFile.close();
  bLogging = false;
  digitalWrite(greenLED, LOW);
   digitalWrite(redLED, HIGH);
}


//-- interrupt routine, set flag
void switchPressed() {
  bSwitchPressed = true;
}





