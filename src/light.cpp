#include <iostream>
#include "light.h"
#include "graphics.h"
#include "energy.h"

Light::Light(Background &background, Energy &energy): _Obackground{background}, _Oenergy{energy}
{
    _Obackground.SetColor(2, Color::RED);
}

Light::~Light()
{

}

void Light::simulate(){
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
                if (Timeflag == false){LightControl(state::ON);}
                else {LightControl(state::OFF);}
            }
            lastUpdate = std::chrono::system_clock::now();
        }
    }   
}


state Light::LightControl(state l_control){
    if (l_control == state::OFF)
    {
        _Obackground.SetColor(2, Color::WHITE);
        _Light_State = state::OFF;
        std::unique_lock<std::mutex> lck(g_mtx);
        std::cout << "light off\n";
        lck.unlock();
        return _Light_State;
    }
    else{
        _Obackground.SetColor(2, Color::ORANGE);
        _Light_State = state::ON;
        std::unique_lock<std::mutex> ene_lck(ene_mtx);
        ene_cond.wait(ene_lck, [this] (){return _Oenergy._energy > 0;});
        if(Timeflag == false){
            _Oenergy._energy -= _light_consump;  
            _Oenergy.PrintEnergy();
            std::unique_lock<std::mutex> lck(g_mtx);
            std::cout << "Energy Reduced\n";
            lck.unlock();
        }
        ene_lck.unlock();
        ene_cond.notify_all();

        std::cout << "light on\n";
        return _Light_State;
    }
    return state::ERROR;
}