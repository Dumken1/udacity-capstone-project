#include <thread>
#include <chrono>
#include "weather.h"

std::mutex wea_mtx;
std::condition_variable wea_cond;

unsigned int weather::_tempValue{17};
w_state weather::_weather_state = w_state::SUN;

weather::weather(background &b, energy &o_EL): c{b}, _EL{o_EL}
{
    c.SetColor(2, Color::ORANGE);

}

weather::~weather()
{

}


bool weather::Incr_Temp(unsigned int value, unsigned int incr_val){
    std::unique_lock<std::mutex> wea_lck(wea_mtx);
    if (_tempValue < value)
    {
        _tempValue += incr_val;
        return true;
    }
    return false;
}

bool weather::Decr_Temp(unsigned int value, unsigned int decr_val){
    std::unique_lock<std::mutex> wea_lck(wea_mtx);
    if (_tempValue > value)
    {
        _tempValue -= decr_val;
        return true;
    } 
    return false;
}

void weather::print(){
    std::cout << "Temperature value is " << _tempValue << "\n";
}

void weather::update(){
        if(get_Temp() < 24){
            c.SetColor(1, Color::GREEN);
        }
        if(get_Temp() >= 24){
            c.SetColor(1, Color::RED);
        }
        
        std::unique_lock<std::mutex> ene_lck(ene_mtx);
        ene_cond.wait(ene_lck, [this] (){return _EL._energy > 0;});
        _EL._energy -= _temp_consumption;  
        std::cout << "Energy Reduced from Weather\n";
        _EL.PrintEnergy();
        ene_lck.unlock();
        ene_cond.notify_all();
}

void weather::change(){
    if (_weather_state == w_state::RAIN)
    {
        unsigned int val = 15;
        unsigned int change_val = 1;
        if(Decr_Temp(val, change_val)){
            std::cout << "Temperature decreased\n";
            c.SetColor(2, Color::GREEN);
            print();
        }
    }
    else if (_weather_state == w_state::SNOW)
    {
        int val = 0;
        unsigned int change_val = 1;
        if(Decr_Temp(val, change_val)){
            std::cout << "Temperature decreased\n";
            c.SetColor(2, Color::WHITE);
            print();
        } 
    }
    else
    {
        int val = 30;
        unsigned int change_val = 1;
        if(Incr_Temp(val, change_val)){
            std::cout << "Temperature Increased\n";
            c.SetColor(2, Color::ORANGE);
            print();
        } 
    }
    weather::update();
}

void weather::simulate(){  // thread to simulate weather effect
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    double cycleDuration = 1;

    lastUpdate = std::chrono::system_clock::now();
    while (true)
    {
        // sleep at every iteration to reduce CPU usage
        std::this_thread::sleep_for(std::chrono::seconds(5));

        // compute time difference to stop watch
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if (timeSinceLastUpdate >= cycleDuration)
        {   
            std::unique_lock<std::mutex> lck(mtx);
            change();
        }

        lastUpdate = std::chrono::system_clock::now();
    }    
}