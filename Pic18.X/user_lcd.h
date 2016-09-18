/* 
 * File:   user_itm-1602a.h
 * Author: Franco
 *
 * Created on 13 de septiembre de 2016, 12:20
 */

#ifndef USER_ITM_1602A_H
#define	USER_ITM_1602A_H

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

/***    COMMANDS    ***/
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80
/***    COMMANDS    ***/

/***    DISP ENTRY MODE    ***/
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
/***    DISP ENTRY MODE    ***/

/***    DISP ON/OFF    ***/
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00
/***    DISP ON/OFF    ***/

/***    DISP/CURSOR SHIFT    ***/
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00
/***    DISP/CURSOR SHIFT    ***/

/***    FUNCTION SET    ***/
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00
/***    FUNCTION SET    ***/

/***    PIN DEFINITION  ***/
#define RSPIN   LATCbits.LATC0      // LOW: command.  HIGH: character.
#define RSTRIS  TRISCbits.TRISC0

#define RWPIN   LATCbits.LATC1      // LOW: write to LCD.  HIGH: read from LCD.
#define RWTRIS  TRISCbits.TRISC1

#define EPIN    LATAbits.LATA5      // activated by a HIGH pulse.
#define ETRIS   TRISAbits.TRISA5

#define DPIN    LATB
#define DTRIS   TRISB
/***    PIN DEFINITION  ***/

#define OUTPUT  0
#define INPUT   1

#define LOW     0
#define HIGH    1


void lcd_init(bool fourbitmode);

void lcd_begin(uint8_t cols, uint8_t rows, uint8_t charsize);

void lcd_clear();
void lcd_home();

void lcd_noDisplay();
void lcd_display();
void lcd_noBlink();
void lcd_blink();
void lcd_noCursor();
void lcd_cursor();
void lcd_scrollDisplayLeft();
void lcd_scrollDisplayRight();
void lcd_leftToRight();
void lcd_rightToLeft();
void lcd_autoscroll();
void lcd_noAutoscroll();

void lcd_setRowOffsets(int row1, int row2, int row3, int row4);
void lcd_createChar(uint8_t, uint8_t[]);
void lcd_setCursor(uint8_t, uint8_t); 
void lcd_write(uint8_t);
void lcd_command(uint8_t);
void lcd_print_char(char);
void lcd_print(char *, unsigned char);

void lcd_send(uint8_t, uint8_t);
void lcd_write4bits(uint8_t);
void lcd_write8bits(uint8_t);
void lcd_pulseEnable();

unsigned * _lcd_rs_pin; // LOW: command.  HIGH: character.
unsigned * _lcd_rw_pin; // LOW: write to LCD.  HIGH: read from LCD.
unsigned * _lcd_enable_pin; // activated by a HIGH pulse.
unsigned * _lcd_data_pins[8];

uint8_t _lcd_displayfunction;
uint8_t _lcd_displaycontrol;
uint8_t _lcd_displaymode;

uint8_t _lcd_initialized;

uint8_t _lcd_numlines;
uint8_t _lcd_row_offsets[4];

#endif	/* USER_ITM_1602A_H */

