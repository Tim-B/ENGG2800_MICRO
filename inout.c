#include "inout.h"
#define BAUD 9600
#define F_CPU 16000000UL
#include <util/setbaud.h>

void setPinMode(int pin, IOMode mode) {
    if(mode) {
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

void setMultiPinMode(Port port, int value) {
    *directionPorts[port] = value;
}

void setMultiPin(Port port, int value) {
    *dataPorts[port] = value;
}

void setupSerial() {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    #if USE_2X
        UCSR0A |= _BV(U2X0);
    #else
        UCSR0A &= ~(_BV(U2X0));
    #endif
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */ 
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
}

void serialPrint(char* string) {
    int i = 0;
    while(string[i] != '\0') {
        serialPutChar(string[i]);
        i++;
    }
}

void serialPrintLn(char* string) {
    
}

void serialPutChar(char c) {
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = c;
}
