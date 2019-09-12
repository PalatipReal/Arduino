#include "Timer.h"
#include "Arduino.h"

hw_timer_t * timer = NULL;
int Count = 0;

void IRAM_ATTR onTimer(){
    Count++;
    
}
Timer::Timer(){
   
}

void Timer::SetTimer(int SelectTimer,int TimeInterval){
    timer = timerBegin(SelectTimer, 80, true);  // timer 0, MWDT clock period = 12.5 ns * TIMGn_Tx_WDT_CLK_PRESCALE -> 12.5 ns * 80 -> 1000 ns = 1 us, countUp
    timerAttachInterrupt(timer, &onTimer, true); // edge (not level) triggered 
    timerAlarmWrite(timer, TimeInterval, true); // 1000000 * 1 us = 1 s, autoreload true
    timerAlarmDisable(timer); // SetTimer Disable,Enable    
}

void Timer::Start(){
    timerAlarmEnable(timer); // SetTimer Disable,Enable 
}

void Timer::Stop(){
    timerAlarmDisable(timer); // SetTimer Disable,Enable 
}

void Timer::Reset(){
     Count = 0 ;
}

int Timer::GetTime(){
    return Count ;   
}
