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
void setup() {
    setupSerial();
    setupPins(); 
    setupClock();
    setupArray();
}

void loop() {
    if(commandWaiting) {
        // readCommand();
    }
}
