#!/usr/bin/env bash

CHIP="ATMEGA328P"
PROGRAMMER="stk500v1" # chip via ArduinoISP
# PROGRAMMER="wiring" # Arduino board
USBPORT="/dev/tty.usbmodemFD121"

PROG_ARGS="-c $PROGRAMMER -b 115200 -P $USBPORT -p $CHIP"

FILE_PREFIX="backup-$CHIP"
echo "Starting read of $CHIP contents..."

avrdude $PROG_ARGS -U flash:r:$FILE_PREFIX-flash.hex:i
echo "Flash saved to $FILE_PREFIX-flash.hex"

avrdude $PROG_ARGS -U eeprom:r:$FILE_PREFIX-eeprom.hex:i
echo "EEPROM saved to $FILE_PREFIX-eeprom.hex"

avrdude $PROG_ARGS -U hfuse:r:$FILE_PREFIX-hfuse.hex:i
echo "hfuse saved to $FILE_PREFIX-hfuse.hex"

avrdude $PROG_ARGS -U lfuse:r:$FILE_PREFIX-lfuse.hex:i
echo "lfuse saved to b$FILE_PREFIX-lfuse.hex"

avrdude $PROG_ARGS -U efuse:r:$FILE_PREFIX-efuse.hex:i
echo "efuse saved to $FILE_PREFIX-efuse.hex"

echo " "
echo "Done!"
