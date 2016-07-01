/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

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

#include "configuration_bits.c"
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user_usart.h"   
#include "user_led.h"
#include "user_spi.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

#define SPI_SLAVE

void main(void)
{
    //ConfigureInterruptPriority();
    
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* TODO <INSERT USER APPLICATION CODE HERE> */

#ifdef SPI_SLAVE    
    usart_init(9600);
    spi_init_slave();
    while(1)
    {
        spi_demo_noInterrupts_slave();
    }
#else
    TRISAbits.RA0 = 1;
    spi_init();
    while(1)
    {
        spi_demo_noInterrupts();
    }
#endif
}

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* High-priority service */

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
      /* This code stub shows general interrupt handling.  Note that these
      conditional statements are not handled within 3 seperate if blocks.
      Do not use a seperate if block for each interrupt flag to avoid run
      time errors. */

    if (INTCONbits.INT0IF == 1){
        while(!TRMT);
        WriteUSART('I');
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
    /* This code stub shows general interrupt handling.  Note that these
    conditional statements are not handled within 3 seperate if blocks.
    Do not use a seperate if block for each interrupt flag to avoid run
    time errors. */

    if (PIR1bits.RCIF == 1){
        fLED_1_Toggle();
        ReadUSART();
        PIR1bits.RCIF = 0;
    }
    
}
