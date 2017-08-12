/* 
 * ATmega8515
 *
 * Bitbang SPI to interface with 74HC595
 * 16 bit shiftOut is used because 
 * 2x 74HC595 are daisy chained together
 *
 * PB0 - dataPin  ->  SER  / DS       = 74HC595 PIN 14 (Data in)
 * PB1 - clockPin -> SRCLK / SH_CP    = 74HC595 PIN 11 (Shift register clock)
 * PB2 - latchPin ->  RCLK / ST_CP    = 74HC595 PIN 12 (Latch / Storage clock)
 */

#include <avr/io.h>
#include <util/delay.h>

/*********************************************************************************/
/* MACROS FOR BIT MANIPULATION  */
#define sbi(value, bit) ((value) |= _BV(bit))
#define cbi(value, bit) ((value) &= ~_BV(bit))
#define writeBit(value, bit, bitvalue) (bitvalue ? sbi(value, bit) : cbi(value, bit))
/*********************************************************************************/


void shiftOut16(volatile uint8_t * dataPort, uint8_t dataPin,
              volatile uint8_t * clockPort, uint8_t clockPin,
              uint8_t bitOrder, uint16_t val) {
    for (uint8_t i = 16; i; i--) {
        if (bitOrder) { // MSB_FIRST
            writeBit(*dataPort, dataPin, (val & _BV(15)) );
            val <<= 1;
        } else { // LSB first
            writeBit(*dataPort, dataPin, (val & 1) );
            val >>= 1;
        }
        // Strobe clock
        sbi(*clockPort, clockPin);
        cbi(*clockPort, clockPin);
    }
}

void shiftOutLatch16(volatile uint8_t * dataPort, uint8_t dataPin,
                   volatile uint8_t * clockPort, uint8_t clockPin,
                   volatile uint8_t * latchPort, uint8_t latchPin,
                   uint8_t bitOrder, uint16_t val) {
    cbi(*latchPort, latchPin);
    shiftOut16(dataPort, dataPin, clockPort, clockPin, bitOrder, val);
    sbi(*latchPort, latchPin);
}

int main(void) {
    DDRA |= _BV(0); // Set PA0 output (LED for debugging)
    DDRB |= 0b111; // Set PB[0:2] output (to 74HC595) 

    while(1) {
        for (int x = 0; x < 16; x++) {
            PORTA ^= _BV(0);

            _delay_ms(500);

            shiftOutLatch16(
                    &PORTB, 0,
                    &PORTB, 1,
                    &PORTB, 2,
                    0, 1 << x
                    //1, x
            );
        }
    }
    return 0;
}