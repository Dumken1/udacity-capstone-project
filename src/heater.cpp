#include "heater.h"


Heater::Heater(Background &background, Energy &energy, Weather &weather): 
                        _Obackground{background}, _Oenergy{energy}, _Oweather{weather}
{
    _Obackground.SetColor(4, Color::RED);
}

Heater::~Heater()
{


}

void Heater::StartHeating(unsigned int heater_consum)
{
    int val = 25;
    unsigned int change_val = 1;
    if (_Oweather._weather_state == w_state::SNOW)
    {
        _Oweather.Incr_Temp(val,change_val);
        std::cout << "Heating the Room\n";
        _Obackground.SetColor(4, Color::YELLOW);
        if(_Oweather._tempValue >= 24)
        {
            _Obackground.SetColor(4, Color::RED);
        }
        std::unique_lock<std::mutex> ene_lck(ene_mtx);
        ene_cond.wait(ene_lck, [this] (){return _Oenergy._energy > 0;});
        _Oenergy._energy -= heater_consum;  
        std::cout << "Energy Reduced from Weather\n";
        _Oenergy.PrintEnergy();
        ene_lck.unlock();
        ene_cond.notify_all();
    }
}

void Heater::Simulate(){
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
            StartHeating(heater_consumption);
        }

        lastUpdate = std::chrono::system_clock::now();
    }   
}