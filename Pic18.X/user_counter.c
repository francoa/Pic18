#include "user_counter.h"

void counter_setup(unsigned char prescaler){
    T0CONbits.T08BIT = 0; //T08BIT: Timer0 8-Bit/16-Bit Control bit
    T0CONbits.T0CS = 0;   //T0CS: Timer0 Clock Source Select bit
    T0CONbits.T0SE = 0;   //T0SE: Timer0 Source Edge Select bit
    if (prescaler == T0_PS_1){
        T0CONbits.PSA = 1;
        _prescaler = 1;
    }
    else
    {
        T0CONbits.PSA = 0;
        int i=0;
        _prescaler = 2;
        for (i=0; i < (prescaler>>1); i++)
            _prescaler *= 2;
        T0CONbits.T0PS0 = (prescaler>>1) & 0x01;
        T0CONbits.T0PS1 = (prescaler>>2) & 0x01;
        T0CONbits.T0PS2 = (prescaler>>3) & 0x01;
    }
    INTCONbits.TMR0IE = 1; // enable TMR0 int
    INTCONbits.TMR0IF = 0;  // Clear TMR0 IRQ flag
    INTCONbits.PEIE = 1; // PERIPHERAL INTERRUPT ENABLE REGISTER 1 bit
    INTCONbits.GIE = 1; 	//enable global interrupts
}

void counter_prepare(WORD ms_time){
    // Frequency: (FOSC/4)/prescaler
    // Period: 1/Frequency [seconds]
    // Total time: regs * period
    DWORD regs = ms_time * (_XTAL_FREQ/(4000*_prescaler)) ;
    _counter_i = 0;
    while(regs > 0xFFFF){
        _counter_i++;
        regs -= 0xFFFF;
    }
    regs = 0xFFFF - regs;
    TMR0L = regs & 0xFF;
    TMR0H = (regs >> 8) & 0xFF;
    __delay_ms(1);
}

void counter_init(){
    /*Timer0 On bit*/
    T0CONbits.TMR0ON = 1; 
}

void counter_stop(){
    T0CONbits.TMR0ON = 0;
    //DWORD regs = ((TMR0L | (TMR0H << 8)) | _counter_i * 65536);
    //float tt = (float)((regs * 4 * _prescaler) / (_XTAL_FREQ/1000000));
    //return tt;
}
