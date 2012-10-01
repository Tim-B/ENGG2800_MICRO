#include <avr/io.h>
#include "inout.h"
#include "display.h"
#include <avr/interrupt.h>
#include "timeutil.h"
#include "common.h"

uint32_t time = 0;
uint32_t nextUpdate = 0;
uint32_t alarmTime = 0;

uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;
uint8_t alarmOn = 0;
Weather weatherValue = SUNNY;

volatile uint8_t tot_overflow;
void refresh();

void setupClock() {

    TIMSK_def = 0x01;
    TCNT0 = (unsigned char) 0x0BDC;
    // TCCR1B = (unsigned char) 0x04;
    TCCR1B = (unsigned char) 0x04;

    setTime(11520);
    alarmTime = 11580;
    refresh();
    updateDisplay();
/*
    TIMSK1 |= (1 << TOIE1);
    TCCR1B |= (1 << CS11);
    sei();
*/
}


ISR(TIMER1_OVF_vect) {
    TCNT0 = (unsigned char) 0x0BDC;
    time = time + 2;
    DEBUG_PRINT("Time: %lu\n", time);
    if(time >= 86400) {
        time = 0;
        refresh();
    } else if(time >= nextUpdate) {
        refresh();
    }
    toggle();
}

void checkAlarm() {
    if(alarmOn && (alarmTime == time)) {
        setBuzzerOn(HIGH);
    } else {
        setBuzzerOn(LOW);
    }
}

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

uint8_t getHour() {
    return hours % 12;
}

uint8_t get24Hour() {
    return hours;
}

uint8_t getMinute() {
    return minutes;
}

uint8_t getSecond() {
    return seconds;
}

uint8_t isPM() {
    return hours / 12;
}


uint8_t alarmActive() {
    return alarmOn;
}

void setTime(uint32_t newTime) {
    time = newTime;
}


void setAlarm(uint32_t newTime) {
    alarmTime = newTime;
}

void setAlarmActive(uint8_t value) {
    alarmOn = value;
}

void setWeather(Weather value) {
    weatherValue = value;
}