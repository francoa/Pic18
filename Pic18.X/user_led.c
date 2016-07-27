#include "user_led.h"

/*void led_init(int led){
    if (led > 0 && led < 7){
        TRISC &= !(0x01 << led);
    }
}

void led_on(int led){
    if (led > 0 && led < 7 && ((TRISC & (0x01 << led)) == (0x01 << led))){
        LATC |= (0x01 << led); 
    }
}

void led_off(int led){
    if (led > 0 && led < 7 && ((TRISC & (0x01 << led)) == (0x01 << led))){
        LATC &= !(0x01 << led); 
    }
}

void led_toggle(int led){
    if (led > 0 && led < 7 && ((TRISC & (0x01 << led)) == (0x01 << led))){
        LATC &= !(0x01 << led); 
        TRISC &= !(0x01 << led);
    }
}*/

void led_demo(){
    int led_counter;
    fLED_1_Init();
    while(1)  {
        fLED_1_Toggle();
        for (led_counter=0; led_counter < 500; led_counter++){
            __delay_ms(5);
        }
    }
};