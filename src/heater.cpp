#include "heater.h"


heater::heater(background &e, energy &o_EL, weather &o_WL): c{e}, _EL{o_EL}, _WL{o_WL}
{
    std::cout << "Heating instantiated\n";
}

heater::~heater()
{


}

void heater::StartHeating()
{
    int val = 25;
    unsigned int change_val = 1;
    if (_WL._weather_state == w_state::SNOW)
    {
        _WL.Incr_Temp(val,change_val);
        std::cout << "Heating the Room\n";
        c.SetColor(8, Color::YELLOW);
        if(_WL._tempValue >= 24)
        {
            c.SetColor(8, Color::RED);
        }
        std::unique_lock<std::mutex> ene_lck(ene_mtx);
        ene_cond.wait(ene_lck, [this] (){return _EL._energy > 0;});
        _EL._energy -= heater_consumption;  
        std::cout << "Energy Reduced from Weather\n";
        _EL.PrintEnergy();
        ene_lck.unlock();
        ene_cond.notify_all();
    }
}

void heater::Simulate(){
        std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    double cycleDuration = 1;

    lastUpdate = std::chrono::system_clock::now();
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if (timeSinceLastUpdate >= cycleDuration)
        {   
            StartHeating();
        }

        lastUpdate = std::chrono::system_clock::now();
    }   
}