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

unsigned char ds18b20_read_rom(UINT32 * highBits, UINT32 * lowBits){
    int j;
    if (ds18b20_num_devices == 0)
        return 0;
    else if (ds18b20_num_devices != 1)
        return 1;
    if (ds18b20_initialization() != 0 ){
        return 0;
    }
    ds18b20_write_byte(READ_ROM);

    *highBits = 0;
    *lowBits = 0;
    
    *lowBits = ds18b20_read_byte();
    
    for (j=1; j<4; j++){
        *lowBits |=  (((UINT32)ds18b20_read_byte()) << (8*j));
    }
    
    for (j=0; j<3; j++){
        *highBits |=  (((UINT32)ds18b20_read_byte()) << (8*j));
    }
    
    *highBits |= (((UINT32)ds18b20_read_byte()) << 24);
    
    return 2;
}

unsigned char ds18b20_T_Conversion(void){
    if (ds18b20_num_devices == 0)
        return 0;
    else if (ds18b20_num_devices != 1)
        return 1;
    else{
        if (ds18b20_initialization() != 0 ){
            return 0;
        }
        ds18b20_write_byte(SKIP_ROM);
        ds18b20_write_byte(CONVERT_T);
        while(!ds18b20_read_bit());
    }
    return 2;
}

void ds18b20_T_Conversion_SpecificROM(int device){
    if (ds18b20_initialization() != 0 ){
        return;
    }
    ds18b20_write_byte(MATCH_ROM);
    
    int counter;
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device+1] >> (counter*8)) & 0xFF);
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device] >> (counter*8)) & 0xFF);
    
    ds18b20_write_byte(CONVERT_T);
    while(!ds18b20_read_bit());
}

unsigned char ds18b20_get_scratch(UINT32 * highBits, UINT32 * lowBits, BYTE * crc){
    if (ds18b20_num_devices == 0)
        return 0;
    else if (ds18b20_num_devices != 1)
        return 1;
    else{
        int counter = 0;
        if (ds18b20_initialization() != 0 ){
            return 0;
        }
        ds18b20_write_byte(SKIP_ROM);
        ds18b20_write_byte(READ_SCRATCH);

        *lowBits = 0;
        for (counter=0; counter<4; counter++)
            *lowBits |= (ds18b20_read_byte() << (8*counter));
        
        *highBits = 0;
        for (counter=0; counter<4; counter++)
            *highBits |= (ds18b20_read_byte() << (8*counter));
        
        *crc = ds18b20_read_byte();
    }
    return 2;
}

void ds18b20_get_scratch_SpecificROM(UINT32 * highBits, UINT32 * lowBits, BYTE * crc,int device){
    if (ds18b20_initialization() != 0 ){
        return;
    }
    ds18b20_write_byte(MATCH_ROM);
    
    int counter = 0;
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device+1] >> (counter*8)) & 0xFF);
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device] >> (counter*8)) & 0xFF);
    
    ds18b20_write_byte(READ_SCRATCH);

    *lowBits = 0;
    for (counter=0; counter<4; counter++)
        *lowBits |= (ds18b20_read_byte() << (8*counter));

    *highBits = 0;
    for (counter=0; counter<4; counter++)
        *highBits |= (ds18b20_read_byte() << (8*counter));

    *crc=ds18b20_read_byte();
    
    return;
}

unsigned char ds18b20_read_T(float* temperature){
    if (ds18b20_num_devices == 0)
        return 0;
    else if (ds18b20_num_devices != 1)
        return 1;
    else{
        BYTE scratchPad[9];
        int counter = 0;
        
        if (ds18b20_initialization() != 0 ){
            return 0;
        }
        ds18b20_write_byte(SKIP_ROM);
        ds18b20_write_byte(READ_SCRATCH);

         for (counter=0; counter<9; counter++)
            scratchPad[counter] = ds18b20_read_byte();
        *temperature = (((scratchPad[1] << 8) | scratchPad[0]));
        *temperature /= 16;

        return 2;
    }  
}

