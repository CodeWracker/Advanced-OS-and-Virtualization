#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP
#include "defines.hpp"
#include <string>
#include <vector>
#include "workMemory.hpp"

class Register
{
public:
    uint8_t high;
    uint8_t low;

public:
    uint16_t getRegister();
    void add(uint16_t value);
    void sub(uint16_t value);
    void mul(uint16_t value);
    void div(uint16_t value);
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
    bool jump;

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
private:
    void handle_operators_for_algebra(string op1, string op2, uint16_t *addr1, uint16_t *word1, bool *dest_is_addr, uint16_t *addr2, uint16_t *word2, bool *src_is_addr);

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
    WorkMemoryTape work_memory;
    vector<uint8_t> physical_memory;

public:
    Processor();
    void load_memories(WorkMemoryTape work_memory_tape, vector<uint8_t> data_memory_tape);
    string getStateHeader();
    string getState();
    void run();
    void execute(AssemblyCode instruction);
    uint16_t getRegisterValue(string reg);
    void setRegisterValue(string reg, uint16_t value);
    void load_args(int argc, char *argv[]);
    void put_on_stack(uint16_t value);

public:
    // instruções
    void mov_(string op1, string op2);
    void int_();
    void xor_(string op1, string op2);
    void sub_(string op1, string op2);
    void add_(string op1, string op2);
    void pop_(string op1);
    void dec_(string op1);
    void hlt_();
    void jne_(string op1);
    void lea_(string op1, string op2);
    void cmp_(string op1, string op2);
    void jnb_(string op1);
    void test_(string op1, string op2);
    void push_(string op1);
    void call_(string op1);
};

#endif // PROCESSOR_HPP