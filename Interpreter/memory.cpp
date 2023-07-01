#include "memory.hpp"

string AsseblyCode::getString()
{
    string str = "";
    str += mnemonic;
    if (operand1 != "")
    {
        str += " " + operand1;
    }
    if (operand2 != "")
    {
        str += ", " + operand2;
    }
    return str;
}

void AsseblyCode::setCode(string code)
{
    // MNEMONIC OPERAND1, OPERAND2
    mnemonic = code.substr(0, code.find(' '));
    code = code.substr(code.find(' ') + 1, code.length() - 1);
    if (code.find(',') != string::npos)
    {
        operand1 = code.substr(0, code.find(','));
        code = code.substr(code.find(',') + 2, code.length() - 1);
        operand2 = code;
    }
    else
    {
        operand1 = code;
        operand2 = "";
    }
}

string MemorySlot::getString()
{
    string str = "";
    str += address + ": " + hex_code + " " + assembly_code.getString();
    return str;
}

void MemoryTape::addMemorySlot(MemorySlot memorySlot)
{
    memory.push_back(memorySlot);
}

void MemoryTape::removeMemorySlot(string address)
{
    for (int i = 0; i < memory.size(); i++)
    {
        if (memory[i].address == address)
        {
            memory.erase(memory.begin() + i);
            break;
        }
    }
}

void MemoryTape::updateMemorySlot(string address, MemorySlot memorySlot)
{
    for (int i = 0; i < memory.size(); i++)
    {
        if (memory[i].address == address)
        {
            memory[i] = memorySlot;
            break;
        }
    }
}

MemorySlot MemoryTape::getMemorySlot(string address)
{
    for (int i = 0; i < memory.size(); i++)
    {
        if (memory[i].address == address)
        {
            return memory[i];
        }
    }
    MemorySlot memorySlot;
    memorySlot.address = "NULL";
    return memorySlot;
}

void MemoryTape::clearMemory()
{
    memory.clear();
}
