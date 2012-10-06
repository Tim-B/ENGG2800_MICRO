#ifndef DISPLAY_H
#define DISPLAY_H
    #include "timeutil.h"
    void updateDisplay();
    void setHourDisplay();
    void setMinuteDisplay();
    void setPMDisplay();
    void toggle();
    void setBuzzerOn(IOBit value);
    void updateWeather(Weather value);
    
#endif	/* INOUT_H */

