#include "control.h"
#include "common.h"
#include "inout.h"
#include "timeutil.h"
#include "display.h"
#include <stdbool.h>
#include <avr/interrupt.h>  
#include <util/delay.h>
#define PC_RATE 160
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
    // enablePCInt();
    EIMSK_def |= EIMSK_OPTIC_VALUE;
    enableIRInt();
    // DDRB |= 0x80;
    DEBUG_PRINT("interrupt setup\n");
    alarmLED(false);
    pmLED(false);
}

void printArray(int *value, int total) {
    int i;
    for(i = 0; i < total; i++) {
        DEBUG_PRINT("Value: %i\n", value[i]);
    }
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

    if(!checkStart()) {
        programFailed();
        return;
    }
    
    uint8_t time1 = readPCWord();
    uint8_t time2 = readPCWord();
    uint8_t alarm1 = readPCWord();
    uint8_t alarm2 = readPCWord();
    uint8_t settings = readPCWord();
    
    uint8_t time1_check = readPCWord();
    uint8_t time2_check = readPCWord();
    uint8_t alarm1_check = readPCWord();
    uint8_t alarm2_check = readPCWord();
    uint8_t settings_check = readPCWord();
    
    uint8_t end_check = ~readPCWord();

    if(time1_check != ~time1) {
        // programFailed();
        // return;
    }
    
    if(time2_check != ~time2) {
        // programFailed();
        // return;
    }
    
    if(alarm1_check != ~alarm1) {
        // programFailed();
        // return;
    }
    
    if(alarm2_check != ~alarm2) {
        // programFailed();
        // return;
    }
    
    if(settings_check != ~settings) {
        // programFailed();
        // return;
    }
    
    uint32_t newTime = ((uint32_t) time1 << 8) | ((uint32_t) time2);
    uint32_t newAlarm = ((uint32_t) alarm1 << 8) | ((uint32_t) alarm2);
    
    setTime(newTime);
    refresh();
    updateDisplay();
    
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
    
    DEBUG_PRINT("Time1: %u, Time2: %u, Full: %lu\n", time1, time2, newTime);
    DEBUG_PRINT("Alarm1: %u, Alarm2: %u, Full: %lu\n", alarm1, alarm2, newAlarm);
    DEBUG_PRINT("Settings: %u\n", settings);
    
    DEBUG_PRINT("Check Time1: %u, Time2: %u\n", time1_check, time2_check);
    DEBUG_PRINT("Check Alarm1: %u, Alarm2: %u\n", alarm1_check, alarm2_check);
    DEBUG_PRINT("Check Settings: %u\n", settings_check);
    
    programSuccess();
}

bool checkStart() {
    int output = 0;
    int i;
    int tmpBit;
    for(i = 0; i < 8; i++) {
        tmpBit = readPCBit();
        output = output << 1;
        output |= tmpBit;
        if((output & 0b00111111) == 0b00101011) {
            // DEBUG_PRINT("Start match: %i\n", output);
            pmLED(true);
            return true;
        }
    }
    // DEBUG_PRINT("Start false match: %i\n", output);
    return false;  
}

uint8_t readPCWord() {
    uint8_t output = 0;
    int i;
    uint8_t tmpBit;
    for(i = 0; i < 8; i++) {
        tmpBit = readPCBit();
        output = output << 1;
        output |= tmpBit;
    }
    return output;
}

void programFailed() {
    enablePCInt();
    // DEBUG_PRINT("Programming failed\n");
}

void programSuccess() {
    enablePCInt();
    // DEBUG_PRINT("Programming success\n");
}

int readPCBit() {
    int avgCount = 0;
    int value = 0;
    int i;
    // _delay_ms(24);
    _delay_ms(40);
    for(i = 0; i < 8; i++) {
        value = SENSOR_PIN & SENSOR_OPTICAL_MASK;
        if(value) {
            avgCount++;
        }
        _delay_ms(PC_SAMPLE);
    }
    _delay_ms(40);
    if(avgCount > 4) {
        return 1;
    } else {        
        return 0; 
    }
}

void disablePCInt() {
    alarmLED(true);
    // PORTB |= 0x80;
    // EIMSK_def &= ~EIMSK_OPTIC_VALUE;
    // DEBUG_PRINT("PC Int disabled\n");
}

void enablePCInt() {
    alarmLED(false);
    pmLED(false);
    // PORTB &= ~0x80;
    // EIMSK_def |= EIMSK_OPTIC_VALUE;
    // DEBUG_PRINT("PC Int enabled\n");
}

void disableIRInt() {
    // EIMSK_def &= ~EIMSK_IR_VALUE;
    // DEBUG_PRINT("IRInt disabled\n");
}

void enableIRInt() {
    // EIMSK_def |= EIMSK_IR_VALUE;
    // DEBUG_PRINT("IR Int enabled\n");
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