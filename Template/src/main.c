#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	DDRC = _BV(0);

    while(1) {
    	PORTC ^= _BV(0);
    	_delay_ms(100);
    }
    return 0;
}
