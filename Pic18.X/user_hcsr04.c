#include "user_hcsr04.h"

void hcsr04_setup(){
    HCSR04_CONFIG;
    capture_setup();
}

void hcsr04_measure(){
    capture_prepare();
    TRIGPIN = 1;
    __delay_us(10);
    TRIGPIN = 0;
    while(!ECHOPIN){};
    capture_start();
    return;
}

float hcsr04_read(){
    float r = capture_read();
    return r*(1000.0*US2CM);
}