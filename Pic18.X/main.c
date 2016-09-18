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
//#define ONEWIRE_DEMO
//#define DS18B20_DEMO
//#define DS18B20_LCD
//#define DISPLAY_DEMO
#define HCSR04_DEMO
//#define COUNTER_DEMO
//#define CAPTURE_DEMO

#if defined (DS18B20_DEMO) || defined(DS18B20_LCD)
    #include "system.h"
    #include "user_usart.h"
    #include "user_led.h"
    #include "user_ds18b20.h"
    #include "user_lcd.h"
#else
    #include "system.h"        /* System funct/params, like osc/peripheral config */
//    #include "user_usart.h"   
//    #include "user_led.h"
//    #include "user_spi.h"
//    #include "user_extInt.h"
//    #include "user_compare.h"
//    #include "user_ds18b20.h"
    #include "user_lcd.h"
    #include "user_hcsr04.h"
    #include "user_counter.h"
    #include "user_capture.h"
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


#elif defined (ONEWIRE_DEMO)
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

#elif defined (DS18B20_DEMO) || defined(DS18B20_LCD)

void printCommands(){
    M_WRITEUSART((char *)"\n\rAvailable Commands:\n\r\t1-> Read ROM\n\r\t2-> Alarm Search\n\r\t3-> Search (10) devices"
            "\n\r\t4-> Start T Conversion\n\r\t5-> Read ScratchPad\n\r\t6-> Read Last T\n\r\t7-> Set Alarms"
            "\n\r8-> Recall EEPROM\n\r9-> Copy to EEPROM\n\rCOMMAND: ");  
}

void printROM(UINT device){
    int z;
    M_WRITEUSART((char *)"0x");
    for (z=3; z>=0; z--){
        BinToHexUSART((_ds18b20_devices[2*device] >> (8*z)) & 0xFF);
    }
    for (z=3; z>=0; z--){
        BinToHexUSART((_ds18b20_devices[2*device+1] >> (8*z)) & 0xFF);
    }
}

void printAlarm(UINT device){
    int z;
    M_WRITEUSART((char *)"0x");
    for (z=3; z>=0; z--){
        BinToHexUSART((_ds18b20_alarms[2*device] >> (8*z)) & 0xFF);
    }
    for (z=3; z>=0; z--){
        BinToHexUSART((_ds18b20_alarms[2*device+1] >> (8*z)) & 0xFF);
    } 
}

int device_selection(){
    int i;
    char c;
    M_WRITEUSART((char *)"\n\rSelect device:");
    for (i=0; i<_ds18b20_num_devices; i++){
        M_WRITEUSART((char *)"\n\r");
        M_WRITEUSART2(i+'0');
        M_WRITEUSART2('-');
        printROM(i);
    }
    M_WRITEUSART((char *)"\n\rDevice: ");
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
            
    M_WRITEUSART((char *)"\n\rRead Rom:");

    result = ds18b20_read_rom(&highBits,&lowBits);
    switch (result){
        case ERR_NODEV:
            M_WRITEUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
            break;
        case ERR_MOREDEV:
            M_WRITEUSART((char *)"\n\rMultiple devices. Cannot READ ROM");
            break;
        case ERR_CRCFAIL:
            M_WRITEUSART((char *)"\n\rCRC Fail");
            break;
        case ERR_INITFAIL:
            M_WRITEUSART((char *)"\n\rInit Fail");
            break;
        default:
            M_WRITEUSART((char *)"\n\rFamily Code:");
            BinToHexUSART((lowBits) & 0xFF);

            M_WRITEUSART((char *)"\n\rRom:");
            int j;
            for (j=2; j>=0; j--)
                BinToHexUSART((highBits >> 8*j) & 0xFF);
            for (j=3; j>=1; j--)
                BinToHexUSART((lowBits >> 8*j) & 0xFF);

            M_WRITEUSART((char *)"\n\rCrc:");
            BinToHexUSART((highBits >> 24) & 0xFF);

            break;
    }
}

