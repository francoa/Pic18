#include "user_usart.h"

void usart_init(int baudrate, bool rxInt, bool txInt){
    int usart_sp;
    //EUSART Asynchronous Mode
    
    /*Initialize the SPBRGH:SPBRG registers for the
    appropriate baud rate. Set or clear the BRGH
    and BRG16 bits, as required, to achieve the
    desired baud rate.*/
    TXSTAbits.BRGH = 1;     //TXSTA<2>
    BAUDCONbits.BRG16 = 1;  //BAUDCON<3>
    BAUDCONbits.ABDEN = 0;  //BAUDCON<0> No Auto-Baud Detect
    BAUDCONbits.WUE = 0;    //BAUDCON<1> Wake-up disabled
    usart_sp = (_XTAL_FREQ/baudrate)/4 - 1;       
    SPBRGH = usart_sp >> 8;
    SPBRG = (usart_sp << 8) >> 8;
    
    /*Enable the asynchronous serial port by clearing
    bit SYNC and setting bit SPEN.*/
    TXSTAbits.SYNC = 0;     //TXSTA<4>
    RCSTAbits.SPEN = 1;     //RCSTA<7>
    
    /*If the signal from the TX/RX pin is to be inverted, set
    the TXCKP/RXDTP bit.*/
    BAUDCON = BAUDCON & 0b11001111;     //BAUDCON<4,5>
    
    /*If interrupts are desired, set enable bit TXIE/RCIE.*/
    if (rxInt){
        PIE1bits.RCIE = 1;
        IPR1bits.RCIP = 0; /*Configures interrupt as low priority*/
    }
    else
        PIE1bits.RCIE = 0;
    if (txInt){
        PIE1bits.TXIE = 1;
        IPR1bits.TXIP = 0; /*Configures interrupt as low priority*/
    }
    else
        PIE1bits.TXIE = 0;
    
    /*If 9-bit transmission/reception is desired, set bit
    TX9/RX9. Can be used as address/data bit.*/
    TXSTAbits.TX9 = 0;      //TXSTA<6>
    RCSTAbits.RX9 = 0;      //RCSTA<6>
    
    /*Enable the transmission/reception by setting bit TXEN/CREN*/
    TXSTAbits.TXEN = 1;     //TXSTA<5>
    RCSTAbits.CREN = 1;     //RCSTA<4>
       
}

void usart_demo_noInterrupts(){
    char usart_char;
    /*Flag bit, RCIF, will be set when reception is complete*/
    while(!PIR1bits.RCIF);
    usart_char = ReadUSART ();
    while(!TRMT);
    WriteUSART(usart_char);
}

void WriteBinUSART(BYTE val){
    BYTE usart_slider;
    int usart_sp;
    while(!TRMT);
    putsUSART((char*)"\n\r");
    usart_slider = 1;
    for (usart_sp = 7; usart_sp>=0; usart_sp--){
        while(!TRMT);
        if ((val & (usart_slider << usart_sp))!=0)
            WriteUSART('1');
        else
            WriteUSART('0');
    }
}

void WriteByteUSART(BYTE val){
    while(!TRMT);
    putsUSART((char*)"\n\r");
    while(!TRMT);
    WriteUSART(val); 
}

void BinToHexUSART(BYTE val){
    BYTE usart_slider;
    usart_slider = val >> 4;
    while(!TRMT);
    WriteUSART(hexChars[usart_slider]);
    usart_slider = val & 0x0F;
    while(!TRMT);
    WriteUSART(hexChars[usart_slider]);
}