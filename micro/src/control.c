#include "control.h"
#include "common.h"
#include "inout.h"
#include "timeutil.h"
#include "display.h"
#include <stdbool.h>
#include <avr/interrupt.h>  
#include <util/delay.h>
#define PC_RATE 80
#define PC_SAMPLE 10
#define TIME_PROGRESS_CODE 254
#define ALARM_PROGRESS_CODE 253
#define INCREMENT_CODE 252
#define TOGGLE_ALARM 251

bool programmingActive = false;
int tmpValue = 0;
Stage stage = NO_COMMAND;
uint32_t newTime = 0;

void setupControl() {
    commandWaiting = 0;
    SENSOR_PORT &= ~SENSOR_DDR_MASK;
    EICR_def |= EICR_value;
    enablePCInt();
    enableIRInt();
    DEBUG_PRINT("interrupt setup\n");
    sei();
}

ISR(INT0_vect) {
    disableIRInt();
    int code = readCommand();
    switch(code) {
        case TOGGLE_ALARM:
            processToggleAlarm();
            break;
        case INCREMENT_CODE:
            processIncrement();
            break;
        case TIME_PROGRESS_CODE:
            processProgressTime();
            break;
        case ALARM_PROGRESS_CODE:
            processProgressAlarm();
            break;
    }
    enableIRInt();
}

void processIncrement() {
    tmpValue++;
    switch(stage) {
        case NO_COMMAND:
            return;
            break;
        case TIME_INNER_MINUTE:
            incrementInner();
            break;
        case TIME_OUTER_MINUTE:
            incrementOuter();
            break;
        case TIME_HOUR:
            incrementOuter();
            break;
        case TIME_PM:
            incrementPM();
            break;
        case ALARM_INNER_MINUTE:
            incrementInner();
            break;
        case ALARM_OUTER_MINUTE:
            incrementOuter();
            break;
        case ALARM_HOUR:
            incrementOuter();
        case ALARM_PM:
            incrementPM();
    }
}

void incrementInner() {
    if(tmpValue > 4) {
        tmpValue = 0;
    }
    displayVal(tmpValue + 12);
}

void incrementOuter() {
    if(tmpValue > 12) {
        tmpValue = 0;
    }
    displayVal(tmpValue);
    
}

void displayVal(int value) {
    clearArray();
    setArray(value, HIGH);
}

void incrementPM() {
    
}

void processToggleAlarm() {

}

void processProgressTime() {
    switch(stage) {
        case NO_COMMAND:
            stage = TIME_INNER_MINUTE;
            newTime = 0;
            tmpValue = 0;
            displayVal(0);
            break;
        case TIME_INNER_MINUTE:
            stage = TIME_OUTER_MINUTE;
            setNewTime(60);
            break;
        case TIME_OUTER_MINUTE:
            stage = TIME_HOUR;
            setNewTime(60 * 5);
            break;
        case TIME_HOUR:
            stage = TIME_PM;
            setNewTime(60 * 60);
            break;
        case TIME_PM:
            stage = NO_COMMAND;
            setTime(newTime);
            updateDisplay();
            break;
        default:
            break;
    } 
}

void setNewTime(int mult) {
    newTime += tmpValue * mult;
    tmpValue = 0;
    displayVal(0);
}

void processProgressAlarm() {
    switch(stage) {
        case NO_COMMAND:
            stage = ALARM_INNER_MINUTE;
            newTime = 0;
            tmpValue = 0;
            displayVal(0);
            break;
        case ALARM_INNER_MINUTE:
            stage = ALARM_OUTER_MINUTE;
            setNewTime(60);
            break;
        case ALARM_OUTER_MINUTE:
            stage = ALARM_HOUR;
            setNewTime(60 * 5);
            break;
        case ALARM_HOUR:
            stage = ALARM_PM;
            setNewTime(60 * 60);
            break;
        case ALARM_PM:
            stage = NO_COMMAND;
            setAlarm(newTime);
            updateDisplay();
            break;
        default:
            break;
    } 
}

