#include <avr/io.h>
#include "inout.h"
#include "display.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "timeutil.h"
#include "common.h"

uint32_t time = 0;
uint32_t nextUpdate = 0;
uint32_t alarmTime = 0;

uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;
bool alarmOn = false;
Weather weatherValue = NONE;
bool toggleBit = false;

volatile uint8_t tot_overflow;
void refresh();

/**
 * Sets up the clock functionality at a 1 second interval.
 */
void setupClock() {

    TCCR1B |= (1 << WGM12)|(1 << CS12);

    // initialize counter
    TCNT1 = 0;

    // initialize compare value
    OCR1A = 32080;

    // enable compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    
    // setTime(11520);
    setTime(0);
    alarmTime = 120;
    refresh();
}

/**
 * Clock interrupt vector, increments the time ever second. If past midnight
 * then resets.
 */
ISR(TIMER1_COMPA_vect) {
    time++;
    // DEBUG_PRINT("Time: %lu\n", time);
    
    if(time >= 86400) {
        time = 0;
        refresh();
    } else if(time >= nextUpdate) {
        refresh();
    }
    toggle();
}

/**
 * Checks to see if an alarm is due, sounds the alarm if so
 */
void checkAlarm() {
    if(alarmOn && (alarmTime == time)) {
        setBuzzerOn(HIGH);
    } else {
        setBuzzerOn(LOW);
    }
}

/**
 * Breaks the time down into it's components then calls updateDisplay()
 */
void refresh() {
    uint32_t breakTime = time;
    seconds = breakTime % 60;
    breakTime /= 60;
    minutes = breakTime % 60;
    breakTime /= 60;
    hours = breakTime;
    updateDisplay();
    nextUpdate = time + 60;
    checkAlarm();
}

/**
 * Returns the hour in 12 hour format
 * @return the hour
 */
uint8_t getHour() {
    return hours % 12;
}

/**
 * Returns the hour in 24 hour format
 * @return The hour
 */
uint8_t get24Hour() {
    return hours;
}

/**
 * Gets the minute
 * @return The minutes
 */
uint8_t getMinute() {
    return minutes;
}

/**
 * Returns the second, not really used
 * @return the second
 */
uint8_t getSecond() {
    return seconds;
}

/**
 * Returns if the time is PM
 * @return true if PM, false otherwise
 */
uint8_t isPM() {
    return hours / 12;
}

/**
 * Returns whether the alarm is active
 * @return true if alarm is on, false otherwise
 */
bool alarmActive() {
    return alarmOn;
}

/**
 * Sets the current timestamp
 * @param newTime Time in seconds since midnight
 */
void setTime(uint32_t newTime) {
    time = newTime;
}

/**
 * Sets the current alarm timestamp
 * @param newTime Time in seconds since midnight
 */
void setAlarm(uint32_t newTime) {
    alarmTime = newTime;
}

/**
 * Sets whether the alarm is active
 * @param value True for active, false otherwise
 */
void setAlarmActive(bool value) {
    alarmOn = value;
}

/**
 * Sets the weather value
 * @param value The value to set the weather to
 */
void setWeather(Weather value) {
    weatherValue = value;
}

/**
 * Returns the weather
 * @return The weather value
 */
Weather getWeather() {
    return weatherValue;
}