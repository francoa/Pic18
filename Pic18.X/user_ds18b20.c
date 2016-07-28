#include "user_ds18b20.h"

void ds18b20_write_bit(unsigned char write_bit){
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

void ds18b20_write_byte(BYTE cmd){
    int counter = 0;
    for(counter = 0; counter < 8; counter++){
        ds18b20_write_bit(cmd & 0x01);
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

unsigned char ds18b20_initialization(void){
    unsigned char presence;
    BUS_TAKE();                     // Take Bus
    BUS_LOW();                      // Drive Bus low
    __delay_us(TRESET);          // Delay 500us
    BUS_RELEASE();                  // Release Bus
    __delay_us(TRESETSAMPLE);
    presence = BUS_STATE();
    __delay_us(TPRESENCE+TRESETRESISTOR-TRESETSAMPLE);
    return presence;
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
         
void ds18b20_write_scratch(int Thigh, int Tlow, UINT8 resolution){
    BYTE Th = 0b00000000; // Sign,64,32,16,8,4,2,1
    BYTE Tl = 0b00000000;
    BYTE Config = 0b00011111; //0,R1,R0,1,1,1,1,1
    
    if (Thigh <= TEMP_MAX && Thigh >= TEMP_MIN){
        Th = Thigh;            
    }
    ds18b20_write_byte(Th);
    if (Tlow <= TEMP_MAX && Tlow >= TEMP_MIN){
        Tl = Tlow;
    }
    ds18b20_write_byte(Tl);
    if (resolution <=3){
        ds18b20_write_byte(Config | (resolution << 5));
    }
    else
        ds18b20_write_byte(Config | (RES_DEFAULT << 5));
}

int ds18b20_search_roms(void){   
    
    UINT64 bit_value;
    UINT64 rom_value;
    int counter;
    unsigned char current_bit,complement_bit;
    //struct ds18b20_device * linkedList;
    //struct ds18b20_device * temp;
    UINT64 list[LIST_NUM];
    UINT count_list[LIST_NUM];
    int num_elements = 0;
    bool first = true;
    
    //deleteAll(ds18b20_devices);
    for (counter=0; counter < ROM_NUM; counter++)
        ds18b20_devices[counter] = 0;
    ds18b20_num_devices = 0;
    
    while((first || num_elements > 0) && ds18b20_num_devices < ROM_NUM){
        bit_value = 1;
        if (ds18b20_initialization() != 0)
            return 0;
        ds18b20_write_byte(SEARCH_ROM);
    
        if (!first){
            num_elements--;
            rom_value = list[num_elements];
            for (counter = 0; counter <= count_list[num_elements]; counter++){
                ds18b20_read_bit();
                ds18b20_read_bit();
                bit_value *= 2;
                ds18b20_write_bit((unsigned char)((rom_value >> counter) & 0x0000000000000001));
            }
        }
        else{
            first = false;
            counter = 0;
            rom_value = 0;
        }
        
        for (counter = counter; counter < 63; counter++){
            current_bit = ds18b20_read_bit();
            complement_bit = ds18b20_read_bit();
            if (current_bit != complement_bit){
                rom_value += bit_value*current_bit;        
            }
            else if (current_bit == 0){
                if (num_elements < LIST_NUM){
                    list[num_elements] = rom_value+bit_value;
                    count_list[num_elements] = counter;
                    num_elements++;
                }
                
                /*temp = searchAndPopElementByRom(rom_value+sum,linkedList);
                if (temp != NULL){
                    current_bit = 1;
                    linkedList = temp;
                }
                else{
                    num_elements++;
                    linkedList = addElement(rom_value+sum,linkedList,num_elements);
                }*/
                
            }
            else
                return 0;
            bit_value *= 2;
            ds18b20_write_bit(current_bit);
        }
        
        current_bit = ds18b20_read_bit();
        complement_bit = ds18b20_read_bit();
        if (current_bit != complement_bit){
            ds18b20_devices[ds18b20_num_devices]=rom_value + bit_value*current_bit;
            ds18b20_num_devices++;
        }
        else if (current_bit == 0){
            ds18b20_devices[ds18b20_num_devices]=rom_value + bit_value*current_bit;
            ds18b20_num_devices++;
            if(ds18b20_num_devices<ROM_NUM){
                ds18b20_devices[ds18b20_num_devices]=rom_value;
                ds18b20_num_devices++;
            }
        }
        //ds18b20_devices = addElement(rom_value,ds18b20_devices,ds18b20_num_devices);
    }
    //deleteAll(linkedList);
    return ds18b20_num_devices;
}