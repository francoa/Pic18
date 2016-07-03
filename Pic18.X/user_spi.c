#include "user_spi.h"

void spi_init(){
    
    /*SSPM3:SSPM0: Master Synchronous Serial Port Mode Select bits
    0101 = SPI Slave mode, clock = SCK pin, SS pin control disabled, SS can be used as I/O pin
    0100 = SPI Slave mode, clock = SCK pin, SS pin control enabled
    0011 = SPI Master mode, clock = TMR2 output/2
    0010 = SPI Master mode, clock = FOSC/64
    0001 = SPI Master mode, clock = FOSC/16
    0000 = SPI Master mode, clock = FOSC/4*/
    SSPCON1bits.SSPM = 0x0;

    /*CKP: Clock Polarity Select bit
    1 = Idle state for clock is a high level
    0 = Idle state for clock is a low level*/
    SSPCON1bits.CKP = 0;
    
    /*CKE: SPI Clock Select bit(1)
        1 = Transmit occurs on transition from active to Idle clock state
        0 = Transmit occurs on transition from Idle to active clock state*/
    SSPSTATbits.CKE = 0;
    
    /*SMP: Sample bit
    SPI Master mode:
        1 = Input data sampled at end of data output time
        0 = Input data sampled at middle of data output time
    SPI Slave mode:
        SMP must be cleared when SPI is used in Slave mode.*/
    SSPSTATbits.SMP = 1;
    
    /*SDI is automatically controlled by the SPI module
    SDO must have TRISC<7> bit cleared
    SCK (Master mode) must have TRISB<1> bit cleared
    SCK (Slave mode) must have TRISB<1> bit set
    SS must have TRISA<5> bit set*/
    TRISCbits.TRISC7 = 0;
    TRISBbits.TRISB1 = 0;
    TRISAbits.TRISA5 = 1;
    
    /*Interrupts*/
    DisableIntSPI;
    
    /*To enable the serial port, MSSP Enable bit, SSPEN (SSPCON1<5>), must be set.*/
    SSPCON1bits.SSPEN = 1;
}

void spi_init_slave(){
    
    SSPCON1bits.SSPM = 0b0101;
    SSPCON1bits.CKP = 0;
    SSPSTATbits.CKE = 0;
    SSPSTATbits.SMP = 0;
    TRISCbits.TRISC7 = 0;
    TRISBbits.TRISB1 = 1;
    TRISAbits.TRISA5 = 1;
    DisableIntSPI;
    SSPCON1bits.SSPEN = 1;
}

void spi_demo_noInterrupts(){
    WriteSPI('H');
}

void spi_demo_noInterrupts_slave(){
    while(!DataRdySPI()){}
    c = ReadSPI();
}