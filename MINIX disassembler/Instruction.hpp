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
        else if (opcode.substr(0, 4) == "1011")
        {
            this->w = opcode[4];
            this->reg = opcode.substr(5, 3);
            this->size = 3;
        }
        else if (opcode.substr(0, 6) == "100010")
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
        }
        else
        {
            this->w = opcode[7];
            this->size = 4;
            cout << endl
                 << opcode << endl;
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

#endif