void alarm_search(){
    unsigned char result;
            
    M_WRITEUSART((char *)"\n\rSearch:");

    result = ds18b20_search_devices(ALARM_SEARCH);
    if (result == OK){
        M_WRITEUSART2('0'+_ds18b20_num_alarms);
        int i;
        for (i=0; i<_ds18b20_num_alarms; i++){
            M_WRITEUSART((char *)"\n\rRom: ");
            printAlarm(i);
        }    
    }
    else{
        M_WRITEUSART((char *)"\n\rFail ");
        M_WRITEUSART2(result+'0');
    }
}

void device_search(){
    unsigned char result;
            
    M_WRITEUSART((char *)"\n\rSearch Rom: ");

    result = ds18b20_search_devices(SEARCH_ROM);
    if (result == OK){
        M_WRITEUSART2('0'+_ds18b20_num_devices);
        int i;
        for (i=0; i<_ds18b20_num_devices; i++){
            M_WRITEUSART((char *)"\n\rRom: ");
            printROM(i);
        }
    }
    else{
        M_WRITEUSART((char *)"\n\rFail ");
        M_WRITEUSART2(result+'0');
    }
}

void t_conversion(){
    M_WRITEUSART((char *)"\n\rStart T Conversion:");

    unsigned char result = ds18b20_T_Conversion();

    if (result==ERR_NODEV){
        M_WRITEUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
    }
    else if (result==ERR_MOREDEV){
        int selection = device_selection();
        if (selection >= 0 && selection < _ds18b20_num_devices)
            result = ds18b20_T_Conversion_SpecificROM(selection);
        else
            M_WRITEUSART((char *)"\n\rCannot do that");
    }
    
    M_WRITEUSART((char *)"\n\rResult ");
    M_WRITEUSART2(result+'0');
}

