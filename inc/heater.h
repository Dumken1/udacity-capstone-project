#ifndef _HEATER_H_
#define _HEATER_H_

#include <iostream>
#include <memory>
#include "graphics.h"
#include "energy.h"
#include "weather.h"

class Heater
{
private:
    Background &_Obackground;
    Energy &_Oenergy;
    Weather &_Oweather;
    const unsigned int heater_consumption = 10;

public:
    Heater(Background &background, Energy &energy, Weather &weather);
    ~Heater();
    void StartHeating(unsigned int heater_consp);
    void Simulate();
};


#endif // !HEATER

