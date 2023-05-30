#ifndef IMAP
#define IMAP

#include <map>
#include <string>
#include "Instruction.hpp"
using namespace std;

// a map of strings to objects of Instruction type
// this will be available in the main file
inline Instruction *createMOV(string opcode, string buffer)
{
    return new MOV(opcode, buffer);
}
inline Instruction *createPUSH(string opcode, string buffer)
{
    return new PUSH(opcode, buffer);
}
inline Instruction *createADD(string opcode, string buffer)
{
    return new ADD(opcode, buffer);
}
inline Instruction *createADC(string opcode, string buffer)
{
    return new ADC(opcode, buffer);
}

inline Instruction *createINC(string opcode, string buffer)
{
    return new INC(opcode, buffer);
}

inline Instruction *createAAA(string opcode, string buffer)
{
    return new AAA(opcode, buffer);
}

inline Instruction *createBAA(string opcode, string buffer)
{
    return new BAA(opcode, buffer);
}

inline Instruction *createSUB(string opcode, string buffer)
{
    return new SUB(opcode, buffer);
}

inline Instruction *createSBB(string opcode, string buffer)
{
    return new SBB(opcode, buffer);
}

inline Instruction *createDEC(string opcode, string buffer)
{
    return new DEC(opcode, buffer);
}

inline Instruction *createNEG(string opcode, string buffer)
{
    return new NEG(opcode, buffer);
}

inline Instruction *createCMP(string opcode, string buffer)
{
    return new CMP(opcode, buffer);
}

inline Instruction *createAAS(string opcode, string buffer)
{
    return new AAS(opcode, buffer);
}

inline Instruction *createDAS(string opcode, string buffer)
{
    return new DAS(opcode, buffer);
}

inline Instruction *createMUL(string opcode, string buffer)
{
    return new MUL(opcode, buffer);
}
inline Instruction *createIMUL(string opcode, string buffer)
{
    return new IMUL(opcode, buffer);
}

inline Instruction *createAAM(string opcode, string buffer)
{
    return new AAM(opcode, buffer);
}

inline Instruction *createDIV(string opcode, string buffer)
{
    return new DIV(opcode, buffer);
}
inline Instruction *createIDIV(string opcode, string buffer)
{
    return new IDIV(opcode, buffer);
}

inline Instruction *createAAD(string opcode, string buffer)
{
    return new AAD(opcode, buffer);
}

inline Instruction *createCBW(string opcode, string buffer)
{
    return new CBW(opcode, buffer);
}

inline Instruction *createCWD(string opcode, string buffer)
{
    return new CWD(opcode, buffer);
}

inline Instruction *createPOP(string opcode, string buffer)
{
    return new POP(opcode, buffer);
}

inline Instruction *createXCHG(string opcode, string buffer)
{
    return new XCHG(opcode, buffer);
}

inline Instruction *createIN(string opcode, string buffer)
{
    return new IN(opcode, buffer);
}

inline Instruction *createOUT(string opcode, string buffer)
{
    return new OUT(opcode, buffer);
}

inline Instruction *createXLAT(string opcode, string buffer)
{
    return new XLAT(opcode, buffer);
}

inline Instruction *createLEA(string opcode, string buffer)
{
    return new LEA(opcode, buffer);
}

inline Instruction *createLDS(string opcode, string buffer)
{
    return new LDS(opcode, buffer);
}

inline Instruction *createLES(string opcode, string buffer)
{
    return new LES(opcode, buffer);
}

inline Instruction *createLAHF(string opcode, string buffer)
{
    return new LAHF(opcode, buffer);
}

inline Instruction *createSAHF(string opcode, string buffer)
{
    return new SAHF(opcode, buffer);
}

inline Instruction *createPUSHF(string opcode, string buffer)
{
    return new PUSHF(opcode, buffer);
}

inline Instruction *createPOPF(string opcode, string buffer)
{
    return new POPF(opcode, buffer);
}

inline Instruction *checkConflictBit7(string opcode, string buffer)
{
    Conflict *inst = new Conflict(opcode, buffer);
    inst->size = 4;
    inst->setType(ConflictTypesEnum::BIT7_1);
    return inst;
}

inline Instruction *createNOT(string opcode, string buffer)
{
    return new NOT(opcode, buffer);
}

inline Instruction *createShift(string opcode, string buffer)
{
    Conflict *inst = new Conflict(opcode, buffer);
    inst->size = 4;
    inst->setType(ConflictTypesEnum::SHIFT);
    return inst;
}

inline Instruction *createSHL(string opcode, string buffer)
{
    return new SHL(opcode, buffer);
}

inline Instruction *createSHR(string opcode, string buffer)
{
    return new SHR(opcode, buffer);
}

inline Instruction *createSAR(string opcode, string buffer)
{
    return new SAR(opcode, buffer);
}

inline Instruction *createROL(string opcode, string buffer)
{
    return new ROL(opcode, buffer);
}

inline Instruction *createROR(string opcode, string buffer)
{
    return new ROR(opcode, buffer);
}

inline Instruction *createRCL(string opcode, string buffer)
{
    return new RCL(opcode, buffer);
}

inline Instruction *createRCR(string opcode, string buffer)
{
    return new RCR(opcode, buffer);
}

inline Instruction *createAND(string opcode, string buffer)
{
    return new AND(opcode, buffer);
}

inline Instruction *createTEST(string opcode, string buffer)
{
    return new TEST(opcode, buffer);
}

inline Instruction *createOR(string opcode, string buffer)
{
    return new OR(opcode, buffer);
}

