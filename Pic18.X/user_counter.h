/* 
 * File:   user_counter.h
 * Author: Franco
 *
 * Created on 14 de septiembre de 2016, 18:45
 */

#ifndef USER_COUNTER_H
#define	USER_COUNTER_H

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

/***    PRESCALER   ***/
#define T0_PS_1   0b0001
#define T0_PS_2   0b0000
#define T0_PS_4   0b0010
#define T0_PS_8   0b0100
#define T0_PS_16  0b0110
#define T0_PS_32  0b1000
#define T0_PS_64  0b1010
#define T0_PS_128 0b1100
#define T0_PS_256 0b1110
/***    PRESCALER   ***/

void counter_setup(unsigned char);
void counter_prepare(WORD);    //IN MS
void counter_init(void);
void counter_stop(void);     //RETURNS US

unsigned char _counter_i;
int _prescaler;

#endif	/* USER_COUNTER_H */

