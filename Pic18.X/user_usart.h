/* 
 * File:   usart.h
 * Author: Franco
 *
 * Created on 29 de junio de 2016, 22:49
 */

#ifndef USER_USART_H
#define	USER_USART_H

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ      16000000L
#endif

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

void usart_init(int,bool,bool);
void usart_demo_noInterrupts();
void WriteBinUSART(BYTE);
void WriteByteUSART(BYTE);
void BinToHexUSART(BYTE);

char hexChars[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
#endif	/* USER_USART_H */

