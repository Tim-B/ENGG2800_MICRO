#include "timeutil.h"
#include "inout.h"
#include "pins.h"
#include "display.h"
#include "common.h"

int toggleLED = 0;
IOBit toggleState = LOW;
IOBit buzzerOn = HIGH;
int alarmCount = 0;

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
    if(buzzerOn == HIGH) {
        buzzer(toggleState);
        DEBUG_PRINT("Alarm count %i\n", alarmCount);
        alarmCount++;
        if(alarmCount > 10) {
            buzzerOn = LOW;
            buzzer(LOW);
        }
    }
    DEBUG_PRINT("Toggle LED: %i\n", toggleLED);
    if(toggleState) {
        toggleState = LOW;
    } else {
        toggleState = HIGH;
    }
}

void setBuzzerOn(IOBit value) {
    buzzerOn = value;
    alarmCount = 0;
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
