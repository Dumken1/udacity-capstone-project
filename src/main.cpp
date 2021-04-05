#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include "graphics.h"
#include "weather.h"
#include "lock.h"
#include "energy.h"
#include "light.h"
#include "heater.h"

int main() {
    unsigned int weather_simulation;
    std::shared_ptr<background> me{std::make_shared<background>()};
    me->points_init();

    lock(*me);
    std::thread th1(&background::BackgroundSimulate, me);

    energy e_me(*me);
    light l_me(*me, e_me);
    weather w_me(*me, e_me);
    heater h_me(*me, e_me, w_me);
    
    while (true)
    {
        while (!(std::cin) || weather_simulation < 1 || weather_simulation > 3) {
            std::cout << "Please enter 1 for Snow.\n";
            std::cout << "Please enter 2 for Rain.\n";
            std::cout << "Please enter 3 for SUN.\n";

            std::cout << "Enter Number: ";
            std::cin >> weather_simulation;

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            lock(*me);
            me->BackgroundSimulate();
        }

        if(weather_simulation == 1){
            w_me._weather_state = w_state::SNOW;
            unlock(*me);
            break;
        }
        else if (weather_simulation == 2)
        {
            w_me._weather_state = w_state::RAIN;
            unlock(*me);
            break;
        }
        else
        {       
            w_me._weather_state = w_state::SUN;
            unlock(*me);
            break;
        }
    }

    std::thread th2(&energy::EnergySimulate, e_me);
    std::thread th3(&light::simulate, l_me);
    std::thread th4(&background::TimeofDaySimulation, me);
    std::thread th5(&weather::simulate, w_me);
    std::thread th6(&heater::Simulate, h_me);

    while(1){  
        me->BackgroundSimulate();  
    } 
    
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();

    return 0;
}