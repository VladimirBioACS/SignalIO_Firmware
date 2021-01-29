#ifndef DEEPSLEEP_H
#define DEEPSLEEP_H

#include <Arduino.h>

#define uS_TO_S_FACTOR 1000000  
#define TMR_MODE 1
#define PIN_TRIGGER_MODE 0
#define SENSOR_PIN_BITMASK 0x1000
                             
class Sleep
{
private:
    //
public:
    void tmr_sleep(unsigned int sleep_time);
    void pin_trigger_sleep();
};

#endif