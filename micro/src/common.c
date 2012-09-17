#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h> 
#include <util/delay.h>
#include <stdlib.h>
#include "inout.h"
#include "pins.h"
#include "timeutil.h"
// #include "control.h"

int step = 0;
char* string;
int count = 0;
void setup() {
    setupSerial();
    setupPins(); 
    setupArray();
    setupClock();
    // setArray(0, HIGH);
}

void loop() {
/*
    if(commandWaiting) {
        // readCommand();
    }
    clearArray();
    // setArray(10, HIGH);
    setArray(step++, HIGH);
    // setArray(1, HIGH);
    if(step == 16) {
        step = 0;
    }
    _delay_ms(1000);
*/

}
