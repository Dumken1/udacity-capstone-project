#ifndef _HEATER_H_
#define _HEATER_H_

#include <iostream>
#include "graphics.h"
#include "energy.h"
#include "weather.h"

class heater
{
private:
    background &c;
    energy &_EL;
    weather &_WL;
    unsigned int heater_consumption = 10;

public:
    heater(background &e, energy &o_EL, weather &o_WL);
    ~heater();
    void StartHeating();
    void Simulate();
};


#endif // !HEATER

