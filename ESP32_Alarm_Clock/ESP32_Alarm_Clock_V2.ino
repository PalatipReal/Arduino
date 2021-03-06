//************ WiFi SetUp ************
#include <WiFi.h>
#include "time.h"
const char* ssid       = "KFC Free WiFi";
const char* password   = "12345677";
//************ Set NTPServer And OffSetTime ************
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 21600;
const int   daylightOffset_sec = 3600;
//************ Set TaskHandle************
xTaskHandle Display_Task_Handle;
xTaskHandle Alarm_Task_Handle;
xTaskHandle NTP_SetUp_Task_Handle;
//************ Set Interruppin ************
#define interruptPin1  33   // For Start,Stop InterruptTimer
#define interruptPin2  12   // For Reset InterrupTimer
#define interruptPin3  0    // For Menu InterrupTimer
//************ Set State ************
bool StateTimer        = false ;   // false = Stop , true = Start...
bool StateDisplay      = true  ;   // false = Stop , true = Start...
int  StateMenuSetTime  = 0 ;   // For MenuSetTime
bool StateAlarm        = false  ;
bool StateNewTime      = false  ;
bool StateNTP          = false  ;
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

//************ FN Timer ************
void IRAM_ATTR onTimer() {
  MiliSec++;
  if (MiliSec == 10) {
    MiliSec = 0;
    Sec++;
    StateDisplay = true ;
    if (Sec == 60) {
      Sec = 0;
      Min++;
      if (Min == 60) {
        Min = 0;
        Hour++;
        if (Hour == 99) {
          Hour = 0;
        }
      }
    }
  }
}
//************ Display_Task ************
void Display_Task(void *p) {
  while (1) {
    if (StateDisplay  ) {
      Serial.print("Hour :");
      Serial.print(Hour);
      Serial.print(" Min :");
      Serial.print(Min);
      Serial.print(" Sec :");
      Serial.print(Sec);
      Serial.println("");
      StateDisplay = false ;
    }
    vTaskDelay(10);
  }
}

void NTP_SetUp_Task(void *p) {
  int Count_Conecting = 0 ;
  struct tm timeinfo;
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      Count_Conecting ++ ;
      delay(500);
      Serial.print(".");
      if(Count_Conecting == 10){
        break ;
        }
  }
  if(Count_Conecting <= 10 ){
    Serial.println(" CONNECTED");
    StateNTP = true ;
    timerStop(timer);
    timerAlarmDisable(timer);
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    if(!getLocalTime(&timeinfo)){
      Serial.println("Failed to obtain time");
    }
    Hour = timeinfo.tm_hour;
    Min = timeinfo.tm_min;
    Sec = timeinfo.tm_sec;
    Serial.println(Hour);
    Serial.println(Min);
    Serial.println(Sec);
    timerStart(timer);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("TaskDelete  NTP_SetUp_Task_Handle");

    }
   else{
    Serial.println("Cann't CONNECTED WiFi ");
    }
    timerAlarmEnable(timer);
    timerStart(timer);
    vTaskDelete(NTP_SetUp_Task_Handle);
}

void setup() {
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  pinMode(interruptPin3, INPUT_PULLUP);

  Serial.begin(9600);


  xTaskCreate(&NTP_SetUp_Task, "NTP_SetUp_Task", 4048, NULL, 1, &NTP_SetUp_Task_Handle);

  timer = timerBegin(0, 80, true);                            // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
  timerAttachInterrupt(timer, &onTimer, true);                // edge (not level) triggered
  timerAlarmWrite(timer, 100000, true);                        // 1000000 * 1 us = 1 s, autoreload true
  timerAlarmEnable(timer);                                    // SetTimer Disable,Enable
  timerStart(timer);
  xTaskCreate(&Display_Task, "Display_Task", 2048, NULL, 1, &Display_Task_Handle);
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
      if (Hour == 24 ) {
        Hour = 0;
      }
      Serial.printf("Set New Hour : %d \n" , Hour);
      break;
    case 2  : StateAlarm = true ;
      StateNewTime = true ;
      Alarm_Hour++;      // set alarm hour
      if (Alarm_Hour == 24 ) {
        Alarm_Hour = 0;
      }
      Serial.printf("Set Alarm Hour : %d \n" , Alarm_Hour);
      break;
  }
}

void Handleinterrupt2() {
  switch (StateMenuSetTime) {
    case 0  :
      break;
    case 1  : Min++;// set time  min
      if (Min == 60 ) {
        Min = 0;
      }
      Serial.printf("Set New Min : %d \n", Min);
      break;
    case 2  : StateAlarm = true ;
      Alarm_Min++;          // set alarm min
      if (Alarm_Min == 60 ) {
        Alarm_Min = 0;
      }
      Serial.printf("Set Alarm Min : %d \n", Alarm_Min);
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
      if (StateNewTime) {
        Sec = 0 ;
        MiliSec = 0 ;
      }

      break;
    case  3 :
      if (StateAlarm) {
        xTaskCreate(&Alarm_Task, "Alarm_Task", 2048, NULL, 2, &Alarm_Task_Handle);
      }
      vTaskResume(Display_Task_Handle);
      StateMenuSetTime = 0 ;
      StateDisplay = true ;
      break;
  }
}

void Alarm_Task(void *p) {
  while (1) {
    if (Alarm_Hour == Hour && Alarm_Min == Min ) {
      vTaskSuspend(Display_Task_Handle);
      for (int i = 0; i < 10 ; i++) {
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
