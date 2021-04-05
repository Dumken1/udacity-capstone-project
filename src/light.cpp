#include <iostream>
#include "light.h"
#include "graphics.h"
#include "energy.h"

light::light(background &l, energy &o_EL): _L{l}, _EL{o_EL}
{
    std::cout << "Light instantiated" << "\n";
}

light::~light()
{

}

void light::simulate(){
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
                std::unique_lock<std::mutex> lck(mtx);
                if (Timeflag == false){LightControl(state::ON);}
                else {LightControl(state::OFF);}
                lck.unlock();
            }
            lastUpdate = std::chrono::system_clock::now();
        }
    }   
}

state light::LightControl(state l_control){
    if (l_control == state::OFF)
    {
        _L.SetColor(4, Color::WHITE);
        _Light_State = state::OFF;
        std::cout << "light off\n";
        return _Light_State;
    }
    else{
        _L.SetColor(4, Color::ORANGE);
        _Light_State = state::ON;
        std::unique_lock<std::mutex> ene_lck(ene_mtx);
        ene_cond.wait(ene_lck, [this] (){return _EL._energy > 0;});
        if(Timeflag == false){
            _EL._energy -= _light_consump;  
            _EL.PrintEnergy();
            std::cout << "Energy Reduced\n";
        }
        ene_lck.unlock();
        ene_cond.notify_all();

        //_EL.CheckEnergy(_light_consump);
        std::cout << "light on\n";
        return _Light_State;
    }
    return state::ERROR;
}