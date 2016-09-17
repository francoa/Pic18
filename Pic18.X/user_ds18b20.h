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
#define BUS_LOW()       LATAbits.LATA4 = 0
#define BUS_HIGH()      LATAbits.LATA4 = 1
#define BUS_RELEASE()   TRISAbits.TRISA4= 1
#define BUS_TAKE()      LATAbits.LATA4 = 0; TRISAbits.TRISA4 = 0
#define BUS_STATE()     PORTAbits.RA4
#define BUS_STATE2()    LATAbits.LATA4
/****   BUS PARAMETERS  ****/

/****   TIMING PARAMETERS   ****/
//Microcontroller reset pulse low: 480us minimum
#define TRESET          480 //500
//Sample presence pulse time: 60us to 240us
#define TRESETSAMPLE    70

//Write time slot: 60us to 120us
#define TWSLOT          60 //100
//Write recovery time: 1us minimum
#define TWRECOV         10 //2
//Write sample time: 15us maximum
#define TWSAMPLE        6 //1

//Read time slot: 60us to 120us
#define TRSLOT          60 //100
//Read recovery time: 1us minimum
#define TRRECOV         10 //1
//Read sample time: 15us - TRLOW maximum
#define TRSAMPLE        9 //14
//Read low time: 1us
#define TRLOW           6 //1
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

/****    ERROR CODES    ****/
#define OK              0
#define ERR_NODEV       1
#define ERR_MOREDEV     2
#define ERR_INITFAIL    3
#define ERR_CRCFAIL     4
#define ERR_UNKCMD      5
#define ERR_UNK         6
/****    ERROR CODES    ****/

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
unsigned char ds18b20_T_Conversion_SpecificROM(int);
unsigned char ds18b20_get_scratch(UINT32 *, UINT32 *);
unsigned char ds18b20_get_scratch_SpecificROM(UINT32 *, UINT32 *, int);
unsigned char ds18b20_read_T(float*);
unsigned char ds18b20_read_T_SpecificROM(float*,int);
unsigned char ds18b20_write_scratch(int, int, UINT8);
unsigned char ds18b20_write_scratch_SpecificROM(int, int, UINT8,int);
void ds18b20_write_values(int, int, UINT8);
unsigned char ds18b20_recall_e2(void);
unsigned char ds18b20_recall_e2_SpecificROM(int);
unsigned char ds18b20_copy_e2(void);
unsigned char ds18b20_copy_e2_SpecificROM(int);
unsigned char ds18b20_search_devices(BYTE cmd);
unsigned char ds18b20_rom_crc(UINT32,UINT32);
unsigned char ds18b20_scratchpad_crc(UINT32,UINT32);

//CRC shift register
void ds18b20_crc_shiftReg_init(void);
void ds18b20_crc_shiftReg_add(unsigned char);

// VARIABLES
unsigned char _ds18b20_num_devices = 0;
unsigned char _ds18b20_num_alarms = 0;
//struct _ds18b20_device * ds18b20_devices;
UINT32 _ds18b20_devices[2*ROM_NUM];
UINT32 _ds18b20_alarms[2*ROM_NUM];

unsigned char _shiftRegister[8];

#endif	/* USER_DS18B20_H */
