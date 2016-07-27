#include "user_ds18b20.h"

void ds18b20_initialization(){
    compare_setup(TPRESENCE,0);    // Set Timer1 in compare mode
    BUS_TAKE();                     // Take Bus
    BUS_LOW();                      // Drive Bus low
    __delay_us(TRESET);          // Delay 500us
    BUS_RELEASE();                  // Release Bus
    __delay_us(TRESETRESISTOR);
}

void ds18b20_presence(){
    compare_init();                 // Start Timer1
}

void ds18b20_write(unsigned char write_bit){
    if (write_bit){
        BUS_TAKE();                         // Take Bus
        BUS_LOW();                          // Drive Bus low
        __delay_us(TWSAMPLE);                 // Delay 5us
        BUS_HIGH();
        __delay_us(TWSLOT-TWSAMPLE);
        BUS_RELEASE();                      // Release Bus
        __delay_us(TWRECOV); // Delay 75us       
    }
    else{
        BUS_TAKE();                         // Take Bus
        BUS_LOW();                          // Drive Bus low
        __delay_us(TWSLOT);                 // Delay 80us
        BUS_RELEASE();                      // Release Bus
        __delay_us(TWRECOV); // Delay 5us
    }
}

void ds18b20_command(BYTE cmd){
    int counter = 0;
    for(counter = 0; counter < 8; counter++){
        ds18b20_write(cmd & 0x01);
        cmd >>= 1;
    }
}

unsigned char ds18b20_read_bit(){
    unsigned char ds18b20_bit;
    BUS_TAKE();                     // Take Bus
    BUS_LOW();                      // Drive Bus low
    __delay_us(TRLOW);              // Delay 6us
    BUS_RELEASE();                  // Release Bus
    __delay_us(TRSAMPLE);           // Delay 8us
    ds18b20_bit = BUS_STATE();   // Sample
    __delay_us(TRSLOT+TRRECOV-TRSAMPLE-TRLOW);   // Delay 70us
    return ds18b20_bit;
}

BYTE ds18b20_read_byte(){
    BYTE slider = 0x00;
    int counter = 0;
    for (counter=0; counter<8; counter++){
        slider |= (ds18b20_read_bit() << counter);
    }
    return slider;
}

BYTE ds18b20_read_byte_raw(){
    BYTE slider = 0x00;
    int counter = 0;
    for (counter=7; counter>=0; counter--){
        slider |= (ds18b20_read_bit() << counter);
    }
    return slider;
}

char * ds18b20_read_T(void){
    int counter = 0;
    BYTE scratchPad[9];
    float temperature;
    
    for (counter=0; counter<9; counter++)
        scratchPad[counter] = ds18b20_read_byte();
    
    temperature = (((scratchPad[1] << 8) | scratchPad[0]));
    temperature /= 16;
    sprintf(ds18b20_temp_str,"%.4f",temperature); // add sign bit        
     
    return ds18b20_temp_str;
}
           