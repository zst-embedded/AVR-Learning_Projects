/* 
 * ATtiny84
 *
 * Use the USI module on tinyAVR for SPI
 * to interface with MAX7219.
 *
 * (SS, manual) PA0
 * (DI / MOSI)  PA6
 * (DO / MISO)  PA5
 * (USCK / SCK) PA4
 *
 * There are 2 functions - rotatingLine and
 * movingRow for visual effects on the display.
 */

#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DDR_SPI DDRA
#define PORT_SPI PORTA
#define DD_DI PA6      // aka MOSI
#define DD_DO PA5       // aka MISO
#define DD_USCK PA4     // aka CLK

#define MAX7219_MODE_DECODE       0x09
#define MAX7219_MODE_INTENSITY    0x0A
#define MAX7219_MODE_SCAN_LIMIT   0x0B
#define MAX7219_MODE_POWER        0x0C
#define MAX7219_MODE_TEST         0x0F
#define MAX7219_MODE_NOOP         0x00

void movingRow(void);
void rotatingLine(int deg);

void USI_SPI_Init(void) {
    DDR_SPI |= (1<<DD_DO) | (1<<DD_USCK);
    DDR_SPI &= ~_BV(DD_DI); // as input (DI) - data in
}

uint8_t USI_SPI_Transmit(uint8_t cData) {
    USIDR = cData; // Put data into register
    USISR |= _BV(USIOIF); // Clear Counter Overflow Interrupt Flag flag by writing 1
    // USIOIF: used to determine when a transfer is completed, incremented every USCK pulse

    /* Data transferred MSB first */
    while ( (USISR & _BV(USIOIF)) == 0 ) { // while counter not overflowed, meaning transfer not complete
        USICR = (1 << USIWM0) | // 3 wire mode
                (1 << USICS1) | // Produce an External, positive edge Clock Source on USCK
                (1 << USICLK) | // Software clock strobe (Controlled by USITC)
                (1 << USITC); // Write 1 to toggle/pulse USCK
    }
    return USIDR;
}

inline uint8_t USI_SPI_Read(void) {
    return USIBR;
}

void max7219_shift2bytes(uint8_t address, uint8_t data) {
    PORTA &= ~_BV(PA0);
    USI_SPI_Transmit(address);
    USI_SPI_Transmit(data);
    PORTA |= _BV(PA0);
}

int main(void) {
    // Setup SPI
    DDRA |= _BV(PA0);
    USI_SPI_Init();

    // Test display for 1 sec
    _delay_ms(1000);
    max7219_shift2bytes(MAX7219_MODE_TEST, 1);
    _delay_ms(1000);
    max7219_shift2bytes(MAX7219_MODE_TEST, 0);

    max7219_shift2bytes(MAX7219_MODE_SCAN_LIMIT, 7);
    max7219_shift2bytes(MAX7219_MODE_INTENSITY, 0x0);
    max7219_shift2bytes(MAX7219_MODE_POWER, 0x1);
    max7219_shift2bytes(MAX7219_MODE_DECODE, 0x0);

    while (1) {
        rotatingLine(0);
        //movingRow();
    }
}

/****************************************************************/

void movingRow() {
    int8_t i, j;

    while (1) {
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

#define LED_WIDTH (8)
#define LED_WIDTH_HALF (4)
#define LED_HEIGHT_HALF (4)

#define PI_OVER_180 (0.01745329251) // = 1/180.0 * M_PI
#define numb_between(a,b,c) ((a) < (b) && (b) < (c))

void rotatingLine(int deg) {
    while (deg <= 360) {
        if (numb_between(80, deg, 100)) {
            max7219_shift2bytes(1, 0xFF);
            max7219_shift2bytes(2, 0xFF);
            max7219_shift2bytes(3, 0xFF);
            max7219_shift2bytes(4, 0xFF);
            max7219_shift2bytes(5, 0x00);
            max7219_shift2bytes(6, 0x00);
            max7219_shift2bytes(7, 0x00);
            max7219_shift2bytes(8, 0x00);
        } else if (numb_between(260, deg, 280)) {
            max7219_shift2bytes(8, 0xFF);
            max7219_shift2bytes(7, 0xFF);
            max7219_shift2bytes(6, 0xFF);
            max7219_shift2bytes(5, 0xFF);
            max7219_shift2bytes(4, 0x00);
            max7219_shift2bytes(3, 0x00);
            max7219_shift2bytes(2, 0x00);
            max7219_shift2bytes(1, 0x00);
        } else {
            uint8_t bool_180 = numb_between(90, deg, 270);
            const double opp_over_adj = tan(-deg * PI_OVER_180); // -deg for clockwise, +deg for anticlockwise
            uint8_t col, fill;
            int8_t adj, offset;
            for (col = 0; col < LED_WIDTH; col++) {
                //adj = (col < LED_WIDTH_HALF) ? (LED_WIDTH_HALF - col) : (LED_WIDTH_HALF - col);
                adj = (LED_WIDTH_HALF - col);
                offset = (int8_t) round(opp_over_adj * adj);

                fill = _BV(LED_HEIGHT_HALF - offset) - 1;
                // If I want position of 5 (0001 1111),
                // then I should use 1<<6 to get 0010 0000.
                // Minus 1 from it to get my position filled.
                max7219_shift2bytes(col+1, bool_180 ? ~fill : fill);
            }
        }

        deg+=1;
        _delay_ms(15);
    }
}

