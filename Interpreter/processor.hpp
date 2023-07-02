#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP
#include "defines.hpp"
#include <string>
#include <vector>
#include "workMemory.hpp"

class Register
{
public:
    int8_t high;
    int8_t low;

public:
    int16_t getRegister();
    void add(int16_t value);
    void sub(int16_t value);
    void mul(int16_t value);
    void div(int16_t value);
};

class Flags
{
public:
    /*
    ## Status Flags (6 flags):

    - CF (Carry): Indicates the carry after an addition or the borrow after a subtraction.
    - PF (Parity): Indicates the parity of the result.
    - AF (Auxiliary): Indicates the carry (half carry) between bit positions 3 and 4 of the result.
    - ZF (Zero): Indicates if the result of an arithmetic or logical operation is zero.
    - SF (Sign): Indicates the sign of the result of an arithmetic/logical instruction.
    - OF (Overflow): Indicates the overflow when signed numbers are added or subtracted.

    ## Control Flags (3 flags):

    - TF (Trap): Enables program debugging through an internal debugging feature.
    - IF (Interrupt): Controls the operation of the interrupt request (INTR).
    - DF (Direction): Selects the increment or decrement mode for the DI and/or SI registers during
    */

    bool CF;
    bool PF;
    bool AF;
    bool ZF;
    bool SF;
    bool OF;
    bool TF;
    bool IF;
    bool DF;

public:
    string getFlags();
};

class Processor
{
    /*
    ## General Purpose Registers (16 bits):

    - AX (Accumulator)
    - BX (Base)
    - CX (Counter)
    - DX (Data)
    - SP (Stack Pointer)
    - BP (Base Pointer)
    - SI (Source Index)
    - DI (Destination Index)

    ## Segment Registers (16 bits):

    - CS (Code Segment)
    - DS (Data Segment)
    - ES (Extra Segment)
    - SS (Stack Segment)
    */

public:
    Register AX;
    Register BX;
    Register CX;
    Register DX;
    Register SP;
    Register BP;
    Register SI;
    Register DI;
    Register CS;
    Register DS;
    Register ES;
    Register SS;
    Register IP;
    Flags flags;
    AssemblyCode lastInstruction;

public:
    Processor();
    string getStateHeader();
    string getState();
    void run(WorkMemoryTape work_memory, vector<int8_t> *data_memory);
    void execute(AssemblyCode instruction, vector<int8_t> *data_memory);
    int16_t getRegisterValue(string reg);

public:
    // instruções
    void mov(string op1, string op2, vector<int8_t> *data_memory);
    void interrupt(vector<int8_t> *data_memory);
};

#endif // PROCESSOR_HPP