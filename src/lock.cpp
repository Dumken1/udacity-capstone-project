#include "lock.h"
#include <vector>


void unlock(Background& background){
    for (size_t i = 0; i < 5; i++)
    {
        background.SetColor(i, Color::GREEN);
    }
}
void lock(Background& backgound){
    for (size_t i = 0; i < 5; i++)
    {
        backgound.SetColor(i, Color::RED);
    }
}