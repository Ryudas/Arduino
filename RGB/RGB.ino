/*
 * Copyright (c) 2016 RedBear
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 */
 
/*
 * This sketch demonstrates controlling the Duo's on-board RGB.
 */

/* 
 * Defaultly disabled. More details: https://docs.particle.io/reference/firmware/photon/#system-thread 
 */
//SYSTEM_THREAD(ENABLED);

/*
 * Defaultly disabled. If BLE setup is enabled, when the Duo is in the Listening Mode, it will de-initialize and re-initialize the BT stack.
 * Then it broadcasts as a BLE peripheral, which enables you to set up the Duo via BLE using the RedBear Duo App or customized
 * App by following the BLE setup protocol: https://github.com/redbear/Duo/blob/master/docs/listening_mode_setup_protocol.md#ble-peripheral 
 * 
 * NOTE: If enabled and upon/after the Duo enters/leaves the Listening Mode, the BLE functionality in your application will not work properly.
 */
//BLE_SETUP(ENABLED);

/*
 * SYSTEM_MODE:
 *     - AUTOMATIC: Automatically try to connect to Wi-Fi and the Particle Cloud and handle the cloud messages.
 *     - SEMI_AUTOMATIC: Manually connect to Wi-Fi and the Particle Cloud, but automatically handle the cloud messages.
 *     - MANUAL: Manually connect to Wi-Fi and the Particle Cloud and handle the cloud messages.
 *     
 * SYSTEM_MODE(AUTOMATIC) does not need to be called, because it is the default state. 
 * However the user can invoke this method to make the mode explicit.
 * Learn more about system modes: https://docs.particle.io/reference/firmware/photon/#system-modes .
 */
#if defined(ARDUINO) 
SYSTEM_MODE(SEMI_AUTOMATIC); 
#endif

// This routine runs only once upon reset
void setup() {
  RGB.control(true);
  Serial.begin(9600);
}

// This routine loops forever
void loop() {
  RGB.color(255, 0, 0);                 // set LED to RED
  Serial.print("RED");
  delay(500);
  
  RGB.color(0, 255 ,0);                 // set LED to GREEN
  Serial.print("GREEN");
  delay(500);
  
  RGB.color(0, 0,255);                 // set LED to BLUE
  Serial.print("Blue");
  delay(500);
}
