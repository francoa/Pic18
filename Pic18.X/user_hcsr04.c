#include "user_hcsr04.h"

void hcsr04_setup(){
    HCSR04_CONFIG;
    //counter_setup(T0_PS_128);
}

float hcsr04_measure(){
    //counter_prepare();
    TRIGPIN = 1;
    __delay_us(10);
    TRIGPIN = 0;
    while(!ECHOPIN){};
    //counter_init();
    while(ECHOPIN){};
    //return counter_stop()/58;+
    return 0;
}