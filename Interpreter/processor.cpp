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
    uint16_t result = getRegister() - value;
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

    SP.high = 255;
    SP.low = 214;

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