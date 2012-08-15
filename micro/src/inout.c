#include "inout.h"
#define BAUD 9600
#define F_CPU 16000000UL
#include <util/setbaud.h>
#include <string.h> 
#include <stdio.h>

#define UBRRH_def UBRRH
#define UBRRL_def UBRRL
#define UCSRA_def UCSRA
#define U2X_def U2X
#define UCSRC_def UCSRC
#define UCSRB_def UCSRB
#define UCSZ1_def UCSZ1
#define UCSZ0_def UCSZ0
#define RXEN_def RXEN
#define TXEN_def TXEN
#define UDR_def UDR
#define UDRE_def UDRE

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
