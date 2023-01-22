#ifndef led_h
#define led_h

const int nLeds = 4;

class Led {
    public:
    Led();
    void SetLeds();
    private:
    int Leds[nLeds];
    int level;
};

#endif