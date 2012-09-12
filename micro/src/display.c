#include "timeutil.h"
#include "inout.h"
#include "pins.h"
#include "display.h"
#include "common.h"

int toggleLED = 0;
IOBit toggleState = LOW;
IOBit buzzerOn = LOW;

void updateDisplay() {
    clearArray();
    setHourDisplay();
    setMinuteDisplay();
    setPMDisplay();
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
    setArray(pin, HIGH);
    DEBUG_PRINT("Minute pin: %i\n", pin);
    int minPin = getMinute() % 5;
    if(minPin > 0) {
        setArray((minPin - 1) + 12, HIGH);
    }
}

void toggle() {
    setArray(toggleLED, toggleState);
    if(buzzerOn) {
        buzzer(toggleState);
    } else {
        buzzer(LOW);
    }
    DEBUG_PRINT("Toggle LED: %i\n", toggleLED);
    if(toggleState) {
        // PORTD = 0xFF;
        toggleState = LOW;
    } else {
        // PORTD = 0x00;
        toggleState = HIGH;
    }
}

void setBuzzerOn(IOBit value) {
    buzzerOn = value;
}

void setPMDisplay() {
    pmLED(isPM());
}

void updateWeather(Weather value) {
    WEATHER_PORT &= ~WEATHER_CLEAR_MASK;
    switch(value) {
        case SUNNY:
           WEATHER_PORT |= WEATHER_FINE;
           break;
        case CLOUDY:
            WEATHER_PORT |= WEATHER_CLOUDY;
            break;
        default:
            WEATHER_PORT |= WEATHER_RAINY;
            break;
    }
}
