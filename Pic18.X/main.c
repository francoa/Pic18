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
#include <stdio.h>

#endif

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

bool busy;

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/


/* SELECT PROGRAM */
//#define USART_DEMO
//#define USART_INT_DEMO
//#define LED_DEMO
//#define EXT_INT_DEMO
//#define SPI_MASTER_DEMO
//#define SPI_SLAVE_DEMO
//#define COMPARE_INT_DEMO
//#define DEV_1WIRE_DEMO
#define DEV_DS18B20_DEBUG

#ifdef DEV_DS18B20_DEBUG
    #include "system.h"
    #include "user_usart.h"
    #include "user_led.h"
    #include "user_ds18b20.h"
#else
    #include "system.h"        /* System funct/params, like osc/peripheral config */
    #include "user_usart.h"   
    #include "user_led.h"
    #include "user_spi.h"
    #include "user_extInt.h"
    #include "user_compare.h"
    #include "user_ds18b20.h"
#endif

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

#elif defined(COMPARE_INT_DEMO)
void main(void){
    fLED_1_Init();
    ConfigureInterruptPriority(true);
    ConfigureInterrupt();
    compare_setup(100,0);
    compare_init();
    while(1){
    }
}


#elif defined (DEV_1WIRE_DEMO)
void init_sequence(void){
    /*INITIALIZATION SEQUENCE*/
    dev_present = false;
    exit = false;
    fLED_1_Init();  //RED LED
    
    ds18b20_initialization();
    ds18b20_presence();
    
    while(BUS_STATE() == 1 && !exit){__delay_us(3);}
    if (exit){
        fLED_1_On();
        while(1){}
    }
    else{
        compare_stop();
        dev_present = true;
    }
    /*INITIALIZATION SEQUENCE*/ 
}

void main (void){
    BYTE romVal[9];
    ConfigureInterruptPriority(true);
    ConfigureInterrupt();
    usart_init(9600,false,false);
    
    init_sequence();
    ds18b20_command(SKIP_ROM);
    ds18b20_command(CONVERT_T);
    while(!ds18b20_read_bit());
    __delay_ms(1);
    
    init_sequence();
    ds18b20_command(SKIP_ROM);
    ds18b20_command(READ_SCRATCH);
    for (j=0; j<9; j++)
        romVal[j] = ds18b20_read_byte();
    
    while(!TRMT);
    WriteUSART('R');
    for (j=8; j>=0; j--){
        WriteBinUSART(romVal[j]);
    }
    while(!TRMT);
    WriteUSART('R');
    /*ROM COMMAND*/
    while(1){}
}

#elif defined (DEV_DS18B20_DEBUG)

void printCommands(){
    while(!TRMT);
    putsUSART((char *)"\n\rAvailable Commands:\n\r\t1-> Read ROM\n\r\t2-> Alarm Search\n\r\t3-> Search (10) devices"
            "\n\r\t4-> Start T Conversion\n\r\t5-> Read ScratchPad\n\r\t6-> Read Last T\n\r\t7-> Set Alarms"
            "\n\r8-> Recall EEPROM\n\r9-> Copy to EEPROM\n\rCOMMAND: ");  
}

void printROM(UINT device){
    int z;
    while(!TRMT);
    putsUSART((char *)"0x");
    for (z=3; z>=0; z--){
        BinToHexUSART((ds18b20_devices[2*device] >> (8*z)) & 0xFF);
    }
    for (z=3; z>=0; z--){
        BinToHexUSART((ds18b20_devices[2*device+1] >> (8*z)) & 0xFF);
    }
}

int device_selection(){
    int i;
    char c;
    while(!TRMT);
    putsUSART((char *)"\n\rSelect device:");
    for (i=0; i<ds18b20_num_devices; i++){
        while(!TRMT);
        putsUSART((char *)"\n\r");
        while(!TRMT);
        WriteUSART(i+'0');
        while(!TRMT);
        WriteUSART('-');
        printROM(i);
    }
    while(!TRMT);
    putsUSART((char *)"\n\rDevice: ");
    while(1){
        if (PIR1bits.RCIF){
            c= ReadUSART();
            PIR1bits.RCIF = 0;
            if (c>='0' && c<='9')
                break;
        }
    }
    return c - '0';
}

void read_rom(){
    unsigned char result;
    UINT32 highBits,lowBits;
            
    while(!TRMT);
    putsUSART((char *)"\n\rRead Rom:");

    result = ds18b20_read_rom(&highBits,&lowBits);
    if (result == 0)
    {
        while(!TRMT);
        putsUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
    }
    else if (result == 1)
    {
        while(!TRMT);
        putsUSART((char *)"\n\rMultiple devices. Cannot READ ROM");
    }
    else{
        while(!TRMT);
        putsUSART((char *)"\n\rFamily Code:");
        BinToHexUSART((lowBits) & 0xFF);

        while(!TRMT);
        putsUSART((char *)"\n\rRom:");
        int j;
        for (j=2; j>=0; j--)
            BinToHexUSART((highBits >> 8*j) & 0xFF);
        for (j=3; j>=1; j--)
            BinToHexUSART((lowBits >> 8*j) & 0xFF);

        while(!TRMT);
        putsUSART((char *)"\n\rCrc:");
        BinToHexUSART((highBits >> 24) & 0xFF);
        
        unsigned char crc = ds18b20_rom_crc(highBits,lowBits);
        while(!TRMT);
        putsUSART((char *)"\n\rCalculated Crc:");
        BinToHexUSART(crc);
    }
}

