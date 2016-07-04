#include "user_ds18b20.h"

void ds18b20_initialization(){
    compare_setup(TRESETHIGH,0);
    BUS_TAKE();
    BUS_LOW();
    __delay_us(TRESETLOW);
    BUS_RELEASE();
}

void ds18b20_presence(){
    compare_init();
}

void ds18b20_write0(){
    BUS_TAKE();
    BUS_LOW();
    __delay_us(TWLOW0);
    BUS_RELEASE();
    __delay_us(TRECOVERY);
}

void ds18b20_write1(){
    BUS_TAKE();
    BUS_LOW();
    __delay_us(TWLOW1);
    BUS_RELEASE();
    __delay_us(TRECOVERY);
}

BYTE ds18b20_read(){
    __delay_us(TRECOVERY);
    slider = 0;
    WriteUSART('R');
    for (i=0; i<8; i++){
        BUS_TAKE();
        BUS_LOW();
        __delay_us(TRLOW);
        BUS_RELEASE();
        __delay_us(TRWAIT);
        slider |= (BUS_STATE() << i);
        WriteUSART(BUS_STATE()+'0');
        __delay_us(TSLOT-TRSAMPLE);
    }
    return slider;
}

void ds18b20_command(BYTE cmd){
    __delay_us(TRECOVERY);
    slider = 1;
    for(i = 0; i < 8; i++){
        if ((cmd & (slider << i))!=0){
            WriteUSART('1');
            ds18b20_write1();
        }
        else{
            WriteUSART('0');
            ds18b20_write0();
        }
    }
}