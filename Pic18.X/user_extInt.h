/* 
 * File:   user_int0.h
 * Author: Franco
 *
 * Created on 30 de junio de 2016, 11:06
 */

#ifndef USER_EXTINT_H
#define	USER_EXTINT_H

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

void extInt_init();

#endif	/* USER_EXTINT_H */

