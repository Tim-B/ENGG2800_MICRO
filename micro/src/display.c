#include "timeutil.h"
#include "inout.h"
#include "pins.h"
#include "display.h"
#include "common.h"

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
    DEBUG_PRINT("Display time: %i:%i\n", getHour(), getMinute());
}

void setHourDisplay() {
    int pin = getHour() - 1;
    setPin(hourPins[pin], HIGH);
    toggleLED = pin;
}

void setMinuteDisplay() {
    int pin = (getMinute() / 5) - 1;
    if(pin < 0) {
        pin = 11;
    }
    setPin(hourPins[pin], HIGH);
    DEBUG_PRINT("Minute pin: %i\n", pin);
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
