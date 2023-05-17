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

inline Instruction *createPOP(string opcode)
{
    return new POP(opcode);
}

inline Instruction *createXCHG(string opcode)
{
    return new XCHG(opcode);
}

inline Instruction *createIN(string opcode)
{
    return new IN(opcode);
}

inline Instruction *createOUT(string opcode)
{
    return new OUT(opcode);
}

inline Instruction *createXLAT(string opcode)
{
    return new XLAT(opcode);
}

inline Instruction *createLEA(string opcode)
{
    return new LEA(opcode);
}

inline Instruction *createLDS(string opcode)
{
    return new LDS(opcode);
}

inline Instruction *createLES(string opcode)
{
    return new LES(opcode);
}

inline Instruction *createLAHF(string opcode)
{
    return new LAHF(opcode);
}

inline Instruction *createSAHF(string opcode)
{
    return new SAHF(opcode);
}

inline Instruction *createPUSHF(string opcode)
{
    return new PUSHF(opcode);
}

inline Instruction *createPOPF(string opcode)
{
    return new POPF(opcode);
}

inline Instruction *checkConflict(string opcode)
{
    Conflict *inst = new Conflict(opcode);
    inst->size = 4;
    inst->setType(ConflictTypesEnum::BIT7_1);
    return inst;
}

inline Instruction *createNOT(string opcode)
{
    return new NOT(opcode);
}

inline Instruction *createShift(string opcode)
{
    Conflict *inst = new Conflict(opcode);
    inst->size = 4;
    inst->setType(ConflictTypesEnum::SHIFT);
    return inst;
}

inline Instruction *createSHL(string opcode)
{
    return new SHL(opcode);
}

inline Instruction *createSHR(string opcode)
{
    return new SHR(opcode);
}

inline Instruction *createSAR(string opcode)
{
    return new SAR(opcode);
}

inline Instruction *createROL(string opcode)
{
    return new ROL(opcode);
}

inline Instruction *createROR(string opcode)
{
    return new ROR(opcode);
}

inline Instruction *createRCL(string opcode)
{
    return new RCL(opcode);
}

inline Instruction *createRCR(string opcode)
{
    return new RCR(opcode);
}

inline Instruction *createAND(string opcode)
{
    return new AND(opcode);
}

inline Instruction *createTEST(string opcode)
{
    return new TEST(opcode);
}

inline Instruction *createOR(string opcode)
{
    return new OR(opcode);
}

inline Instruction *createXOR(string opcode)
{
    return new XOR(opcode);
}

inline Instruction *createREP(string opcode)
{
    return new REP(opcode);
}

inline Instruction *createMOVS(string opcode)
{
    return new MOVS(opcode);
}

inline Instruction *createCMPS(string opcode)
{
    return new CMPS(opcode);
}

inline Instruction *createSCAS(string opcode)
{
    return new SCAS(opcode);
}

inline Instruction *createLODS(string opcode)
{
    return new LODS(opcode);
}

inline Instruction *createSTOS(string opcode)
{
    return new STOS(opcode);
}

inline Instruction *createCALL(string opcode)
{
    return new CALL(opcode);
}

extern map<string, Instruction *(*)(string)> instructionMap;

// createMOV

#endif
