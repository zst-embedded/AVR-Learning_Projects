# AVR Learning Projects

This is a collection of projects which I have done using various tinyAVR and megaAVR microcontrollers.
These projects are done for learning purposes and I have also heavily commented in them. I used the ArduinoISP as the programmer

Project                                            | Done on    | Category           | Extra Hardware 
---------------------------------------------------|:----------:| ------------------ | -----------------
[Blink-atmega328]                                  | 2016-09-01 | Digital I/O        | LED
[PWM-atmega8515]                                   | 2016-11-20 | PWM                | LED
[SPI_Bitbang-74hc595-atmega8515]                   | 2016-11-20 | SPI (bitbanging)   | 2x 74HC595
[SPI_Bitbang-max7219-attiny84]                     | 2016-12-26 | SPI (bitbanging)   | MAX7219 + 8x8 LED Matrix
[SPI_HW-max7219-atmega8515]                        | 2017-01-16 | SPI (hardware)     | MAX7219 + 8x8 LED Matrix
[SPI_USI-max7219-attiny84]                         | 2017-01-18 | SPI (USI module)   | MAX7219 + 8x8 LED Matrix
[USART-atmega328]                                  | 2016-10-08 | USART              | LED, Push Button
[USART-attiny4313]                                 | 2017-01-01 | USART              | LED
[PWM-ADC-LCD-attiny84]                             | 2016-12-04 | PWM, ADC, Interfacing | HD44780 LCD display, Potentiometer
[I2C_USI-LCD_PCF8574-DHT11-attiny85]               | 2017-02-02 | I2C (USI module), Interfacing | DHT11 sensor, HD44780 LCD display + PCF8574 Backpack

*CLion template project used: [Template]*

### Resources
The following are some well-written learning resources which have helped me get into microcontroller programming:
+ https://sites.google.com/site/qeewiki/books/avr-guide (Really good! Covers from the very basics)
+ http://www.fourwalledcubicle.com/AVRArticles.php
+ http://www.avrfreaks.net/forums/tutorials
+ https://www.reddit.com/r/electronics/comments/uumj4/avr_8bit_microcontroller/

[Blink-atmega328]: ./Blink-atmega328
[PWM-atmega8515]: ./PWM-atmega8515
[SPI_Bitbang-74hc595-atmega8515]: ./SPI_Bitbang-74hc595-atmega8515
[SPI_Bitbang-max7219-attiny84]: ./SPI_Bitbang-max7219-attiny84
[SPI_HW-max7219-atmega8515]: ./SPI_HW-max7219-atmega8515
[SPI_USI-max7219-attiny84]: ./SPI_USI-max7219-attiny84
[Template]: ./Template
[USART-atmega328]: ./USART-atmega328
[USART-attiny4313]: ./USART-attiny4313
[PWM-ADC-LCD-attiny84]: ./PWM-ADC-LCD-attiny84
[I2C_USI-LCD_PCF8574-DHT11-attiny85]: ./I2C_USI-LCD_PCF8574-DHT11-attiny85