void alarm_search(){
    int devices;
            
    while(!TRMT);
    putsUSART((char *)"\n\rSearch:");

    devices = ds18b20_search_devices(ALARM_SEARCH);
    while(!TRMT);
    WriteUSART('0'+devices);
    int i,z;
    for (i=0; i<devices; i++){
        while(!TRMT);
        putsUSART((char *)"\n\rRom: ");
        printROM(i);
    }
}

void device_search(){
    int roms;
    unsigned char crc;
            
    while(!TRMT);
    putsUSART((char *)"\n\rSearch Rom: ");

    roms = ds18b20_search_devices(SEARCH_ROM);
    while(!TRMT);
    WriteUSART('0'+roms);
    int i,z;
    for (i=0; i<roms; i++){
        while(!TRMT);
        putsUSART((char *)"\n\rRom: ");
        printROM(i);
        while(!TRMT);
        putsUSART((char *)"  Calculated Crc: ");
        crc = ds18b20_rom_crc(ds18b20_devices[2*i],ds18b20_devices[2*i+1]);
        BinToHexUSART(crc);
    }
}

void t_conversion(){
    while(!TRMT);
    putsUSART((char *)"\n\rStart T Conversion:");

    UINT8 result = ds18b20_T_Conversion();

    if (result==0){
        while(!TRMT);
        putsUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
    }
    else if (result==1){
        int selection = device_selection();
        if (selection >= 0 && selection < ds18b20_num_devices)
            ds18b20_T_Conversion_SpecificROM(selection);
        else{
            while(!TRMT);
            putsUSART((char *)"\n\rCannot do that");
        }   
    }
}

