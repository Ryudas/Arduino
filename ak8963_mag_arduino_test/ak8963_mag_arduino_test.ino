#include "Wire.h"
#include "AK8963.h"

AK8963 mag(0x0F);

int16_t mx, my, mz;

void setup() {
  // put your setup code here, to run once:
  // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();
 
    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);
 
    // initialize device
    Serial.println("Initializing I2C devices...");
    mag.initialize();
 
    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(mag.testConnection() ? "AK8963 connection successful" : "AK8963 connection failed");
    
    // put your main code here, to run repeatedly:
    Serial.print("Device ID ");
    Serial.println(mag.getDeviceID());
  
    Serial.print("Device mode ");
    Serial.println(mag.getMode());

    Serial.print("Mode before, ");
    Serial.println(mag.getMode());
    mag.setMode(AK8963_MODE_CONTINUOUS_8HZ);
    Serial.print("Mode after, ");
    Serial.println(mag.getMode());


}

void loop() {
  if(mag.getDataReady())
    mag.getHeading(&mx, &my, &mz);

  
  Serial.print(mag.getDataReady());
  Serial.print(", ");
  Serial.print(mag.getMode());

  Serial.print(", ");
  Serial.print("Mag Data: ");
  Serial.print(mx);
  Serial.print(", ");
  Serial.print(my);
  Serial.print(", ");
  Serial.print(mz);
  Serial.println("");
  
  delay(150);
}
