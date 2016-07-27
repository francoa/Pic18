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
#include <stdio.h>

#endif

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ      16000000L
#endif

#include "user_compare.h"

#define BUS_LOW()       LATBbits.LATB0 = 0
#define BUS_HIGH()      LATBbits.LATB0 = 1
#define BUS_RELEASE()   TRISBbits.TRISB0 = 1
#define BUS_TAKE()      LATBbits.LATB0 = 0; TRISBbits.TRISB0 = 0
#define BUS_STATE()     PORTBbits.RB0
#define BUS_STATE2()    LATBbits.LATB0

//Microcontroller reset pulse low: 480us minimum
#define TRESET          500

//Reset resistor pullup: 15us to 60us
#define TRESETRESISTOR  30

//DS18B20 presence pulse: 60us to 240us
#define TPRESENCE       500

//Write time slot: 60us to 120us
#define TWSLOT          100

//Write recovery time: 1us minimum
#define TWRECOV         2

//Write sample time: 15us maximum
#define TWSAMPLE        10

//Read time slot: 60us to 120us
#define TRSLOT          100

//Read recovery time: 1us minimum
#define TRRECOV         1

//Read sample time: 15us - TRLOW maximum
#define TRSAMPLE        9

//Read low time: 1us
#define TRLOW           1

//COMMANDS
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
void ds18b20_write(unsigned char write_bit);
unsigned char ds18b20_read_bit(void);
BYTE ds18b20_read_byte(void);
BYTE ds18b20_read_byte_raw(void);
char * ds18b20_read_T(void);

char ds18b20_temp_str[10];

#endif	/* USER_DS18B20_H */
