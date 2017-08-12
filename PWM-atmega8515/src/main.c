/* 
 * ATmega8515
 * PWM for LED on PB0/OC0
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= 0x01; // Set PB0 as output

    // Mode 1 - PWM, Phase Correct
    TCCR0 |= _BV(WGM00);

    // Clock select - no prescaler
    TCCR0 |= _BV(CS00);

    // COM01:0 = 0b10
    // Clear OC0 on Compare Match when up-counting.
    // Set OC0 on Compare Match when downcounting.
    TCCR0 |= _BV(COM01);

    while(1) {
        for (uint8_t x = 0; x < 255; x += 10) {
            OCR0 = x;
            _delay_ms(50);
        }
    }
    return 0;
}