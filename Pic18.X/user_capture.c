#include "user_capture.h"

void capture_setup(){
    //TODO
    _capture_tmr1prescaler = 1;
    _capture_prescaler = 1;
    
    /*In Capture mode, the appropriate CCPx pin should be
    configured as an input by setting the corresponding
    TRIS direction bit.*/
    TRISCbits.TRISC2 = 1;
    
    /*When the Capture mode is changed, a false capture
    interrupt may be generated. The user should keep the
    CCPxIE interrupt enable bit clear to avoid false
    interrupts. The interrupt flag bit, CCPxIF, should also be
    cleared following any such change in operating mode.*/
    PIE1bits.CCP1IE = 0;
    PIR1bits.CCP1IF = 0;
    
    /*CCPxM3:CCPxM0: CCPx Module Mode Select bits*/
    CCP1CONbits.CCP1M = 0b0100; //Capture mode: every falling edge
    PIR1bits.CCP1IF = 0;
    
    /*The assignment of a particular timer to a module is
    determined by the Timer to CCP enable bits in the
    T3CON register */
    T3CONbits.T3CCP2=0;
    T3CONbits.T3CCP1=1;
    
    /*** Timer1 setup ***/
    T1CONbits.TMR1CS = 0;   //T0CS: Timer1 Clock Source Select bit
    T1CONbits.T1OSCEN = 0;
    T1CONbits.T1CKPS = 0b00;
    T1CONbits.T1RUN = 0;
    
    TMR1L = 0x00;
    TMR1H = 0x00;

    PIE1bits.TMR1IE = 1; // enable TMR1 int
    PIR1bits.TMR1IF = 0;  // Clear TMR1 IRQ flag
    PIE1bits.CCP1IE = 1;
    PIR1bits.CCP1IF = 0; 
}

void capture_start(){        
    T1CONbits.TMR1ON = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

void capture_stop(){
    INTCONbits.GIE = 0;
    INTCONbits.PEIE = 0;
    T1CONbits.TMR1ON = 0;
}

void capture_prepare(){
    T1CONbits.TMR1ON = 0;
    TMR1L = 0x00;
    TMR1H = 0x00;
    PIR1bits.CCP1IF = 0;
    PIR1bits.TMR1IF = 0;
    _capture_ofcounter = 0;
}

float capture_read(){
    DWORD tics = CCPR1 + _capture_ofcounter*65535;
    // Frequency: (FOSC/4)/prescaler
    // Period: 1/Frequency [seconds]
    // Total time: regs * period
    WORD c = (_XTAL_FREQ / 4000);
    float ms = ((float)tics / (float)c) * _capture_tmr1prescaler * _capture_prescaler;
    return ms;
}