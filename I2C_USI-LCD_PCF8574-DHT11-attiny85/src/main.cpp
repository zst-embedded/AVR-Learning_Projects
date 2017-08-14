/* 
 * ATtiny85
 *
 * A DHT11 sensor is connected and readings are
 * taken. An Arduino library (SimpleDHT) is
 * modified to work with pure AVR code.
 *
 * Interface with an I2C LCD display to display
 * the readings of the DHT11. The HD44780 LCD 
 * display with PCF8574 backpack is used.
 * 
 * The USI module of the tinyAVR is used for I2C.
 * An Arduino library (TinyWireM) is modified to
 * work with pure AVR code.
 *
 * The LCD library is modified to "push" changes
 * of the LCD port register using "LCD_I2C_Push"
 * every time there is a strobe.
 *
 * Connections:
 *     PB4 - DHT11 data pin
 *     PB2 - I2C SCL
 *     PB0 - I2C SDA
 */

#include <avr/io.h>
#include <util/delay.h>
#include <SimpleDHT.h>
#include "zst-i2c-lcd-lib.h"
#include "TinyWireM.h"

#define LCD_I2C_ADDRESS (0x78 >> 1)

// http://playground.arduino.cc/Code/USIi2c
void LCD_I2C_Push(uint8_t push) {
    TinyWireM.beginTransmission(LCD_I2C_ADDRESS);
    TinyWireM.send(push);
    TinyWireM.endTransmission();
}

int main(void) {
    /* Setup I2C with USI */
    TinyWireM.begin();

    /* Setup LCD */
    LCD_Init();
    LCD_MoveCursor(0,0);
    LCD_Message("Hello world!!!");

    /* Setup DHT11 library */
    SimpleDHT11 dht11;
    const pinType pin_type = {
            .ddr = &DDRB, .pin = &PINB, .port = &PORTB, .pos = PB4
    };

    LCD_Clear();

    while (1) {
        uint8_t temperature = 0, humidity = 0, err = 0;
        err = dht11.read(pin_type, &temperature, &humidity, NULL);
        if (err) {
            LCD_MoveCursor(0,0);
            LCD_Message("Read DHT11 failed.");

            LCD_MoveCursor(0,1);
            LCD_Integer(err);
        } else {
            LCD_MoveCursor(0,0);
            LCD_Message("Humidity: ");
            LCD_Integer((int) humidity);

            LCD_MoveCursor(0,1);
            LCD_Message("Temperature: ");
            LCD_Integer((int) temperature);
        }
        // DHT11 sampling rate is 1 Hz.
        _delay_ms(1000);
    }
}