#include "InstructionMap.hpp"

map<string, Instruction *(*)(string)> instructionMap = {
    {"100010", &createMOV},
    {"1100011", &createMOV},
    {"1011", &createMOV},
    {"1010000", &createMOV},
    {"1010001", &createMOV},
    {"10001110", &createMOV},
    {"10001100", &createMOV}};
