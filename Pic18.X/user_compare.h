/* 
 * File:   user_timer.h
 * Author: Franco
 *
 * Created on 3 de julio de 2016, 20:26
 */

#ifndef USER_COMPARE_H
#define	USER_COMPARE_H

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

void compare_setup(int us, int ms);
void compare_init(void);
void compare_stop(void);

int time;
float value;

#endif	/* USER_COMPARE_H */

