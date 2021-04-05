#include <iostream>
#include <thread>
#include <chrono>

#include "graphics.h"
#include "weather.h"
#include "lock.h"
#include "energy.h"
#include "light.h"
#include "heater.h"

int main() {
    unsigned int password = 0;
    background me;
    me.points_init();

    lock(me);
    std::thread th1(&background::BackgroundSimulate, me);
    
    while (true)
    {
        std::cout << "Enter your Password: ";
        std::cin >> password;
        if(password == 12345){
            unlock(me);
            break;
        }
        else
        {       
            lock(me);
            me.BackgroundSimulate();
            std::cout << "Password wrong\n";
        }
    }

    energy e_me(me);
    light l_me(me, e_me);
    weather w_me(me, e_me);
    heater h_me(me, e_me, w_me);

    std::thread th2(&energy::EnergySimulate, e_me);
    std::thread th3(&light::simulate, l_me);
    std::thread th4(&background::TimeofDaySimulation, me);
    std::thread th5(&weather::simulate, w_me);
    std::thread th6(&heater::Simulate, h_me);

    while(1){  
        me.BackgroundSimulate();  
    } 
    
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();

    return 0;
}