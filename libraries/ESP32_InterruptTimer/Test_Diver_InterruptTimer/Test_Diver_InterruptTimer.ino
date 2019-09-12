#include <Timer.h>
Timer t1;

void setup() {
  Serial.begin(9600);
  Serial.println("Test Diver Interrupt_Timer ESP32...");
  delay(1000);
  t1.SetTimer(0,100000); // t1.SetTimer(SelectTimer 0 , )
  t1.Start();
}

void loop() {
  if(t1.GetTime() < 101)
    Serial.println(t1.GetTime());
  else{
    t1.Stop();
    delay(1000);
    t1.Reset();
    t1.Start();
    }
}
