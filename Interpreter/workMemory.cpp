#include "workMemory.hpp"

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

string WorkMemorySlot::getString()
{
    string str = "";
    str += address + ": " + hex_code + " " + assembly_code.getString();
    return str;
}

void WorkMemoryTape::addWorkMemorySlot(WorkMemorySlot WorkMemorySlot)
{
    memory.push_back(WorkMemorySlot);
}

void WorkMemoryTape::removeWorkMemorySlot(string address)
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

void WorkMemoryTape::updateWorkMemorySlot(string address, WorkMemorySlot WorkMemorySlot)
{
    for (int i = 0; i < memory.size(); i++)
    {
        if (memory[i].address == address)
        {
            memory[i] = WorkMemorySlot;
            break;
        }
    }
}

WorkMemorySlot WorkMemoryTape::getWorkMemorySlot(string address)
{
    for (int i = 0; i < memory.size(); i++)
    {
        if (memory[i].address == address)
        {
            return memory[i];
        }
    }
    WorkMemorySlot work_memory_slot;
    work_memory_slot.address = "NULL";
    return work_memory_slot;
}

void WorkMemoryTape::clearMemory()
{
    memory.clear();
}
