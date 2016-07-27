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
    __delay_us(TRSAMPLE);           // Delay 8us
    ds18b20_bit = BUS_STATE();   // Sample
    __delay_us(TRSLOT+TRRECOV-TRSAMPLE-TRLOW);   // Delay 70us
    return ds18b20_bit;
}

BYTE ds18b20_read_byte(){
    ds18b20_slider = 0x00;
    for (ds18b20_counter2=0; ds18b20_counter2<8; ds18b20_counter2++){
        ds18b20_slider |= (ds18b20_read_bit() << ds18b20_counter2);
    }
    return ds18b20_slider;
}

BYTE ds18b20_read_byte_raw(){
    ds18b20_slider = 0x00;
    for (ds18b20_counter2=7; ds18b20_counter2>=0; ds18b20_counter2--){
        ds18b20_slider |= (ds18b20_read_bit() << ds18b20_counter2);
    }
    return ds18b20_slider;
}

char * ds18b20_read_T(void){
    for (ds18b20_counter=0; ds18b20_counter<9; ds18b20_counter++)
        ds18b20_array[ds18b20_counter] = ds18b20_read_byte();
    
    
    
    //ds18b20_temp_lsb = (((ds18b20_temp_lsb >> 4) | (ds18b20_temp_msb << 4)) & 0x7F); //dont care about decimals
    //ds18b20_temp_msb = ds18b20_temp_msb & 0x80; //sign bit: 0 for positive, 1 for negative
    
    //if ((ds18b20_array[1] & 0x80) == 0x80) {
        ds18b20_temperature = (((ds18b20_array[1] << 8) | ds18b20_array[0]));
        ds18b20_temperature /= 16;
        sprintf(ds18b20_temp_str,"%.4f",ds18b20_temperature);/*} // add sign bit
    else{
        ds18b20_temperature = ((ds18b20_array[1] << 8) | ds18b20_array[0]);
        ds18b20_temperature /= 16;
        sprintf(ds18b20_temp_str,"%.4f",ds18b20_temperature);
    }*/
        
     
    return ds18b20_temp_str;
}
           