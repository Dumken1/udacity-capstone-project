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
    std::shared_ptr<Background> I_background{std::make_shared<Background>()};
    I_background->points_init();

    lock(*I_background);
    std::thread th1(&Background::BackgroundSimulate, I_background);

    Energy IEnergy(*I_background);
    Light ILight(*I_background, IEnergy);
    Weather Iweather(*I_background, IEnergy);
    Heater IHeater(*I_background, IEnergy, Iweather);
    
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
            lock(*I_background);
            I_background->BackgroundSimulate();
        }

        if(weather_simulation == 1)
        {
            Iweather._weather_state = w_state::SNOW;
            unlock(*I_background);
            break;
        }
        else if (weather_simulation == 2)
        {
            Iweather._weather_state = w_state::RAIN;
            unlock(*I_background);
            break;
        }
        else
        {       
            Iweather._weather_state = w_state::SUN;
            unlock(*I_background);
            break;
        }
    }

    std::thread th2(&Energy::simulate, IEnergy);
    std::thread th3(&Light::simulate, ILight);
    std::thread th4(&Background::TimeofDaySimulation, I_background);
    std::thread th5(&Weather::simulate, Iweather);
    std::thread th6(&Heater::Simulate, IHeater);

    //main loop simulates the background
    while(1){  
        I_background->BackgroundSimulate();  
    } 
    

    th1.join();
    th2.join();
    th3.join();
    th4.join();
    th5.join();
    th6.join();

    return 0;
}