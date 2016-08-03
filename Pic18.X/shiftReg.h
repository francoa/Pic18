/* 
 * File:   shiftReg.h
 * Author: Franco
 *
 * Created on 2 de agosto de 2016, 15:37
 */

#ifndef SHIFTREG_H
#define	SHIFTREG_H

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


BYTE shiftRegister;

#endif	/* SHIFTREG_H */

