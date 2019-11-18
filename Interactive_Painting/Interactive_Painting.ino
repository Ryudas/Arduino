// include SPI, MP3 and SD libraries
#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// define the pins used
#define CLK 13       // SPI Clock, shared with SD card
#define MISO 12      // Input data, from VS1053/SD card
#define MOSI 11      // Output data, to VS1053/SD card
// Connect CLK, MISO and MOSI to hardware SPI pins. 
// See http://arduino.cc/en/Reference/SPI "Connections"

// These are the pins used for the breakout example
#define BREAKOUT_RESET  9      // VS1053 reset pin (output)
#define BREAKOUT_CS     10     // VS1053 chip select pin (output)
#define BREAKOUT_DCS    8      // VS1053 Data/command select pin (output)
// These are the pins used for the music maker shield
#define SHIELD_RESET  -1      // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)

// These are common pins between breakout and shield
#define CARDCS 4     // Card chip select pin
// DREQ should be an Int pin, see http://arduino.cc/en/Reference/attachInterrupt
#define DREQ 3       // VS1053 Data request, ideally an Interrupt pin

// Lets make our vibration motors
#define VIB_MOT_1 2 // Careful, here we have to use a pin that can be used for pwm.
#define VIB_MOT_2 3 // Careful, here we have to use a pin that can be used for pwm.
#define VIB_MOT_3 4 // Careful, here we have to use a pin that can be used for pwm.
#define VIB_MOT_4 5 // Careful, here we have to use a pin that can be used for pwm.


Adafruit_VS1053_FilePlayer musicPlayer = 
  // create breakout-example object!
  //Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
  // create shield-example object!
  Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

// timers that contains time of 
// most recent play of audio file
unsigned long start_timer;

// first timer locatiin, defined in milliseconds 
#define timer_position_1 60*1000 // 60 seconds
// first timer event duration
#define duration_1 3 *1000 // 3 seconds

void setup() {
  // put your setup code here, to run once:
  pinMode(VIB_MOT_1, OUTPUT);
  pinMode(VIB_MOT_2, OUTPUT);
  pinMode(VIB_MOT_3, OUTPUT);
  pinMode(VIB_MOT_4, OUTPUT);
  pinMode(VIB_MOT_1, OUTPUT);
  
  Serial.begin(9600); // setting baud speed for Serial (a baud is a pulse)


  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("VS1053 found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }

  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(20,20);

  // Timer interrupts are not suggested, better to use DREQ interrupt!
  //musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  
  // Play one file, don't return until complete
  Serial.println(F("Playing track 001"));
  musicPlayer.playFullFile("/track001.mp3");
  
  // Play another file in the background, REQUIRES interrupts!
  Serial.println(F("Playing track 002"));
  // after println... start timer 1

  start_timer = millis();
  musicPlayer.startPlayingFile("/track002.mp3");
}

void loop() {

    //analogWrite(VIB_MOT_1, 160);
    //analogWrite(VIB_MOT_2, 160);
    //analogWrite(VIB_MOT_3, 160);
    //analogWrite(VIB_MOT_4, 160);

// simplest way
//  // once we cross threshold 1, do something, for duration duration_1
//  if ( ( millis() - start_timer ) >= timer_position_1 )
//  {
//     // for example 
//     if( ( millis() - start_timer ) <= timer_position_1 + duration_1 ){
//     // analogWrite(VIB_MOT_1, 160); // turn on
//     } else {
//     // analogWrite(VIB_MOT_1, 0); // turn off
//     }
//  }

  // alternative using set_up_vibration, must be called every cycle
  // note you can use just values here :
  //set_up_vibration(VIB_MOT_1, 60 *1000, 3*1000);
  set_up_vibration(VIB_MOT_1, timer_position_1, duration_1);
  
  
  // File is playing in the background
  if (musicPlayer.stopped()) {
    //Serial.println("Done playing music");
    // play again
    start_timer = millis();
    musicPlayer.playFullFile("/track001.mp3");
  }

   // 50 ms break
   delay(50); 
  
}

// takes motor n, and activates it at given time position
// (in ms) for duration (in ms) (note it uses start_timer
// in its workings, so dont delete that variable
// must be called as often as possible
void set_up_vibration( uint8_t motor_pin,
                      unsigned long time_position,
                      unsigned long duration ){
                      
    // once we cross threshold 1, do something, for duration duration_1
  if ( ( millis() - start_timer ) >= time_position )
  {
     // for example 
     if( ( millis() - start_timer ) <= time_position + duration ){
         analogWrite(motor_pin, 160); // turn on
     } else {
         analogWrite(motor_pin, 0); // turn off
     }
  }


}
                    

/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
