#include "zst-i2c-lcd-lib.h"
uint8_t PCF8574_REGISTER = 0<<LCD_RW | 1<<LCD_LED;

void LCD_Init() {
    LCD_Cmd(0x33); // initialize controller
    LCD_Cmd(0x32); // set to 4-bit input mode
    LCD_Cmd(0x28); // 2 line, 5x7 matrix
    LCD_Cmd(0x0C); // turn cursor off (0x0E to enable)
    LCD_Cmd(0x06); // cursor direction = right
    LCD_Cmd(0x01); // start with clear display
    _delay_ms(3); // wait for LCD to initialize
}

void LCD_PulseEnable() {
    sbi(LCD_CONTROL_PORT, LCD_E); // take LCD enable line high
    LCD_I2C_Push(PCF8574_REGISTER);
    _delay_us(40); // wait 40 microseconds
    cbi(LCD_CONTROL_PORT, LCD_E); // take LCD enable line low
    LCD_I2C_Push(PCF8574_REGISTER);
}

void LCD_SendNibble(const uint8_t data) {
    LCD_DATA_PORT &= ~(_BV(LCD_D4) | _BV(LCD_D5) | _BV(LCD_D6) | _BV(LCD_D7));
    // clear data bits
    if (data & _BV(4)) sbi(LCD_DATA_PORT, LCD_D4);
    if (data & _BV(5)) sbi(LCD_DATA_PORT, LCD_D5);
    if (data & _BV(6)) sbi(LCD_DATA_PORT, LCD_D6);
    if (data & _BV(7)) sbi(LCD_DATA_PORT, LCD_D7);
    // set data bits
    LCD_PulseEnable();
    // clock 4 bits into controller
}

void LCD_SendByte(uint8_t data) {
    LCD_SendNibble(data); // send upper 4 bits
    LCD_SendNibble(data<<4); // send lower 4 bits
}

void LCD_Cmd(const uint8_t cmd)
{
    cbi(LCD_CONTROL_PORT, LCD_RS); // R/S line 0 = command data
    LCD_SendByte(cmd); // send it
}

void LCD_Char(const uint8_t ch)
{
    sbi(LCD_CONTROL_PORT, LCD_RS); // R/S line 1 = character data
    LCD_SendByte(ch); // send it
}


void LCD_Clear() {
    LCD_Cmd(CLEARDISPLAY);
    _delay_ms(3); // wait for LCD to process command
};

void LCD_MoveCursor(const uint8_t x, const uint8_t y) // put LCD cursor on specified line
{
    uint8_t addr = 0; // line 0 begins at addr 0x00
    switch (y) {
        case 1: addr = 0x40; break; // line 1 begins at addr 0x40
        case 2: addr = 0x14; break;
        case 3: addr = 0x54; break;
    }
    LCD_Cmd(SETCURSOR+addr+x); // update cursor with x,y position
}

void LCD_Message(const char *text) // display string on LCD
{
    while (*text) // do until /0 character
        LCD_Char(*text++); // send char & update char pointer
}

void LCD_Hex(int data)
// displays the hex value of DATA at current LCD cursor position
{
    char st[8] = ""; // save enough space for result
    itoa(data,st,16); // convert to ascii hex
    //LCD_Message("0x"); // add prefix "0x" if desired
    LCD_Message(st); // display it on LCD
}

void LCD_Integer(int data)
// displays the integer value of DATA at current LCD cursor position
{
    char st[8] = ""; // save enough space for result
    itoa(data,st,10); // convert to ascii
    LCD_Message(st); // display in on LCD
}
