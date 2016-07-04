#include "user_ds18b20.h"

void ds18b20_initialization(){
    BUS_TAKE();
    BUS_LOW();
    __delay_us(500);
    BUS_RELEASE();
}