#ifndef PIRSENSOR_H
#define PIRSENSOR_H

#include "mbed.h"



class PirSensor {


public:
    /**
     * @brief 
     */
    PirSensor(PinName pin);

    PirSensor();

    /**
     * @brief Handle the event about door status.
     * @param func handle of function.
     */
    void handleCallback(Callback<void()> func);
private:
    InterruptIn *interruptPin;

};






#endif