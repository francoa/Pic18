/* 
 * File:   user_ds18b20.h
 * Author: Franco
 *
 * Created on 3 de julio de 2016, 19:55
 */

#ifndef USER_DS18B20_H
#define	USER_DS18B20_H

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

//#include "linkedList.h"   //DYNAMIC MEMORY ALLOCATION NOT ADVICED ON PIC
#include "user_usart.h"

#ifndef _XTAL_FREQ
    #define _XTAL_FREQ      16000000L
#endif

#define TOT     //JUST FOR VISUALIZATION
#ifdef TOT

/****   BUS PARAMETERS  ****/
#define BUS_LOW()       LATBbits.LATB0 = 0
#define BUS_HIGH()      LATBbits.LATB0 = 1
#define BUS_RELEASE()   TRISBbits.TRISB0 = 1
#define BUS_TAKE()      LATBbits.LATB0 = 0; TRISBbits.TRISB0 = 0
#define BUS_STATE()     PORTBbits.RB0
#define BUS_STATE2()    LATBbits.LATB0
/****   BUS PARAMETERS  ****/

/****   TIMING PARAMETERS   ****/
//Microcontroller reset pulse low: 480us minimum
#define TRESET          500
//Reset resistor pullup: 15us to 60us
#define TRESETRESISTOR  30
//DS18B20 presence pulse: 60us to 240us
#define TPRESENCE       500
//Sample presence pulse time
#define TRESETSAMPLE    60
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
/****   TIMING PARAMETERS   ****/

/****   COMMANDS   ****/
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
/****   COMMANDS   ****/

/****   RESOLUTION   ****/
#define RES_9BIT    0b00
#define RES_10BIT    0b01
#define RES_11BIT    0b10
#define RES_12BIT    0b11
#define RES_DEFAULT  0b11
/****   RESOLUTION   ****/

/****   TEMPERATURE   ****/
#define TEMP_MAX    125
#define TEMP_MIN    -55
/****   TEMPERATURE   ****/

/****   POSSIBLE ROMS   ****/
#define ROM_NUM     10
#define LIST_NUM    5
/****   POSSIBLE ROMS   ****/

#endif

// BASIC COMMANDS
void ds18b20_write_bit(unsigned char write_bit);
void ds18b20_write_byte(BYTE cmd);
unsigned char ds18b20_read_bit(void);
BYTE ds18b20_read_byte(void);

// COMPLEX COMMANDS     UINT64 DATA TYPE NOT USED IN THIS PIC
unsigned char ds18b20_initialization(void);
unsigned char ds18b20_read_rom(UINT32 *, UINT32 *);
unsigned char ds18b20_T_Conversion(void);
void ds18b20_T_Conversion_SpecificROM(int);
unsigned char ds18b20_get_scratch(UINT32 *, UINT32 *, BYTE *);
void ds18b20_get_scratch_SpecificROM(UINT32 *, UINT32 *, BYTE *, int);
unsigned char ds18b20_read_T(float*);
void ds18b20_read_T_SpecificROM(float*,int);
unsigned char ds18b20_write_scratch(int, int, UINT8);
void ds18b20_write_scratch_SpecificROM(int, int, UINT8,int);
void ds18b20_write_values(int, int, UINT8);
unsigned char ds18b20_recall_e2(void);
void ds18b20_recall_e2_SpecificROM(int);
unsigned char ds18b20_copy_e2(void);
void ds18b20_copy_e2_SpecificROM(int);
int ds18b20_search_devices(BYTE cmd);
unsigned char ds18b20_rom_crc(UINT32,UINT32);
unsigned char ds18b20_scratchpad_crc(UINT32,UINT32);

//CRC shift register
void ds18b20_crc_shiftReg_init(void);
void ds18b20_crc_shiftReg_add(unsigned char);

// VARIABLES
int ds18b20_num_devices = 0;
int ds18b20_num_alarms = 0;
//struct ds18b20_device * ds18b20_devices;
UINT32 ds18b20_devices[2*ROM_NUM];
UINT32 ds18b20_alarms[2*ROM_NUM];

unsigned char shiftRegister[8];

#endif	/* USER_DS18B20_H */