void ds18b20_read_T_SpecificROM(float* temperature, int device){
    *temperature = 0;
    BYTE scratchPad[9];
    int counter = 0;

    if (ds18b20_initialization() != 0 ){
        return;
    }
    
    ds18b20_write_byte(MATCH_ROM);
    
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device+1] >> (counter*8)) & 0xFF);
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device] >> (counter*8)) & 0xFF);
    
    ds18b20_write_byte(READ_SCRATCH);

     for (counter=0; counter<9; counter++)
        scratchPad[counter] = ds18b20_read_byte();
    *temperature = (((scratchPad[1] << 8) | scratchPad[0]));
    *temperature /= 16;
}
         
unsigned char ds18b20_write_scratch(int Thigh, int Tlow, UINT8 resolution){
    
    if (ds18b20_num_devices == 0)
        return 0;
    else if (ds18b20_num_devices != 1)
        return 1;
    else{
        if (ds18b20_initialization() != 0)
            return 0;
        
        ds18b20_write_byte(SKIP_ROM);
        ds18b20_write_byte(WRITE_SCRATCH);

        ds18b20_write_values(Thigh,Tlow,resolution);
    }
    return 2;
}

void ds18b20_write_scratch_SpecificROM(int Thigh, int Tlow, UINT8 resolution,int device){
    
    if (ds18b20_initialization() != 0)
        return;
    
    ds18b20_write_byte(MATCH_ROM);
    int counter;
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device+1] >> (counter*8)) & 0xFF);
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device] >> (counter*8)) & 0xFF);

    ds18b20_write_byte(WRITE_SCRATCH);

    ds18b20_write_values(Thigh,Tlow,resolution);

}

void ds18b20_write_values(int Thigh, int Tlow, UINT8 resolution){
    BYTE Th = 0b00000000; // Sign,64,32,16,8,4,2,1
    BYTE Tl = 0b00000000;
    BYTE Config = 0b00011111; //0,R1,R0,1,1,1,1,1
    // TODO CORRECT
    if (Thigh <= TEMP_MAX && Thigh >= TEMP_MIN){
        while(!TRMT);
        Th = (BYTE)Thigh;
        if (Thigh >= 0)
            Th = Th & 0x7F;
        else
            Th = Th | 0x80;
    }

    ds18b20_write_byte(Th);
    if (Tlow <= TEMP_MAX && Tlow >= TEMP_MIN){
        while(!TRMT);
        Tl = (BYTE)Tlow;
        if (Tlow >= 0)
            Tl = Tl & 0x7F;
        else
            Tl = Tl | 0x80;
    }

    ds18b20_write_byte(Tl);
    if (resolution <=3){
        ds18b20_write_byte(Config | (resolution << 5));
    }
    else
        ds18b20_write_byte(Config | (RES_DEFAULT << 5));
    
    __delay_us(10);
}

unsigned char ds18b20_recall_e2(void){
    if (ds18b20_num_devices == 0)
        return 0;
    else if (ds18b20_num_devices != 1)
        return 1;
    else{
        
        if (ds18b20_initialization() != 0 ){
            return 0;
        }
        ds18b20_write_byte(SKIP_ROM);
        ds18b20_write_byte(RECALL_E2);
        while(!ds18b20_read_bit());
        
        return 2;
    }   
}

void ds18b20_recall_e2_SpecificROM(int device){
    if (ds18b20_initialization() != 0 ){
        return;
    }
    
    ds18b20_write_byte(MATCH_ROM);
    int counter;
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device+1] >> (counter*8)) & 0xFF);
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device] >> (counter*8)) & 0xFF);
    
    ds18b20_write_byte(RECALL_E2);
    while(!ds18b20_read_bit());
}

unsigned char ds18b20_copy_e2(void){
    if (ds18b20_num_devices == 0)
        return 0;
    else if (ds18b20_num_devices != 1)
        return 1;
    else{
        if (ds18b20_initialization() != 0)
            return 0;
        
        ds18b20_write_byte(SKIP_ROM);
        ds18b20_write_byte(COPY_SCRATCH);

        while(!ds18b20_read_bit());
    }
    return 2;
}

