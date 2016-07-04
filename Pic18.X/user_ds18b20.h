/* 
 * File:   user_ds18b20.h
 * Author: Franco
 *
 * Created on 3 de julio de 2016, 19:55
 */

#ifndef USER_DS18B20_H
#define	USER_DS18B20_H

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

#define BUS_LOW()       PORTBbits.RB0 = 0
#define BUS_HIGH()      PORTBbits.RB0 = 1
#define BUS_RELEASE()   TRISBbits.TRISB0 = 1
#define BUS_TAKE()      TRISBbits.TRISB0 = 0; PORTBbits.RB0 = 1

void ds18b20_initialization(void);
bool ds18b20_presence(void);

#endif	/* USER_DS18B20_H */

