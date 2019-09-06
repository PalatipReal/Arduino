#include <LED.h>
  LED led(2);         // set pin led
void setup() {
  
}

void loop() {
  led.Toggle(500);    // Toggle State Led
//   led.On();
//   led.Off();
}