void read_scratch(){
    int counter;
    unsigned char result;
    UINT32 highBits,lowBits;
    M_WRITEUSART((char *)"\n\rRead ScratchPad:");
    
    result = ds18b20_get_scratch(&highBits,&lowBits);   
    
    if (result==ERR_NODEV){
        M_WRITEUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    else if (result==ERR_MOREDEV){
        int selection = device_selection();
        if (selection >= 0 && selection < _ds18b20_num_devices)
            result = ds18b20_get_scratch_SpecificROM(&highBits,&lowBits,selection);
        else{
            M_WRITEUSART((char *)"\n\rCannot do that");
            return;
        }   
    }
    M_WRITEUSART((char *)"\n\rScracthPad: 0x");
    for (counter=3; counter>=0; counter--)
       BinToHexUSART((highBits >> (counter*8)) & 0xFF);        
    for (counter=3; counter>=0; counter--)
       BinToHexUSART((lowBits >> (counter*8)) & 0xFF);   
}

void read_T(){
    char str[10];
    float value;
    unsigned char result;
    
    M_WRITEUSART((char *)"\n\rRead Temperature: ");
    
    result = ds18b20_read_T(&value);   
    
    if (result==ERR_NODEV){
        M_WRITEUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    else if (result==ERR_MOREDEV){
        int selection = device_selection();
        if (selection >= 0 && selection < _ds18b20_num_devices)
            result = ds18b20_read_T_SpecificROM(&value, selection);
        else{
            M_WRITEUSART((char *)"\n\rCannot do that");
            return;
        }   
    }
#ifdef DS18B20_DEMO
    sprintf(str,"\n\r%.4f",value);
    M_WRITEUSART(str);
#else    
    int i = sprintf(str,"\n\r%.4f",value);
    lcd_setCursor(6,0);
    lcd_print(str,i);
#endif
}

int alarm_value(){
    int T = 0;
    char c;
    bool negative = false;
    
    M_WRITEUSART((char *)"\n\rAlarm: 1-Positive , 2-Negative ");
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
    
    M_WRITEUSART((char *)"\n\rInput alarm value (press r when finished): ");
    
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

    if (_ds18b20_num_devices==ERR_NODEV){
        M_WRITEUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    
    int Th = 0;
    int Tl = 0;
    char c = '0';
    
    M_WRITEUSART((char *)"\n\rWrite ScratchPad:");
    
    M_WRITEUSART((char *)"\n\rHigh Limit ALARM:");
    Th = alarm_value();
    M_WRITEUSART((char *)"\n\rLow Limit ALARM:");
    Tl = alarm_value();
    
    M_WRITEUSART((char *)"\n\rResolution: 0-9bits , 1-10bits , 2-11bits , 3-12bits , 3-defaults ");
    while(1){
        if (PIR1bits.RCIF){
            c= ReadUSART();
            PIR1bits.RCIF = 0;
            if (c>='0' && c<='3')
                break;
        }
    }
    
    unsigned char result = ds18b20_write_scratch(Th, Tl, c-'0');
    
    if (result==ERR_NODEV){
        M_WRITEUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    else if (result==ERR_MOREDEV){
        int selection = device_selection();
        if (selection >= 0 && selection < _ds18b20_num_devices)
            result = ds18b20_write_scratch_SpecificROM(Th, Tl, c-'0',selection);
        else{
            M_WRITEUSART((char *)"\n\rCannot do that");
            return;
        }   
    }
    
}

void recall_from_eeprom(){
    unsigned char result;
    
    M_WRITEUSART((char *)"\n\rRecall:");
    
    result = ds18b20_recall_e2();   
    
    if (result==ERR_NODEV){
        M_WRITEUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    else if (result==ERR_MOREDEV){
        int selection = device_selection();
        if (selection >= 0 && selection < _ds18b20_num_devices)
            result = ds18b20_recall_e2_SpecificROM(selection);
        else{
            M_WRITEUSART((char *)"\n\rCannot do that");
            return;
        }   
    }
}

void copy_to_eeprom(){
    unsigned char result;
    
    M_WRITEUSART((char *)"\n\rCopy:");
    
    result = ds18b20_copy_e2();   
    
    if (result==ERR_NODEV){
        M_WRITEUSART((char *)"\n\rNo devices. Perform SEARCH ROM");
        return;
    }
    else if (result==ERR_MOREDEV){
        int selection = device_selection();
        if (selection >= 0 && selection < _ds18b20_num_devices)
            result = ds18b20_copy_e2_SpecificROM(selection);
        else{
            M_WRITEUSART((char *)"\n\rCannot do that");
            return;
        }   
    }
}

void command_parse(char cmd){
    
    switch(cmd){                        //CHECKS    1 DEV       2 or more       PIC
        case '1':   //READ ROM                      CORRECT     N/A             CORRECT
            read_rom();           
            break;
            
        case '2':   //ALARM SEARCH                                              CORRECT
            alarm_search();
            break;
            
        case '3':   //SEARCH ROM                    CORRECT     CORRECT         CORRECT
            device_search();
            break;
            
        case '4':   //T CONVERSION                  CORRECT     CORRECT         CORRECT
            t_conversion();
            break;
            
        case '5':   //READ SCRATCHPAD               Incorrect value
            read_scratch();
            break;
            
        case '6':   //READ TEMPERATURE              CORRECT     CORRECT         CORRECT
            read_T();   
            break;
            
        case '7':   //WRITE SCRATCHPAD              Th and Tl not setting       CORRECT
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
    //printCommands();
    M_WRITEUSART((char *)"\n\r");
}

void main (void){
    int i;
    ConfigureInterruptPriority(true);
    ConfigureInterrupt();
    usart_init(9600,false,false);
    busy = false;
    while(!TRMT);
    putsUSART((char *)"Ready: ");
    printCommands();
    
#ifdef DS18B20_LCD
    for (i=0; i<200; i++) 
        __delay_ms(25);
    lcd_init(false);
    lcd_begin(16, 2, LCD_5x8DOTS);
    lcd_setCursor(0,0);
    lcd_print((char *)"TEMP: ",6);
    device_search();
#endif
    while(1){
#ifdef DS18B20_DEMO
        if (PIR1bits.RCIF){
            command_parse(ReadUSART());
            PIR1bits.RCIF = 0;
        }
#else
        for (i=0; i<80; i++) 
        __delay_ms(25);
        t_conversion();
        read_T();
#endif
    }
}

#elif defined(DISPLAY_DEMO)

void main(void)
{
    int i;
    for (i=0; i<200; i++) 
        __delay_ms(25);
    lcd_init(false);
    lcd_begin(16, 2, LCD_5x8DOTS);
    lcd_setCursor(0,0);
    lcd_print((char *)"LIGHT:",6);
    usart_init(9600,false,false);
    while(1){
        if (PIR1bits.RCIF){
            lcd_print_char(ReadUSART());
        }
    }
}

#elif defined(HCSR04_DEMO)

void main(void)
{
    int i = 0;
    for (i=0; i<80; i++) 
        __delay_ms(25);
    char str[8];
    
    lcd_init(false);
    lcd_begin(16, 2, LCD_5x8DOTS);
    lcd_setCursor(0,0);
    lcd_print((char *)"DIST: ",6);
    
    hcsr04_setup();
    while(1){
        busy = true;
        hcsr04_measure();
        while(busy){};
        i = sprintf(str,"%.4f",hcsr04_read());   //60 ms between consecutive measures
        if (i>0){
            lcd_setCursor(6,0);
            lcd_print(str,i);
        }
        else{
            lcd_setCursor(6,0);
            lcd_print((char *)"err",3);
        }
        for (i=0; i < 80; i++)
            __delay_ms(25);
    }
}

#elif defined(COUNTER_DEMO)

void main(void)
{   
    counter_setup(T0_PS_4);
    fLED_1_Init();
    fLED_1_Off();
    
    while(1){
        __delay_ms(10);
        counter_prepare(20);
        busy = true;
        fLED_1_On();
        counter_init();
        while(busy){};
        fLED_1_Off();
    }
}

#elif defined(CAPTURE_DEMO)

void main(void)
{
    usart_init(9600,false,false);
    char str[10];
    int i;
    busy = true;
    for (i=0; i<80; i++) 
        __delay_ms(25);
    capture_setup();
    capture_prepare();
    while(!PORTCbits.RC2){};
    capture_start();
    while(busy){};
    capture_stop();
    float result = capture_read();
    sprintf(str,"%.4f",result);
    M_WRITEUSART(str);
    M_WRITEUSART((char *)" Finished");
    while(1){};
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

#if defined(USART_INT_DEMO) || defined(EXT_INT_DEMO) || defined(COMPARE_INT_DEMO) || defined(ONEWIRE_DEMO)

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

#if defined(COUNTER_DEMO) || defined(CAPTURE_DEMO) || defined(HCSR04_DEMO)

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
    /*if (INTCONbits.TMR0IF == 1){
        if (_counter_i==0){
            counter_stop();
            busy = false;
        }
        else
            _counter_i--;
        INTCONbits.TMR0IF = 0;
    }*/
        
    // When IPEN bit is cleared (default state), all interrupt handlers go here.
        
    if(PIR1bits.CCP1IF == 1){
        busy = false;
        capture_stop();
        PIR1bits.CCP1IF = 0;
    }
    else if(PIR1bits.TMR1IF == 1){
        _capture_ofcounter++;
        PIR1bits.TMR1IF = 0;
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
    
    if(PIR1bits.CCP1IF == 1){
        busy = false;
        PIR1bits.CCP1IF = 0;
    }
    else if(PIR1bits.TMR1IF == 1){
        _capture_ofcounter++;
        PIR1bits.TMR1IF = 0;
    }
    /*else if (PIR1bits.RCIF == 1){
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
    }*/
}

#endif