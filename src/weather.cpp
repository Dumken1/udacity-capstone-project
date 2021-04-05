#include <thread>
#include <chrono>
#include <random>

#include "weather.h"

std::mutex wea_mtx;
std::condition_variable wea_cond;

unsigned int Weather::_tempValue{15};
w_state Weather::_weather_state;

Weather::Weather(Background &background, Energy &energy): _Obackground{background}, _Oenergy{energy}
{
    _Obackground.SetColor(1, Color::RED);
}

Weather::~Weather(){

}

bool Weather::Incr_Temp(unsigned int value, unsigned int incr_val){
    std::unique_lock<std::mutex> wea_lck(wea_mtx);
    if (_tempValue < value)
    {
        _tempValue += incr_val;
        return true;
    }
    return false;
}

bool Weather::Decr_Temp(unsigned int value, unsigned int decr_val){
    std::unique_lock<std::mutex> wea_lck(wea_mtx);
    if (_tempValue > value)
    {
        _tempValue -= decr_val;
        return true;
    } 
    return false;
}

void Weather::print(){
    std::unique_lock<std::mutex> lck(g_mtx);
    std::cout << "Temperature value is " << _tempValue << "\n";
}

void Weather::update(){
        if(get_Temp() < 24){
            _Obackground.SetColor(0, Color::GREEN);
        }
        if(get_Temp() >= 24){
            _Obackground.SetColor(0, Color::RED);
        }
}

void Weather::change(){
    if (_weather_state == w_state::RAIN)
    {
        unsigned int val = 15;
        unsigned int change_val = 1;
        if(Decr_Temp(val, change_val)){
            std::unique_lock<std::mutex> lck(g_mtx);
            std::cout << "Temperature decreased\n";
            lck.unlock();
            _Obackground.SetColor(1, Color::GREEN);
            print();
        }
    }
    
    else if (_weather_state == w_state::SNOW)
    {
        _Obackground.SetColor(1, Color::WHITE);
        int val = 0;
        unsigned int change_val = 1;
        if(Decr_Temp(val, change_val)){
            std::unique_lock<std::mutex> lck(g_mtx);
            std::cout << "Temperature decreased\n";
            lck.unlock();
            _Obackground.SetColor(1, Color::WHITE);
            print();
        } 
    }
    else
    {
        int val = 30;
        unsigned int change_val = 1;
        if(Incr_Temp(val, change_val)){
            std::unique_lock<std::mutex> lck(g_mtx);
            std::cout << "Temperature Increased\n";
            lck.unlock();
            _Obackground.SetColor(1, Color::ORANGE);
            print();
        } 
        else if (Incr_Temp(val, change_val) == false && Timeflag == false)
        {
            _Obackground.SetColor(1, Color::BLACK);
        }
        else{
            _Obackground.SetColor(1, Color::ORANGE);
        }
    }
    Weather::update();
}

void Weather::simulate(){  // thread to simulate weather 
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
            change();
        }
        lastUpdate = std::chrono::system_clock::now();
    }    
}