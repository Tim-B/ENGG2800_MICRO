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
#define TIME_PROGRESS_CODE 147
#define ALARM_PROGRESS_CODE 163
#define INCREMENT_CODE 131
#define CANCEL_CODE 171
#define TOGGLE_ALARM 155

bool programmingActive = false;
int tmpValue = 0;
Stage stage = NO_COMMAND;
uint32_t newTime = 0;
bool IRwaiting = false;

void setupControl() {
    commandWaiting = 0;
    SENSOR_PORT &= ~SENSOR_DDR_MASK;
    EICR_def |= EICR_value;
    EIMSK_def |= EIMSK_OPTIC_VALUE;
    EIMSK_def |= EIMSK_IR_VALUE;
    DEBUG_PRINT("interrupt setup\n");
}

void printArray(int *value, int total) {
    int i;
    for (i = 0; i < total; i++) {
        DEBUG_PRINT("Value: %i\n", value[i]);
    }
}

ISR(INT0_vect) {
    disableIRInt();
    IRwaiting= true;
}

void IRIncomming() {
    int code = readCommand();
    if (code == 255) {
        IRwaiting = false;
        enableIRInt();
        
        // DEBUG_PRINT("wrong code mate\n");
        return;
    }
    switch (code) {
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
        case CANCEL_CODE:
            processCancel();
            break;
    }
    _delay_ms(1000);
    IRwaiting = false;
    enableIRInt();
}

void processIncrement() {
    tmpValue++;
    switch (stage) {
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
            incrementHour();
            break;
        case ALARM_INNER_MINUTE:
            incrementInner();
            break;
        case ALARM_OUTER_MINUTE:
            incrementOuter();
            break;
        case ALARM_HOUR:
            incrementHour();
            break;
    }
}

void incrementInner() {
    if (tmpValue > 4) {
        tmpValue = 0;
    }
    if (tmpValue == 0) {
        clearArray();
    } else {
        displayVal(tmpValue + 11);
    }
}

void incrementOuter() {
    if (tmpValue > 11) {
        tmpValue = 0;
    }
    displayVal(tmpValue);

}

void incrementHour() {
    DEBUG_PRINT("TMPVal: %i\n", tmpValue);
    if (tmpValue > 23) {
        tmpValue = 1;
    }
    int displayIndex = tmpValue - 1;
    if (tmpValue < 12) {
        displayVal(displayIndex);
        pmLED(false);
    } else {
        displayVal(displayIndex - 11);
        pmLED(true);
    }
}

void displayVal(int value) {
    DEBUG_PRINT("Display: %i\n", value);
    clearArray();
    setArray(value, HIGH);
}

void processToggleAlarm() {
    if (alarmActive()) {
        setAlarmActive(false);
        alarmLED(false);
        DEBUG_PRINT("alarm inactive\n");
    } else {
        setAlarmActive(true);
        alarmLED(true);
        DEBUG_PRINT("alarm active\n");
    }
}

void processProgressTime() {
    int tmpNewTime = 0;
    switch (stage) {
        case NO_COMMAND:
            DEBUG_PRINT("Programming time\n");
            stage = TIME_INNER_MINUTE;
            tmpValue = 0;
            newTime = 0;
            displayVal(0);
            break;
        case TIME_INNER_MINUTE:
            stage = TIME_OUTER_MINUTE;
            setNewTime(60 * tmpValue);
            tmpValue = 0;
            break;
        case TIME_OUTER_MINUTE:
            stage = TIME_HOUR;
            tmpNewTime = tmpValue + 1;
            if(tmpNewTime > 11) {
                tmpNewTime = 0;
            }
            setNewTime(60 * 5 * tmpNewTime);
            tmpValue = 1;
            displayVal(tmpValue);
            break;
        case TIME_HOUR:
            stage = NO_COMMAND;
            setNewTime(60 * 60 * tmpValue);
            setTime(newTime);
            refresh();
            DEBUG_PRINT("New time: %lu\n", newTime);
            break;
        default:
            break;
    }
}

void setNewTime(uint16_t mult) {
    newTime += mult;
    DEBUG_PRINT("Time: %lu\n", newTime);
}

void processProgressAlarm() {
    switch (stage) {
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
            stage = NO_COMMAND;
            setNewTime(60 * 60);
            setAlarm(newTime);
            refresh();
            updateDisplay();
            break;
        default:
            break;
    }
}

