/* 
 * File:   user_led.h
 * Author: Franco
 *
 * Created on 29 de junio de 2016, 23:32
 */

#ifndef USER_LED_H
#define	USER_LED_H

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
    #define _XTAL_FREQ      20000000L
#endif

#define fLED_1              LATAbits.LATA0
#define fLED_2              LATAbits.LATA1
#define fLED_3              LATAbits.LATA2

#define fLED_1_Init()       fLED_1 = 0; TRISAbits.TRISA0 = 0;
#define fLED_2_Init()       fLED_2 = 0; TRISAbits.TRISA1 = 0;
#define fLED_3_Init()       fLED_3 = 0; TRISAbits.TRISA2 = 0;

#define fLED_1_On()         fLED_1 = 1;
#define fLED_2_On()         fLED_2 = 1;
#define fLED_3_On()         fLED_3 = 1;

#define fLED_1_Off()        fLED_1 = 0;
#define fLED_2_Off()        fLED_2 = 0;
#define fLED_3_Off()        fLED_3 = 0;

#define fLED_1_Toggle()     fLED_1 = !fLED_1;
#define fLED_2_Toggle()     fLED_2 = !fLED_2;
#define fLED_3_Toggle()     fLED_3 = !fLED_3;

void led_demo();

#endif	/* USER_LED_H */

