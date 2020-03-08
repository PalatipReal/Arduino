#include <WiFi.h>
#include "AMC.h"

//const char* ssid       = "iot-lab";
//const char* password   = "computing";
//const char* ssid       = "Renat";
//const char* password   = "nattapong2539nat";
const char* ssid       = "KFC Free WiFi";
const char* password   = "12345677";
xTaskHandle Display_Task_Handle;
//xTaskHandle Alarm_Task_Handle;

void setup()
{

  Serial.begin(115200);

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");

  xTaskCreate(&Display_Task, "Display_Task", 2048, NULL, 1, &Display_Task_Handle);

}

//************ Display_Task ************
void Display_Task(void *p) {
  AMC Timer;
  Timer.SetAlarm(22,30);
  while (1) {
        Timer.DisplayNowTime();
      //Timer.GetNowTime();
      //Serial.printf("Time:%d:%d:%d ",Timer.GetNowHour(),Timer.GetNowMin(),Timer.GetNowSec());
      vTaskDelay(1);
  }
}
void loop()
{
  vTaskDelay(portMAX_DELAY);
}

/*
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
      break;
    case  2 :
      Serial.println("Push Button For Set Alarm");
      if (StateNewTime) {
        getLocalTime(&timeinfo);
        Sec = timeinfo.tm_sec ;

      }

      break;
    case  3 :
      if (StateAlarm) {
        xTaskCreate(&Alarm_Task, "Alarm_Task", 2048, NULL, 2, &Alarm_Task_Handle);
      }
      vTaskResume(Display_Task_Handle);
      StateMenuSetTime = 0 ;
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
*/
