

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  uint8_t temp = (1<<7) | 0x06;

  Serial.println(temp, HEX);
  delay(100);
}
