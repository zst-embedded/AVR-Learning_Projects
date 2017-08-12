/* 
 * ATtiny84
 *
 * Bitbang SPI to interface with MAX7219.
 * Shift out address byte first, then data byte.
 */

#include <avr/io.h>
#include <util/delay.h>

//https://gist.github.com/adnbr/2352797

#define MAX7219_DDR     DDRA
#define MAX7219_PORT    PORTA
#define MAX7219_DIN     _BV(PA0) // Data Pin
#define MAX7219_LOAD    _BV(PA1) // Latch Pin
#define MAX7219_CLK     _BV(PA2) // Clock Pin

#define MAX7219_LOAD_LOW()  (MAX7219_PORT &= ~MAX7219_LOAD);
#define MAX7219_LOAD_HIGH() (MAX7219_PORT |= MAX7219_LOAD);
#define MAX7219_CLK_LOW()   (MAX7219_PORT &= ~MAX7219_CLK);
#define MAX7219_CLK_HIGH()  (MAX7219_PORT |= MAX7219_CLK);

#define MAX7219_MODE_DECODE       0x09
#define MAX7219_MODE_INTENSITY    0x0A
#define MAX7219_MODE_SCAN_LIMIT   0x0B
#define MAX7219_MODE_POWER        0x0C
#define MAX7219_MODE_TEST         0x0F
#define MAX7219_MODE_NOOP         0x00

void max7219_setup() {
    MAX7219_DDR |= MAX7219_DIN | MAX7219_LOAD | MAX7219_CLK;
    MAX7219_PORT &= ~(MAX7219_DIN | MAX7219_LOAD | MAX7219_CLK);
}

void max7219_shift2bytes(uint8_t address, uint8_t data) {
    int8_t bit;
    MAX7219_LOAD_LOW(); // Set load to Low

    for (bit = 7; bit >=0; bit--) {
        MAX7219_CLK_LOW();
        if (address & _BV(7)) // Send MSB first
            MAX7219_PORT |= MAX7219_DIN;
        else
            MAX7219_PORT &= ~MAX7219_DIN;
        address <<= 1;
        MAX7219_CLK_HIGH(); // On CLK’s rising edge, data is shifted
    }
    for (bit = 7; bit >=0; bit--) {
        MAX7219_CLK_LOW();
        if (data & _BV(7)) // Send MSB first
            MAX7219_PORT |= MAX7219_DIN;
        else
            MAX7219_PORT &= ~MAX7219_DIN;
        data <<= 1;
        MAX7219_CLK_HIGH(); // On CLK’s rising edge, data is shifted
    }
    MAX7219_LOAD_HIGH(); // Set load to high to latch data
}

int main(void) {
    max7219_setup();

    // Test display for 1 sec
    _delay_ms(1000);
    max7219_shift2bytes(MAX7219_MODE_TEST, 1);
    _delay_ms(1000);
    max7219_shift2bytes(MAX7219_MODE_TEST, 0);

    max7219_shift2bytes(MAX7219_MODE_SCAN_LIMIT, 7);
    max7219_shift2bytes(MAX7219_MODE_INTENSITY, 0xF);
    max7219_shift2bytes(MAX7219_MODE_POWER, 0x1);

    DDRA |= _BV(5);
    PORTA &= ~_BV(5);

    int8_t i, j;
    while (1) {
        max7219_shift2bytes(j, 0);

        /* Test every dot one by one */
        for (i = 0; i < 8; i++) {
            for (j = 1; j <= 0x8; j++) {
                max7219_shift2bytes(j, _BV(i));
            }
            _delay_ms(30);
        }

        for (i = 7; i >= 0; i--) {
            for (j = 1; j <= 0x8; j++) {
                max7219_shift2bytes(j, _BV(i));
            }
            _delay_ms(30);
        }
    }

    return 0;
}