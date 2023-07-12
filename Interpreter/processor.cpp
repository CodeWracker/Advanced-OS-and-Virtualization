#include "processor.hpp"
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
uint16_t Register::getRegister()
{
    return (uint16_t)((high << 8) | low);
}

void Register::add(uint16_t value)
{
    int result = this->getRegister() + value;
    high = (uint8_t)(result >> 8);
    low = (uint8_t)(result & 0x00FF);
}

void Register::sub(uint16_t value)
{
    // cout << this->getRegister() << " - " << value << endl;
    uint16_t result = this->getRegister() - value;
    high = (uint8_t)(result >> 8);
    low = (uint8_t)(result & 0x00FF);
}

void Register::mul(uint16_t value)
{
    uint16_t result = getRegister() * value;
    high = (uint8_t)(result >> 8);
    low = (uint8_t)(result & 0x00FF);
}

void Register::div(uint16_t value)
{
    uint16_t result = getRegister() / value;
    high = (uint8_t)(result >> 8);
    low = (uint8_t)(result & 0x00FF);
}

string Flags::getFlags()
{
    stringstream ss;
    // OF DF IF TF SF ZF AF PF CF
    ss << OF << DF << IF << TF << SF << ZF << AF << PF << CF;

    return ss.str();
}

uint16_t Processor::getRegisterValue(string reg)
{
    if (reg == "ax")
    {
        return AX.getRegister();
    }
    else if (reg == "bx")
    {
        return BX.getRegister();
    }
    else if (reg == "cx")
    {
        return CX.getRegister();
    }
    else if (reg == "dx")
    {
        return DX.getRegister();
    }
    else if (reg == "sp")
    {
        return SP.getRegister();
    }
    else if (reg == "bp")
    {
        return BP.getRegister();
    }
    else if (reg == "si")
    {
        return SI.getRegister();
    }
    else if (reg == "di")
    {
        return DI.getRegister();
    }
    else if (reg == "ip")
    {
        return IP.getRegister();
    }
    else
    {
        return 0;
    }
}

void Processor::setRegisterValue(string reg, uint16_t value)
{
    if (reg == "ax")
    {
        // atualiza o valor do registrador ax
        AX.high = (value >> 8) & 0xFF;
        AX.low = value & 0xFF;
    }
    else if (reg == "bx")
    {
        // atualiza o valor do registrador bx
        BX.high = (value >> 8) & 0xFF;
        BX.low = value & 0xFF;
    }
    else if (reg == "cx")
    {
        // atualiza o valor do registrador cx
        CX.high = (value >> 8) & 0xFF;
        CX.low = value & 0xFF;
    }
    else if (reg == "dx")
    {
        // atualiza o valor do registrador dx
        DX.high = (value >> 8) & 0xFF;
        DX.low = value & 0xFF;
    }
    else if (reg == "sp")
    {
        // atualiza o valor do registrador sp
        SP.high = (value >> 8) & 0xFF;
        SP.low = value & 0xFF;
    }
    else if (reg == "bp")
    {
        // atualiza o valor do registrador bp
        BP.high = (value >> 8) & 0xFF;
        BP.low = value & 0xFF;
    }
    else if (reg == "si")
    {
        // atualiza o valor do registrador si
        SI.high = (value >> 8) & 0xFF;
        SI.low = value & 0xFF;
    }
    else if (reg == "di")
    {
        // atualiza o valor do registrador di
        DI.high = (value >> 8) & 0xFF;
        DI.low = value & 0xFF;
    }
    else if (reg == "cs")
    {
        // atualiza o valor do registrador cs
        CS.high = (value >> 8) & 0xFF;
        CS.low = value & 0xFF;
    }
    else if (reg == "ds")
    {
        // atualiza o valor do registrador ds
        DS.high = (value >> 8) & 0xFF;
        DS.low = value & 0xFF;
    }
    else if (reg == "es")
    {
        // atualiza o valor do registrador es
        ES.high = (value >> 8) & 0xFF;
        ES.low = value & 0xFF;
    }
    else if (reg == "ss")
    {
        // atualiza o valor do registrador ss
        SS.high = (value >> 8) & 0xFF;
        SS.low = value & 0xFF;
    }
    else if (reg == "ip")
    {
        // atualiza o valor do registrador ip
        IP.high = (value >> 8) & 0xFF;
        IP.low = value & 0xFF;
    }
}

