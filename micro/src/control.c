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
bool pcWaiting = false;

/**
 * Initializes the control component, sets the data direction registers
 * on both the sensors and configures interrupts.
 */
void setupControl() {
    commandWaiting = 0;
    SENSOR_PORT &= ~SENSOR_DDR_MASK;
    EICR_def |= EICR_value;
    EIMSK_def |= EIMSK_OPTIC_VALUE;
    EIMSK_def |= EIMSK_IR_VALUE;
    DEBUG_PRINT("interrupt setup\n");
}


/**
 * Debugging function used for outputting an array
 * @param value an array to be printed.
 * @param total The total number of elements in the array.
 */
void printArray(int *value, int total) {
    int i;
    for (i = 0; i < total; i++) {
        DEBUG_PRINT("Value: %i\n", value[i]);
    }
}

/**
 * Interrupt vector for IR proramming.
 */
ISR(INT0_vect) {
    disableIRInt();
    IRwaiting = true;
}

/**
 * Processes an incoming IR request, run when the IRwaiting flag is true.
 */
void IRIncomming() {
    int code = readCommand();
    if (code == 255) {
        IRwaiting = false;
        enableIRInt();
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


/**
 * Processes an increment of the current IR value.
 */
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

/**
 * Increments the inner 4 minute LEDs, starts at 0 (in which case no LEDs
 * are displayed).
 */
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

/**
 * Increments the outer ring to display minutes while programming (ie. 5 minute
 * blocks).
 */
void incrementOuter() {
    if (tmpValue > 11) {
        tmpValue = 0;
    }
    displayVal(tmpValue);

}

/**
 * Increments the hour display, includes displaying times greater than
 * midday in which case the PM LED is illuminated.
 */
void incrementHour() {
    // DEBUG_PRINT("TMPVal: %i\n", tmpValue);
    if (tmpValue > 23) {
        tmpValue = 0;
    }
    int displayIndex = tmpValue;
    if (tmpValue < 12) {
        displayVal(displayIndex);
        pmLED(false);
    } else {
        displayVal(displayIndex - 12);
        pmLED(true);
    }
}

/**
 * Clears the LED array then sets an index to high.
 * @param value The LED index to be illuminated.
 */
void displayVal(int value) {
    DEBUG_PRINT("Display: %i\n", value);
    clearArray();
    setArray(value, HIGH);
}

/**
 * Toggles the alarm setting including displaying the alarm value on the
 * alarm LED.
 */
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

/**
 * Handles the IR time programming progressing to the next "stage", or entering
 * or exiting depending on where in the programming sequence the clock 
 * currently is.
 */
void processProgressTime() {
    int tmpNewTime = 0;
    switch (stage) {
        case NO_COMMAND:
            DEBUG_PRINT("Programming time\n");
            stage = TIME_INNER_MINUTE;
            tmpValue = 0;
            newTime = 0;
            clearArray();
            break;
        case TIME_INNER_MINUTE:
            stage = TIME_OUTER_MINUTE;
            setNewTime((uint32_t) 60 * tmpValue);
            tmpValue = 0;
            displayVal(tmpValue);
            break;
        case TIME_OUTER_MINUTE:
            stage = TIME_HOUR;
            tmpNewTime = tmpValue;
            if(tmpNewTime > 11) {
                tmpNewTime = 0;
            }
            setNewTime((uint32_t) 60 * 5 * tmpNewTime);
            tmpValue = 0;
            displayVal(tmpValue);
            break;
        case TIME_HOUR:
            stage = NO_COMMAND;
            setNewTime((uint32_t) 60 * 60 * tmpValue);
            setTime(newTime);
            refresh();
            DEBUG_PRINT("New time: %lu\n", newTime);
            break;
        default:
            break;
    }
}

/**
 * Adds time to the temporary new time used to build a time up over the
 * stages of the programming sequence.
 * @param mult The value to be added.
 */
void setNewTime(uint32_t mult) {
    newTime = newTime + mult;
    DEBUG_PRINT("Time: %lu\n", newTime);
}

/**
 * Handles the IR alarm time programming progressing to the next "stage", or entering
 * or exiting depending on where in the programming sequence the clock 
 * currently is.
 */
void processProgressAlarm() {
    int tmpNewTime = 0;
    switch (stage) {
       case NO_COMMAND:
            DEBUG_PRINT("Programming alarm\n");
            stage = ALARM_INNER_MINUTE;
            tmpValue = 0;
            newTime = 0;
            clearArray();
            break;
        case ALARM_INNER_MINUTE:
            stage = ALARM_OUTER_MINUTE;
            setNewTime((uint32_t) 60 * tmpValue);
            tmpValue = 0;
            displayVal(tmpValue);
            break;
        case ALARM_OUTER_MINUTE:
            stage = ALARM_HOUR;
            tmpNewTime = tmpValue;
            if(tmpNewTime > 11) {
                tmpNewTime = 0;
            }
            setNewTime((uint32_t) 60 * 5 * tmpNewTime);
            tmpValue = 0;
            displayVal(tmpValue);
            break;
        case ALARM_HOUR:
            stage = NO_COMMAND;
            setNewTime((uint32_t) 60 * 60 * tmpValue);
            setAlarm(newTime);
            refresh();
            DEBUG_PRINT("New alarm: %lu\n", newTime);
            break;
        default:
            break;
    }
}

/**
 * Cancels the current IR programming process
 */
void processCancel() {
    if (stage == NO_COMMAND) {
        return;
    }
    stage = NO_COMMAND;
    refresh();
    updateDisplay();
    DEBUG_PRINT("Programming cancelled\n");
}

/**
 * Returns whether the clock is in programming mode.
 * @return true if in programming mode, false otherwise
 */
bool isProgramming() {
    return !(stage == NO_COMMAND);
}

/**
 * Optical interrupt
 */
ISR(INT1_vect) {
    disablePCInt();
    pcWaiting = true;
}

/**
 * Processes an incoming PC programming request. Runs when the pcIncomming
 * flag is true.
 */
void pcIncomming() {
    // updateWeather(SUNNY);
    
    if (!checkStart()) {
        // updateWeather(RAINY);
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

    setTime(newTime * 60);
    
    if (settings & 0x80) {
        setAlarm(newAlarm * 60);
        DEBUG_PRINT("Alarm set\n");
    }

    if (settings & 0x40) {
        setAlarmActive(1);
        DEBUG_PRINT("Alarm active\n");
    } else {
        setAlarmActive(0);
    }

    if(settings & 0x20) {
        int weather = (settings & 0x18) >> 3;
        DEBUG_PRINT("Weather: %i\n", weather);
        switch (weather) {
            case 0x00:
                setWeather(SUNNY);
                DEBUG_PRINT("Sunneh\n");
                break;
            case 0x01:
                setWeather(CLOUDY);
                DEBUG_PRINT("Cloudeh\n");
                break;
            case 0x02:
                DEBUG_PRINT("Raineh\n");
                setWeather(RAINY);
                break;
            default:
                setWeather(NONE);
                break;
        }
    }
    
    
    refresh();

    DEBUG_PRINT("Time1: %u, Time2: %u, Full: %lu\n", time1, time2, newTime);
    DEBUG_PRINT("Alarm1: %u, Alarm2: %u, Full: %lu\n", alarm1, alarm2, newAlarm);
    DEBUG_PRINT("Settings: %u\n", settings);

    DEBUG_PRINT("Check Time1: %u, Time2: %u\n", time1_check, time2_check);
    DEBUG_PRINT("Check Alarm1: %u, Alarm2: %u\n", alarm1_check, alarm2_check);
    DEBUG_PRINT("Check Settings: %u\n", settings_check);
    DEBUG_PRINT("Check end: %u\n", end_check);
    
    
    // updateWeather(NONE);
    programSuccess();
}

/**
 * Checks for a valid start sequence. Ignores the first two bits and checks
 * the last six for a match. Returns as soon as a match is found regardless
 * of if 8 bits have been read.
 * @return true if a match found, false otherwise
 */
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

/**
 * Reads 8 bits from the optical input
 * @return an integer representation of the 8 buts read
 */
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

/**
 * Indicates programming failed and enables optical interrupts again
 */
void programFailed() {
    // DEBUG_PRINT("PC Programming failed\n");
    pcWaiting = false;
    enablePCInt();
}

/**
 * Indicates programming was successful and enables optical interrupts again
 */
void programSuccess() {
    pcWaiting = false;
    enablePCInt();
}

/**
 * Reads a bit from the optical input. Takes 8 readings and averages them
 * to determine the value
 * @return 1 if high, 0 otherwise
 */
uint8_t readPCBit() {
    int avgCount = 0;
    int value = 0;
    int i;
    // _delay_ms(24);
    _delay_ms(43);
    for (i = 0; i < 8; i++) {
        value = SENSOR_PIN & SENSOR_OPTICAL_MASK;
        if (value) {
            avgCount++;
        }
        _delay_ms(PC_SAMPLE);
    }
    _delay_ms(43);
    if (avgCount > 4) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Disables the optical interrupt
 */
void disablePCInt() {
    EIMSK_def &= ~EIMSK_OPTIC_VALUE;
    // DEBUG_PRINT("PC Int disabled\n");
}

/**
 * Enables the optical interrupt
 */
void enablePCInt() {
    EIMSK_def |= EIMSK_OPTIC_VALUE;
    // DEBUG_PRINT("PC Int enabled\n");
}

/**
 * Disables the IR interrupt
 */
void disableIRInt() {
    EIMSK_def &= ~EIMSK_IR_VALUE;
    // DEBUG_PRINT("IRInt disabled\n");
}

/**
 * Enables the IR interrupt
 */
void enableIRInt() {
    EIMSK_def |= EIMSK_IR_VALUE;
    // DEBUG_PRINT("IR Int enabled\n");
}

/**
 * Reads 8 bits from the IR input, takes the average of 8 readings for each
 * bit.
 * @return an 8 bit integer representation of the code. 
 */
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

/**
 * Checks if there is an incoming IR or optical signal and if so trigger
 * the processing.
 */
void cycle() {
    if (IRwaiting) {
        IRIncomming();
    }
    if (pcWaiting) {
        pcIncomming();
    }
}