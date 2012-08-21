#include "inout.h"
#include "common.h"
#define BAUD 9600
#define F_CPU 16000000UL
#include <util/setbaud.h>
#include <string.h> 
#include <stdio.h>
#include <util/delay.h>


int serialPutChar(char c, FILE *stream);
FILE uart_output = FDEV_SETUP_STREAM(serialPutChar, NULL, _FDEV_SETUP_WRITE);


void setPinMode(int pin, IOMode mode) {
    if(mode == OUTPUT) {
        setMultiPinMode(pins[pin].port, *directionPorts[pins[pin].port] | pins[pin].pin);
    } else {
        setMultiPinMode(pins[pin].port, *directionPorts[pins[pin].port] & ~pins[pin].pin);
    }
}


void setPin(int pin, IOBit mode) {
    if(mode) {
        setMultiPin(pins[pin].port, *dataPorts[pins[pin].port] | pins[pin].pin);
    } else {
        setMultiPin(pins[pin].port, *dataPorts[pins[pin].port] & ~pins[pin].pin);
    }
}

int getPin(int pin) {
    return pins[pin].port & pins[pin].pin;
}

void setMultiPinMode(Port port, int value) {
    *directionPorts[port] = value;
}

void setMultiPin(Port port, int value) {
    *dataPorts[port] = value;
}

void setupSerial() {
    UBRRH_def = UBRRH_VALUE;
    UBRRL_def = UBRRL_VALUE;

    #if USE_2X
    UCSR0A |= _BV(U2X0);
    #else
    UCSRA_def &= ~(_BV(U2X_def));
    #endif

    UCSRC_def = _BV(UCSZ1_def) | _BV(UCSZ0_def); /* 8-bit data */ 
    UCSRB_def = _BV(RXEN_def) | _BV(TXEN_def);   /* Enable RX and TX */
    
    stdout = &uart_output;
}

int serialPutChar(char c, FILE *stream) {
    if (c == '\n') {
        serialPutChar('\r', stream);
    }
    loop_until_bit_is_set(UCSRA_def, UDRE_def);
    UDR_def = c;
    return 0;
}

int serialGetChar(FILE *stream) {
    return 0;
}

void clearPort(Port port) {
    clearMultiPins(port, 0xFF);
}

void clearMultiPins(Port port, int value) {
    *dataPorts[port] &= ~value;
}

void setupArray() {
    LATCH1_DDR |= 0x3F;
    LATCH2_DDR |= 0x3F;
    clearArray();
    // setArray(12, HIGH);
/*
    DEBUG_PRINT("Setting 0:\n");
    setArray(0, HIGH);
    DEBUG_PRINT("Setting 2:\n");
    setArray(2, HIGH);
    DEBUG_PRINT("Setting 9:\n");
    setArray(9, HIGH);
*/
}

void clearArray() {
    LATCH1_PORT = LATCH1_CLEAR;
    LATCH2_PORT = LATCH2_CLEAR;
    _delay_us(1);
    LATCH1_PORT = LATCH1_MEMORY;
    LATCH2_PORT = LATCH2_MEMORY;
    DEBUG_PRINT("Cleared\n");
}

void setArray(int value, IOMode mode) {
    if(value > 7) {
        value = value - 8;
        LATCH2_PORT = LATCH2_SET | mode << 2 | value << 3;
/*
        DEBUG_PRINT("LATCH2_SET: %i\n", LATCH2_SET);
        DEBUG_PRINT("Mode: %i, Origional: %i\n", mode << 2, mode);
        DEBUG_PRINT("Value: %i, Origional: %i\n", value << 3, value);
        DEBUG_PRINT("Port: %i\n", LATCH2_PORT);
*/
        _delay_us(1);
        LATCH2_PORT = LATCH2_MEMORY;
    } else {
        LATCH1_PORT = LATCH1_SET | mode << 2 | value << 3;
/*
        DEBUG_PRINT("LATCH1_SET: %i\n", LATCH1_SET);
        DEBUG_PRINT("Mode: %i, Origional: %i\n", mode << 2, mode);
        DEBUG_PRINT("Value: %i, Origional: %i\n", value << 3, value);
        DEBUG_PRINT("Port: %i\n", LATCH1_PORT);
*/
        _delay_us(1);
        LATCH1_PORT = LATCH1_MEMORY;
    }
}