Processor::Processor()
{
    // inicializa os registradores
    AX.high = 0;
    AX.low = 0;
    BX.high = 0;
    BX.low = 0;
    CX.high = 0;
    CX.low = 0;
    DX.high = 0;
    DX.low = 0;

    SP.high = 0;
    SP.low = 0;

    BP.high = 0;
    BP.low = 0;
    SI.high = 0;
    SI.low = 0;
    DI.high = 0;
    DI.low = 0;
    flags.CF = 0;
    flags.PF = 0;
    flags.AF = 0;
    flags.ZF = 0;
    flags.SF = 0;
    flags.OF = 0;
    flags.TF = 0;
    flags.IF = 0;
    flags.DF = 0;
    IP.high = 0;
    IP.low = 0;

    // load physical memory as an vector  of size FFFFh and start with 0
    this->physical_memory = vector<uint8_t>(0xFFFF, 0);
}

void Processor::load_memories(WorkMemoryTape work_memory_tape, vector<uint8_t> data_memory_tape)
{
    this->work_memory = work_memory_tape;

    // carrega a memoria de dados na memoria fisica
    int index;
    for (index = 0; index < data_memory_tape.size(); index++)
    {
        this->physical_memory[index] = data_memory_tape[index];
    }
    // coloca a memoria de trabalho na fisica tb
    // mas a de trabalho tem uma estrutura diferente, cada indice dela possui mais de 1 byte,então a cada indice da memoria de trabalho tem que contar quantos bytes tem nela e ai cortar eles e colocar de um em um na memoria fisica

    for (int i = 0; i < work_memory_tape.memory.size(); i++)
    {
        string hex_bytes = work_memory_tape.memory[i].hex_code;
        int num_bytes = hex_bytes.size() / 2;
        for (int j = 0; j < num_bytes; j++)
        {
            string byte = hex_bytes.substr(j * 2, 2);
            this->physical_memory[index] = stoi(byte, nullptr, 16);
            index++;
        }
    }
}
void Processor::load_args(int qtd, char **args)
{
    // cout << this->SP.getRegister() << endl;
    // load the stack with the args byte by byte from the last byte of the last arg to the first byte of the first arg, then put the number of args in the stack. each slot is 2 bytes
    for (int i = qtd - 1; i >= 0; i--)
    {
        string arg = args[i];
        int number = 0;
        bool taking_number = false;
        bool number_tooked = false;
        for (char c : arg)
        {

            this->SP.sub(2);
            stringstream ss;
            ss << hex << uppercase;
            ss << setw(4) << this->SP.getRegister() << " ";
            string hex_sp = ss.str();
            // cout << hex_sp << ": " << (int)c << "-" << c << "|" << endl;
            // save the byte in the stack using two positions

            uint16_t word = c;

            this->physical_memory[SP.getRegister()] = (word >> 8) & 0xFF;
            // cout << "mem[" << SP.getRegister() << "] = " << (int)this->physical_memory[SP.getRegister()] << endl;

            this->physical_memory[SP.getRegister() + 1] = word & 0xFF;
            // cout << "mem[" << SP.getRegister() + 1 << "] = " << (int)this->physical_memory[SP.getRegister() + 1] << endl;
        }
        // save the byte in the stack using two positions
    }
    SP.sub(2);
    uint16_t word = qtd;
    this->physical_memory[SP.getRegister() + 1] = word & 0xFF;
    this->physical_memory[SP.getRegister()] = (word >> 8) & 0xFF;
}

string Processor::getStateHeader()
{
    return " AX   BX   CX   DX   SP   BP   SI   DI  FLAGS     IP";
}

string Processor::getState()
{
    /*
    Exemplo de saida
    0000 0000 0000 0000 ffd6 0000 0000 0000 ---- 0000:bb0000       mov bx, 0000
    */
    stringstream ss;
    ss << hex << uppercase << setfill('0');
    ss << setw(4) << AX.getRegister() << " ";
    ss << setw(4) << BX.getRegister() << " ";
    ss << setw(4) << CX.getRegister() << " ";
    ss << setw(4) << DX.getRegister() << " ";
    ss << setw(4) << SP.getRegister() << " ";
    ss << setw(4) << BP.getRegister() << " ";
    ss << setw(4) << SI.getRegister() << " ";
    ss << setw(4) << DI.getRegister() << " ";
    ss << setw(4) << flags.getFlags() << " ";
    ss << setw(4) << IP.getRegister() << " ";
    ss << ": ";
    // coloca o hexcode da instrução, mas entre o ":" e o assembly são 13 caracteres, então tem que completar com " " o que faltar depois de escrever o hexcode
    // <hexcode><espaços><assembly>
    ss << lastInstruction.hex_code;
    int hex_size = lastInstruction.hex_code.size();
    for (int i = hex_size; i < 13; i++)
    {
        ss << " ";
    }
    ss << lastInstruction.getString();
    return ss.str();
}