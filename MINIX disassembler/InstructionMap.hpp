#ifndef IMAP
#define IMAP

#include <map>
#include <string>
#include "Instruction.hpp"
using namespace std;

// a map of strings to objects of Instruction type
// this will be available in the main file
inline Instruction *createMOV(string opcode)
{
    return new MOV(opcode);
}
extern map<string, Instruction *(*)(string)> instructionMap;

// createMOV

#endif