inline Instruction *createXOR(string opcode, string buffer)
{
    return new XOR(opcode, buffer);
}

inline Instruction *createREP(string opcode, string buffer)
{
    return new REP(opcode, buffer);
}

inline Instruction *createMOVS(string opcode, string buffer)
{
    return new MOVS(opcode, buffer);
}

inline Instruction *createCMPS(string opcode, string buffer)
{
    return new CMPS(opcode, buffer);
}

inline Instruction *createSCAS(string opcode, string buffer)
{
    return new SCAS(opcode, buffer);
}

inline Instruction *createLODS(string opcode, string buffer)
{
    return new LODS(opcode, buffer);
}

inline Instruction *createSTOS(string opcode, string buffer)
{
    return new STOS(opcode, buffer);
}

inline Instruction *createCALL(string opcode, string buffer)
{
    return new CALL(opcode, buffer);
}

inline Instruction *checkConflictAll1(string opcode, string buffer)
{
    Conflict *inst = new Conflict(opcode, buffer);
    inst->size = 2;
    inst->setType(ConflictTypesEnum::ALL_1);
    return inst;
}

inline Instruction *checkConflictTestMulOthers(string opcode, string buffer)
{
    Conflict *inst = new Conflict(opcode, buffer);
    inst->size = 2;
    inst->setType(ConflictTypesEnum::TEST_MUL_OTHERS);
    inst->name = "CONFLICT_TEST_MUL_OTHERS";
    return inst;
}

inline Instruction *createJMP(string opcode, string buffer)
{
    return new JMP(opcode, buffer);
}

inline Instruction *createRET(string opcode, string buffer)
{
    return new RET(opcode, buffer);
}

inline Instruction *createJE(string opcode, string buffer)
{
    return new JE(opcode, buffer);
}

inline Instruction *createJL(string opcode, string buffer)
{
    return new JL(opcode, buffer);
}

inline Instruction *createJLE(string opcode, string buffer)
{
    return new JLE(opcode, buffer);
}

inline Instruction *createJB(string opcode, string buffer)
{
    return new JB(opcode, buffer);
}

inline Instruction *createJBE(string opcode, string buffer)
{
    return new JBE(opcode, buffer);
}

inline Instruction *createJP(string opcode, string buffer)
{
    return new JP(opcode, buffer);
}

inline Instruction *createJO(string opcode, string buffer)
{
    return new JO(opcode, buffer);
}

inline Instruction *createJS(string opcode, string buffer)
{
    return new JS(opcode, buffer);
}

inline Instruction *createJNE(string opcode, string buffer)
{
    return new JNE(opcode, buffer);
}

inline Instruction *createJNL(string opcode, string buffer)
{
    return new JNL(opcode, buffer);
}

inline Instruction *createJNLE(string opcode, string buffer)
{
    return new JNLE(opcode, buffer);
}

inline Instruction *createJNB(string opcode, string buffer)
{
    return new JNB(opcode, buffer);
}

inline Instruction *createJNBE(string opcode, string buffer)
{
    return new JNBE(opcode, buffer);
}

inline Instruction *createJNP(string opcode, string buffer)
{
    return new JNP(opcode, buffer);
}

inline Instruction *createJNO(string opcode, string buffer)
{
    return new JNO(opcode, buffer);
}

inline Instruction *createJNS(string opcode, string buffer)
{
    return new JNS(opcode, buffer);
}

inline Instruction *createLOOP(string opcode, string buffer)
{
    return new LOOP(opcode, buffer);
}

inline Instruction *createLOOPZ(string opcode, string buffer)
{
    return new LOOPZ(opcode, buffer);
}

inline Instruction *createLOOPNZ(string opcode, string buffer)
{
    return new LOOPNZ(opcode, buffer);
}

inline Instruction *createJCXZ(string opcode, string buffer)
{
    return new JCXZ(opcode, buffer);
}

inline Instruction *createINT(string opcode, string buffer)
{
    return new INT(opcode, buffer);
}

inline Instruction *createINTO(string opcode, string buffer)
{
    return new INTO(opcode, buffer);
}

inline Instruction *createIRET(string opcode, string buffer)
{
    return new IRET(opcode, buffer);
}

inline Instruction *createCLC(string opcode, string buffer)
{
    return new CLC(opcode, buffer);
}

inline Instruction *createCMC(string opcode, string buffer)
{
    return new CMC(opcode, buffer);
}

inline Instruction *createSTC(string opcode, string buffer)
{
    return new STC(opcode, buffer);
}

inline Instruction *createCLD(string opcode, string buffer)
{
    return new CLD(opcode, buffer);
}

inline Instruction *createSTD(string opcode, string buffer)
{
    return new STD(opcode, buffer);
}

inline Instruction *createCLI(string opcode, string buffer)
{
    return new CLI(opcode, buffer);
}

inline Instruction *createSTI(string opcode, string buffer)
{
    return new STI(opcode, buffer);
}

inline Instruction *createHLT(string opcode, string buffer)
{
    return new HLT(opcode, buffer);
}

inline Instruction *createWAIT(string opcode, string buffer)
{
    return new WAIT(opcode, buffer);
}

inline Instruction *createESC(string opcode, string buffer)
{
    return new ESC(opcode, buffer);
}

inline Instruction *createLOCK(string opcode, string buffer)
{
    return new LOCK(opcode, buffer);
}

extern map<string, Instruction *(*)(string, string)> instructionMap;

// createMOV

#endif
