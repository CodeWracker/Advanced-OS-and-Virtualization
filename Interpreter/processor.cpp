#include "processor.hpp"
using namespace std;
int16_t Register::getRegister()
{
    return (int16_t)((high << 8) | low);
}
string Processor::getStateHeader()
{
    return "AX   BX   CX   DX   SP   BP   SI   DI  FLAGS IP";
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
    ss << setw(4) << flags.getRegister() << " ";
    ss << setw(4) << IP.getRegister() << " ";
    ss << ":";
    // coloca o hexcode da instrução, mas entre o ":" e o assembly são 13 caracteres, então tem que completar com espaço o que faltar depois de escrever o hexcode
    ss << setw(13) << lastInstruction.hex_code << " ";
    ss << lastInstruction.getString();
    return ss.str();
}