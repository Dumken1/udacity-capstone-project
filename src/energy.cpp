#include "energy.h"
#include <iostream>

std::mutex ene_mtx;
std::condition_variable ene_cond;

energy::energy(background &e): _e{e}
{
    e.SetColor(6, Color::RED);
}

energy::~energy()
{

}

unsigned int energy::_energy{30};

unsigned int energy::GetEnergy(){
    return _energy;
}

void energy::PrintEnergy(){
    std::cout << /*GetEnergy*/_energy << "KWS" << std::endl;
}

void energy::SetEnergy(int e_value){
    _energy = e_value;
    std::cout << "Energy Set to "  << /*GetEnergy*/_energy << "KWS" << std::endl;
    _e.SetColor(6, Color::GREEN);
}

void energy::SupplyEnergy(){
    _e.SetColor(6, Color::GREEN);
    std::unique_lock<std::mutex> ene_lck(ene_mtx);
    ene_cond.wait(ene_lck, [this] (){return this->_energy < 250;});
    if(Timeflag == true){
        _energy += 10;
        PrintEnergy();
        _e.SetColor(6, Color::YELLOW);
        std::cout << "Energy Supplied" << std::endl;
    }
    ene_lck.unlock();
    ene_cond.notify_all();
    /*else
    {
        std::cout << "Enter Energy Value: ";
        int temp_eval = 0;
        std::cin >> temp_eval;
        SetEnergy(temp_eval);
    }*/
}

bool energy::CheckEnergy(int value){
    std::unique_lock<std::mutex> ene_lck(ene_mtx);
    ene_cond.wait(ene_lck, [this] (){return this->_energy > 0;});
    if(Timeflag==false){
        _energy -= value;  
        PrintEnergy();
        std::cout << "Energy Reduced\n";
    }
    ene_lck.unlock();
    ene_cond.notify_one();
    return true; 
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