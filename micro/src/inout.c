#include "inout.h"
#include "common.h"
#define BAUD 9600
#include <util/setbaud.h>
#include <string.h> 
#include <stdio.h>
#include <util/delay.h>
#include <stdbool.h>


int serialPutChar(char c, FILE *stream);
FILE uart_output = FDEV_SETUP_STREAM(serialPutChar, NULL, _FDEV_SETUP_WRITE);

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

void alarmLED(bool on) {
    if(on) {
        ALARM_LED_PORT |= ALARM_LED_VALUE;
    } else {
        ALARM_LED_PORT &= ~ALARM_LED_VALUE;
    }
}

void pmLED(bool on) {
    if(on) {
        PM_LED_PORT |= PM_LED_VALUE;
    } else {
        PM_LED_PORT &= ~PM_LED_VALUE;
    }
}

void buzzer(bool on) {
    if(on) {
        ALARM_BUZZER_PORT |= ALARM_BUZZER_VALUE;
    } else {
        ALARM_BUZZER_PORT &= ~ALARM_BUZZER_VALUE;
    }
}

void setupArray() {
    LATCH1_DDR |= LATCH1_DDRMASK;
    LATCH2_DDR |= LATCH2_DDRMASK;
    clearArray();
    setArray(3, HIGH);
}

void clearArray() {
    LATCH_PORT2 = LATCH2_CLEAR;
    _delay_us(1);
    LATCH_PORT2 = LATCH2_MEMORY;
    DEBUG_PRINT("Cleared\n");
}

void setArray(int value, IOMode mode) {
    LATCH_PORT1 = 0x04;
    LATCH_PORT1 = LATCH2_SET | 0x40;
    _delay_us(5);
    // LATCH2_PORT = LATCH2_MEMORY;
    return;
    if(value > 7) {
        value = value - 8;
        LATCH_PORT2 = ~(LATCH2_SET | mode << 2 | value << 3);

/*
        DEBUG_PRINT("LATCH2_SET: %i\n", LATCH2_SET);
        DEBUG_PRINT("Mode: %i, Origional: %i\n", mode << 2, mode);
        DEBUG_PRINT("Value: %i, Origional: %i\n", value << 3, value);
        DEBUG_PRINT("Port: %i\n", LATCH2_PORT);
        
*/
        _delay_us(5);
        LATCH_PORT2 = ~LATCH2_MEMORY;
    } else {
        LATCH_PORT1 = ~(LATCH1_SET | mode << 2 | value << 3);
/*
        DEBUG_PRINT("LATCH1_SET: %i\n", LATCH1_SET);
        DEBUG_PRINT("Mode: %i, Origional: %i\n", mode << 2, mode);
        DEBUG_PRINT("Value: %i, Origional: %i\n", value << 3, value);
        DEBUG_PRINT("Port: %i\n", LATCH1_PORT);
*/
        _delay_us(5);
        LATCH_PORT1 = ~LATCH1_MEMORY;
    }
}