bool isProgramming() {
    return !(stage == NO_COMMAND);
}

ISR(INT1_vect) {
    disablePCInt();
    int startSequence = readPCWord();
    if(!(startSequence == 0b01001101)) {
        enablePCInt();
        return;
    }
    int time1 = readPCWord();
    int time2 = readPCWord();
    int alarm1 = readPCWord();
    int alarm2 = readPCWord();
    int settings = readPCWord();
    
    if(readPCWord() != ~time1) {
        programFailed();
        return;
    }
    
    if(readPCWord() != ~time2) {
        programFailed();
        return;
    }
    
    if(readPCWord() != ~alarm1) {
        programFailed();
        return;
    }
    
    if(readPCWord() != ~alarm2) {
        programFailed();
        return;
    }
    
    if(readPCWord() != ~settings) {
        programFailed();
        return;
    }
    
    if(readPCWord() != ~0b01001101) {
        programFailed();
        return;
    }
    
    uint32_t newTime = ((uint32_t) time1 << 8) | ((uint32_t) time2);
    uint32_t newAlarm = ((uint32_t) alarm1 << 8) | ((uint32_t) alarm2);
    
    setTime(newTime);
    
    if(settings & 0x01) {
        setAlarm(newAlarm);
    }
    
    if(settings & 0x02) {
        setAlarmActive(1);
    } else {
        setAlarmActive(0);
    }
    
    int weather = (settings & 0x18) >> 3;
    switch(weather) {
        case 0x01:
            setWeather(SUNNY);
            break;
        case 0x02:
            setWeather(CLOUDY);
            break;
        case 0x03:
            setWeather(RAINY);
            break;
        default:
            setWeather(NONE);
            break;
    }
    
    DEBUG_PRINT("Time: %lu", newTime);
    DEBUG_PRINT("Alarm: %lu", newAlarm);
    DEBUG_PRINT("Settings: %i", settings);
    
    programSuccess();
}

int readPCWord() {
    int output = 0;
    int i;
    for(i = 0; i < 8; i++) {
        output |= readPCBit() << i;
    }
    return output;
}

void programFailed() {
    enablePCInt();
    DEBUG_PRINT("Programming failed\n");
}

void programSuccess() {
    enablePCInt();
    DEBUG_PRINT("Programming success\n");
}

int readPCBit() {
    int avgCount = 0;
    int value = 0;
    int i;
    for(i = 0; i < 8; i++) {
        value = SENSOR_PIN & SENSOR_OPTICAL_MASK;
        if(value) {
            avgCount++;
        }
        _delay_ms(PC_SAMPLE);
    }
    if(avgCount > 4) {
        return 1;
    } else {
        return 0;
    }
}

void disablePCInt() {
    EIMSK_def &= ~EIMSK_OPTIC_VALUE;
    DEBUG_PRINT("PC Int disabled\n");
}

void enablePCInt() {
    EIMSK_def |= EIMSK_OPTIC_VALUE;
    DEBUG_PRINT("PC Int enabled\n");
}

void disableIRInt() {
    EIMSK_def &= ~EIMSK_IR_VALUE;
    DEBUG_PRINT("IRInt disabled\n");
}

void enableIRInt() {
    EIMSK_def |= EIMSK_IR_VALUE;
    DEBUG_PRINT("IR Int enabled\n");
}

int readCommand() {
    int command = 0;
    int highCount = 0;
    int i, k;
    for(i = 0; i < 8; i++) {
        highCount = 0;
        for(k = 0; k < 5; k++) {
            if(PIND & 0x04) {
                highCount++;
            }
            _delay_us(320);
        }
        if(highCount > 2) {
            command |= 1 << i;
        }
    }
    
    DEBUG_PRINT("%i\n", command);
    _delay_ms(1000);
    return command;
}