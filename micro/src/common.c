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
    // setupPins(); 
    // setupClock();
    
    
    // setupArray();
    // setupControl();
    // serialPutChar('t', stderr);
/*
    int i = 0;
    for(i = 0; i < 16; i++) {
        setPinMode(i, OUTPUT);
    }
    setPinMode(16, INPUT);
*/
    setupArray();
    /*
    DDRC = 0x1F;
    DDRB = 0x3F;
    DDRD = 0xE1;
    */
}

void loop() {
/*
    if(commandWaiting) {
        readCommand();
    }
*/
    // PORTC |= ~0xF8;
    /*
    PORTC |= 0x1F;
    PORTB |= 0x3F;
    PORTD |= 0xE1;
    _delay_ms(1000);
    PORTC &= ~0x1F;
    PORTB &= ~0x3F;
    PORTD &= ~0xE1;
    DEBUG_PRINT("IR: %i, LDR: %i\n", (~PINC & 0x20) >> 5, (~PIND & 0x04) >> 2);
    _delay_ms(1000);
     * */
}