void ds18b20_copy_e2_SpecificROM(int device){
    if (ds18b20_initialization() != 0)
        return;
    int counter;
    
    ds18b20_write_byte(MATCH_ROM);
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device+1] >> (counter*8)) & 0xFF);
    for (counter=0; counter<4; counter++)
        ds18b20_write_byte((ds18b20_devices[2*device] >> (counter*8)) & 0xFF);

    ds18b20_write_byte(COPY_SCRATCH);

    while(!ds18b20_read_bit());
}

int ds18b20_search_devices(BYTE cmd){   
    
    UINT32 bit_value;
    UINT32 rom_value_High;
    UINT32 rom_value_Low;
    int counter;
    unsigned char current_bit,complement_bit;
    UINT32 list[2*LIST_NUM];
    UINT count_list[2*LIST_NUM];
    int num_elements = 0;
    bool first = true;
    
    if (cmd == SEARCH_ROM){
        for (counter=0; counter < 2*ROM_NUM; counter++)
            ds18b20_devices[counter] = 0;
        ds18b20_num_devices = 0;
    }
    else if (cmd == ALARM_SEARCH){
        for (counter=0; counter < 2*ROM_NUM; counter++)
            ds18b20_alarms[counter] = 0;
        ds18b20_num_alarms = 0;
    }
    else{
        return 0;
    }
    
    while((first || num_elements > 0) && ds18b20_num_devices < ROM_NUM && ds18b20_num_alarms < ROM_NUM){
        bool erase = false;
        bit_value = 1;
        if (ds18b20_initialization() != 0)
            return 0;
        ds18b20_write_byte(cmd);
    
        if (!first){
            num_elements--;
            rom_value_High = list[2*num_elements];
            rom_value_Low = list[2*num_elements+1];
            if (count_list[num_elements] >= 32){
                for (counter = 0; counter < 32; counter++){
                    ds18b20_read_bit();
                    ds18b20_read_bit();
                    ds18b20_write_bit((unsigned char)((rom_value_Low >> counter) & 0x01));
                }
                for (counter; counter < count_list[num_elements]; counter++){
                    ds18b20_read_bit();
                    ds18b20_read_bit();
                    bit_value *= 2;
                    ds18b20_write_bit((unsigned char)((rom_value_High >> (counter-32)) & 0x01));
                }
            }
            else{
                for (counter = 0; counter < count_list[num_elements]; counter++){
                    ds18b20_read_bit();
                    ds18b20_read_bit();
                    bit_value *= 2;
                    ds18b20_write_bit((unsigned char)((rom_value_Low >> counter) & 0x01));
                }
                erase = true;
            }
        }
        else{
            first = false;
            counter = 0;
            rom_value_Low = 0;
            rom_value_High = 0;
            erase = true;
        }
        
        while(counter < 32){
            current_bit = ds18b20_read_bit();
            complement_bit = ds18b20_read_bit();
            if (current_bit != complement_bit){
                rom_value_Low += bit_value*current_bit;        
            }
            else if (current_bit == 0){
                if (num_elements < LIST_NUM){
                    list[2*num_elements+1] = rom_value_Low+bit_value;
                    list[2*num_elements] = 0;
                    count_list[num_elements] = counter+1;
                    num_elements++;
                }
            }
            else
                return 0;
            bit_value *= 2;
            ds18b20_write_bit(current_bit);
            counter++;
        }
        if (erase){
            bit_value = 1;
            rom_value_High = 0;
        }
        while(counter < 63){
            current_bit = ds18b20_read_bit();
            complement_bit = ds18b20_read_bit();
            if (current_bit != complement_bit){
                rom_value_High += bit_value*current_bit;        
            }
            else if (current_bit == 0){
                if (num_elements < LIST_NUM){
                    list[2*num_elements+1] = rom_value_Low;
                    list[2*num_elements] = rom_value_High+bit_value;
                    count_list[num_elements] = counter+1;
                    num_elements++;
                }
            }
            else
                return 0;
            bit_value *= 2;
            ds18b20_write_bit(current_bit);
            counter++;
        }
        
        current_bit = ds18b20_read_bit();
        complement_bit = ds18b20_read_bit();
        if (current_bit != complement_bit){
            if (cmd == SEARCH_ROM){
                ds18b20_devices[2*ds18b20_num_devices]=rom_value_High + bit_value*current_bit;
                ds18b20_devices[2*ds18b20_num_devices+1]=rom_value_Low;
                ds18b20_num_devices++;
            }
            else if (cmd == ALARM_SEARCH){
                ds18b20_alarms[2*ds18b20_num_alarms]=rom_value_High + bit_value*current_bit;
                ds18b20_alarms[2*ds18b20_num_alarms+1]=rom_value_Low;
                ds18b20_num_alarms++;
            }
        }
        else if (current_bit == 0){
            if (cmd == SEARCH_ROM){
                ds18b20_devices[2*ds18b20_num_devices]=rom_value_High + bit_value*current_bit;
                ds18b20_devices[2*ds18b20_num_devices+1]=rom_value_Low;
                ds18b20_num_devices++;
                if(ds18b20_num_devices<ROM_NUM){
                    ds18b20_devices[2*ds18b20_num_devices]=rom_value_High;
                    ds18b20_devices[2*ds18b20_num_devices+1]=rom_value_Low;
                    ds18b20_num_devices++;
                }
            }
            else if (cmd == ALARM_SEARCH){
                ds18b20_alarms[2*ds18b20_num_alarms]=rom_value_High + bit_value*current_bit;
                ds18b20_alarms[2*ds18b20_num_alarms+1]=rom_value_Low;
                ds18b20_num_alarms++;
                if(ds18b20_num_alarms<ROM_NUM){
                    ds18b20_alarms[2*ds18b20_num_alarms]=rom_value_High;
                ds18b20_alarms[2*ds18b20_num_alarms+1]=rom_value_Low;
                    ds18b20_num_alarms++;
                }
            }
        }
    }
    
    if (cmd == SEARCH_ROM)
        return ds18b20_num_devices;
    else if (cmd == ALARM_SEARCH)
        return ds18b20_num_alarms;
    return 0;
}

