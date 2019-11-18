// In this example, the vibration motor is increasing speed up to max speed, then
// decreases to half speed before starting increasing  again

// Lets make our vibration example, using PWM!
#define VIB_MOT_1 2 // Careful, here we have to use a pin that can be used for pwm.
#define VIB_MOT_2 3 // Careful, here we have to use a pin that can be used for pwm.
#define VIB_MOT_3 4 // Careful, here we have to use a pin that can be used for pwm.
#define VIB_MOT_4 5 // Careful, here we have to use a pin that can be used for pwm.




void setup() {
  // put your setup code here, to run once:
  pinMode(VIB_MOT_1, OUTPUT);
  pinMode(VIB_MOT_2, OUTPUT);
  pinMode(VIB_MOT_3, OUTPUT);
  pinMode(VIB_MOT_4, OUTPUT);
  pinMode(VIB_MOT_1, OUTPUT);
  
  Serial.begin(9600); // setting baud speed for Serial (a baud is a pulse)
  Serial.println("Lets start our pulsing vibration example!");
}

void loop() {

    analogWrite(VIB_PIN, i);
    delay(50); // in each step of pwm, we vibrate for i * 0.01 seconds 
  
}
