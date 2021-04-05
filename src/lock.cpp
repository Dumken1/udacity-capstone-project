#include "lock.h"
#include <vector>

unsigned int password;

void unlock(background& a){
    for (size_t i = 0; i < 5; i++)
    {
        a.SetColor(i, Color::GREEN);
    }
}
void lock(background& a){
    for (size_t i = 0; i < 5; i++)
    {
        a.SetColor(i, Color::RED);
    }
}