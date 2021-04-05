#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "energy.h"
#include "graphics.h"

enum state{OFF, ON, ERROR};

class Light
{
private:
    state _Light_State = state::ERROR; 
    Background &_Obackground;
    Energy &_Oenergy;
    const unsigned int _light_consump = 2;

public:
    Light(Background &background, Energy &energy);
    ~Light();
    void simulate();
    state LightControl(state l_control);
};

#endif // !_LIGHT_H_
