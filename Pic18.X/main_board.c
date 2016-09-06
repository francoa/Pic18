/* 
 * File:   main_board.c
 * Author: Franco
 *
 * Created on 1 de septiembre de 2016, 15:02
 */

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
int temperaturesInt[2];

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

#include "system.h"
#include "user_usart.h"
#include "user_led.h"
#include "user_ds18b20.h"

unsigned char t_conversion(){
    unsigned char result = ds18b20_T_Conversion();
    
    //TODO
    /*if (result==ERR_MOREDEV){
        int selection = device_selection();
        if (selection >= 0 && selection < ds18b20_num_devices)
            result = ds18b20_T_Conversion_SpecificROM(selection);
    }*/
    
    return result;
}

bool single_read(){
    
    if (t_conversion() != OK)
        return false;
    
    char str[9];
    float value;
    unsigned char result;
    
    result = ds18b20_read_T(&value);   
    
    //TODO
    /*if (result==0){
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
    }*/
    
    if (result == OK){
        M_WRITEUSART((char *)":A;");
        sprintf(str,"%.4f",value);
        M_WRITEUSART(str);
        M_WRITEUSART2('#');
        return true;
    }
    return false;
}

bool read_temp_limits(){
    char str[5];
    unsigned char result;
    UINT32 highBits,lowBits;
    
    result = ds18b20_get_scratch(&highBits,&lowBits);   

    //TODO
    /*if (result==ERR_MOREDEV){
        int selection = device_selection();
        if (selection >= 0 && selection < ds18b20_num_devices)
            result = ds18b20_get_scratch_SpecificROM(&highBits,&lowBits,&crc,selection);
    }*/
    
    if (result == OK){
        //Transform to Temp
        temperaturesInt[0] = (lowBits >> 16) & 0xFF;
        temperaturesInt[1] = (lowBits >> 24) & 0xFF;
        M_WRITEUSART((char *)":R");
        sprintf(str,";%d",temperaturesInt[0]);
        M_WRITEUSART(str);
        sprintf(str,";%d",temperaturesInt[1]);
        M_WRITEUSART(str);
        M_WRITEUSART2('#');
        return true;
    }
    return false;
}

bool write_scratch(){

    unsigned char result = ds18b20_write_scratch(temperaturesInt[0], temperaturesInt[1], 3);
    
    
    //TODO
    /*if (result==ERR_MOREDEV){
        int selection = device_selection();
        if (selection >= 0 && selection < ds18b20_num_devices)
            ds18b20_write_scratch_SpecificROM(temperaturesInt[0], temperaturesInt[1], 3);
    }*/
    
    if (result == OK)
        return true;
    return false;
}

bool set_temp_limits(){   //SET TEMPERATURE LIMITS    :T;THigh;TLow#   (Only Integer Values)
    PIR1bits.RCIF = 0;
    UINT8 i = 0;
    char c;
    bool negative = false;
    while(1){
        if (PIR1bits.RCIF){
            c = ReadUSART();
            if (c == ';'){
                i++;
                temperaturesInt[i-1]=0;
                negative = false;
            }
            else if (i > 0 && c>='0' && c <='9'){
                if (negative)
                    temperaturesInt[i-1] = temperaturesInt[i-1]*10 - (c - '0');
                else
                    temperaturesInt[i-1] = temperaturesInt[i-1]*10 + (c - '0');
            }
            else if (i>0 && temperaturesInt[i-1]==0 && c=='-')
                negative = true;
            else{
                if (c=='#' && i == 2 && write_scratch()){
                    M_WRITEUSART((char *)":T");
                    return true;                        
                }
                return false;
            }
            PIR1bits.RCIF = 0;
        }
    }
    return false;
}

void command_parse(){
    //TODO: for multiple devices
    while(1){
        if (PIR1bits.RCIF){
            bool result = true;
            switch(ReadUSART()){
                case 'S':   //START
                    break;
                    
                case 'T':   //SET TEMPERATURE LIMITS    :T;THigh;TLow#   (Only Integer Values)
                    result = set_temp_limits();
                    break;
                    
                case 'R':   //READ TEMPERATURE LIMITS   :R
                    result = read_temp_limits();
                    break;
                    
                case 'Q':   //STOP
                    break;
                    
                case 'A':   //SINGLE READ
                    result = single_read();
                    break;
                    
                case ':':   //JUST CONTINUE
                    continue;
                    break;
                    
                default:
                    result = false;
                    break;
            }
            PIR1bits.RCIF = 0;
            if (!result){
                M_WRITEUSART((char *)":-");
                break;
            }
            else
                break;
        }
    }
}

void main (void){
    ConfigureInterruptPriority(true);
    ConfigureInterrupt();
    usart_init(9600,false,false);
    busy = false;
    //PERFORM SEARCH DEVICES (ROMS)
    if (ds18b20_search_devices(SEARCH_ROM)==OK){
        while(1){
            if (PIR1bits.RCIF){
                if (ReadUSART() == ':')
                    command_parse();
                PIR1bits.RCIF = 0;
            }
        }
    }
    M_WRITEUSART((char *)":*");
}
