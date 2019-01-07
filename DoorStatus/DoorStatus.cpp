#include "DoorStatus.h"




#define DEFAULT_DOR_PIN PK7


DoorStatus::DoorStatus()
{
     interruptPin = new InterruptIn(DEFAULT_DOR_PIN);
}

DoorStatus::DoorStatus(PinName pin)
{
    interruptPin = new InterruptIn(pin);
}

void DoorStatus::handleCallback(Callback<void()> func)
{
    interruptPin->fall(func);
}