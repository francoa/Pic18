#include "system.h"

/* Refer to the device datasheet for information about available
oscillator configurations. */
void ConfigureOscillator(void)
{
    /* TODO Add clock switching code if appropriate.  */

    /* Typical actions in this function are to tweak the oscillator tuning
    register, select new clock sources, and to wait until new clock sources
    are stable before resuming execution of the main project. */
}

void ConfigureInterruptPriority(void){
    /*The interrupt priority feature is enabled by setting the IPEN bit (RCON<7>).*/
    RCONbits.IPEN = 1;
    
    usart_init(9600);
    
    /*When interrupt priority is enabled, there are two bits which enable interrupts
    globally. Setting the GIEH bit (INTCON<7>) enables all
    interrupts that have the priority bit set (high priority).
    Setting the GIEL bit (INTCON<6>) enables all
    interrupts that have the priority bit cleared (low priority).*/
    INTCONbits.GIEH = 1;
    //INTCONbits.GIE = 1;
    INTCONbits.GIEL = 1;
    //INTCONbits.PEIE = 1;
}
