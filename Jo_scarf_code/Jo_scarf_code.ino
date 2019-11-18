//#include <IRremote.h>
//#include <VirtualWire.h>
#include <WS2812FX.h>

#define LED_COUNT 100
#define LED_PIN A1
#define transmit_en_pin  3
#define  RECV_PIN  A0

// holds IR message
char msg[2];


WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//IRrecv irrecv(RECV_PIN);
//decode_results results;

void setup(){
  // Serial connection handling
  Serial.begin(9600);

  // IR remote initializations  
//  irrecv.enableIRIn();
 // irrecv.blink13(true);
  

 // LED Library start
  ws2812fx.init();
  ws2812fx.setBrightness(100);
  ws2812fx.setSpeed(200);
  ws2812fx.setMode(FX_MODE_FADE);
  ws2812fx.start();
}



void loop(){
  /*
  if (irrecv.decode(&results))
  {
      get_key(results.value);
      Serial.println(msg);    
      //Serial.println(results.value, HEX);
      irrecv.resume();
  }
  */
  
  ws2812fx.service();
}

// gets IR code converts into key keyboard key 
// string representation
void get_key(unsigned long value)
{
  String message;
  switch(value)
  {
      case 0xFFA25D: 
        message = "1";
        message.toCharArray(msg,2);      
        break;
      case 0xFF629D: 
        message = "2";     
        message.toCharArray(msg,2);
        break;
      case 0xFFE21D: 
        message  = "3";  
        message.toCharArray(msg,2);
        break;
        
     
      case 0xFF22DD: 
        message = "4";
        message.toCharArray(msg,2);
        
        break;
      case 0xFF02FD: 
        message = "5";     
        message.toCharArray(msg,2);
        break;
      case 0xFFC23D: 
        message  = "6";  
        message.toCharArray(msg,2);
        break;  

      case 0xFFE01F: 
        message = "7";
        message.toCharArray(msg,2);
        
        break;
      case 0xFFA857: 
        message = "8";     
        message.toCharArray(msg,2);
        break;
      case 0xFF906F: 
        message  = "9";  
        message.toCharArray(msg,2);
        break;  

      case 0xFF6897: 
        message = "*";
        message.toCharArray(msg,2);
        
        break;
      case 0xFF9867: 
        message = "0";     
        message.toCharArray(msg,2);
        break;
      case 0xFFB04F: 
        message  = "#";  
        message.toCharArray(msg,2);
        break;  

      case 0xFF18E7: 
        message = "^";
        message.toCharArray(msg,2);
        
        break;
      case 0xFF10EF: 
        message = "<";     
        message.toCharArray(msg,2);
        break;
      case 0xFF5AA5: 
        message  = ">";  
        message.toCharArray(msg,2);
        break;  

      case 0xFF38C7: 
        message = "k";
        message.toCharArray(msg,2);
        
        break;
      case 0xFF4AB5: 
        message = "d";     
        message.toCharArray(msg,2);
        break;
                                                       
   }
}

// sends rf message through transmitter
//void send_rf(char str_msg[], int length)
//{ 
//    vw_send((uint8_t *)str_msg, length);
//    // Wait until the whole message is gone  
//    vw_wait_tx(); 
//}
