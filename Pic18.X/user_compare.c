#include "user_compare.h"

void compare_setup(int us, int ms){
    time = us + ms*1000;
    
    /*CCPxM3:CCPxM0: CCPx Module Mode Select bits*/
    CCP1CONbits.CCP1M = 0b1010; //Compare mode: generate software interrupt on compare match 
                                // (CCPxIF bit is set, CCPx pin reflects I/O state)
    
    /*The assignment of a particular timer to a module is
    determined by the Timer to CCP enable bits in the
    T3CON register (Register 14-1).*/
    T3CONbits.T3CCP1 = 1; //Timer1 for CCP1
    T3CONbits.T3CCP2 = 0; //Timer1 for CCP1
    
    
    /*The user must configure the CCPx pin as an output by
    clearing the appropriate TRIS bit.*/
    TRISBbits.TRISB3 = 0;
    
    /*Timer1 and/or Timer3 must be running in Timer mode,
    or Synchronized Counter mode, if the CCP module is
    using the compare feature. In Asynchronous Counter
    mode, the compare operation may not work. The operating 
    mode is determined by the clock select
    bit, TMR1CS (T1CON<1>). When TMR1CS is cleared
    (= 0), Timer1 increments on every internal instruction
    cycle (FOSC/4).*/
    T1CONbits.TMR1CS = 0; //Timer1 Clock Source Select bit: Internal clock (FOSC/4)
    T1CONbits.T1OSCEN = 0; //Timer1 Oscillator Enable bit
    T1CONbits.RD16 = 1;   //Enables register read/write of Timer1 in one 16-bit operation
    
    /*The Timer1 interrupt, if enabled, is generated on overflow
    which is latched in interrupt flag bit, TMR1IF
    (PIR1<0>). This interrupt can be enabled or disabled
    by setting or clearing the Timer1 Interrupt Enable bit,
    TMR1IE (PIE1<0>).*/
    PIE1bits.TMR1IE = 0;
    
    /*The data register. @16MHz, FOSC/4, 0xF0FF = 0.01542s*/
    //1 -> 1000000/(FOSC/4) us
    //x -> time us
    value = time*(_XTAL_FREQ/4)/(1000000);
    CCPR1L = (int)value;
    CCPR1H = (int)value >> 8;
    
    /*Enable CCP1 interrupt*/
    IPR1bits.CCP1IP = 0;
    PIR1bits.CCP1IF = 0;
    PIE1bits.CCP1IE = 1;
}

void compare_init(){ 
    /*Timer1 On bit*/
    T1CONbits.TMR1ON = 1; 
}

void compare_stop(){
    T1CONbits.TMR1ON = 0;
}