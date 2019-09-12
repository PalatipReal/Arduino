#ifndef StopWatch_h
#define StopWatch_h

class StopWatch
{
    public:

        StopWatch();
        void SetPin(int Pin_Start_Stop,int Pin_Reset);
        void Start();
        void Stop();
        void Reset();
        int GetHour();
        int GetMin();
        int GetSec();
        int GetMiliSec();
        void SerialPrint();

    private:
       int _Pin1;
       int _pin2;
};
#endif