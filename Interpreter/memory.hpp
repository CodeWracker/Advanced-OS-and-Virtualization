#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string>
#include <vector>

using namespace std;

class AsseblyCode
{
public:
    string mnemonic;
    string operand1;
    string operand2;

public:
    void setCode(string code);
    string getString();
};

class MemorySlot
{
public:
    string address;
    string hex_code;
    AsseblyCode assembly_code;

public:
    string getString();
};

class MemoryTape
{
public:
    vector<MemorySlot> memory;

public:
    void addMemorySlot(MemorySlot memorySlot);
    void removeMemorySlot(string address);
    void updateMemorySlot(string address, MemorySlot memorySlot);
    MemorySlot getMemorySlot(string address);
    void clearMemory();
};

#endif // MEMORY_HPP