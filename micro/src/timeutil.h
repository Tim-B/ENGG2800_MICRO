
#ifndef TIMEUTIL_H
#define	TIMEUTIL_H
#include <stdint.h>
#include <stdbool.h>
typedef enum {
    NONE = 0,
    SUNNY = 1,
    CLOUDY = 2,
    RAINY = 3
} Weather;
void setupClock();
uint8_t getHour();
uint8_t get24Hour();
uint8_t getMinute();
uint8_t getSecond();
uint8_t isPM();
bool alarmActive();
void setAlarmActive(bool);
void setTime(uint32_t);
void setWeather(Weather);
void setAlarm(uint32_t);
void refresh();

#endif	/* TIMEUTIL_H */

