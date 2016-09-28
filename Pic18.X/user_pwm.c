#include "user_pwm.h"

void pwm_init(WORD period_us, unsigned char duty, unsigned char prescaler){
    int i;
    _pwm_prescaler_value = 1;
    
    if (prescaler > TMR2_PRESCALER_16)
        prescaler = TMR2_PRESCALER_16;
    
    for (i=0; i < prescaler; i++){
        _pwm_prescaler_value*=4;
    }
    
    // 1. Set the PWM period by writing to the PR2 register.
    // PWM Period = [(PR2) + 1] x 4 x TOSC x (TMR2 Prescale Value)
    // Min period -> PR2 = 0 -> 0s
    // Max period -> PR2 = 255 -> 256*4*TOSC*Prescaler*1000000 (us)
    if (prescaler == TMR2_PRESCALER_1){
        _pwm_max_period = MAX_PERIOD_PRESCALER_1;
        if (period_us > _pwm_max_period){
            prescaler = TMR2_PRESCALER_4;
            _pwm_prescaler_value*=4;
        }
    }
    if (prescaler == TMR2_PRESCALER_4){
        _pwm_max_period = MAX_PERIOD_PRESCALER_4;
        if (period_us > _pwm_max_period){
            prescaler = TMR2_PRESCALER_16;
            _pwm_prescaler_value*=4;
        }
    }
    if (prescaler == TMR2_PRESCALER_16){
        _pwm_max_period = MAX_PERIOD_PRESCALER_16;
        if (period_us > _pwm_max_period)
            period_us = _pwm_max_period;
    }
    
    WORD period = (period_us*(_XTAL_FREQ/4000000))/_pwm_prescaler_value - 1;
    _pwm_period = period_us;
    PR2 = period & 0xFF;
    
    //2. Set the PWM duty cycle by writing to the CCPRxL register and CCPxCON<5:4> bits.
    //PWM Duty Cycle = (CCPRXL:CCPXCON<5:4>) x TOSC x (TMR2 Prescale Value)
    if (duty > 100)
        duty = 100;
    WORD dc = _pwm_period * duty / 100;
    CCP1CONbits.DC1B0 = dc & 0x0001;
    CCP1CONbits.DC1B1 = dc & 0x0002;
    CCPR1L = (dc >> 2) & 0x00FF;
    
    //3. Make the CCPx pin an output by clearing the appropriate TRIS bit.
    TRISCbits.TRISC2 = 0;
    
    //4. Set the TMR2 prescale value, then enable Timer2 by writing to T2CON.
    T2CONbits.T2CKPS0 = prescaler & 0x01;
    T2CONbits.T2CKPS1 = (prescaler >> 1) & 0x01;
    T2CONbits.TMR2ON = 1;
    
    //5. Configure the CCPx module for PWM operation.
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
}

void pwm_period(WORD period_us){
    if (period_us > _pwm_max_period)
        period_us = _pwm_max_period;
    
    WORD period = (period_us*(_XTAL_FREQ/4000000))/_pwm_prescaler_value - 1;
    _pwm_period = period_us;
    PR2 = period & 0xFF;
}

void pwm_duty(unsigned char duty){
    if (duty > 100)
        duty = 100;
    WORD dc = _pwm_period * duty / 100;
    CCP1CONbits.DC1B0 = dc & 0x0001;
    CCP1CONbits.DC1B1 = dc & 0x0002;
    CCPR1L = (dc >> 2) & 0x00FF;
}