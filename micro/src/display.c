#include "timeutil.h"
#include "inout.h"
#include "pins.h"
#include "display.h"

int toggleLED = 0;
IOBit toggleState = LOW;

void updateDisplay() {
    clearPort(E);
    clearPort(G);
    clearPort(H);
    clearPort(B);
    clearPort(A);
    setHourDisplay();
    setMinuteDisplay();
}

void setHourDisplay() {
    int pin = getHour();
    setPin(hourPins[pin], HIGH);
    toggleLED = pin;
}

void setMinuteDisplay() {
    int pin = getMinute() / 5;
    setPin(hourPins[pin], HIGH);
    int minPin = getMinute() % 5;
    if(minPin > 0) {
        setPin(minPins[minPin - 1], HIGH);
    }
}

void toggle() {
    setPin(toggleLED, toggleState);
    if(toggleState) {
        toggleState = LOW;
    } else {
        toggleState = HIGH;
    }
}

void setPMDisplay() {
    
}