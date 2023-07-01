#include "processor.hpp"

int16_t Register::getRegister()
{
    return (int16_t)((high << 8) | low);
}