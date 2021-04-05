#ifndef _WEATHER_H_
#define _WEATHER_H_

#include <stdint.h>
#include "graphics.h"
#include "energy.h"

enum w_state{SUN, SNOW, RAIN};

extern std::mutex wea_mtx;
extern std::condition_variable wea_cond;

class weather
{
private:
    background &c;
    energy &_EL;
    const unsigned int _temp_consumption = 4;
    
public:
    static unsigned int _tempValue;
    static w_state _weather_state;

    weather(background &b, energy &o_EL);
    ~weather();
    bool Incr_Temp(unsigned int value, unsigned int incr_val);
    bool Decr_Temp(unsigned int value, unsigned int decr_val);
    void update();
    void print();
    void change();
    void simulate();
    unsigned int get_Temp() const{return _tempValue;};
};


#endif // !_WEATHER_H_
