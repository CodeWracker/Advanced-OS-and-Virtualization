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
    string hex_code;

public:
    void setCode(string code);
    string getString();
};

class WorkMemorySlot
{
public:
    string address;
    string hex_code;
    AsseblyCode assembly_code;

public:
    string getString();
};

class WorkMemoryTape
{
public:
    vector<WorkMemorySlot> memory;

public:
    void addWorkMemorySlot(WorkMemorySlot work_memory_slot);
    void removeWorkMemorySlot(string address);
    void updateWorkMemorySlot(string address, WorkMemorySlot work_memory_slot);
    WorkMemorySlot getWorkMemorySlot(string address);
    void clearMemory();
};

#endif // MEMORY_HPP