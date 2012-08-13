/* 
 * File:   pins.h
 * Author: Tim
 *
 * Created on 7 August 2012, 6:08 PM
 */

#ifndef TPINS_H
    #define TPINS_H

    #include <avr/io.h>

    typedef enum {
        A = 0,
        B = 1,
        C = 2,
        D = 3,
        E = 4,
        F = 5,   
        G = 6,
        H = 7,
        J = 9,
        K = 10        
    } Port;

    typedef struct {
        uint8_t pin;
        Port port; 
    } Pin;

    Pin pins[32];
    int hourPins[12];
    int minPins[4];

    volatile uint8_t * directionPorts[11];

    volatile uint8_t * dataPorts[11];
    
    void setupPins();

#endif	/* TPINS_H */

