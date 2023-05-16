#ifndef instruction
#define instruction

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <sstream>
using namespace std;

class Instruction
{
public:
    string name;
    string opcode;
    int size;
    char d;
    char w;
    char s;
    string reg;
    string mod;
    string rm;

    vector<string> payload;

public:
    Instruction(){};
    Instruction(string name, string opcode, int size)
    {
        this->name = name;
        this->opcode = opcode;
        this->size = size;
    };
    void setPayload(vector<string> payload)
    {
        this->payload = payload;
    };

    string getHex()
    {
        string hexstr = "";
        stringstream s;
        s << hex << stoll(this->opcode, NULL, 2);
        hexstr = hexstr + s.str();

        for (string str : this->payload)
        {
            stringstream s1;
            s1 << hex << stoll(str, NULL, 2);
            hexstr = hexstr + s1.str();
        }

        return hexstr;
    }
    string getName()
    {
        return this->name;
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
        else if (opcode.substr(0, 4) == "1011") // imediate to register
        {
            this->w = opcode[4];
            this->reg = opcode.substr(5, 3);
            if (w == 1)
            {
                this->size = 3;
            }
            else
            {
                this->size = 2;
            }
        }
        else if (opcode.substr(0, 6) == "100010") // register/memmory to/from register
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
        }
        else // immediate to register memmory
        {
            this->w = opcode[7];
            if (w == 1)
            {
                this->size = 4;
            }
            else
            {
                this->size = 3;
            }
        }
    };
    ~MOV(){};
};

class PUSH : public Instruction
{
public:
    PUSH(string opcode)
    {
        this->name = "PUSH";
        this->opcode = opcode;
        if (opcode == "11111111")
        {
            this->size = 2;
        }
        else if (opcode.substr(0, 5) == "01011")
        {
            this->reg = opcode.substr(5, 3);
            this->size = 1;
        }
        else
        {
            this->size = 1;
            this->reg = opcode.substr(3, 3);
        }
    };
    ~PUSH(){};
};

class ADD : public Instruction
{
public:
    ADD(string opcode)
    {
        this->name = "ADD";
        this->opcode = opcode;

        // reg/mem with register to either
        if (opcode.substr(0, 6) == "000000")
        {
            this->size = 2;
            this->d = opcode[6];
            this->w = opcode[7];
        }
        else if (opcode.substr(0, 6) == "100000")
        { // immediate to reg/mem
            this->s = opcode[6];
            this->w = opcode[7];
            if (w == 1)
            {
                this->size = 4;
            }
            else
            {
                this->size = 3;
            }
        }
        else
        { // immediate to acc
            this->w = opcode[7];
            if (w == 1)
            {
                this->size = 3;
            }
            else
            {
                this->size = 2;
            }
        }
    };
};

class ADC : public Instruction
{
public:
    ADC(string opcode)
    {
        this->name = "ADC";
        this->opcode = opcode;

        // reg/mem with register to either
        if (opcode.substr(0, 6) == "000100")
        {
            this->size = 2;
            this->d = opcode[6];
            this->w = opcode[7];
        }
        else if (opcode.substr(0, 6) == "100000")
        { // immediate to reg/mem
            this->s = opcode[6];
            this->w = opcode[7];
            if (w == 1)
            {
                this->size = 4;
            }
            else
            {
                this->size = 3;
            }
        }
        else
        { // immediate to acc with carry
            this->w = opcode[7];
            if (w == 1)
            {
                this->size = 3;
            }
            else
            {
                this->size = 2;
            }
        }
    }
};

class INC : public Instruction
{
public:
    INC(string opcode)
    {
        this->name = "INC";
        this->opcode = opcode;

        if (opcode.substr(0, 8) == "1111111")
        {
            this->w = opcode[8];
            this->mod = opcode.substr(9, 3);
            this->rm = opcode.substr(12, 3);
            this->size = 2;
        }
        else
        {
            this->reg = opcode.substr(5, 3);
            this->size = 1;
        }
    }
};

class AAA : public Instruction
{
public:
    AAA(string opcode)
    {
        this->name = "AAA";
        this->opcode = opcode;
        this->size = 1;
    }
};

class BAA : public Instruction
{
public:
    BAA(string opcode)
    {
        this->name = "BAA";
        this->opcode = opcode;
        this->size = 1;
    }
};

