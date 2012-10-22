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
    
    setupSerial();
    setupArray();
    setupClock();
    setupControl();

    sei();
    
}


/**
 * Run continuously in a loop.
 */
void loop() {
    // Check for any waiting commands or display updates.
    cycle();
    timeCycle();
}
