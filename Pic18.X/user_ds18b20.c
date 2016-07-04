#include "user_ds18b20.h"

void ds18b20_initialization(){
    compare_setup(TRESETHIGH,0);    // Set Timer1 in compare mode
    BUS_TAKE();                     // Take Bus
    BUS_LOW();                      // Drive Bus low
    __delay_us(TRESETLOW);          // Delay 500us
    BUS_RELEASE();                  // Release Bus
}

void ds18b20_presence(){
    compare_init();                 // Start Timer1
}

void ds18b20_write(unsigned char write_bit){
    if (write_bit){
        BUS_TAKE();                         // Take Bus
        BUS_LOW();                          // Drive Bus low
        __delay_us(TWLOW1);                 // Delay 5us
        BUS_RELEASE();                      // Release Bus
        __delay_us(TSLOT+TRECOVERY-TWLOW1); // Delay 75us       
    }
    else{
        BUS_TAKE();                         // Take Bus
        BUS_LOW();                          // Drive Bus low
        __delay_us(TWLOW0);                 // Delay 80us
        BUS_RELEASE();                      // Release Bus
        __delay_us(TSLOT+TRECOVERY-TWLOW0); // Delay 5us
    }
}

void ds18b20_command(BYTE cmd){
    ds18b20_slider = 1;
    for(ds18b20_counter = 0; ds18b20_counter < 8; ds18b20_counter++){
        ds18b20_write(cmd & 0x01);
        cmd >>= 1;
    }
}

unsigned char ds18b20_read_bit(){
    BUS_TAKE();                     // Take Bus
    BUS_LOW();                      // Drive Bus low
    __delay_us(TRLOW);              // Delay 6us
    BUS_RELEASE();                  // Release Bus
    __delay_us(TRSAMPLE-TRLOW-1);   // Delay 8us
    ds18b20_slider = BUS_STATE();   // Sample
    __delay_us(TSLOT+TRECOVERY-TRSAMPLE);   // Delay 70us
    return ds18b20_slider;
}

BYTE ds18b20_read_byte(){
    ds18b20_slider = 0x00;
    for (ds18b20_counter=0; ds18b20_counter<8; ds18b20_counter++){
        BUS_TAKE();                     // Take Bus
        BUS_LOW();                      // Drive Bus low
        __delay_us(TRLOW);              // Delay 6us
        BUS_RELEASE();                  // Release Bus
        __delay_us(TRSAMPLE-TRLOW-1);   // Delay 8us
        ds18b20_slider |= (BUS_STATE() << ds18b20_counter);     // Sample
        __delay_us(TSLOT+TRECOVERY-TRSAMPLE);
    }
    return ds18b20_slider;
}