#define F_CPU 16000000UL
#include <avr/io.h>
#include "inout.h"
#include "display.h"
#include <avr/interrupt.h>
#include "timeutil.h"
#include "common.h"

uint32_t time = 0;
uint32_t nextUpdate = 0;

uint8_t seconds = 0;
uint8_t minutes = 0;
uint8_t hours = 0;

#define TIMSK_def TIMSK

volatile uint8_t tot_overflow;
void refresh();

void setupClock() {
    sei();
    TIMSK_def = 0x01;
    TCCR1A = 0x00;
    TCNT1 = 0x0BDC;
    TCCR1B = 0x04;
    setTime(36720);
    refresh();
}

ISR(TIMER1_OVF_vect) {
    TCNT1 = 0x0BDC;
    time++;
    DEBUG_PRINT("Time: %lu\n", time);
    if(time >= 86400) {
        time = 0;
        refresh();
    } else if(time >= nextUpdate) {
        refresh();
    }
    toggle();
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
    return hours % 12;
}

void setTime(uint32_t newTime) {
    time = newTime;
}