unsigned char ds18b20_rom_crc(UINT32 highBits,UINT32 lowBits){
    int i;
    unsigned char dowcrc;
    ds18b20_crc_shiftReg_init();
    for (i=0; i<32; i++){
        ds18b20_crc_shiftReg_add(lowBits & 0x00000001);
        lowBits >>= 1;
    }
    for (i=0; i<24; i++){
        ds18b20_crc_shiftReg_add(highBits & 0x00000001);
        highBits >>= 1;
    }
    unsigned char temp = 1;
    dowcrc = 0;
    for (i=0; i<8; i++){
        dowcrc += shiftRegister[i]*temp;
        temp *= 2;
    }
    return dowcrc;
}

unsigned char ds18b20_scratchpad_crc(UINT32 highBits ,UINT32 lowBits){
    int i;
    unsigned char dowcrc;
    ds18b20_crc_shiftReg_init();
    for (i=0; i<32; i++){
        ds18b20_crc_shiftReg_add(lowBits & 0x00000001);
        lowBits >>= 1;
    }
    for (i=0; i<32; i++){
        ds18b20_crc_shiftReg_add(highBits & 0x00000001);
        highBits >>= 1;
    }
    unsigned char temp = 1;
    dowcrc = 0;
    for (i=0; i<8; i++){
        dowcrc += shiftRegister[i]*temp;
        temp *= 2;
    }
    return dowcrc;
}

void ds18b20_crc_shiftReg_init(void){
    int i;
    for (i=0; i<8; i++)
        shiftRegister[i] = 0;
}

void ds18b20_crc_shiftReg_add(unsigned char x){
    unsigned char input = x ^ shiftRegister[0];
    shiftRegister[0] = shiftRegister[1];
    shiftRegister[1] = shiftRegister[2];
    shiftRegister[2] = input ^ shiftRegister[3];
    shiftRegister[3] = input ^ shiftRegister[4];
    shiftRegister[4] = shiftRegister[5];
    shiftRegister[5] = shiftRegister[6];
    shiftRegister[6] = shiftRegister[7];
    shiftRegister[7] = input;
}