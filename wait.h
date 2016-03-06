#ifndef TIME__H
#define TIME__H

#include <Arduino.h>

#define SCREEN_DELAY_DEFAULT 4000


class wait{
public:
    wait();
    unsigned int step_cur;
    uint16_t steps;


    void set_time(unsigned long t);
    void set_steps(uint16_t s);
    int end();
    int start();
    int start(unsigned long t);
    unsigned int step();
private:
    unsigned long ts, time;
};



#endif // TIME__H
