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

#include "user_compare.h"

#define BUS_LOW()       PORTBbits.RB0 = 0
#define BUS_HIGH()      PORTBbits.RB0 = 1
#define BUS_RELEASE()   TRISBbits.TRISB0 = 1
#define BUS_TAKE()      TRISBbits.TRISB0 = 0; PORTBbits.RB0 = 1
#define BUS_STATE()     LATBbits.LATB0

#define TRECOVERY       3
#define TSLOT           80

#define TRESETLOW       500
#define TRESETHIGH      500

#define TWLOW0          80
#define TWLOW1          5

#define TRLOW           2
#define TRWAIT          8
#define TRSAMPLE        15

#define SEARCH_ROM      0xF0
#define READ_ROM        0x33
#define MATCH_ROM       0x55
#define SKIP_ROM        0xCC
#define ALARM_SEARCH    0xEC
#define CONVERT_T       0x44
#define WRITE_SCRATCH   0x4E
#define READ_SCRATCH    0xBE
#define COPY_SCRATCH    0x48
#define RECALL_E2       0xB8
#define READ_SUPPLY     0xB4

void ds18b20_initialization(void);
void ds18b20_presence(void);
void ds18b20_command(BYTE cmd);
void ds18b20_write0(void);
void ds18b20_write1(void);
BYTE ds18b20_read(void);

int i;
BYTE slider;

#endif	/* USER_DS18B20_H */

