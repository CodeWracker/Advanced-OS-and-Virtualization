#ifndef instruction
#define instruction

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <sstream>
#include <map>
using namespace std;

class Instruction
{
public:
    string name;
    string opcode;
    int size;
    char d, w, s, z, v;
    string reg;
    string mod;
    string rm;
    int address;

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
    void setAddress(int addr)
    {
        this->address = addr;
    }
    string getHexAddr()
    {
        // return the hex code for the addr number using 2 bytes
        stringstream stream;
        stream << setfill('0') << setw(sizeof(int))
               << hex << this->address;
        return stream.str();
    }
    string getHex()
    {
        string hexstr = "";
        stringstream s;
        s << hex << stoll(this->opcode, NULL, 2);
        // verify if s has 2 chars
        if (s.str().size() == 1)
        {
            s.str("0" + s.str());
        }
        hexstr = hexstr + s.str();

        for (string str : this->payload)
        {
            stringstream s1;
            s1 << hex << stoll(str, NULL, 2);
            // verify if s1 has 2 chars
            if (s1.str().size() == 1)
            {
                s1.str("0" + s1.str());
            }
            hexstr = hexstr + s1.str();
        }

        return hexstr;
    }
    string getName()
    {
        return this->name;
    }
    virtual ~Instruction(){};
};

// enum of the conflicts types
enum class ConflictTypesEnum
{
    BIT7_1,
    SHIFT
};
class Conflict : public Instruction
{
public:
    ConflictTypesEnum type;
    Conflict(string opcode)
    {
        this->opcode = opcode;
    }
    void setType(ConflictTypesEnum tp)
    {

        this->type = tp;
    }
};

// data transfer cmds
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
            if (w == '1')
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
            if (w == '1')
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

class POP : public Instruction
{
public:
    POP(string opcode)
    {
        this->name = "POP";
        this->opcode = opcode;

        if (opcode.substr(0, 5) == "01011")
        {
            this->reg = opcode.substr(5, 3);
            this->size = 1;
        }
        else if (opcode.substr(0, 5) == "000")
        {
            this->reg = opcode.substr(5, 3);
            this->size = 1;
        }
        else
        {
            this->mod = opcode.substr(2, 2);
            this->rm = opcode.substr(4, 3);
            this->size = 2;
        }
    }
};

class XCHG : public Instruction
{
public:
    XCHG(string opcode)
    {
        this->name = "XCHG";
        this->opcode = opcode;

        if (opcode.substr(0, 5) == "10010")
        {
            this->reg = opcode.substr(5, 3);
            this->size = 1;
        }
        else
        {
            this->mod = opcode.substr(2, 2);
            this->rm = opcode.substr(4, 3);
            this->size = 2;
        }
    }
};

class IN : public Instruction
{
public:
    IN(string opcode)
    {
        this->name = "IN";
        this->opcode = opcode;

        if (opcode.substr(0, 7) == "1110010")
        {
            this->size = 2;
        }
        else
        {
            this->size = 2;
        }
    }
};

class OUT : public Instruction
{
public:
    OUT(string opcode)
    {
        this->name = "OUT";
        this->opcode = opcode;

        if (opcode.substr(0, 7) == "1110011")
        {
            this->size = 2;
        }
        else
        {
            this->size = 2;
        }
    }
};

class XLAT : public Instruction
{
public:
    XLAT(string opcode)
    {
        this->name = "XLAT";
        this->opcode = opcode;
        this->size = 1;
    }
};

class LEA : public Instruction
{
public:
    LEA(string opcode)
    {
        this->name = "LEA";
        this->opcode = opcode;
        this->size = 2 + 1;
    }
};

class LDS : public Instruction
{
public:
    LDS(string opcode)
    {
        this->name = "LDS";
        this->opcode = opcode;
        this->size = 2;
    }
};

class LES : public Instruction
{
public:
    LES(string opcode)
    {
        this->name = "LES";
        this->opcode = opcode;
        this->size = 2;
    }
};

class LAHF : public Instruction
{
public:
    LAHF(string opcode)
    {
        this->name = "LAHF";
        this->opcode = opcode;
        this->size = 1;
    }
};

class SAHF : public Instruction
{
public:
    SAHF(string opcode)
    {
        this->name = "SAHF";
        this->opcode = opcode;
        this->size = 1;
    }
};

class PUSHF : public Instruction
{
public:
    PUSHF(string opcode)
    {
        this->name = "PUSHF";
        this->opcode = opcode;
        this->size = 1;
    }
};

class POPF : public Instruction
{
public:
    POPF(string opcode)
    {
        this->name = "POPF";
        this->opcode = opcode;
        this->size = 1;
    }
};

// arithmetic commands
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
            if (w == '1')
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
            if (w == '1')
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
            if (w == '1')
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
            if (w == '1')
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
            if (w == '1')
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
            if (w == '1')
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
            if (w == '1')
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
            if (w == '1')
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
            if (w == '1')
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
            if (w == '1')
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

// logic instructions
class SHIFT : public Instruction
{
public:
    SHIFT(){};
    SHIFT(string opcode)
    {
        this->name = "SHIFT";
        this->opcode = opcode;
        this->size = 2;
    }
    void getArgs()
    {
        this->v = opcode[6];
        this->w = opcode[7];
    }

