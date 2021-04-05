#include "energy.h"
#include <iostream>

std::mutex ene_mtx;
std::condition_variable ene_cond;

Energy::Energy(Background &background): _background{background}
{
    _background.SetColor(3, Color::RED);
}

Energy::~Energy()
{

}

//initial Energy value
unsigned int Energy::_energy{30}; 

unsigned int Energy::GetEnergy(){
    return _energy;
}

void Energy::PrintEnergy(){
    std::unique_lock<std::mutex> lck(g_mtx);
    std::cout << _energy << "KWS" << std::endl;
}


void Energy::SupplyEnergy(){
    _background.SetColor(3, Color::GREEN);
    std::unique_lock<std::mutex> ene_lck(ene_mtx);
    ene_cond.wait(ene_lck, [this] (){return this->_energy < 250;});
    if(Timeflag == true){
        _energy += 10; // Supply Energy
        PrintEnergy();
        _background.SetColor(3, Color::YELLOW);
        std::unique_lock<std::mutex> lck(g_mtx);
        std::cout << "Energy Supplied" << std::endl;
        lck.unlock();
    }
    ene_lck.unlock();
    ene_cond.notify_all();
}


void Energy::simulate(){
    while (true)
    {
        std::chrono::time_point<std::chrono::system_clock> lastUpdate;
        double cycleDuration = 1;

        lastUpdate = std::chrono::system_clock::now();
        while (true)
        {
            // Supply Energy every one second
            std::this_thread::sleep_for(std::chrono::seconds(1));

        // compute time difference to stop watch
            long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - lastUpdate).count();
            if (timeSinceLastUpdate >= cycleDuration){
                SupplyEnergy();
            }
        lastUpdate = std::chrono::system_clock::now();
        }
    }
}