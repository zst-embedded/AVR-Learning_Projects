#ifndef __ZST_LCD_HD44780_LIB__
#define __ZST_LCD_HD44780_LIB__

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
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


// Set up port defines (6 I/O pins: 2 control lines & 4 data lines)
// Data lines must be on the same port
#define LCD_DATA_PORT PCF8574_REGISTER
#define LCD_D4 4
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
// Control lines must be on the same port
#define LCD_CONTROL_PORT PCF8574_REGISTER
#define LCD_RW 1
#define LCD_RS 0
#define LCD_E 2
#define LCD_LED 3

// The following defines are HD44780 controller commands
#define CLEARDISPLAY 0x01
#define SETCURSOR 0x80

#ifdef __cplusplus
extern "C" {
#endif

void LCD_I2C_Push(uint8_t push);
void LCD_Init(void);
void LCD_PulseEnable(void);
void LCD_SendNibble(const uint8_t data);
void LCD_SendByte(uint8_t data);
void LCD_Cmd(const uint8_t cmd);
void LCD_Char(const uint8_t ch);
void LCD_Clear(void);
void LCD_MoveCursor(const uint8_t x, const uint8_t y);
void LCD_Message(const char *text);
void LCD_Integer(int data);
void LCD_Hex(int data);

#ifdef __cplusplus
}
#endif

#endif