    virtual ~SHIFT(){};
};
class SHL : public SHIFT
{
public:
    SHL(string opcode)
    {
        this->name = "SHL";
        this->opcode = opcode;
        this->size = 2;
        this->getArgs();
    }
};
// create classes for all the shift instructions: SHR,SAR,ROL,ROR.RCL and RCR
class SHR : public SHIFT
{
public:
    SHR(string opcode)
    {
        this->name = "SHR";
        this->opcode = opcode;
        this->size = 2;
        this->getArgs();
    }
};
class SAR : public SHIFT
{
public:
    SAR(string opcode)
    {
        this->name = "SAR";
        this->opcode = opcode;
        this->size = 2;
        this->getArgs();
    }
};
class ROL : public SHIFT
{
public:
    ROL(string opcode)
    {
        this->name = "ROL";
        this->opcode = opcode;
        this->size = 2;
        this->getArgs();
    }
};
class ROR : public SHIFT
{
public:
    ROR(string opcode)
    {
        this->name = "ROR";
        this->opcode = opcode;
        this->size = 2;
        this->getArgs();
    }
};
class RCL : public SHIFT
{
public:
    RCL(string opcode)
    {
        this->name = "RCL";
        this->opcode = opcode;
        this->size = 2;
        this->getArgs();
    }
};
class RCR : public SHIFT
{
public:
    RCR(string opcode)
    {
        this->name = "RCR";
        this->opcode = opcode;
        this->size = 2;
        this->getArgs();
    }
};

class NOT : public Instruction
{
public:
    NOT(string opcode)
    {
        this->name = "NOT";
        this->opcode = opcode;
        this->size = 2;
    }
};

// class Shift : public Instruction
// {
// public:
//     Shift(string opcode)
//     {
//         this->name = "Shift";
//         this->opcode = opcode;
//         this->size = 2;
//     }
// };

class AND : public Instruction
{
public:
    AND(string opcode)
    {
        this->name = "AND";
        this->opcode = opcode;
        if (opcode.substr(0, 6) == "001000")
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
        }
        else if (opcode.substr(0, 6) == "100000")
        {
            this->w = opcode[7];
            if (w == '1')
            {
                this->size = 4;
            }
            else
            {
                this->size = 3;
            }
        }
        else
        {
            this->w = opcode[7];
            if (w == '1')
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

class TEST : public Instruction
{
public:
    TEST(string opcode)
    {
        this->name = "TEST";
        this->opcode = opcode;
        if (opcode.substr(0, 7) == "1000010")
        {
            this->w = opcode[7];
            this->size = 2;
        }
        else if (opcode.substr(0, 7) == "1111011")
        {
            this->w = opcode[7];
            if (w == '1')
            {
                this->size = 4;
            }
            else
            {
                this->size = 3;
            }
        }
        else
        {
            this->w = opcode[7];
            if (w == '1')
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

class OR : public Instruction
{
public:
    OR(string opcode)
    {
        this->name = "OR";
        this->opcode = opcode;
        if (opcode.substr(0, 6) == "000010")
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
        }
        else if (opcode.substr(0, 7) == "1000000")
        {
            this->w = opcode[7];
            if (w == '1')
            {
                this->size = 4;
            }
            else
            {
                this->size = 3;
            }
        }
        else
        {
            this->w = opcode[7];
            if (w == '1')
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

class XOR : public Instruction
{
public:
    XOR(string opcode)
    {
        this->name = "XOR";
        this->opcode = opcode;
        if (opcode.substr(0, 6) == "001100")
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
        }
        else if (opcode.substr(0, 7) == "1000000")
        {
            this->w = opcode[7];
            if (w == '1')
            {
                this->size = 4;
            }
            else
            {
                this->size = 3;
            }
        }
        else
        {
            this->w = opcode[7];
            if (w == '1')
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

class REP : public Instruction
{
public:
    REP(string opcode)
    {
        this->name = "REP";
        this->opcode = opcode;
        this->size = 2;
        this->z = opcode[7];
    }
};

class MOVS : public Instruction
{
public:
    MOVS(string opcode)
    {
        this->name = "MOVS";
        this->opcode = opcode;
        this->size = 2;
        this->w = opcode[7];
    }
};

class CMPS : public Instruction
{
public:
    CMPS(string opcode)
    {
        this->name = "CMPS";
        this->opcode = opcode;
        this->size = 2;
        this->w = opcode[7];
    }
};

class SCAS : public Instruction
{
public:
    SCAS(string opcode)
    {
        this->name = "SCAS";
        this->opcode = opcode;
        this->size = 2;
        this->w = opcode[7];
    }
};

class LODS : public Instruction
{
public:
    LODS(string opcode)
    {
        this->name = "LODS";
        this->opcode = opcode;
        this->size = 2;
        this->w = opcode[7];
    }
};

class STOS : public Instruction
{
public:
    STOS(string opcode)
    {
        this->name = "STOS";
        this->opcode = opcode;
        this->size = 2;
        this->w = opcode[7];
    }
};

class CALL : public Instruction
{
public:
    CALL(string opcode)
    {
        this->name = "CALL";
        this->opcode = opcode;
        this->size = 2;
        this->w = opcode[7];
    }
};

#endif