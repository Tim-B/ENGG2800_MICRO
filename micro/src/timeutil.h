/* 
 * File:   timeutil.h
 * Author: Tim
 *
 * Created on 7 August 2012, 6:08 PM
 */

#ifndef TIMEUTIL_H
#define	TIMEUTIL_H
void setupClock();
#include <stdint.h>
uint8_t getHour();
uint8_t get24Hour();
uint8_t getMinute();
uint8_t getSecond();
uint8_t isPM();
uint8_t alarmActive();
void setTime(uint32_t);

typedef enum {
    SUNNY = 0,
    CLOUDY = 1,
    RAINY = 2
} Weather;

#endif	/* TIMEUTIL_H */