void read_scratch(){
    int counter;
    unsigned char result;
    UINT32 highBits,lowBits;
    BYTE crc;
    while(!TRMT);
    putsUSART((char *)"\n\rRead ScratchPad:");
    
    result = ds18b20_get_scratch(&highBits,&lowBits,&crc);   
    
    if (result==0){
        while(!TRMT);
        putsUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    else if (result==1){
        int selection = device_selection();
        if (selection >= 0 && selection < ds18b20_num_devices)
            ds18b20_get_scratch_SpecificROM(&highBits,&lowBits,&crc,selection);
        else{
            while(!TRMT);
            putsUSART((char *)"\n\rCannot do that");
            return;
        }   
    }
    while(!TRMT);
    putsUSART((char *)"\n\rScracthPad: 0x");
    for (counter=3; counter>=0; counter--)
       BinToHexUSART((highBits >> (counter*8)) & 0xFF);        
    for (counter=3; counter>=0; counter--)
       BinToHexUSART((lowBits >> (counter*8)) & 0xFF);   
    while(!TRMT);
    putsUSART((char *)"\n\rCrc: 0x");
    BinToHexUSART(crc);
    while(!TRMT);
    putsUSART((char *)"  Calculated crc: 0x");
    BinToHexUSART(ds18b20_scratchpad_crc(highBits,lowBits));
}

void read_T(){
    char str[10];
    float value;
    unsigned char result;
    
    while(!TRMT);
    putsUSART((char *)"\n\rRead Temperature:");
    
    result = ds18b20_read_T(&value);   
    
    if (result==0){
        while(!TRMT);
        putsUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    else if (result==1){
        int selection = device_selection();
        if (selection >= 0 && selection < ds18b20_num_devices)
            ds18b20_read_T_SpecificROM(&value, selection);
        else{
            while(!TRMT);
            putsUSART((char *)"\n\rCannot do that");
            return;
        }   
    }
    
    sprintf(str,"\n\rTemperature: %.4f",value);
    while(!TRMT);
    putsUSART(str);
}

int alarm_value(){
    int T = 0;
    char c;
    bool negative = false;
    
    while(!TRMT);
    putsUSART((char *)"\n\rAlarm: 1-Positive , 2-Negative ");
    while(1){
        if (PIR1bits.RCIF){
            c= ReadUSART();
            PIR1bits.RCIF = 0;
            if (c=='1' || c=='2')
                break;
        }
    }
    if (c=='2')
        negative = true;
    
    while(!TRMT);
    putsUSART((char *)"\n\rInput alarm value (press r when finished): ");
    
    while(c != 'R' && c!= 'r'){
        if (PIR1bits.RCIF){
            c= ReadUSART();
            PIR1bits.RCIF = 0;
            if (c>= '0' && c<='9')
                T = T*10 + (c - '0');
        }
    }
    if (negative)
        T = -T;
    return T;
}

void write_scratch(){

    if (ds18b20_num_devices==0){
        while(!TRMT);
        putsUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    
    int Th = 0;
    int Tl = 0;
    char c = '0';
    
    while(!TRMT);
    putsUSART((char *)"\n\rWrite ScratchPad:");
    
    while(!TRMT);
    putsUSART((char *)"\n\rHigh Limit ALARM:");
    Th = alarm_value();
    while(!TRMT);
    putsUSART((char *)"\n\rLow Limit ALARM:");
    Tl = alarm_value();
    
    while(!TRMT);
    putsUSART((char *)"\n\rResolution: 0-9bits , 1-10bits , 2-11bits , 3-12bits , 3-defaults ");
    while(1){
        if (PIR1bits.RCIF){
            c= ReadUSART();
            PIR1bits.RCIF = 0;
            if (c>='0' && c<='3')
                break;
        }
    }
    
    unsigned char result = ds18b20_write_scratch(Th, Tl, c-'0');
    
    if (result==0){
        while(!TRMT);
        putsUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    else if (result==1){
        int selection = device_selection();
        if (selection >= 0 && selection < ds18b20_num_devices)
            ds18b20_write_scratch_SpecificROM(Th, Tl, c-'0',selection);
        else{
            while(!TRMT);
            putsUSART((char *)"\n\rCannot do that");
            return;
        }   
    }
    
}

void recall_from_eeprom(){
    unsigned char result;
    
    while(!TRMT);
    putsUSART((char *)"\n\rRecall:");
    
    result = ds18b20_recall_e2();   
    
    if (result==0){
        while(!TRMT);
        putsUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    else if (result==1){
        int selection = device_selection();
        if (selection >= 0 && selection < ds18b20_num_devices)
            ds18b20_recall_e2_SpecificROM(selection);
        else{
            while(!TRMT);
            putsUSART((char *)"\n\rCannot do that");
            return;
        }   
    }
}

void copy_to_eeprom(){
    unsigned char result;
    
    while(!TRMT);
    putsUSART((char *)"\n\rCopy:");
    
    result = ds18b20_copy_e2();   
    
    if (result==0){
        while(!TRMT);
        putsUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    else if (result==1){
        int selection = device_selection();
        if (selection >= 0 && selection < ds18b20_num_devices)
            ds18b20_copy_e2_SpecificROM(selection);
        else{
            while(!TRMT);
            putsUSART((char *)"\n\rCannot do that");
            return;
        }   
    }
}

void command_parse(char cmd){
    
    switch(cmd){                        //CHECKS    1 DEV       2 or more
        case '1':   //READ ROM                      CORRECT     N/A
            read_rom();           
            break;
            
        case '2':   //ALARM SEARCH
            alarm_search();
            break;
            
        case '3':   //SEARCH ROM                    CORRECT     CORRECT
            device_search();
            break;
            
        case '4':   //T CONVERSION                  CORRECT     CORRECT             
            t_conversion();
            break;
            
        case '5':   //READ SCRATCHPAD               Incorrect value
            read_scratch();
            break;
            
        case '6':   //READ TEMPERATURE              CORRECT     CORRECT
            read_T();   
            break;
            
        case '7':   //WRITE SCRATCHPAD              Th and Tl not setting
            write_scratch();
            break;
            
        case '8':   //RECALL E2                     CORRECT     CORRECT
            recall_from_eeprom();
            break;
            
        case '9':   //COPY TO E2                    CORRECT     CORRECT
            copy_to_eeprom();
            break;
                    
        default:
            putsUSART((char *)"\n\rUnrecognized command");
            break;
    }
    printCommands();
}

void main (void){
    ConfigureInterruptPriority(true);
    ConfigureInterrupt();
    usart_init(9600,false,false);
    busy = false;
    while(!TRMT);
    putsUSART((char *)"Ready: ");
    printCommands();
    while(1){
        if (PIR1bits.RCIF){
            command_parse(ReadUSART());
            PIR1bits.RCIF = 0;
        }
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

#if defined(USART_INT_DEMO) || defined(EXT_INT_DEMO) || defined(COMPARE_INT_DEMO) || defined(DEV_1WIRE_DEMO)

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
    else if (PIR1bits.CCP1IF == 1)
    {
        compare_stop();
        if (!dev_present)
            exit = true;
        TMR1H = 0x00;
        TMR1L = 0x00;
        PIR1bits.CCP1IF = 0;
    }
}

#endif

#if defined(DEV_DS18B20_)

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
        PIR1bits.RCIF = 0;
        if (busy)
        {
            while(!TRMT);
            putsUSART((char *)"\n\rBusy.");
            ReadUSART();
        }
        else{
            busy = true;
            command_parse(ReadUSART());
            busy = false;
        }
    }
}

#endif