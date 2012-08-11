#include "timeutil.h"
#include "inout.h"
#include "pins.h"
#include "display.h"

int toggleLED = 0;
IOBit toggleState = LOW;

void updateDisplay() {
    clearPort(E);
    clearPort(H);
    clearPort(B);
    setHourDisplay();
    setMinuteDisplay();
}

void setHourDisplay() {
    int pin = getHour();
    setPin(pin, HIGH);
    toggleLED = pin;
}

void setMinuteDisplay() {
    int pin = getMinute() / 5;
    setPin(pin, HIGH);
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