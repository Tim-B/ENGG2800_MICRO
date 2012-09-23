/* 
 * File:   pins.h
 * Author: Tim
 *
 * Created on 7 August 2012, 6:08 PM
 */

#ifndef TCONTROL_H
    #define TCONTROL_H

    void setupControl();
    void readCommand();
    int readPCWord();
    void programFailed();
    void programSuccess();
    int readPCBit();
    void disablePCInt();
    void enablePCInt();

#endif	/* TCONTROL_H */
