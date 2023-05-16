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
inline Instruction *createPUSH(string opcode)
{
    return new PUSH(opcode);
}
inline Instruction *createADD(string opcode)
{
    return new ADD(opcode);
}
inline Instruction *createADC(string opcode)
{
    return new ADC(opcode);
}

inline Instruction *createINC(string opcode)
{
    return new INC(opcode);
}

inline Instruction *createAAA(string opcode)
{
    return new AAA(opcode);
}

inline Instruction *createBAA(string opcode)
{
    return new BAA(opcode);
}

inline Instruction *createSUB(string opcode)
{
    return new SUB(opcode);
}

inline Instruction *createSSB(string opcode)
{
    return new SSB(opcode);
}

inline Instruction *createDEC(string opcode)
{
    return new DEC(opcode);
}

inline Instruction *createNEG(string opcode)
{
    return new NEG(opcode);
}

inline Instruction *createCMP(string opcode)
{
    return new CMP(opcode);
}

inline Instruction *createAAS(string opcode)
{
    return new AAS(opcode);
}

inline Instruction *createDAS(string opcode)
{
    return new DAS(opcode);
}

inline Instruction *createMUL(string opcode)
{
    return new MUL(opcode);
}

inline Instruction *createAAM(string opcode)
{
    return new AAM(opcode);
}

inline Instruction *createDIV(string opcode)
{
    return new DIV(opcode);
}

inline Instruction *createAAD(string opcode)
{
    return new AAD(opcode);
}

inline Instruction *createCBW(string opcode)
{
    return new CBW(opcode);
}

inline Instruction *createCWD(string opcode)
{
    return new CWD(opcode);
}

extern map<string, Instruction *(*)(string)> instructionMap;

// createMOV

#endif
