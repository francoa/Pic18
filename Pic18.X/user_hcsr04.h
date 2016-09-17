/* 
 * File:   user_hcsr04.h
 * Author: Franco
 *
 * Created on 13 de septiembre de 2016, 17:31
 */

#ifndef USER_HCSR04_H
#define	USER_HCSR04_H

/* Necessary include files */
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
#include <stdio.h>

#endif

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ      16000000L
#endif

/***    PINS    ***/
#define TRIGPIN     LATAbits.LATA4
#define TRIGTRIS    TRISAbits.TRISA4
#define ECHOPIN     PORTAbits.RA5
#define ECHOTRIS    TRISAbits.TRISA5
/***    PINS    ***/

#define RANGE       (500) //cm
#define CM2US       (58)
#define US2CM       (1/58)

#define HCSR04_CONFIG   TRIGPIN = 0; TRIGTRIS = 0; ECHOTRIS = 1

void hcsr04_setup();
float hcsr04_measure();     //RETURNS CM

#endif	/* USER_HCSR04_H */

