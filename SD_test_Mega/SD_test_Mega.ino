#include <SD.h>                      // need to include the SD library
#define SD_ChipSelectPin PA15  //example uses hardware SS pin 53 on Mega2560
//#define SD_ChipSelectPin 10  //using digital pin 4 on arduino nano 328, can use other pins
//#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>

//TMRpcm tmrpcm;   // create an object for use in this sketch
File myFile;
ICM20649 sensor(D11,D12,D13, D10);
void setup(){
//  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
    SPI.setSCLK(PA5);
  SPI.setMISO(PA6);
  SPI.setMOSI(PA7);

  Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }

  Serial.print("OK");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("tdsdest.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  //tmrpcm.play("music"); //the sound file "music" will play each time the arduino powers up, or is reset
}

void loop(){  
  if(Serial.available()){    
    if(Serial.read() == 'p'){ //send the letter p over the serial monitor to start playback
      //tmrpcm.play("music");
    }
  }
}
