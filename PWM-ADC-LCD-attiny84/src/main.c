/* 
 * ATtiny84
 *
 * Interface with a HD44780 LCD display with
 * RGB backlights (common anodes). A library
 * is used for interfacing.
 *
 * The ADC reads the values of a potentiometer
 * on PA7/ADC7 and displays the percentage on
 * the LCD display.
 *
 * 3 PWM channels are used for cycling the RGB
 * backlight brightness of the LCD display.
 * PWM inverting mode is used as the backlights
 * are common anode and active-LOW.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>
#include "zst-lcd-lib.h"

/* 
 * ----------------------------------
 * PIN CONNECTIONS FOR LCD
 * (01) Vss - GND
 * (02) Vdd - 5V
 * (03) Vee - Pot Contrast
 * (04) RS  - PB0
 * (05) R/W - GND
 * (06) En  - PB1
 * (07) DB0
 * (08) DB1
 * (09) DB2
 * (10) DB3
 * (11) DB4 - PA0
 * (12) DB5 - PA1
 * (13) DB6 - PA2
 * (14) DB7 - PA3
 * (15) LED (+) - 5V
 * (16) LED (R-) - PB2/OCR0A
 * (17) LED (G-) - PA6/OCR1A
 * (18) LED (B-) - PA5/OCR1B
 * ----------------------------------
 */

int main(void) {
    DDRA = 0x0F; // PA0-3 as output
    DDRB = 0x03; // PB0-1 as output

    DDRB |= _BV(PB2); // OC0A
    // DDRA |= _BV(PA7); // OC0B
    DDRA |= _BV(PA6); // OC1A
    DDRA |= _BV(PA5); // OC1B


    /* Setup ADC */
    DIDR0 |= _BV(ADC7D); // Disable digital input on ADC7
    ADMUX |= 7; // Select ADC7 pin
    ADMUX &= ~(_BV(REFS0) | _BV(REFS0)); // VCC used as analog reference

    ADCSRA |= _BV(ADEN); // enable ADC
    ADCSRB &= ~_BV(ADLAR); // Disable left adjusted result in ADC Data Register -> result is presented right adjusted

    /* Setup LCD */
    LCD_Init();
    LCD_MoveCursor(0, 0);
    LCD_Message("Hello World."); // welcome message
    _delay_ms(2000); // wait

    /* Setup PWM: OC0 */
    TCCR0A |= _BV(WGM00) | _BV(WGM01); // Mode 3 - Fast PWM
    TCCR0A |= _BV(COM0A0) | _BV(COM0A1); // Enable OC0A Inverting mode for fast PWM
    // TCCR0A |= _BV(COM0B0) | _BV(COM0B1); // Enable OC0B Inverting mode for fast PWM
    TCCR0B |= _BV(CS00); // No prescaler, begin PWM
    // TIMSK0 is for interrupts - not used
    // TIFR0 is for interrupt flags - not used

    /* Set PWM value */
    OCR0A = 128; // Compared with TCNT0 which is counting in real time;

    /* Setup PWM: OC1 */
    //Mode 14  - fast PWM (TOP is ICR1)
    TCCR1A |= _BV(WGM11);
    TCCR1B |= _BV(WGM12);
    TCCR1B |= _BV(WGM13);
    ICR1 = 0xFF; // Setup TOP for OC1

    TCCR1A |= _BV(COM1B0) | _BV(COM1B1); // Enable OC1B Inverting mode for fast PWM
    TCCR1A |= _BV(COM1A0) | _BV(COM1A1); // Enable OC1A Inverting mode for fast PWM
    TCCR1B |= _BV(CS10); // No prescaler, begin PWM

    /*
    // Mode 5 - fast PWM (TOP is 0xFF, 8 bit)
    TCCR1A |= _BV(WGM10); TCCR1B |= _BV(WGM12); 
    */

    /* Set PWM value */
    OCR1A = 128;
    OCR1B = 128;


    uint8_t count = 0;
    uint8_t type = 0;
    uint16_t ADCresult = 0;
    while(1) {
        OCR0A = 0;
        OCR0B = 0;
        OCR1B = 0;
        OCR1A = 0;
        count = 0;
        
        LCD_Clear();
        LCD_MoveCursor(0, 0);
        LCD_Message("ADC: ");

        if (((ADCSRA >> ADSC) & 1) == 0) { // if conversion is done
            /**
             * Single Conversion
             * A single conversion is started by writing a logical one to 
             * the ADC Start Conversion bit, ADSC.
             * This bit stays high as long as the conversion is in progress
             * ADCL must be read first, then ADCH, to ensure that the content
             * of the data registers belongs to the same conversion.
             */
            ADCresult = ADCL + (ADCH << 8); // Read previous conversion
            ADCSRA |= _BV(ADSC); // Start next conversion
            LCD_Integer((ADCresult/1023.0) * 100);
        } else {
            LCD_Message("Converting: ");
        }

        /* Cycle the RGB backlights */
        if (type == 0) {
            do {
                LCD_MoveCursor(0, 1);
                LCD_Message("R: ");
                LCD_Integer(count);
                OCR0A = count;
                count+=10;
                _delay_ms(50);
            } while (count < 250);
            type++;
            continue;
        } else if (type == 1) {
            do {
                LCD_MoveCursor(0, 1);
                LCD_Message("G: ");
                LCD_Integer(count);
                OCR1A = count;
                count+=10;
                _delay_ms(50);
            } while (count < 250);
            type++;
            continue;
        } else if (type == 2) {
            do {
                LCD_MoveCursor(0, 1);
                LCD_Message("B: ");
                LCD_Integer(count);
                OCR1B = count;
                count+=10;
                _delay_ms(50);
            } while (count < 250);
            type = 0;
            continue;
        }
    }
}