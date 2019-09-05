//************ include LiquidCrystal_I2C ************
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display

//************ Set Interruppin ************
int interruptPin1     = 33;   // For Start,Stop InterruptTimer
int interruptPin2     = 12;   // For Reset InterrupTimer
int CountInterrupt33  = 0 ; 
int CountInterrupt12  = 0 ;
bool StateTimer        = false;    // false = Stop , true = Start...

//************ Set Time ************
int Hour              = 0 ;
int Min               = 0 ;
int Sec               = 0 ;
int MiliSec           = 0 ;

//************ Set Debouncing_time ************
long debouncing_time1 = 300; //Debouncing Time in Milliseconds
volatile unsigned long last_micros1;

long debouncing_time2 = 300; //Debouncing Time in Milliseconds
volatile unsigned long last_micros2;

hw_timer_t * timer = NULL;

void LCDDisplay(){
  if(Hour < 10){
    lcd.setCursor(3,0);
    lcd.print("0");      
    lcd.setCursor(4,0);
    lcd.print(Hour);      // Set Cursor For Hour 
    }
  if(Hour >= 10){
    lcd.setCursor(3,0);
    lcd.print(Hour);      // Set Cursor For Hour 
    }
  lcd.setCursor(5,0);
  lcd.print(":");

  if(Min < 10){
    lcd.setCursor(6,0);
    lcd.print("0");      
    lcd.setCursor(7,0);
    lcd.print(Min);      // Set Cursor For Min 
    }
  if(Min >= 10){
    lcd.setCursor(6,0);
    lcd.print(Min);      // Set Cursor For Min 
    }
  lcd.setCursor(8,0);
  lcd.print(":");

  if(Sec < 10){
    lcd.setCursor(9,0);
    lcd.print("0");      
    lcd.setCursor(10,0);
    lcd.print(Sec);      // Set Cursor For Sec 
    }
  if(Sec >= 10){
    lcd.setCursor(9,0);
    lcd.print(Sec);      // Set Cursor For Sec 
    }
  lcd.setCursor(11,0);
  lcd.print(":");
  lcd.setCursor(12,0);  // Set Cursor For MiliSec 
  lcd.print(MiliSec);
}
void IRAM_ATTR onTimer(){
  MiliSec++;
  if(MiliSec == 10){
    MiliSec = 0;
    Sec++;
    if(Sec == 60){
      Sec = 0;
      Min++;
      if(Min == 60){
        Min = 0;
        Hour++;
        if(Hour == 99){
          Hour = 0;
        }
      }
    }
  }

  //SerialPrint();
}
void setup() {
  pinMode(interruptPin1,INPUT_PULLUP);
  pinMode(interruptPin2,INPUT_PULLUP);
  
  timer = timerBegin(0, 80, true);                            // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer, &onTimer, true);                // edge (not level) triggered 
  timerAlarmWrite(timer, 100000, true);                       // 1000000 * 1 us = 1 s, autoreload true
  timerAlarmDisable(timer);                                   // SetTimer Disable,Enable   
    
  attachInterrupt(interruptPin1,debounceInterrupt1,FALLING);
  attachInterrupt(interruptPin2,debounceInterrupt2,FALLING);
  
  lcd.begin();                                                // initialize the LCD
  lcd.backlight();                                            // Turn on the blacklight and print a message.
  lcd.clear();
  
  Serial.begin(9600);
  Serial.println("Test Interrupt_And_Timer ESP32...");
  LCDDisplay();                                               // Display Timer
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
    timerAlarmEnable(timer);          // Enable Timer
    StateTimer = true ;               // Timer is Start
    Serial.println("Timer is Start...");
    }
  else{
    timerAlarmDisable(timer);  // Disable Timer
    StateTimer = false;
    Serial.println("Timer is Stop...");
    }
  
}
void Handleinterrupt2() {
//  if(StateTimer == 1){                  // if Timer is Starting
    timerAlarmDisable(timer);             // Disable Timer
    Serial.println("Timer is Stop...");
    StateTimer  = 0 ;
    Hour        = 0 ;
    Min         = 0 ;
    Sec         = 0 ;
    MiliSec     = 0 ;
    StateTimer  = 0 ;                    // Reset Timer
    Serial.println("Timer is Reset...");
    SerialPrint();                       // Display To SerialPort
//    }
//  else{                                   // if Timer is Stop
//    StateTimer  = 0 ;
//    Hour        = 0 ;
//    Min         = 0 ;
//    Sec         = 0 ;
//    MiliSec     = 0 ;
//    StateTimer  = 0 ;
//    delay(250);
//    Serial.println("Timer is Reset...");
//    SerialPrint();
//    }
}
void SerialPrint(){
  Serial.print("Hour :");
  Serial.print(Hour);
  Serial.print(" Min :");
  Serial.print(Min);
  Serial.print(" Sec :");
  Serial.print(Sec);
  Serial.print(" MiliSec :");
  Serial.print(MiliSec);
  Serial.println("");
  }
  
void loop() {
    LCDDisplay();
}
