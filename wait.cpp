#include "wait.h"




wait::wait(){
    //ts = 0;
}

int wait::start(){
    ts = millis();
}

void wait::set_time(unsigned long t){
    time = t;
}

int wait::end(){
    if(millis() - ts > time) return 1;
    else return 0;
}

int wait::start(unsigned long t){
    time = t;
    wait::start();
}

unsigned int wait::step(){
    uint16_t sub;
    sub = (millis() - wait::ts);

    if (sub > SCREEN_DELAY_DEFAULT ){

        step_cur += sub/SCREEN_DELAY_DEFAULT;

        //Serial.println(F("Sub increment to: ")); Serial.println(step_cur);

        wait::ts = millis();
        if(step_cur > steps) step_cur = 1;

        return step_cur;
    }
    else return step_cur;

}

void wait::set_steps(uint16_t s){
    steps = s;
}
