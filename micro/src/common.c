#define F_CPU 16000000UL // Clock frequency (8MHz)

#include <avr/io.h>
#include <math.h> 
#include <util/delay.h>
#include <stdio.h>
#include "inout.h"
#include "pins.h"

#define TARGET_PIN 11
int step = 0;

void setup() {
    setupSerial();
    setupPins();
    int i = 0;
    for(i = 0; i < 12; i++) {
        setPinMode(i, OUTPUT);
    }
}

void loop() {
    setPin(step, HIGH);
    _delay_ms(100);
    setPin(step, LOW);
    step++;
    if(step > 11) {
        step = 0;
    }
}
