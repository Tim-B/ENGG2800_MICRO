#include "pins.h"
#include "inout.h"

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
                
        pins[0].pin = 0x10;
        pins[0].port = E;
        pins[1].pin = 0x20;
        pins[1].port = E;
        pins[2].pin = 0x20;
        pins[2].port = G;
        pins[3].pin = 0x08;
        pins[3].port = E;
        pins[4].pin = 0x08;
        pins[4].port = H;
        pins[5].pin = 0x10;
        pins[5].port = H;        
        pins[6].pin = 0x20;
        pins[6].port = H;         
        pins[7].pin = 0x40;
        pins[7].port = H; 
        pins[8].pin = 0x10;
        pins[8].port = B; 
        pins[9].pin = 0x20;
        pins[9].port = B;
        pins[10].pin = 0x40;
        pins[10].port = B;
        pins[11].pin = 0x80;
        pins[11].port = B; 
        
        pins[12].pin = 0x01;
        pins[12].port = A;
        pins[13].pin = 0x02;
        pins[13].port = A;
        pins[14].pin = 0x04;
        pins[14].port = A;
        pins[15].pin = 0x08;
        pins[15].port = A;
        
        hourPins[0] = 0;
        hourPins[1] = 1;
        hourPins[2] = 2;
        hourPins[3] = 3;
        hourPins[4] = 4;
        hourPins[5] = 5;
        hourPins[6] = 6;
        hourPins[7] = 7;
        hourPins[8] = 8;
        hourPins[9] = 9;
        hourPins[10] = 10;
        hourPins[11] = 11;
        
        minPins[0] = 12;
        minPins[1] = 13;
        minPins[2] = 14;
        minPins[3] = 15;
}