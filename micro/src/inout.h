/* 
 * File:   inout.h
 * Author: Tim
 *
 * Created on 7 August 2012, 6:08 PM
 */

#ifndef INOUT_H
    #define INOUT_H

    #include "pins.h"

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

    void setMultiPinMode(Port, int);
    void setMultiPin(Port, int);
    
    void setupSerial();
    void serialPrint(char*);
    void serialPrintLn(char*);
    void serialPutChar(char);
    void clearPort(Port);
    void clearMultiPins(Port, int);
    
#endif	/* INOUT_H */

