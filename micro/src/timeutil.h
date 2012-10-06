/* 
 * File:   timeutil.h
 * Author: Tim
 *
 * Created on 7 August 2012, 6:08 PM
 */

typedef enum {
    NONE = 0,
    SUNNY = 1,
    CLOUDY = 2,
    RAINY = 3
} Weather;

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
void setAlarmActive(uint8_t value);
void setTime(uint32_t);
void setWeather(Weather);
void setAlarm(uint32_t);
void refresh();

#endif	/* TIMEUTIL_H */

