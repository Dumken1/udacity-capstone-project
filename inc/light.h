#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "energy.h"
#include "graphics.h"

enum state{OFF, ON, ERROR};

class light
{
private:
    state _Light_State = state::ERROR; 
    background &_L;
    energy &_EL;
    const unsigned int _light_consump = 2;
    
public:
    light(background &l, energy &o_EL);
    ~light();
    void simulate();
    state LightControl(state l_control);
};

#endif // !_LIGHT_H_
