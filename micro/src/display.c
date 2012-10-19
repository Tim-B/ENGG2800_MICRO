#include "timeutil.h"
#include "inout.h"
#include "display.h"
#include "common.h"
#include "control.h"
#include <stdio.h>
#include <avr/io.h>

int toggleLED = 0;
IOBit toggleState = LOW;
IOBit buzzerOn = HIGH;
int alarmCount = 0;
bool cancelToggle = false;

/**
 * If the clock isn't in programming mode, clear the display then set the
 * hours, minutes, PM, alarm and weather
 */
void updateDisplay() {
    if(isProgramming()) {
        return;
    }
    clearArray();
    setHourDisplay();
    setMinuteDisplay();
    setPMDisplay();
    setAlarmDisplay();
    updateWeather(getWeather());
    DEBUG_PRINT("Display time: %i:%i\n", getHour(), getMinute());
}

/**
 * Sets the hour array index to the current hour value
 */
void setHourDisplay() {
    int pin = getHour();
    setArray(pin, HIGH);
    toggleLED = pin;
}

/**
 * Sets both the outer and the inner minute LEDs, sets the outer minute to the
 * toggleLED value and if the minute and hour are aligned sets cancelToggle
 * to true.
 */
void setMinuteDisplay() {
    int pin = getMinute() / 5;
    setArray(pin, HIGH);
    if(pin == toggleLED) {
        cancelToggle = true;
    } else {
        cancelToggle = false;
    }
    DEBUG_PRINT("Minute pin: %i\n", pin);
    int minPin = getMinute() % 5;
    if(minPin > 0) {
        setArray(minPin + 11, HIGH);
        DEBUG_PRINT("5 Minute pin: %i\n", (minPin - 1) + 12);
    }
}

/**
 * If not in programming mode and the toggle isn't canceled it toggles
 * the toggleLED
 * 
 * Also toggles the middle weather LED if in programming mode.
 * 
 * If the buzzer is on then the buzzer is toggled and alarmCount incremented.
 */
void toggle() {
    if(!isProgramming()) {
        if(!cancelToggle) {
            setArray(toggleLED, toggleState);
        } else {
            setArray(toggleLED, HIGH);
        }
    } else {
        if(toggleState == HIGH) {
            updateWeather(CLOUDY);
        } else {
            updateWeather(NONE);
        }
    }
    if(buzzerOn == HIGH) {
        buzzer(toggleState);
        DEBUG_PRINT("Alarm count %i\n", alarmCount);
        alarmCount++;
        if(alarmCount > 10) {
            buzzerOn = LOW;
            buzzer(LOW);
        }
    }
    // DEBUG_PRINT("Toggle LED: %i\n", toggleLED);
    if(toggleState) {
        toggleState = LOW;
    } else {
        toggleState = HIGH;
    }
}

/**
 * Starts the buzzer sounding
 * @param value the value for buzzerOn
 */
void setBuzzerOn(IOBit value) {
    buzzerOn = value;
    alarmCount = 0;
}

/**
 * Sets the PM led if the time is PM
 */
void setPMDisplay() {
    pmLED(isPM());
}

/**
 * Sets the alarm LED if the alarm is active
 */
void setAlarmDisplay() {
    if (alarmActive()) {
        alarmLED(HIGH);
    } else {
        alarmLED(LOW);
    }
}

/**
 * Sets the respective weather LED
 * @param value The weather value to be set.
 */
void updateWeather(Weather value) {
    WEATHER_PORT &= ~WEATHER_CLEAR_MASK;
    switch(value) {
        case SUNNY:
           WEATHER_PORT |= WEATHER_FINE;
           break;
        case CLOUDY:
            WEATHER_PORT |= WEATHER_CLOUDY;
            break;
        case RAINY:
            WEATHER_PORT |= WEATHER_RAINY;
            break;
        default:
            break;
    }
}
