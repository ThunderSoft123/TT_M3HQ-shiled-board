#include "PirSensor.h"



#if defined(__TMPM4G9__)
#define DEFAULT_DOR_PIN PL0
#endif


#if defined(__TMPM3HQ__)
#define DEFAULT_DOR_PIN PK7
#endif


PirSensor::PirSensor()
{
     interruptPin = new InterruptIn(DEFAULT_DOR_PIN);
}

PirSensor::PirSensor(PinName pin)
{
    interruptPin = new InterruptIn(pin);
}

void PirSensor::handleCallback(Callback<void()> func)
{
    interruptPin->fall(func);
}