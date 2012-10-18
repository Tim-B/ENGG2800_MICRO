
#include "pins.h"
#include "inout.h"

/**
 * Depreciated.
 */
void setupPins() {
        directionPorts[B] = &DDRB;
        directionPorts[C] = &DDRC;
        directionPorts[D] = &DDRD;
       
        dataPorts[B] = &PORTB;
        dataPorts[C] = &PORTC;
        dataPorts[D] = &PORTD;
  
                
        pins[0].pin = 0x01;
        pins[0].port = C;
        pins[1].pin = 0x02;
        pins[1].port = C;
        pins[2].pin = 0x04;
        pins[2].port = C;
        pins[3].pin = 0x08;
        pins[3].port = C;
        pins[4].pin = 0x10;
        pins[4].port = C;
        
        hourPins[0] = 4;
        hourPins[1] = 4;
        hourPins[2] = 4;
        hourPins[3] = 4;
        hourPins[4] = 4;
        hourPins[5] = 4;
        hourPins[6] = 4;
        hourPins[7] = 4;
        hourPins[8] = 4;
        hourPins[9] = 4;
        hourPins[10] = 4;
        hourPins[11] = 4;
        
        minPins[0] = 0;
        minPins[1] = 1;
        minPins[2] = 2;
        minPins[3] = 3;
}