class SUB : public Instruction
{
public:
    SUB(string opcode)
    {
        this->name = "SUB";
        this->opcode = opcode;

        // reg/mem and register to either
        if (opcode.substr(0, 6) == "001010")
        {
            this->size = 2;
            this->d = opcode[6];
            this->w = opcode[7];
        }
        else if (opcode.substr(0, 6) == "100000")
        { // immediate from reg/mem
            this->s = opcode[6];
            this->w = opcode[7];
            if (w == 1)
            {
                this->size = 4;
            }
            else
            {
                this->size = 3;
            }
        }
        else
        { // immediate from acc
            this->w = opcode[7];
            if (w == 1)
            {
                this->size = 3;
            }
            else
            {
                this->size = 2;
            }
        }
    }
};
class SSB : public Instruction
{
public:
    SSB(string opcode)
    {
        this->name = "SSB";
        this->opcode = opcode;

        // reg/mem and register with borrow
        if (opcode.substr(0, 6) == "000110")
        {
            this->size = 2;
            this->d = opcode[6];
            this->w = opcode[7];
        }
        else if (opcode.substr(0, 6) == "100000")
        { // immediate from reg/mem with borrow
            this->s = opcode[6];
            this->w = opcode[7];
            if (w == 1)
            {
                this->size = 4;
            }
            else
            {
                this->size = 3;
            }
        }
        else
        { // immediate from acc with borrow (WRONG!)
            this->w = opcode[7];
            if (w == 1)
            {
                this->size = 3;
            }
            else
            {
                this->size = 2;
            }
        }
    }
};

class DEC : public Instruction
{
public:
    DEC(string opcode)
    {
        this->name = "DEC";
        this->opcode = opcode;

        if (opcode.substr(0, 8) == "1111111")
        {
            this->w = opcode[8];
            this->mod = opcode.substr(9, 3);
            this->rm = opcode.substr(12, 3);
            this->size = 2;
        }
        else
        {
            this->reg = opcode.substr(5, 3);
            this->size = 1;
        }
    }
};

class NEG : public Instruction
{
public:
    NEG(string opcode)
    {
        this->name = "NEG";
        this->opcode = opcode;

        this->w = opcode[8];
        this->mod = opcode.substr(9, 3);
        this->rm = opcode.substr(12, 3);
        this->size = 2;
    }
};

class CMP : public Instruction
{
public:
    CMP(string opcode)
    {
        this->name = "CMP";
        this->opcode = opcode;

        // reg/mem and register
        if (opcode.substr(0, 6) == "001111")
        {
            this->size = 2;
            this->d = opcode[6];
            this->w = opcode[7];
        }
        else if (opcode.substr(0, 6) == "100000")
        { // immediate with reg/mem
            this->s = opcode[6];
            this->w = opcode[7];
            if (w == 1)
            {
                this->size = 4;
            }
            else
            {
                this->size = 3;
            }
        }
        else
        { // immediate with acc
            this->w = opcode[7];
            if (w == 1)
            {
                this->size = 3;
            }
            else
            {
                this->size = 2;
            }
        }
    }
};

class AAS : public Instruction
{
public:
    AAS(string opcode)
    {
        this->name = "AAS";
        this->opcode = opcode;
        this->size = 1;
    }
};

class DAS : public Instruction
{
public:
    DAS(string opcode)
    {
        this->name = "DAS";
        this->opcode = opcode;
        this->size = 1;
    }
};
class MUL : public Instruction
{
public:
    MUL(string opcode)
    {
        this->name = "MUL";
        this->opcode = opcode;
        this->size = 2;
    }
};

class AAM : public Instruction
{
public:
    AAM(string opcode)
    {
        this->name = "AAM";
        this->opcode = opcode;
        this->size = 2;
    }
};

class DIV : public Instruction
{
public:
    DIV(string opcode)
    {
        this->name = "DIV";
        this->opcode = opcode;
        this->size = 2;
    }
};

class AAD : public Instruction
{
public:
    AAD(string opcode)
    {
        this->name = "AAD";
        this->opcode = opcode;
        this->size = 2;
    }
};

class CBW : public Instruction
{
public:
    CBW(string opcode)
    {
        this->name = "CBW";
        this->opcode = opcode;
        this->size = 1;
    }
};

class CWD : public Instruction
{
public:
    CWD(string opcode)
    {
        this->name = "CWD";
        this->opcode = opcode;
        this->size = 1;
    }
};
#endif