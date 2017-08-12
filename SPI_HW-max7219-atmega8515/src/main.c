/* 
 * ATmega8515
 *
 * Use HW SPI to interface with MAX7219.
 * (SS) PB4
 * (MOSI) PB5
 * (MISO) PB6
 * (SCK) PB7
 */

#include <avr/io.h>
#include <util/delay.h>

#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define DD_MOSI PB5
#define DD_SCK PB7
#define DD_SS PB4

#define MAX7219_MODE_DECODE       0x09
#define MAX7219_MODE_INTENSITY    0x0A
#define MAX7219_MODE_SCAN_LIMIT   0x0B
#define MAX7219_MODE_POWER        0x0C
#define MAX7219_MODE_TEST         0x0F
#define MAX7219_MODE_NOOP         0x00

void SPI_MasterInit(void) {
    /* Set MOSI and SCK output, all others input */
    DDR_SPI |= (1<<DD_MOSI)|(1<<DD_SCK);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(uint8_t data) {
    /* Start transmission */
    SPDR = data;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));
}

void max7219_shift2bytes(uint8_t address, uint8_t data) {
    PORT_SPI &= ~_BV(DD_SS); // Set SS to Low
    SPI_MasterTransmit(address);
    SPI_MasterTransmit(data);
    PORT_SPI |= _BV(DD_SS); // Set SS to high to latch data
}

int main(void) {
    // Setup SPI
    DDR_SPI |= _BV(DD_SS);
    SPI_MasterInit();

    // Test display for 1 sec
    _delay_ms(1000);
    max7219_shift2bytes(MAX7219_MODE_TEST, 1);
    _delay_ms(1000);
    max7219_shift2bytes(MAX7219_MODE_TEST, 0);

    max7219_shift2bytes(MAX7219_MODE_SCAN_LIMIT, 7);
    max7219_shift2bytes(MAX7219_MODE_INTENSITY, 0xF);
    max7219_shift2bytes(MAX7219_MODE_POWER, 0x1);

    int8_t i, j, k;
    while (1) {
        /* Test every dot one by one, varying intensity */
        for(k = 0; k < 0x10; k+=2) {
            max7219_shift2bytes(MAX7219_MODE_INTENSITY, k);
            max7219_shift2bytes(j, 0);

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
    }

    return 0;
}