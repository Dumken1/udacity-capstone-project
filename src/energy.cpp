#include "energy.h"
#include <iostream>

std::mutex ene_mtx;
std::condition_variable ene_cond;

energy::energy(background &e): _e{e}
{
    e.SetColor(3, Color::RED);
}

energy::~energy()
{

}

unsigned int energy::_energy{30};

unsigned int energy::GetEnergy(){
    return _energy;
}

void energy::PrintEnergy(){
    std::unique_lock<std::mutex> lck(g_mtx);
    std::cout << /*GetEnergy*/_energy << "KWS" << std::endl;
}

void energy::SupplyEnergy(){
    _e.SetColor(3, Color::GREEN);
    std::unique_lock<std::mutex> ene_lck(ene_mtx);
    ene_cond.wait(ene_lck, [this] (){return this->_energy < 250;});
    if(Timeflag == true){
        _energy += 10;
        PrintEnergy();
        _e.SetColor(3, Color::YELLOW);
        std::unique_lock<std::mutex> lck(g_mtx);
        std::cout << "Energy Supplied" << std::endl;
        lck.unlock();
    }
    ene_lck.unlock();
    ene_cond.notify_all();
}


void energy::EnergySimulate(){
    while (true)
    {
        std::chrono::time_point<std::chrono::system_clock> lastUpdate;
        double cycleDuration = 1;

        lastUpdate = std::chrono::system_clock::now();
        while (true)
        {
        // sleep at every iteration to reduce CPU usage
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