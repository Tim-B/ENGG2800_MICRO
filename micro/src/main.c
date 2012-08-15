#include "common.h"

int main(void) {
/*
    setupPins();
    setPinMode(4, OUTPUT);
    setPin(4, HIGH);
    // DDRC = 0xFF;
    // PORTC = 0x00;
    while(1) {
       
    }
    return 0;
*/
    setup();
    while(1) {
        loop();
    }
    return 0;
}
