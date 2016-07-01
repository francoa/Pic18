/* 
 * File:   user_spi.h
 * Author: Franco
 *
 * Created on 30 de junio de 2016, 12:16
 */

#ifndef USER_SPI_H
#define	USER_SPI_H

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

#include "user_led.h"

void spi_init();
void spi_init_slave();
void spi_demo_noInterrupts();
void spi_demo_noInterrupts_slave();

char c;

#endif	/* USER_SPI_H */

