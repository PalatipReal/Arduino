#ifndef Timer_h
#define Timer_h

class Timer
{
    public:

        Timer();
        void SetTimer(int SelectTimer,int TimeInterval);
        void Start();
        void Stop();
        void Reset();
        int  GetTime();

    private:

};
#endif