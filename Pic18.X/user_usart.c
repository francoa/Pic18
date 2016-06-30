#include "user_usart.h"

void usart_init(int baudrate){
    
    //EUSART Asynchronous Mode
    
    /*Initialize the SPBRGH:SPBRG registers for the
    appropriate baud rate. Set or clear the BRGH
    and BRG16 bits, as required, to achieve the
    desired baud rate.*/
    TXSTAbits.BRGH = 1;     //TXSTA<2>
    BAUDCONbits.BRG16 = 1;  //BAUDCON<3>
    BAUDCONbits.ABDEN = 0;  //BAUDCON<0> No Auto-Baud Detect
    BAUDCONbits.WUE = 0;    //BAUDCON<1> Wake-up disabled
    sp = (_XTAL_FREQ/baudrate)/4 - 1;       
    SPBRGH = sp >> 8;
    SPBRG = (sp << 8) >> 8;
    
    /*Enable the asynchronous serial port by clearing
    bit SYNC and setting bit SPEN.*/
    TXSTAbits.SYNC = 0;     //TXSTA<4>
    RCSTAbits.SPEN = 1;     //RCSTA<7>
    
    /*If the signal from the TX/RX pin is to be inverted, set
    the TXCKP/RXDTP bit.*/
    BAUDCON = BAUDCON & 0b11001111;     //BAUDCON<4,5>
    
    /*If interrupts are desired, set enable bit TXIE/RCIE.*/
    //PIE1bits.RCIE = 1;
    //PIE1bits.TXIE = 1;
    
    
    /*If 9-bit transmission/reception is desired, set bit
    TX9/RX9. Can be used as address/data bit.*/
    TXSTAbits.TX9 = 0;      //TXSTA<6>
    RCSTAbits.RX9 = 0;      //RCSTA<6>
    
    /*Enable the transmission/reception by setting bit TXEN/CREN*/
    TXSTAbits.TXEN = 1;     //TXSTA<5>
    RCSTAbits.CREN = 1;     //RCSTA<4>
    
    /*If using interrupts, ensure that the GIE and PEIE
    bits in the INTCON register (INTCON<7:6>) are
    set.*/      
    //INTCONbits.GIE = 1;
    //INTCONbits.PEIE = 1;
       
}

void usart_demo_noInterrupts(){
    /*Flag bit, RCIF, will be set when reception is complete*/
    while(!PIR1bits.RCIF);
    c = ReadUSART ();
    WriteUSART(c);
}
