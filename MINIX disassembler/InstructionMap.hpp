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
inline Instruction *createIMUL(string opcode)
{
    return new IMUL(opcode);
}

inline Instruction *createAAM(string opcode)
{
    return new AAM(opcode);
}

inline Instruction *createDIV(string opcode)
{
    return new DIV(opcode);
}
inline Instruction *createIDIV(string opcode)
{
    return new IDIV(opcode);
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

inline Instruction *checkConflictBit7(string opcode)
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

inline Instruction *checkConflictAll1(string opcode)
{
    Conflict *inst = new Conflict(opcode);
    inst->size = 2;
    inst->setType(ConflictTypesEnum::ALL_1);
    return inst;
}

inline Instruction *checkConflictTestMulOthers(string opcode)
{
    Conflict *inst = new Conflict(opcode);
    inst->size = 2;
    inst->setType(ConflictTypesEnum::TEST_MUL_OTHERS);
    return inst;
}

inline Instruction *createJMP(string opcode)
{
    return new JMP(opcode);
}

inline Instruction *createRET(string opcode)
{
    return new RET(opcode);
}

inline Instruction *createJE(string opcode)
{
    return new JE(opcode);
}

inline Instruction *createJL(string opcode)
{
    return new JL(opcode);
}

inline Instruction *createJLE(string opcode)
{
    return new JLE(opcode);
}

inline Instruction *createJB(string opcode)
{
    return new JB(opcode);
}

inline Instruction *createJBE(string opcode)
{
    return new JBE(opcode);
}

inline Instruction *createJP(string opcode)
{
    return new JP(opcode);
}

inline Instruction *createJO(string opcode)
{
    return new JO(opcode);
}

inline Instruction *createJS(string opcode)
{
    return new JS(opcode);
}

inline Instruction *createJNE(string opcode)
{
    return new JNE(opcode);
}

inline Instruction *createJNL(string opcode)
{
    return new JNL(opcode);
}

inline Instruction *createJNLE(string opcode)
{
    return new JNLE(opcode);
}

inline Instruction *createJNB(string opcode)
{
    return new JNB(opcode);
}

inline Instruction *createJNBE(string opcode)
{
    return new JNBE(opcode);
}

inline Instruction *createJNP(string opcode)
{
    return new JNP(opcode);
}

inline Instruction *createJNO(string opcode)
{
    return new JNO(opcode);
}

inline Instruction *createJNS(string opcode)
{
    return new JNS(opcode);
}

inline Instruction *createLOOP(string opcode)
{
    return new LOOP(opcode);
}

inline Instruction *createLOOPZ(string opcode)
{
    return new LOOPZ(opcode);
}

inline Instruction *createLOOPNZ(string opcode)
{
    return new LOOPNZ(opcode);
}

inline Instruction *createJCXZ(string opcode)
{
    return new JCXZ(opcode);
}

inline Instruction *createINT(string opcode)
{
    return new INT(opcode);
}

inline Instruction *createINTO(string opcode)
{
    return new INTO(opcode);
}

inline Instruction *createIRET(string opcode)
{
    return new IRET(opcode);
}

extern map<string, Instruction *(*)(string)> instructionMap;

// createMOV

#endif