void processCancel() {
    if (stage == NO_COMMAND) {
        return;
    }
    stage = NO_COMMAND;
    refresh();
    updateDisplay();
    DEBUG_PRINT("Programming cancelled\n");
}

bool isProgramming() {
    return !(stage == NO_COMMAND);
}

ISR(INT1_vect) {
    if (!checkStart()) {
        programFailed();
        // DEBUG_PRINT("prigram not thing\n");
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

    uint8_t end_check = readPCWord();

    if ((~time1_check & 0x00FF) != time1) {
        DEBUG_PRINT("Time check 1 failed: %u %u\n", ~time1_check, time1);
        programFailed();
        return;
    }

    if ((~time2_check & 0x00FF) != time2) {
        DEBUG_PRINT("Time check 2 failed\n");
        programFailed();
        return;
    }

    if ((~alarm1_check & 0x00FF) != alarm1) {
        DEBUG_PRINT("Alarm check 1 failed\n");
        programFailed();
        return;
    }

    if ((~alarm2_check & 0x00FF) != alarm2) {
        DEBUG_PRINT("Alarm check 2 failed\n");
        programFailed();
        return;
    }

    if ((~settings_check & 0x00FF) != settings) {
        DEBUG_PRINT("Settings failed failed\n");
        programFailed();
        return;
    }
    
    
    if ((~end_check & 0x00FF) != 0b01101011) {
        DEBUG_PRINT("End check failed\n");
        programFailed();
        return;
    }

    uint32_t newTime = ((uint32_t) time1 << 8) | ((uint32_t) time2);
    uint32_t newAlarm = ((uint32_t) alarm1 << 8) | ((uint32_t) alarm2);

    setTime(newTime);
    refresh();
    updateDisplay();

    if (settings & 0x01) {
        setAlarm(newAlarm);
    }

    if (settings & 0x02) {
        setAlarmActive(1);
    } else {
        setAlarmActive(0);
    }

    int weather = (settings & 0x18) >> 3;
    switch (weather) {
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
    DEBUG_PRINT("Check end: %u\n", end_check);

    programSuccess();
}

bool checkStart() {
    int output = 0;
    int i;
    int tmpBit;
    for (i = 0; i < 8; i++) {
        tmpBit = readPCBit();
        output = output << 1;
        output |= tmpBit;
        if ((output & 0b00111111) == 0b00101011) {
            // DEBUG_PRINT("Start match: %i\n", output);
            return true;
        }
    }
    DEBUG_PRINT("Start false match: %i\n", output);
    return false;
}

uint8_t readPCWord() {
    uint8_t output = 0;
    int i;
    uint8_t tmpBit;
    for (i = 0; i < 8; i++) {
        tmpBit = readPCBit();
        output = output << 1;
        output |= tmpBit;
    }
    return output;
}

void programFailed() {
    // DEBUG_PRINT("PC Programming failed\n");
}

void programSuccess() {

}

uint8_t readPCBit() {
    int avgCount = 0;
    int value = 0;
    int i;
    // _delay_ms(24);
    _delay_ms(41);
    for (i = 0; i < 8; i++) {
        value = SENSOR_PIN & SENSOR_OPTICAL_MASK;
        if (value) {
            avgCount++;
        }
        _delay_ms(PC_SAMPLE);
    }
    _delay_ms(40);
    if (avgCount > 4) {
        return 0;
    } else {
        return 1;
    }
}

void disablePCInt() {
    EIMSK_def &= ~EIMSK_OPTIC_VALUE;
    // DEBUG_PRINT("PC Int disabled\n");
}

void enablePCInt() {
    EIMSK_def |= EIMSK_OPTIC_VALUE;
    // DEBUG_PRINT("PC Int enabled\n");
}

void disableIRInt() {
    EIMSK_def &= ~EIMSK_IR_VALUE;
    // DEBUG_PRINT("IRInt disabled\n");
}

void enableIRInt() {
    EIMSK_def |= EIMSK_IR_VALUE;
    // DEBUG_PRINT("IR Int enabled\n");
}

int readCommand() {
    int command = 0;
    int highCount = 0;
    int i, k;
    for (i = 0; i < 8; i++) {
        highCount = 0;
        _delay_us(100);
        for (k = 0; k < 8; k++) {
            if (SENSOR_PIN & SENSOR_IR_MASK) {
                highCount++;
            }
            _delay_us(175);
        }
        _delay_us(100);
        command = command << 1;
        if (highCount > 4) {
            command |= 1;
        }
    }
    return command;
}

void cycle() {
    if (IRwaiting) {
        IRIncomming();
    }
}