#ifndef _ENERGY_H_
#define _ENERGY_H_

#include <condition_variable>
#include "graphics.h"


/* mutex to control Energy consumption  relationship 
Producer-comsumer relationship with 
light and heater */
extern std::mutex ene_mtx; 
extern std::condition_variable ene_cond;

class Energy
{
private:
    Background &_background; 
public:
    static unsigned int _energy;

    Energy(Background &background);
    ~Energy();

    void PrintEnergy();
    void simulate();
    unsigned int GetEnergy();
    void SupplyEnergy();
};

#endif // !_ENERGY_H_
