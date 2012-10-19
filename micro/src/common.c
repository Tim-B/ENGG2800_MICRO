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
    
    // DDRD &= ~0x08;
    sei();
    // DEBUG_PRINT("START\n");
}


/**
 * Run continuously in a loop.
 */
void loop() {
    // DEBUG_PRINT("TICK\n");
    cycle();
    timeCycle();
}
