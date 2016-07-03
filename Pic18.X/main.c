/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include "configuration_bits.c"

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user_usart.h"   
#include "user_led.h"
#include "user_spi.h"
#include "user_extInt.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/


/* SELECT PROGRAM */
//#define USART_DEMO
//#define USART_INT_DEMO
//#define LED_DEMO
//#define EXT_INT_DEMO
//#define SPI_MASTER_DEMO
#define SPI_SLAVE_DEMO

#ifdef USART_DEMO
void main(void)
{
    usart_init(9600,false,false);
    while(1){
        usart_demo_noInterrupts();
    }
}

#elif defined (USART_INT_DEMO)
void main(void)
{
    ConfigureInterruptPriority(true);
    usart_init(9600,true,false);
    ConfigureInterrupt();
    fLED_1_Init();
    while(1){
        
    }
}

#elif defined (LED_DEMO)
void main(void)
{
    while(1){
        led_demo();
    }
}

#elif defined (EXT_INT_DEMO)
void main(void)
{
    extInt_init();
    
    ConfigureInterruptPriority(false);
    ConfigureInterrupt();
    
    fLED_1_Init();
    while(1){
    }
}

#elif defined (SPI_MASTER_DEMO)
void main(void)
{
    spi_init();
    TRISAbits.TRISA0 = 1;
    while(1)
    {
        while(LATAbits.LATA0){}
        spi_demo_noInterrupts();
        while(!LATAbits.LATA0){}
    }
}

#elif defined (SPI_SLAVE_DEMO)
void main(void)
{
    spi_init_slave();
    while(1)
    {
        spi_demo_noInterrupts_slave();
    }
}

#else
void main(void)
{
    while(1){}
}

#endif

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* High-priority service */

#if defined(USART_INT_DEMO) || defined(EXT_INT_DEMO)

#if defined(__XC) || defined(HI_TECH_C)
void interrupt high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{
    if (INTCONbits.INT0IF == 1){
        fLED_1_Toggle();
        INTCONbits.INT0IF = 0;
    }
}

/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void low_priority interrupt low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{
    if (PIR1bits.RCIF == 1){
        fLED_1_Toggle();
        ReadUSART();
        PIR1bits.RCIF = 0;
    }
}

#endif