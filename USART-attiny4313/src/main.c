/* 
 * ATtiny4313
 *
 * Setup HW USART (transmit and receive)
 * Upon receiving 't', toggle LED on PD6
 *
 * Create custom `putchar`, `puts`, and
 * `printf` functions for USART.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdarg.h>

#define UART_BAUD (F_CPU/(16UL*BAUD) - 1)

void USART_Init() {
    /* Set baud rate */
    UBRRH = (UART_BAUD>>8);
    UBRRL = UART_BAUD;
    /* Enable receiver and transmitter and receiving interrupt */
    UCSRB |= (1<<RXCIE) | (1<<RXEN) | (1<<TXEN);
    /* Frame format: 8data, No parity, 1 stop bit */
    UCSRC |= (1<<UCSZ0) | (1<<UCSZ1);
    //UCSRC |= (1<<UMSEL0) | (1<<UMSEL1);
}


uint8_t usart_getch() {
    /* Wait for data to be received */
    while ( !(UCSRA & (1<<RXC)) );
    /* Get and return received data from buffer */
    return UDR;
}


void usart_putch(const uint8_t data) {
    /* Wait for empty transmit buffer */
    while ( !( UCSRA & (1<<UDRE)) );
    /* Put data into buffer, sends the data */
    UDR = data;
}

void usart_puts(char * s) {
    while (*s)
        usart_putch(*s++);
}

void usart_printf(const char * format, ...) {
    va_list arg_list;
    va_start(arg_list, format);
    char s[64];
    vsprintf(s, format, arg_list);
    usart_puts(s);
    va_end(arg_list);
}

int main(void) {
    DDRD |= _BV(6);

    USART_Init();

    sei();

    uint8_t count = 0;
    while (1) {
        _delay_ms(500);
        usart_puts(PSTR("Hello\n"));
        usart_printf(PSTR("We have looped %d times.\n"), count++);
    }
}


ISR(USART_RX_vect) {
    char c = usart_getch();
    if (c == 't') {
        PORTD ^= _BV(6);
    }
    usart_putch(c);
}