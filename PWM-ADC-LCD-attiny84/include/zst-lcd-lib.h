/* ----------------------------------
 * PIN CONNECTIONS FOR HD44780 displays
 * ----------------------------------
 *
 * (01) Vss - GND
 * (02) Vdd - 5V
 * (03) Vee - Contrast (Max LOW)
 * (04) RS  - ?
 * (05) R/W - Write (Active LOW)
 * (06) En  - ?
 * (07) DB0
 * (08) DB1
 * (09) DB2
 * (10) DB3
 * (11) DB4 - ?
 * (12) DB5 - ?
 * (13) DB6 - ?
 * (14) DB7 - ?
 * (15) LED (+)
 * (16) LED (R-)
 * (17) LED (G-)
 * (18) LED (B-)
 */

/* ----------------------------------
 * HD44780-LCD DRIVER ROUTINES
 * ----------------------------------
 *
 *  - LCD_Init initializes the LCD controller
 *  - LCD_Cmd sends LCD controller command
 *  - LCD_Clear clears the LCD display & homes cursor
 *  - LCD_MoveCursor puts cursor at position (x,y)
 *
 *  - LCD_Integer displays an integer value
 *  - LCD_Char sends single ascii character to display
 *  - LCD_Message displays a string
*/



#ifndef __ZST_LCD_HD44780_LIB__
#define __ZST_LCD_HD44780__LIB__

// Macros for bit manipulation
#ifndef _BV
    #define _BV(x) (1UL << (x))
#endif

#ifndef sbi
    #define sbi(value, bit) ((value) |= _BV(bit))
#endif

#ifndef cbi
    #define cbi(value, bit) ((value) &= ~_BV(bit))
#endif


// Set up port defines (6 I/O pins: 2 control lines & 4 data lines)
// Data lines must be on the same port
#define LCD_DATA_PORT PORTA
#define LCD_D4 PA0
#define LCD_D5 PA1
#define LCD_D6 PA2
#define LCD_D7 PA3
// Control lines must be on the same port
#define LCD_CONTROL_PORT PORTB
#define LCD_RS PB0
#define LCD_E PB1

// The following defines are HD44780 controller commands
#define CLEARDISPLAY 0x01
#define SETCURSOR 0x80

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
    _delay_us(40); // wait 40 microseconds
    cbi(LCD_CONTROL_PORT, LCD_E); // take LCD enable line low
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

/*void LCD_Hex(int data)
// displays the hex value of DATA at current LCD cursor position
{
    char st[8] = ""; // save enough space for result
    itoa(data,st,16); // convert to ascii hex
    //LCD_Message("0x"); // add prefix "0x" if desired
    LCD_Message(st); // display it on LCD
}
*/
void LCD_Integer(int data)
// displays the integer value of DATA at current LCD cursor position
{
    char st[8] = ""; // save enough space for result
    itoa(data,st,10); // convert to ascii
    LCD_Message(st); // display in on LCD
}


#endif
