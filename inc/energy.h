#ifndef _ENERGY_H_
#define _ENERGY_H_

#include <condition_variable>
#include "graphics.h"

extern std::mutex ene_mtx;
extern std::condition_variable ene_cond;
extern unsigned int g_energylevel;



class energy
{
private:
    background &_e;
    bool solar;   
public:
    static unsigned int _energy;

    energy(background &e);
    ~energy();
    //energy& operator=(const energy &RHS);

    void PrintEnergy();
    void EnergySimulate();
    unsigned int GetEnergy();
    bool CheckEnergy(int value);
    void SetEnergy(int value);
    void SupplyEnergy();
};

#endif // !_ENERGY_H_
