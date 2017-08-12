/* 
 * ATtiny4313
 *
 * Setup HW USART (transmit and receive)
 * Redirect USART to stdin and stdout
 *
 * Toggle LED on PB1 and transmit text
 * after pressing button on PB0.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "zst-avr-usart-lib.h"

int main(void) {
    DDRB |= _BV(PB1); // PB1 (LED) as output
    PORTB |= _BV(PB0); // PB0 input using internal pull-up

    uart_init(); // Setup UART with <util/setbaud.h> for baud rate calculation
    uart_redirect(); // Redirect UART to stdin and out

    uint8_t count = 0;
    while(1) {
    	PORTB ^= _BV(1);
    	_delay_ms(100);
        if ((PINB & _BV(PB0)) == 0) {
            printf("Hello, you pressed the button %d times\n", count);
            count++;
        }
    }
    
    return 0;
}
