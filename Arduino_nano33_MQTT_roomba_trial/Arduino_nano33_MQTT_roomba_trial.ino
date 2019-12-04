// The example uses an arduno sdk for DCD hub to connect and update
// individual thing's property.
// You can check live update on your dcd hub dashboard after a successful
// connection here: https://dwd.tudelft.nl/subject.
//
// by Nirav Malsattar <n.malsattar@tudelft.nl>
// https://github.com/datacentricdesign/dcd-sdk-arduino


#include <dcd_hub_arduino.h>
#include <Arduino.h>
#include "wiring_private.h"
#include "arduino_secrets.h"

Uart mySerial (&sercom1, 13, 8, SERCOM_RX_PAD_1, UART_TX_PAD_2);

// A list of masks for the sensordata. Used to check validity of data: if an unused bit is set, there is a (sync?) error.
//                        0    1   2   3   4   5   6   7    8    9   10   11  12   13   14   15   16  17   18   19   20   21   22   23   24   25
//                      BMP ,WALL (6x)              ,Mcur,Dirt 2x  ,R/C, BTN,Distance ,Angle    ,CHG,Vbat     ,Ibat     ,Tbat,Bcharge  ,Bcapacity,
byte sensorMask[26] = { 0xFF, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1F, 0xFF, 0xFF, 0xFF, 0xF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
byte sensorData[26];

// Attach the interrupt handler to the SERCOM
void SERCOM1_Handler()
{
  mySerial.IrqHandler();
}


dcd_hub_arduino dcdHub; //creates a class object from library

void setup() {
  //Initialize serial and wait for port to open:
  // Reassign pins 13 and 8 to SERCOM (not alt this time)
  pinPeripheral(13, PIO_SERCOM);
  pinPeripheral(8, PIO_SERCOM);
  // Start my new hardware serial
  mySerial.begin(57600);
  
  initRoomba();
 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // Connects to dcd hub using your secret credential from "arduino_secrets.h"
  //{class_object}.connect(SECRET_SSID, SECRET_PASS, THING_ID, THING_TOKEN)
  
  // Make sure you have stored your credentials on "arduino_secrets.h" before running this command
  
  //dcdHub.connect(SECRET_SSID, SECRET_PASS, THING_ID, THING_TOKEN, "Roomba");
  Serial.println();
}

void initRoomba() {

    while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Start Up");

  // set baudrate 
  mySerial.write(129);
  mySerial.write(byte(10)); // 57600 bps
  
  Serial.println("START Mode (PASSIVE)"); //
  mySerial.write(128);
  delay(50);
  //    Serial.println("CLEAN Mode");
  //    mySerial.write(byte(135));
  //    delay(50);

  Serial.println("SAFE Mode");
  mySerial.write(130); // Control - goto SAFE mode
  delay(50);
  mySerial.write(byte(138));  // MOTORSc  mySerial.write(byte(1)); // Main brush
  delay(500);
  mySerial.write(byte(138));  // MOTORS
  mySerial.write(byte(2)); // Vacuum on
  delay(500);
  mySerial.write(byte(138));  // MOTORS
  mySerial.write(byte(4)); // Whiper on
  delay(500);
  mySerial.write(138);  // MOTORS
  mySerial.write(byte(0)); // OFF
  delay(500);

  Serial.println("End Init");

}

const int timeOut = 25; // 20 ms is maximum response time

//
bool dataReady() {
  // Wait for response or timeout
  unsigned long now = millis();
  while ((!mySerial.available()) && ((millis() - now) < timeOut)); // wait for response...
  if ((millis() - now) >= 25) {
    Serial.println("read timeout");
    return false; // no valid data.
  } else {
    return true;
  }
}

int flushSerial() {
  // flush the serial data stream (if any)
  int i = 0;
  while (mySerial.available()) {
    mySerial.read(); // flush serial buffer
    i++;
    delayMicroseconds(1);
  }
  return i;
}

byte readPhysical() {
  // Send command
  mySerial.write(142);
  mySerial.write((byte)1);

  //  // Wait for response or timeout
  //  unsigned long now=millis();
  //  while ((!mySerial.available()) && ((millis()-now)<timeOut)); // wait for response...
  //  if ((millis()-now)>=25) { Serial.println("read timeout"); return 0; }// no valid data.

  if (dataReady()) {
    // Read data
    byte bytes;

    //Serial.println("Read (raw) Bytes:");
      Serial.println("");
      Serial.println("RAW Data    ");
    for (bytes = 0; bytes < 10; bytes++) {
      byte b = mySerial.read();
      Serial.print(b,HEX); // debug: print read byte
      Serial.print(" ");
      if (b & (~sensorMask[bytes])) { // Check if unused bits are set, if so there is a read error
        //Serial.print("ERR! ");
        break; // break out of the for loop
      } else { // byte is ok, store and continue
        sensorData[bytes] = b;
      }
    }
    //    flushSerial();
    return bytes;
  } else {
    // return the number of bytes read - should always be 10, otherwise an error has occurred
    flushSerial();
    return 0;
  }
}

byte readInternal() { // Read values of physical sensors
  mySerial.write(142);
  mySerial.write((byte)2);

  if (dataReady()) {
    byte bytes;
    for (bytes = 10; bytes <= 15; bytes++) {
      byte b = mySerial.read();
      sensorData[bytes] = b;

      Serial.print(b, HEX); // debug: print read byte
      Serial.print(" ");

      if (sensorData[10] != 0) {
        Serial.print("ERR");
      }

      if (sensorData[11] > 7) {
        Serial.print("ERR");
      }
    }
    //    flushSerial();
    return bytes;
  } else {
    // return the number of bytes read - should always be 10, otherwise an error has occurred
    flushSerial();
    return 0;
  }
}

byte readBattery() { // Read values of battery
  mySerial.write(142);
  mySerial.write((byte)3);
  //delay(readDelay); //allow some time for response.

  if (dataReady()) {
    byte bytes;
    for (bytes = 16; bytes <= 25; bytes++) {
      sensorData[bytes] = mySerial.read();
      if (sensorData[16] > 5) {
        //Serial.print("ERR! ");
        break;
      }
    }
    //    flushSerial();
    return bytes;
  } else {
    // return the number of bytes read - should always be 10, otherwise an error has occurred
    flushSerial();
    return 0;
  }
}


void printPhysical() {
  Serial.println("");
  Serial.println("Serial Data");
  Serial.print("Wheel: ");
  Serial.print(sensorData[0], HEX); // Bump and wheeldrop
  Serial.print(", Wall: ");
  Serial.print(sensorData[1], BIN); // Wall
  Serial.print(sensorData[2], BIN); // Cliff left
  Serial.print(sensorData[3], BIN); // Cliff Front Left
  Serial.print(sensorData[4], BIN); // Cliff Front Right
  Serial.print(sensorData[5], BIN); // Cliff Right
  Serial.print(", Dirt L: ");
  Serial.print(sensorData[8], BIN); //
  Serial.print(", R: ");
  Serial.print(sensorData[9], BIN); //
  Serial.println();
}


void loop() {
  
  // call keep_alive_mqtt() regularly to avoids being disconnected by the MQTT broker
  dcdHub.keep_alive_mqtt();

  //call to an update_property object to update property value as an array according to it's "property_id"
  //{class_object}.update property (property_id, value[], dimension)

  mySerial.write(130); // Control - goto SAFE mode
  
  readPhysical();
  //readInternal();
  //readBattery();
  //printPhysical();
 // printInternal();
 // printBattery();
  send_data_to_hub();
  
  // 100 hz
  //delay(10);
  
}


// parses and sends roomba data to dcd HUB
// in premade 
void send_data_to_hub() 
{
  // Handles Roomba data packet 
  // dcdHub.update_property("random-02d3",value, 1);
  // dcdHub.update_property("my-random-property3-e0cf",value2, 2);
  // dcdHub.update_property("my-random-property2-563b",value3, 3);

  // Some random 1D, 2D, 3D values to upload on the hub. Later, you can replace these with your sensors value.  
  float value_1D[] = {random(5)};
  float value_2D[] = {random(80), random(25)};
  float value_3D[] = {random(80), random(25), random(60)};
  float value_4D[] = {random(80), random(25), random(60), random(60)};
// A list of masks for the sensordata. Used to check validity of data: if an unused bit is set, there is a (sync?) error.
//                        0    1   2   3   4   5   6   7    8    9   10   11  12   13   14   15   16  17   18   19   20   21   22   23   24   25
//                      BMP ,WALL (6x)              ,Mcur,Dirt 2x  ,R/C, BTN,Distance ,Angle    ,CHG,Vbat     ,Ibat     ,Tbat,Bcharge  ,Bcapacity,
byte sensorMask[26] = { 0xFF, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1F, 0xFF, 0xFF, 0xFF, 0xF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

//  bump_map 
  // get values , L, R 
  value_2D[0] = float( (sensorData[0]&0x2)>>1);  value_2D[1] = float((sensorData[0]&0x1));
  
  dcdHub.update_property("bump_map",value_2D, 2);
//  wheeldrop_map, L, R, C

  value_3D[0] = float( (sensorData[0]&0x8)>>3);  value_3D[1] = float( (sensorData[0]&0x4)>>2); value_3D[2] = float( (sensorData[0]&0x10)>>4);
  dcdHub.update_property("wheeldrop_map",value_3D, 3);
  
//  wall
  value_1D[0] = (float) sensorData[1];
  dcdHub.update_property("wall", value_1D, 1);
  
//  cliff_map L, R, FL, FR
  value_4D[0] = (float)sensorData[2];  value_4D[1] = (float)sensorData[5]; value_4D[2] = (float)sensorData[3]; value_4D[3] = (float)sensorData[4];
  dcdHub.update_property("cliff_map", value_4D, 4);
  
//  virtual_wall
  value_1D[0] = (float) sensorData[6];
  dcdHub.update_property("virtual_wall", value_1D, 1);
  
//  drive_overcurrent
    value_2D[0] = float( (sensorData[7]&0x10)>>4);  value_2D[1] = float((sensorData[7]&0x8)>>3);
  dcdHub.update_property("drive_overcurrent", value_2D, 2);
  
//  brush_overcurrent
    value_2D[0] = float( (sensorData[7]&0x4)>>2);  value_2D[1] = float((sensorData[7]&0x1));
  dcdHub.update_property("brush_overcurrent", value_2D, 2);
  
//  vacuum_overcurrent
  value_1D[0] = float( (sensorData[7]&0x2)>>1);
  dcdHub.update_property("vacuum_overcurrent", value_1D, 1);
  
//  dirt_map , L, R
  value_2D[0] = (float)sensorData[8];  value_2D[1] = (float)sensorData[9];
  dcdHub.update_property("dirt_map", value_2D, 2);
  
//  inner_distance
  value_1D[0] = float( (sensorData[12] * 256 + sensorData[13]) / 1000.0);
  dcdHub.update_property("inner_distance", value_1D, 1);
  
//  inner_angle
  value_1D[0] = float( (sensorData[14] * 256 + sensorData[15]) / 1000.0);
  dcdHub.update_property("inner_angle", value_1D, 1);

  
//  battery_current
  value_1D[0] = float( (sensorData[19] * 256 + sensorData[20]) / 1000.0);
  dcdHub.update_property("battery_current", value_1D, 1);
  
//  battery_charge
  value_1D[0] = float( (sensorData[22] * 256 + sensorData[23]) / 1000.0);;
  dcdHub.update_property("battery_charge", value_1D, 1);
  

}


// Print internal data
void printInternal() {
  Serial.print("R/C: ");
  Serial.print(sensorData[10], HEX); // Bump and wheeldrop
  Serial.print(", Buttons: ");
  Serial.print(sensorData[11], HEX);
  Serial.print(", Distance :");
  Serial.print((int(sensorData[12]) * 256 + sensorData[13]) / 1000.0);
  Serial.print("m, Angle:");
  Serial.print((int(sensorData[14]) * 256 + sensorData[15]) / 1000.0);
  Serial.print(" degrees");
  Serial.println();
}

// Print sensor data
void printBattery() {
  Serial.print("Charging state: ");
  Serial.print(sensorData[16], HEX); // Bump and wheeldrop
  Serial.print(", ");
  Serial.print((int(sensorData[17]) * 256 + sensorData[18]) / 1000.0);
  Serial.print("V, ");
  Serial.print((int(sensorData[19]) * 256 + sensorData[20]) / 1000.0);
  Serial.print("A, ");
  Serial.print(sensorData[21]);
  Serial.print("C, ");
  Serial.print(int(sensorData[22]) * 256 + sensorData[23]);
  Serial.print("/");
  Serial.print(int(sensorData[24]) * 256 + sensorData[25]);
  Serial.print("mAh, ");
  Serial.println();
}
