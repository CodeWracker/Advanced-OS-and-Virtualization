#ifndef instruction
#define instruction

#include <iostream>
#include <string>
using namespace std;
class Instruction
{
public:
    string name;
    string opcode;
    int size;
    char d;
    char w;
    string reg;
    string mod;
    string rm;

public:
    Instruction(){};
    Instruction(string name, string opcode, int size)
    {
        this->name = name;
        this->opcode = opcode;
        this->size = size;
    }
    ~Instruction(){};
};

class MOV : public Instruction
{

public:
    MOV(string opcode)
    {
        this->name = "MOV";
        this->opcode = opcode;

        if (opcode == "10001110")
        {
            this->size = 2;
        }
        else if (opcode == "10001100")
        {
            this->size = 2;
        }
        else if (opcode.substr(0, 4) == "1010")
        {
            this->w = opcode[7];
            this->size = 3;
        }
        else if (opcode.substr(0, 4) == "1011")
        {
            this->w = opcode[4];
            this->reg = opcode.substr(5, 3);
            this->size = 3;
        }
        else if (opcode.substr(0, 7) == "100010")
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
        }
        else
        {
            this->w = opcode[7];
            this->size = 4;
        }
    };
    ~MOV(){};
};

#endif