#ifndef LED_h
#define LED_h

class LED
{
    public:

        LED(int pin);
        void On(int Timeinterval);
        void Off(int Timeinterval);
        void Toggle(int Timeinterval);

    private:

        int _pin;


};
#endif