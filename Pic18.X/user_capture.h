/* 
 * File:   user_capture.h
 * Author: Franco
 *
 * Created on 15 de septiembre de 2016, 17:24
 */

#ifndef USER_CAPTURE_H
#define	USER_CAPTURE_H

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

void capture_setup();
void capture_start();
void capture_prepare();
float capture_read();
void capture_stop();

unsigned char _capture_ofcounter;
unsigned char _capture_prescaler;
unsigned char _capture_tmr1prescaler;

#endif	/* USER_CAPTURE_H */

