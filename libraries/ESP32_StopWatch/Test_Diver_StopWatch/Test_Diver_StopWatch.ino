//************ StopWatch ************
#include <StopWatch.h>
StopWatch s1;

//************ Set Interruppin ************
int interruptPin1     = 33;   // For Start,Stop InterruptTimer
int interruptPin2     = 12;   // For Reset InterrupTimer
int CountInterrupt33  = 0 ; 
int CountInterrupt12  = 0 ;
bool StateTimer        = false;    // false = Stop , true = Start...

//************ Set Debouncing_time1 ************
long debouncing_time1 = 300; //Debouncing Time in Milliseconds
volatile unsigned long last_micros1;

//************ Set Debouncing_time2 ************
long debouncing_time2 = 300; //Debouncing Time in Milliseconds
volatile unsigned long last_micros2;

void setup() {
  s1.SetPin(interruptPin1,interruptPin2);                     // s1.SetPin(interruptPin1,interruptPin2);   
  attachInterrupt(interruptPin1,debounceInterrupt1,FALLING);  // Set InrerruptPin1 
  attachInterrupt(interruptPin2,debounceInterrupt2,FALLING);  // Set InrerruptPin1
  Serial.begin(9600);
  Serial.println("Test Diver_StopWacth ESP32...");                                          
  Serial.println("Try Push Button To Start");
}

void debounceInterrupt1() {
  if((long)(micros() - last_micros1) >= debouncing_time1 * 1000) {
    Handleinterrupt1();
    last_micros1 = micros();
  }
}

void debounceInterrupt2() {
  if((long)(micros() - last_micros2) >= debouncing_time2 * 1000) {
    Handleinterrupt2();
    last_micros2 = micros();
  }
}

void Handleinterrupt1() {
  if(StateTimer == false){            // if Timer is Stop
    s1.Start();                       // Enable Timer
    StateTimer = true ;        // Timer is Start
    }
  else{
    s1.Stop();                        // Disable Timer
    StateTimer = false;
    }
}
void Handleinterrupt2() {
    s1.Stop();                        // Disable Timer
    s1.Reset();
    StateTimer = false ;
}
void loop() {

    s1.SerialPrint();
}
