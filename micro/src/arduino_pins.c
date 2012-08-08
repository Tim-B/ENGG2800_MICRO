#include "pins.h"

void setupPins() {
        directionPorts[A] = &DDRA;
        directionPorts[B] = &DDRB;
        directionPorts[C] = &DDRC;
        directionPorts[D] = &DDRD;
        directionPorts[E] = &DDRE;
        directionPorts[F] = &DDRF;
        directionPorts[G] = &DDRG;
        directionPorts[H] = &DDRH;
        directionPorts[J] = &DDRJ;
        directionPorts[K] = &DDRK;
                
        dataPorts[A] = &PORTA;
        dataPorts[B] = &PORTB;
        dataPorts[C] = &PORTC;
        dataPorts[D] = &PORTD;
        dataPorts[E] = &PORTE;
        dataPorts[F] = &PORTF;
        dataPorts[G] = &PORTG;
        dataPorts[H] = &PORTH;
        dataPorts[J] = &PORTJ;
        dataPorts[K] = &PORTK;    
                
        pins[0].pin = 0x80;
        pins[0].port = E;
}