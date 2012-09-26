/* 
 * File:   pins.h
 * Author: Tim
 *
 * Created on 7 August 2012, 6:08 PM
 */

#ifndef TCONTROL_H
    #define TCONTROL_H
    #include <stdbool.h>
    void setupControl();
    int readCommand();
    int readPCWord();
    void programFailed();
    void programSuccess();
    int readPCBit();
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
    void setNewTime();
    bool isProgramming();
    
    typedef enum {
        NO_COMMAND = 0,
        TIME_INNER_MINUTE = 1,
        TIME_OUTER_MINUTE = 2,
        TIME_HOUR = 3,
        TIME_PM = 4,
        ALARM_INNER_MINUTE = 5,
        ALARM_OUTER_MINUTE = 6,
        ALARM_HOUR = 7,
        ALARM_PM = 8
    } Stage;

#endif	/* TCONTROL_H */
