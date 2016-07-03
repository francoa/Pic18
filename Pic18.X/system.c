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

void ConfigureInterruptPriority(bool ipen){
    /*The interrupt priority feature is enabled by setting the IPEN bit (RCON<7>).*/
    if (ipen)
        RCONbits.IPEN = 1;
    else
        RCONbits.IPEN = 0;
}

void ConfigureInterrupt(void){
    /*When interrupt priority is enabled, there are two bits which enable interrupts
    globally. Setting the GIEH bit (INTCON<7>) enables all
    interrupts that have the priority bit set (high priority).
    Setting the GIEL bit (INTCON<6>) enables all
    interrupts that have the priority bit cleared (low priority).*/
    if (RCONbits.IPEN == 1){
        INTCONbits.GIEH = 1;
        INTCONbits.GIEL = 1;
    }
    else{
        INTCONbits.PEIE = 1; //Same bit
        INTCONbits.GIE = 1; //Same bit
    }
}
