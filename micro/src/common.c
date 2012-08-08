#define F_CPU 16000000UL // Clock frequency (8MHz)

#include <avr/io.h>
#include <math.h> 
#include <util/delay.h>
#include "inout.h"
#include "pins.h"

void setup() {
    setupPins();
    setPinMode(0, OUTPUT);
    setupSerial();
}

void loop() {
    setPin(0, HIGH);
    _delay_ms(1000);
    setPin(0, LOW);
    serialPrint("hello world");
    // serialPutChar('a');
    _delay_ms(1000);	
}
