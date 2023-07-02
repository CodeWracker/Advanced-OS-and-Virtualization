#include "processor.hpp"
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
int16_t Register::getRegister()
{
    return (int16_t)((high << 8) | low);
}

void Register::add(int16_t value)
{
    int16_t result = getRegister() + value;
    high = (int8_t)(result >> 8);
    low = (int8_t)(result & 0x00FF);
}

void Register::sub(int16_t value)
{
    int16_t result = getRegister() - value;
    high = (int8_t)(result >> 8);
    low = (int8_t)(result & 0x00FF);
}

void Register::mul(int16_t value)
{
    int16_t result = getRegister() * value;
    high = (int8_t)(result >> 8);
    low = (int8_t)(result & 0x00FF);
}

void Register::div(int16_t value)
{
    int16_t result = getRegister() / value;
    high = (int8_t)(result >> 8);
    low = (int8_t)(result & 0x00FF);
}

string Flags::getFlags()
{
    stringstream ss;
    // OF DF IF TF SF ZF AF PF CF
    ss << OF << DF << IF << TF << SF << ZF << AF << PF << CF;

    return ss.str();
}

int16_t Processor::getRegisterValue(string reg)
{
    if (reg == "AX")
    {
        return AX.getRegister();
    }
    else if (reg == "BX")
    {
        return BX.getRegister();
    }
    else if (reg == "CX")
    {
        return CX.getRegister();
    }
    else if (reg == "DX")
    {
        return DX.getRegister();
    }
    else if (reg == "SP")
    {
        return SP.getRegister();
    }
    else if (reg == "BP")
    {
        return BP.getRegister();
    }
    else if (reg == "SI")
    {
        return SI.getRegister();
    }
    else if (reg == "DI")
    {
        return DI.getRegister();
    }
    else if (reg == "IP")
    {
        return IP.getRegister();
    }
    else
    {
        return 0;
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