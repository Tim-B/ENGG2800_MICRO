/* 
 * File:   inout.h
 * Author: Tim
 *
 * Created on 7 August 2012, 6:08 PM
 */

#ifndef INOUT_H
    #define INOUT_H
    #include <stdio.h>
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

    void setPinMode(int, IOMode);
    void setPin(int, IOBit);
    int getPin(int);

    void setMultiPinMode(Port, int);
    void setMultiPin(Port, int);
    
    void setupSerial();
    
    void clearPort(Port);
    void clearMultiPins(Port, int);
    
    void setupArray();
    void clearArray();
    void setArray(int, IOMode);
    
#endif	/* INOUT_H */

