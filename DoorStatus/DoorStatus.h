#ifndef DOORSTATUS_H
#define DOORSTATUS_H

#include "mbed.h"



class DoorStatus {


public:
    /**
     * @brief 
     */
    DoorStatus(PinName pin);

    DoorStatus();

    /**
     * @brief Handle the event about door status.
     * @param func handle of function.
     */
    void handleCallback(Callback<void()> func);
private:
    InterruptIn *interruptPin;

};






#endif