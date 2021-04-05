#include "lock.h"
#include <vector>

unsigned int password;

void unlock(background& a){
    std::unique_lock<std::mutex> lck(mtx);
    for (size_t i = 0; i < 9; i++)
    {
        a.SetColor(i, Color::GREEN);
    }
}
void lock(background& a){
    std::unique_lock<std::mutex> lck(mtx);
    for (size_t i = 0; i < 9; i++)
    {
        a.SetColor(i, Color::RED);
    }
}