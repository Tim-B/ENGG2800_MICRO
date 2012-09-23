#include "control.h"
#include "common.h"
#include "inout.h"
#include "timeutil.h"
#include <avr/interrupt.h>  
#include <util/delay.h>
#define PC_RATE 80
#define PC_SAMPLE 10

void setupControl() {
    commandWaiting = 0;
    SENSOR_PORT &= ~SENSOR_DDR_MASK;
    EICR_def |= EICR_value;
    EIMSK_def |= EIMSK_IR_VALUE | EIMSK_OPTIC_VALUE;
    DEBUG_PRINT("interrupt setup\n");
    sei();
}

ISR(INT0_vect) {
    if(commandWaiting) {
        return;
    }
    commandWaiting = 1;
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

void readCommand() {
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
    if(command == 157) {
        setArray(2, HIGH);
    }
    if(command == 137) {
        clearArray();
    }
    DEBUG_PRINT("%i\n", command);
    _delay_ms(3000);
    commandWaiting = 0;
}