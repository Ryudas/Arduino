#include <SD.h>                      // need to include the SD library
//#define SD_ChipSelectPin PA15  //example uses hardware SS pin 53 on Mega2560
//#define SD_ChipSelectPin 10  //using digital pin 4 on arduino nano 328, can use other pins
#include <SPI.h>
#include "ICM20649.h"
//
//
//
//File myFile;

ICM20649 sensor(D11,D12,D13, D10);

void setup(){

  // Set SPI Variables
  SPI.setSCLK(PA5);
  SPI.setMISO(PA6);
  SPI.setMOSI(PA7);

  Serial.begin(9600);
//  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
//    Serial.println("SD fail");  
//    return;   // don't do anything more if not
//  }

    if (sensor.open()) {
         Serial.println("Device detected!\n");
    }
//
//  Serial.print("OK");
//
//  // open the file. note that only one file can be open at a time,
//  // so you have to close this one before opening another.
//  myFile = SD.open("data.txt", FILE_WRITE);
//
//  // if the file opened okay, write to it:
//  if (myFile) {
//    Serial.print("Writing to test.txt...");
//    myFile.println("testing 1, 2, 3.");
//    // close the file:
//    myFile.close();
//    Serial.println("done.");
//  } else {
//    // if the file didn't open, print an error:
//    Serial.println("error opening test.txt");
//  }
//  //tmrpcm.play("music"); //the sound file "music" will play each time the arduino powers up, or is reset
}

void loop(){  

  float acc_x, acc_y, acc_z;
  float gyr_x, gyr_y, gyr_z;

  sensor.measure();

  sensor.get_accelerometer(&acc_x, &acc_y, &acc_z);
  sensor.get_gyroscope(&gyr_x, &gyr_y, &gyr_z);

  //Print the current accelerometer measurement
  //printf("acc: %.3f  %.3f  %.3f\n", acc_x, acc_y, acc_z);

   Serial.print("acc: ");
   Serial.print( acc_x,3); 
   Serial.print( acc_y, 3);
   Serial.println( acc_z, 3);
   
  //Print the current gyroscope measurement
//  printf("gyr: %.3f  %.3f  %.3f\n", gyr_x, gyr_y, gyr_z);

  //Sleep for 0.5 seconds
  delay(500);
}
