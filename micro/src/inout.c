#include "inout.h"
#include "common.h"
#define BAUD 9600
#include <util/setbaud.h>
#include <string.h> 
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>


int serialPutChar(char c, FILE *stream);
FILE uart_output = FDEV_SETUP_STREAM(serialPutChar, NULL, _FDEV_SETUP_WRITE);

/**
 * Sets up the serial connection.
 */
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

/**
 * Prints a character to serial
 * @param c The character to print
 * @param stream The IO streme
 * @return always 0
 */
int serialPutChar(char c, FILE *stream) {
    if (c == '\n') {
        serialPutChar('\r', stream);
    }
    loop_until_bit_is_set(UCSRA_def, UDRE_def);
    UDR_def = c;
    return 0;
}

/**
 * Sets the alarm LED
 * @param on If true then the LED is on, off otherwise
 */
void alarmLED(bool on) {
    if(on) {
        ALARM_LED_PORT |= ALARM_LED_VALUE;
    } else {
        ALARM_LED_PORT &= ~ALARM_LED_VALUE;
    }
}

/**
 * Sets the PM led
 * @param on If true then the LED is on, off otherwise
 */
void pmLED(bool on) {
    if(on) {
        PM_LED_PORT |= PM_LED_VALUE;
    } else {
        PM_LED_PORT &= ~PM_LED_VALUE;
    }
}

/**
 * Sets the buzzer on or off
 * @param on On if HIGH, off otherwise
 */
void buzzer(IOBit on) {
    if(on == HIGH) {
        ALARM_BUZZER_PORT |= ALARM_BUZZER_VALUE;
        DEBUG_PRINT("Buzzer on\n");
    } else {
        ALARM_BUZZER_PORT &= ~ALARM_BUZZER_VALUE;
        DEBUG_PRINT("Buzzer off\n");
    }
}

/**
 * Initializes the LED array and latches.
 */
void setupArray() {
    // Set data direction registers
    DDRC |= ALARM_BUZZER_VALUE;
    DDRC |= ALARM_LED_VALUE;
    DDRC |= PM_LED_VALUE;
    DDRC |= 0x07;
    
    latchAddressTranslate[0] = 4;
    latchAddressTranslate[1] = 5;
    latchAddressTranslate[2] = 7;
    latchAddressTranslate[3] = 12;
    latchAddressTranslate[4] = 8;
    latchAddressTranslate[5] = 10;
    latchAddressTranslate[6] = 14;
    latchAddressTranslate[7] = 11;
    latchAddressTranslate[8] = 13;
    latchAddressTranslate[9] = 1;
    latchAddressTranslate[10] = 6;
    latchAddressTranslate[11] = 2;
    latchAddressTranslate[12] = 15;
    latchAddressTranslate[13] = 9;
    latchAddressTranslate[14] = 0;
    latchAddressTranslate[15] = 3;
    
    LATCH1_DDR |= LATCH1_DDRMASK;
    LATCH2_DDR |= LATCH2_DDRMASK;
    clearArray();
}

/**
 * Clears the LED array;
 */
void clearArray() {
    // Long, but make sure the bits are set correctly
    LATCH_PORT1 = (LATCH_PORT1 & ~LATCH1_MEMORY) | LATCH1_CLEAR;
    LATCH_PORT2 = (LATCH_PORT2 & ~LATCH2_MEMORY) | LATCH2_CLEAR;
    _delay_us(100);
    LATCH_PORT1 |= LATCH1_MEMORY;
    LATCH_PORT2 |= LATCH2_MEMORY;
    // DEBUG_PRINT("Cleared\n");
}

/**
 * Sets an LED in the array
 * @param value The index to be set
 * @param mode if HIGH set the LED on, off otherwise
 */
void setArray(int value, IOMode mode) {
    value = latchAddressTranslate[value];

    int secondLatch = 0;
    if(value > 7) {
        value = value - 8;
        secondLatch = 1;
    }
    value = value & 0x07;

    // Make sure address lines are clear then set them
    LATCH_PORT1 = (LATCH_PORT1 & 0xF8) | value;

    if(mode) {
        LATCH_PORT2 |= 0x40;
    } else {
        LATCH_PORT2 &= ~0x40;
    }
    // Put the latch into write mode
    if(secondLatch) {
        LATCH_PORT2 = (LATCH_PORT2 & ~LATCH2_MEMORY) | LATCH2_SET;
    } else {
        LATCH_PORT1 = (LATCH_PORT1 & ~LATCH1_MEMORY) | LATCH1_SET;
    }
    
    _delay_us(100);
    
    // Back to memory mode!
    LATCH_PORT1 |= LATCH1_MEMORY;
    LATCH_PORT2 |= LATCH2_MEMORY;
}