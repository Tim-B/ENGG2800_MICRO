#define F_CPU 16000000UL // Clock frequency (8MHz)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h> 
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include "inout.h"
#include "pins.h"
#include "timeutil.h"

#define TARGET_PIN 11
int step = 0;
char* string;

void setup() {
    setupSerial();
    setupPins();
    setupClock();
    
    int i = 0;
    for(i = 0; i < 12; i++) {
        setPinMode(i, OUTPUT);
    }
}

void loop() {}
