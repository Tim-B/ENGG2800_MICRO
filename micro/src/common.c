#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h> 
#include <util/delay.h>
#include <stdlib.h>
#include "inout.h"
#include "timeutil.h"
#include "control.h"

int step = 0;
char* string;
int count = 0;
int old = 0;

/**
 * Run once at the start of the application.
 * Initializes pins, clock, IO, etc.
 */
void setup() {
    // DDRE = 0x00;
    // setupSerial();
    // setupPins(); 
    // DEBUG_PRINT("START\n");
    setupSerial();
    setupArray();
    setupClock();
    setupControl();
    DEBUG_PRINT("START\n");
    // DDRD &= ~0x08;
    sei();
}


/**
 * Run continuously in a loop.
 */
void loop() {
    cycle();
/*
    if(PIND & 0x08) {
        DEBUG_PRINT("High\n");
    } else {
        DEBUG_PRINT("LOW\n");
    }
*/
    // _delay_ms(1000);
/*
    clearArray();
    setArray(count, HIGH);
    _delay_ms(2000);
    if(count == 0) {
        clearArray();
        _delay_ms(1000);
        setArray(count, HIGH);
        _delay_ms(1000);
    }
    count++;
    if(count > 15) {
        count = 0;
    }
*/
}
