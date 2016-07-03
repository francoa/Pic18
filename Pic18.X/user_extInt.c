#include "user_extInt.h"

void extInt_init(){
    
    TRISBbits.RB0 = 1;
    
    /*INT0 External Interrupt Enable bit*/
    INTCONbits.INT0IE = 1;
    
    /*PORTB Pull-up Enable bit*/
    INTCON2bits.RBPU = 1;
    
    /*If the corresponding INTEDGx bit in the INTCON2 register is set (= 1), the
    interrupt is triggered by a rising edge; if the bit is clear,
    the trigger is on the falling edge.*/
    INTCON2bits.INTEDG0 = 1;
    
    /*Reset interrupt flag*/
    INTCONbits.INT0IF = 0;
}
