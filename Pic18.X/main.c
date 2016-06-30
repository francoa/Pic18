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

#ifdef PIC18F2550
    #include "configuration_bits_2550.c"
#elif PIC18F4550
    #include "configuration_bits_4550.c"
#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user_usart.h"   
#include "user_led.h"
//#include "interrupts.c"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* TODO <INSERT USER APPLICATION CODE HERE> */

    usart_init(9600);

    while(1)
    {
    }

}

