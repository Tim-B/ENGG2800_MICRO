#include "timeutil.h"
#include "inout.h"
#include "pins.h"
#include "display.h"
#include "common.h"

int toggleLED = 0;
IOBit toggleState = LOW;

void updateDisplay() {
    clearArray();
    setHourDisplay();
    setMinuteDisplay();
    DEBUG_PRINT("Display time: %i:%i\n", getHour(), getMinute());
}

void setHourDisplay() {
    int pin = getHour() - 1;
    setArray(pin, HIGH);
    toggleLED = pin;
}

void setMinuteDisplay() {
    int pin = (getMinute() / 5) - 1;
    if(pin < 0) {
        pin = 11;
    }
    setPin(hourPins[pin], HIGH);
    setArray(pin, HIGH);
    DEBUG_PRINT("Minute pin: %i\n", pin);
    int minPin = getMinute() % 5;
    if(minPin > 0) {
        setArray((minPin - 1) + 12, HIGH);
    }
}

void toggle() {
    //setArray(toggleLED, toggleState);
    DEBUG_PRINT("Toggle LED: %i\n", toggleLED);
    if(toggleState) {
        PORTD = 0xFF;
        toggleState = LOW;
    } else {
        PORTD = 0x00;
        toggleState = HIGH;
    }
}

void setPMDisplay() {
    
}
