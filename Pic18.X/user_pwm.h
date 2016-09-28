/* 
 * File:   user_pwm.h
 * Author: Franco
 *
 * Created on 25 de septiembre de 2016, 20:10
 */

#ifndef USER_PWM_H
#define	USER_PWM_H

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#endif

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ      16000000L
#endif

#define TMR2_PRESCALER_1    0b00
#define TMR2_PRESCALER_4    0b01
#define TMR2_PRESCALER_16   0b10

#define MAX_PERIOD_PRESCALER_1     (((0xFF+1)*4)/(_XTAL_FREQ/1000000))
#define MAX_PERIOD_PRESCALER_4     (((0xFF+1)*16)/(_XTAL_FREQ/1000000))
#define MAX_PERIOD_PRESCALER_16    (((0xFF+1)*64)/(_XTAL_FREQ/1000000))

void pwm_init(WORD,unsigned char,unsigned char);
void pwm_period(WORD);
void pwm_duty(unsigned char);

unsigned char _pwm_prescaler_value;
WORD _pwm_period;
WORD _pwm_max_period;
        

#endif	/* USER_PWM_H */

