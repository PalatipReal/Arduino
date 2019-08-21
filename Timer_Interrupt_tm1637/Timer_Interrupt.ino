#include <TimerOne.h>
#include <TM1637.h>
#define CLK 3//pins definitions for TM1637 and can be changed to other ports 
#define DIO 2
int TimeCount = 0 ;

TM1637 tm1637(CLK,DIO);
void setup() 
{ 
  Serial.begin(9600);
  Serial.println("Pls Input Someting For Start");
  while(!Serial.available()){}
  tm1637.init();
  tm1637.setBrightness(5); 
  Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here
  
}
 
void loop()
{
  tm1637.dispNumber(TimeCount);
  
}
 
/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
    TimeCount=TimeCount+1 ;
    Serial.println("Timer Interrupt ");
    Serial.print("TimerCount : ");
    Serial.print(TimeCount);
    Serial.println(" ");
    
    
}
