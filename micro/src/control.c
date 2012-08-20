#include "control.h"
#include "common.h"
#include "inout.h"
#include <avr/interrupt.h>  
#include <util/delay.h>

void setupControl() {
    commandWaiting = 0;
    EICRB |= 0x03;
	
    EIMSK |= 0x10;
    DDRE = 0x00;
    sei();
}

ISR(INT4_vect)
{
    if(commandWaiting) {
        return;
    }
    commandWaiting = 1;
}

void readCommand() {
    int command = 0;
    int highCount = 0;
    int i, k;
    for(i = 0; i < 8; i++) {
        highCount = 0;
        for(k = 0; k < 5; k++) {
            if(PINE & 0x10) {
                highCount++;
            }
            _delay_us(320);
        }
        if(highCount > 2) {
            command |= 1 << i;
        }
    }
    if(command == 157) {
        setArray(2, HIGH);
    }
    if(command == 137) {
        clearArray();
    }
    DEBUG_PRINT("%i\n", command);
    _delay_ms(3000);
    commandWaiting = 0;
}