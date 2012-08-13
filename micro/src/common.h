/* 
 * File:   common.h
 * Author: Tim
 *
 * Created on 7 August 2012, 6:58 PM
 */

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(a, ...) printf(a, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(a, ...) (void)0
#endif


#ifndef COMMON_H
    #define COMMON_H

    #include <stdio.h>
    
    void setup();
    void loop();

#endif	/* COMMON_H */

