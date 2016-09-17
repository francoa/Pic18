#include "user_lcd.h"

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set: 
//    DL = 1; 8-bit interface data 
//    N = 0; 1-line display 
//    F = 0; 5x8 dot character font 
// 3. Display on/off control: 
//    D = 0; Display off 
//    C = 0; Cursor off 
//    B = 0; Blinking off 
// 4. Entry mode set: 
//    I/D = 1; Increment by 1 
//    S = 0; No shift 
//

void lcd_init(bool fourbitmode)
{
    if (fourbitmode)
        _lcd_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    else 
        _lcd_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;

}

void lcd_begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
    
    if (lines > 1) {
        _lcd_displayfunction |= LCD_2LINE;
    }
    _lcd_numlines = lines;

    lcd_setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);  

    RSTRIS = OUTPUT;
    RWTRIS = OUTPUT;
    ETRIS = OUTPUT;
    DTRIS = OUTPUT;

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands.
    __delay_ms(25); 
    __delay_ms(25); 
    
    // Now we pull both RS and R/W low to begin commands
    RSPIN = LOW;
    EPIN = LOW;
    RWPIN = LOW;

    //put the LCD into 4 bit or 8 bit mode
    if (! (_lcd_displayfunction & LCD_8BITMODE)) {
        // this is according to the hitachi HD44780 datasheet
        // figure 24, pg 46

        // we start in 8bit mode, try to set 4 bit mode
        lcd_write4bits(0x03);
        __delay_ms(5); // wait min 4.1ms

        // second try
        lcd_write4bits(0x03);
        __delay_ms(5); // wait min 4.1ms

        // third go!
        lcd_write4bits(0x03); 
        __delay_us(150);

        // finally, set to 4-bit interface
        lcd_write4bits(0x02); 
    } else {
        // this is according to the hitachi HD44780 datasheet
        // page 45 figure 23

        // Send function set command sequence
        lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
        __delay_ms(5);  // wait more than 4.1ms

        // second try
        lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
        __delay_us(150);

        // third go
        lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);
    }

    // finally, set # lines, font size, etc.
    lcd_command(LCD_FUNCTIONSET | _lcd_displayfunction);  

    // turn the display on with no cursor or blinking default
    _lcd_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
    lcd_display();

    // clear it off
    lcd_clear();

    // Initialize to default text direction (for romance languages)
    _lcd_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);

}

void lcd_setRowOffsets(int row0, int row1, int row2, int row3)
{
    _lcd_row_offsets[0] = row0;
    _lcd_row_offsets[1] = row1;
    _lcd_row_offsets[2] = row2;
    _lcd_row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void lcd_clear()
{
    lcd_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    __delay_ms(2);  // this command takes a long time!
}

void lcd_home()
{
    lcd_command(LCD_RETURNHOME);  // set cursor position to zero
    __delay_ms(2);  // this command takes a long time!
}

void lcd_setCursor(uint8_t col, uint8_t row)
{
    const size_t max_lines = sizeof(_lcd_row_offsets) / sizeof(*_lcd_row_offsets);
    if ( row >= max_lines ) {
        row = max_lines - 1;    // we count rows starting w/0
    }
    if ( row >= _lcd_numlines ) {
        row = _lcd_numlines - 1;    // we count rows starting w/0
    }
  
    lcd_command(LCD_SETDDRAMADDR | (col + _lcd_row_offsets[row]));
}

// Turn the display on/off (quickly)
void lcd_noDisplay() {
    _lcd_displaycontrol &= ~LCD_DISPLAYON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_display() {
    _lcd_displaycontrol |= LCD_DISPLAYON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// Turns the underline cursor on/off
void lcd_noCursor() {
    _lcd_displaycontrol &= ~LCD_CURSORON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_cursor() {
    _lcd_displaycontrol |= LCD_CURSORON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// Turn on and off the blinking cursor
void lcd_noBlink() {
    _lcd_displaycontrol &= ~LCD_BLINKON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}
void lcd_blink() {
    _lcd_displaycontrol |= LCD_BLINKON;
    lcd_command(LCD_DISPLAYCONTROL | _lcd_displaycontrol);
}

// These commands scroll the display without changing the RAM
void lcd_scrollDisplayLeft(void) {
    lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcd_scrollDisplayRight(void) {
    lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void lcd_leftToRight(void) {
    _lcd_displaymode |= LCD_ENTRYLEFT;
    lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This is for text that flows Right to Left
void lcd_rightToLeft(void) {
    _lcd_displaymode &= ~LCD_ENTRYLEFT;
    lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This will 'right justify' text from the cursor
void lcd_autoscroll(void) {
    _lcd_displaymode |= LCD_ENTRYSHIFTINCREMENT;
    lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// This will 'left justify' text from the cursor
void lcd_noAutoscroll(void) {
    _lcd_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    lcd_command(LCD_ENTRYMODESET | _lcd_displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void lcd_createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    lcd_command(LCD_SETCGRAMADDR | (location << 3));
    for (int i=0; i<8; i++) {
        lcd_write(charmap[i]);
    }
}

/*********** mid level commands, for sending data/cmds */

void lcd_command(uint8_t value) {
    lcd_send(value, LOW);
}

void lcd_write(uint8_t value) {
    lcd_send(value, HIGH);
}

void lcd_print_char(char c){
    lcd_write(c);
}

void lcd_print(char * string, unsigned char size){
    unsigned char i;
    for (i=0; i<size; i++)
        lcd_write(string[i]);
}

/************ low level data pushing commands **********/

// write either command or data, with automatic 4/8-bit selection
void lcd_send(uint8_t value, uint8_t mode) {
    RSPIN = mode;

    // if there is a RW pin indicated, set it low to Write
    RWPIN = LOW;

    if (_lcd_displayfunction & LCD_8BITMODE) {
        lcd_write8bits(value); 
    } else {
        lcd_write4bits(value>>4);
        lcd_write4bits(value);
    }
}

void lcd_pulseEnable(void) {
    EPIN = LOW;
    __delay_us(1);    
    EPIN = HIGH;
    __delay_us(1);    // enable pulse must be >450ns
    EPIN = LOW;
    __delay_us(100);   // commands need > 37us to settle
}

void lcd_write4bits(uint8_t value) {
    value = value & 0x0F;
    DPIN = value;
    lcd_pulseEnable();
}

void lcd_write8bits(uint8_t value) {
    DPIN = value;
    lcd_pulseEnable();
}