/* 
 * File:   pins.h
 * Author: Tim
 *
 * Created on 7 August 2012, 6:08 PM
 */
#include <stdio.h>
#ifndef TCONTROL_H
    #define TCONTROL_H
    #include <stdbool.h>
    void setupControl();
    int readCommand();
    uint8_t readPCWord();
    void programFailed();
    void programSuccess();
    uint8_t readPCBit();
    void disablePCInt();
    void enablePCInt();
    void disableIRInt();
    void enableIRInt();
    void incrementPM();
    void processIncrement();
    void processToggleAlarm();
    void processProgressTime();
    void processProgressAlarm();
    void incrementInner();
    void incrementOuter();
    void displayVal(int value);
    void setNewTime(uint32_t);
    bool isProgramming();
    bool checkStart();
    void processCancel();
    void incrementHour();
    void cycle();
    void pcIncomming();
    
    typedef enum {
        NO_COMMAND = 0,
        TIME_INNER_MINUTE = 1,
        TIME_OUTER_MINUTE = 2,
        TIME_HOUR = 3,
        ALARM_INNER_MINUTE = 4,
        ALARM_OUTER_MINUTE = 5,
        ALARM_HOUR = 6,
    } Stage;

#endif	/* TCONTROL_H */
