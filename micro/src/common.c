#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h> 
#include <util/delay.h>
#include <stdlib.h>
#include "inout.h"
#include "pins.h"
#include "timeutil.h"
#include "control.h"

int step = 0;
char* string;
int count = 0;
int old = 0;
void setup() {
    DDRE = 0x00;
    setupSerial();
    setupPins(); 
    DEBUG_PRINT("START\n");
/*
    
    setupPins(); 
    setupArray();
    setupClock();
    setupControl();
*/
}

void loop() {
    int new = PINE & 0x10;
    DEBUG_PRINT("Changed: %i\n", new);
    _delay_ms(1000);
}
