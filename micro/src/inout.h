/* 
 * File:   inout.h
 * Author: Tim
 *
 * Created on 7 August 2012, 6:08 PM
 */

#ifndef INOUT_H
    #define INOUT_H
    #include <stdio.h>
    #include <stdbool.h>
    #include "pins.h"
    #define RX_BUFSIZE 80
    #define UART_BAUD  9600

    typedef enum {
        HIGH = 1,
        LOW = 0
    } IOBit;

    typedef enum {
        INPUT = 0x00,
        OUTPUT = 0xFF,
    } IOMode;
    
    int latchAddressTranslate[16];
    
    void setupSerial();
    
    void clearPort(Port);
    void clearMultiPins(Port, int);
    
    void setupArray();
    void clearArray();
    void setArray(int, IOMode);
    
    void alarmLED(bool on);
    void buzzer(bool on);
    void pmLED(bool on);
    
#endif	/* INOUT_H */

