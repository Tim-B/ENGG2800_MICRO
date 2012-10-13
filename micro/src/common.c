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
    // DDRE = 0x00;
    // setupSerial();
    // setupPins(); 
    // DEBUG_PRINT("START\n");
    setupSerial();
    setupPins(); 
    setupArray();
    setupClock();
    setupControl();
    DEBUG_PRINT("START\n");
    sei();
    
}

void loop() {
    cycle();
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
