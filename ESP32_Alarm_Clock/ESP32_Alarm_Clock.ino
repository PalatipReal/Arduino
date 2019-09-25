//************ Set Interruppin ************
xTaskHandle Display_Task_Handle;
xTaskHandle Alarm_Task_Handle;
#define interruptPin1  33   // For Start,Stop InterruptTimer
#define interruptPin2  12   // For Reset InterrupTimer
#define interruptPin3  0    // For Menu InterrupTimer
int CountInterrupt33  = 0 ;
int CountInterrupt12  = 0 ;
int CountInterrupt0   = 0 ; 

bool StateTimer        = false ;   // false = Stop , true = Start...
bool StateDisplay      = true  ;   // false = Stop , true = Start...
int  StateMenuSetTime  = 0 ;   // For MenuSetTime
bool StateAlarm        = false  ;
bool StateNewTime      = false  ;      
//************ Set Time ************
int Hour              = 0 ;
int Min               = 0 ;
int Sec               = 0 ;
int MiliSec           = 0 ;
int Alarm_Hour        = 0 ;
int Alarm_Min         = 0 ;
//************ Set Debouncing_time ************
long debouncing_time1 = 300; //Debouncing Time in Milliseconds
volatile unsigned long last_micros1;

long debouncing_time2 = 300; //Debouncing Time in Milliseconds
volatile unsigned long last_micros2;


long debouncing_time3 = 300; //Debouncing Time in Milliseconds
volatile unsigned long last_micros3;

hw_timer_t * timer = NULL;

void IRAM_ATTR onTimer(){
  MiliSec++;
  if(MiliSec == 10){
    MiliSec = 0;
    Sec++;
    StateDisplay = true ;
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
}
void Display_Task(void *p){ 
  while(1) {
    if(StateDisplay){
      Serial.print("Hour :");
      Serial.print(Hour);
      Serial.print(" Min :");
      Serial.print(Min);
      Serial.print(" Sec :");
      Serial.print(Sec);
      Serial.println("");
      StateDisplay = false ;
      }
    else{
      }
      vTaskDelay(10);
      }
  }


void setup() {
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  pinMode(interruptPin3, INPUT_PULLUP);
  
  Serial.begin(9600);
  
  xTaskCreate(&Display_Task, "Display_Task", 2048, NULL, 1,&Display_Task_Handle);

  timer = timerBegin(0, 80, true);                            // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer, &onTimer, true);                // edge (not level) triggered 
  timerAlarmWrite(timer, 100000, true);                        // 1000000 * 1 us = 1 s, autoreload true
  timerAlarmEnable(timer);                                    // SetTimer Disable,Enable    
  timerStart(timer); 
  
  attachInterrupt(interruptPin1, debounceInterrupt1, RISING);
  attachInterrupt(interruptPin2, debounceInterrupt2, RISING);
  attachInterrupt(interruptPin3, debounceInterrupt3, RISING);
}

void loop() {
  vTaskDelay(portMAX_DELAY);
}



void debounceInterrupt1() {
  if ((long)(micros() - last_micros1) >= debouncing_time1 * 1000) {
    Handleinterrupt1();
    last_micros1 = micros();
  }
}

void debounceInterrupt2() {
  if ((long)(micros() - last_micros2) >= debouncing_time2 * 1000) {
    Handleinterrupt2();
    last_micros2 = micros();
  }
}

void debounceInterrupt3() {
  if ((long)(micros() - last_micros3) >= debouncing_time3 * 1000) {
    Handleinterrupt3();
    last_micros3 = micros();
  }
}

void Handleinterrupt1() {
  switch (StateMenuSetTime) {
    case 0  : 
              break; 
    case 1  : Hour++; // set time  hour
              StateNewTime = true ; 
              if(Hour == 24 ){
                Hour = 0;
                }
              Serial.printf("Set New Hour : %d \n" ,Hour);
              break;
    case 2  : StateAlarm = true ;
              StateNewTime = true ;
              Alarm_Hour++;      // set alarm hour
              if(Alarm_Hour == 24 ){
                Alarm_Hour = 0;
                }              
              Serial.printf("Set Alarm Hour : %d \n" ,Alarm_Hour);
              break;                            
    }
  }

void Handleinterrupt2() {
  switch (StateMenuSetTime) {
    case 0  : 
              break; 
    case 1  : Min++;// set time  min
              if(Min == 60 ){
                Min = 0;
                }
              Serial.printf("Set New Min : %d \n",Min);
              break;
    case 2  : StateAlarm = true ;
              Alarm_Min++;          // set alarm min
              if(Alarm_Min == 60 ){
                Alarm_Min = 0;
                }   
              Serial.printf("Set Alarm Min : %d \n",Alarm_Min);
              break;                            
    }
}

void Handleinterrupt3() {
  StateMenuSetTime++;
  switch (StateMenuSetTime) {
    case  0 : // Idle
              break;
    case  1 : 
              Serial.println("Push Button For Set New Hour And Min ");
              vTaskSuspend(Display_Task_Handle);
              StateDisplay = false ;
              break;
    case  2 :
              Serial.println("Push Button For Set Alarm");
              if(StateNewTime){
                Sec = 0 ;
                MiliSec = 0 ;
                }

              break;
    case  3 :
              if(StateAlarm){
                xTaskCreate(&Alarm_Task, "Alarm_Task", 2048, NULL, 2,&Alarm_Task_Handle);
                }
              vTaskResume(Display_Task_Handle);
              StateMenuSetTime = 0 ;
              StateDisplay = true ;
              break;
  }
}

void Alarm_Task(void *p){
  while(1) {
    if(Alarm_Hour == Hour && Alarm_Min == Min ){
      vTaskSuspend(Display_Task_Handle);
      for(int i = 0; i < 10 ; i++){
         Serial.printf("Alarm Time... !!!! \n");
         vTaskDelay(1000);
        }
       StateAlarm = false ;
       vTaskResume(Display_Task_Handle);
       vTaskDelete(Alarm_Task_Handle);
      }
    vTaskDelay(10);
  }
}
