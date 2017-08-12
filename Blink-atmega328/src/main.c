/* 
 * ATmega328P
 * Toggle LED on PC1 every 100ms
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRC = _BV(1); // Set PC1 direction to output
    while(1){
        PORTC ^= _BV(1); // Toggle PC1
        _delay_ms(100);
    }
    return 0; // Never reached
}
