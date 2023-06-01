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
    int subtype;

    map<string, string> regMap1 = {
        {"000", "AX"},
        {"001", "CX"},
        {"010", "DX"},
        {"011", "BX"},
        {"100", "SP"},
        {"101", "BP"},
        {"110", "SI"},
        {"111", "DI"}};
    map<string, string> regMap0 = {
        {"000", "AL"},
        {"001", "CL"},
        {"010", "DL"},
        {"011", "BL"},
        {"100", "AH"},
        {"101", "CH"},
        {"110", "DH"},
        {"111", "BH"}};

    map<char, map<string, string>> regMapChooserMap = {
        {'0', regMap0},
        {'1', regMap1}};

    map<string, string> rmMap = {
        {"000", "BX+SI"},
        {"001", "BX+DI"},
        {"010", "BP+SI"},
        {"011", "BP+DI"},
        {"100", "SI"},
        {"101", "DI"},
        {"110", "BP"},
        {"111", "BX"}};

public:
    Instruction(){};
    Instruction(string name, string opcode, int size)
    {
        this->getBasicInfo(name, opcode);
        this->size = size;
    };
    virtual void getBasicInfo(string name, string opcode)
    {
        this->name = name;
        this->opcode = opcode;
    }
    virtual void getBufferInfo(string buffer, bool mod, bool reg, bool rm)
    {
        if (mod)
        {
            this->mod = buffer.substr(0, 2);
        }
        if (reg)
        {
            this->reg = buffer.substr(2, 3);
        }
        if (rm)
        {
            this->rm = buffer.substr(5, 3);
        }
    }

    virtual void checkWSize(char w, int size1, int size0)
    {
        if (w == '1')
        {
            this->size = size1;
        }
        else
        {
            this->size = size0;
        }
    }
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
    virtual string getPayloadInfo()
    {
        return "";
    };
    virtual ~Instruction(){};
};

// enum of the conflicts types
enum class ConflictTypesEnum
{
    BIT7_1,
    SHIFT,
    ALL_1,
    TEST_MUL_OTHERS
};
class Conflict : public Instruction
{
public:
    ConflictTypesEnum type;
    Conflict(string opcode, string buffer)
    {
        this->opcode = opcode;
        this->name = "CONFLICT";
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
    MOV(string opcode, string buffer)
    {

        this->getBasicInfo("MOV", opcode);
        if (opcode == "10001110")
        {
            this->size = 2;
            this->subtype = 5;
            this->getBufferInfo(buffer, true, true, true);
        }
        else if (opcode == "10001100")
        {
            this->size = 2;
            this->subtype = 6;
            this->getBufferInfo(buffer, true, true, true);
        }
        else if (opcode.substr(0, 4) == "1010000")
        {
            this->w = opcode[7];
            this->size = 3;
            this->subtype = 3;
        }
        else if (opcode.substr(0, 4) == "1010001")
        {
            this->w = opcode[7];
            this->size = 3;
            this->subtype = 4;
        }
        else if (opcode.substr(0, 4) == "1011") // imediate to register
        {
            this->w = opcode[4];
            this->reg = opcode.substr(5, 3);
            this->checkWSize(this->w, 3, 2);
            this->subtype = 2;
        }
        else if (opcode.substr(0, 6) == "100010") // register/memmory to/from register
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
            this->subtype = 0;
            this->getBufferInfo(buffer, true, true, true);
            if (this->w == '1' && this->mod == "00" && this->rm == "110")
            {
                this->size = 4;
            }
            if (this->mod == "10")
            {
                this->size = 4;
            }
            if (this->mod == "01")
            {
                this->size = 3;
            }
        }
        else // immediate to register memmory
        {
            this->w = opcode[7];
            this->subtype = 1;
            this->checkWSize(this->w, 4, 3);
            this->getBufferInfo(buffer, true, false, true);
        }
    };
    // override the getPayloadInfo
    string getPayloadInfo()
    {
        string payloadInfo = "";
        string p1, p2;
        if (this->subtype == 5)
        {
            p1 = regMapChooserMap[this->w][this->reg];
            p2 = regMapChooserMap[this->w][this->rm];
            if (this->d == '0')
            {
                payloadInfo = p2 + ", " + p1;
            }
            else
            {
                payloadInfo = p1 + ", " + p2;
            }
        }
        else if (this->subtype == 6)
        {
            p1 = regMapChooserMap[this->w][this->reg];
            p2 = regMapChooserMap[this->w][this->rm];
            if (this->d == '0')
            {
                payloadInfo = p2 + ", " + p1;
            }
            else
            {
                payloadInfo = p1 + ", " + p2;
            }
        }
        else if (this->subtype == 3)
        {
            payloadInfo = "3";
        }
        else if (this->subtype == 4)
        {
            payloadInfo = "4";
        }
        else if (this->subtype == 2) // imediate to register
        {
            p1 = regMapChooserMap[this->w][this->reg];
            payloadInfo = p1 + ", ";
            string hexPayload;
            for (string str : this->payload)
            {
                stringstream s;
                s << hex << stoll(str, NULL, 2);

                // verify if s has 2 chars
                if (s.str().size() == 1)
                {
                    s.str("0" + s.str());
                }

                hexPayload = hexPayload + s.str();
            }
            payloadInfo = payloadInfo + hexPayload.substr(2, 2) + hexPayload.substr(0, 2);
        }
        else if (this->subtype == 0) // register/memmory to/from register
        {
            p1 = regMapChooserMap[this->w][this->reg];
            if (this->d == '0')
            {
                p2 = regMapChooserMap[this->w][this->rm];
                payloadInfo = p2 + ", " + p1;
            }
            else
            {
                p2 = rmMap[this->rm];
                payloadInfo = p1 + ", [" + p2 + "]";
            }
        }
        else if (this->subtype == 1) // immediate to register memmory
        {
            p1 = regMapChooserMap[this->w][this->reg];
            p2 = regMapChooserMap[this->w][this->rm];
            if (this->d == '0')
            {
                payloadInfo = p2 + ", " + p1;
            }
            else
            {
                payloadInfo = p1 + ", " + p2;
            }
        }
        return payloadInfo;
    }
    ~MOV(){};
};

class PUSH : public Instruction
{
public:
    PUSH(string opcode, string buffer)
    {
        this->getBasicInfo("PUSH", opcode);
        if (opcode == "11111111")
        {
            this->size = 3;

            this->getBufferInfo(buffer, true, false, true);
        }
        else if (opcode.substr(0, 5) == "01010")
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
    POP(string opcode, string buffer)
    {

        this->getBasicInfo("POP", opcode);
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

            this->getBufferInfo(buffer, true, false, true);
        }
    }
};

class XCHG : public Instruction
{
public:
    XCHG(string opcode, string buffer)
    {

        this->getBasicInfo("XCHG", opcode);
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
            this->getBufferInfo(buffer, true, true, true);
        }
    }
};

class IN : public Instruction
{
public:
    IN(string opcode, string buffer)
    {

        this->getBasicInfo("IN", opcode);
        if (opcode.substr(0, 7) == "1110010")
        {
            this->size = 2;
        }
        else
        {
            this->size = 1;
        }
    }
};

class OUT : public Instruction
{
public:
    OUT(string opcode, string buffer)
    {

        this->getBasicInfo("OUT", opcode);
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
    XLAT(string opcode, string buffer)
    {

        this->getBasicInfo("XLAT", opcode);
        this->size = 1;
    }
};

class LOAD : public Instruction
{
public:
    LOAD(){};
    LOAD(string opcode, string buffer)
    {
        this->getBasicInfo("LOAD", opcode);
        this->size = 1;
    }
    virtual void getInfos(string opcode, string buffer)
    {
        this->getBufferInfo(buffer, true, true, true);
        if (this->mod == "01")
        {
            this->size = 3;
        }
        else
        {
            this->size = 2;
        }
    }
    virtual string getPayloadInfo()
    {
        string payloadInfo;
        string p1, p2;
        p1 = regMapChooserMap['1'][this->reg];
        if (this->mod == "01")
        {
            p2 = rmMap[this->rm];
            stringstream disp;
            disp << hex << stoll(this->payload[1], NULL, 2);
            payloadInfo = p1 + ", [" + p2 + "+" + disp.str() + "]";
        }
        else
        {
            p2 = regMapChooserMap[this->w][this->rm];
            payloadInfo = p1 + ", " + p2;
        }
        return payloadInfo;
    }
};
class LEA : public LOAD
{
public:
    LEA(string opcode, string buffer)
    {

        this->getBasicInfo("LEA", opcode);
        this->getInfos(opcode, buffer);
    };
};

class LDS : public LOAD
{
public:
    LDS(string opcode, string buffer)
    {

        this->getBasicInfo("LDS", opcode);
        this->getInfos(opcode, buffer);
    }
};

class LES : public LOAD
{
public:
    LES(string opcode, string buffer)
    {

        this->getBasicInfo("LES", opcode);
        this->getInfos(opcode, buffer);
    }
};

class LAHF : public LOAD
{
public:
    LAHF(string opcode, string buffer)
    {

        this->getBasicInfo("LAHF", opcode);
        this->size = 1;
    }
};

class SAHF : public Instruction
{
public:
    SAHF(string opcode, string buffer)
    {

        this->getBasicInfo("SAHF", opcode);
        this->size = 1;
    }
};

class PUSHF : public Instruction
{
public:
    PUSHF(string opcode, string buffer)
    {

        this->getBasicInfo("PUSHF", opcode);
        this->size = 1;
    }
};

class POPF : public Instruction
{
public:
    POPF(string opcode, string buffer)
    {

        this->getBasicInfo("POPF", opcode);
        this->size = 1;
    }
};

// arithmetic commands
class ADD : public Instruction
{
public:
    ADD(string opcode, string buffer)
    {

        this->getBasicInfo("ADD", opcode);
        // reg/mem with register to either
        if (opcode.substr(0, 6) == "000000")
        {
            this->subtype = 0;
            this->size = 2;
            this->d = opcode[6];
            this->w = opcode[7];

            this->getBufferInfo(buffer, true, true, true);

            if (mod == "01")
            {
                this->size = 3;
            }
        }
        else if (opcode.substr(0, 6) == "100000")
        { // immediate to reg/mem
            this->subtype = 1;
            this->s = opcode[6];
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
            this->getBufferInfo(buffer, true, false, true);
        }
        else
        { // immediate to acc
            this->subtype = 2;
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);
        }
    };
    string getPayloadInfo()
    {
        string payloadInfo = "";
        string p1, p2;
        if (this->subtype == 0)
        {
            p1 = regMapChooserMap[this->w][this->reg];
            p2 = regMapChooserMap[this->w][this->rm];
            if (this->d == '0')
            {
                payloadInfo = p2 + ", " + p1;
            }
            else
            {
                payloadInfo = p1 + ", " + p2;
            }
        }
        return payloadInfo;
    }
};

class ADC : public Instruction
{
public:
    ADC(string opcode, string buffer)
    {

        this->getBasicInfo("ADC", opcode);
        // reg/mem with register to either
        if (opcode.substr(0, 6) == "000100")
        {
            this->size = 2;
            this->d = opcode[6];
            this->w = opcode[7];
            this->getBufferInfo(buffer, true, true, true);
        }
        else if (opcode.substr(0, 6) == "100000")
        { // immediate to reg/mem
            this->s = opcode[6];
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
            this->getBufferInfo(buffer, true, false, true);
        }
        else
        { // immediate to acc with carry
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);
        }
    }
};

class INC : public Instruction
{
public:
    INC(string opcode, string buffer)
    {

        this->getBasicInfo("INC", opcode);
        if (opcode.substr(0, 8) == "1111111")
        {
            this->w = opcode[8];
            this->mod = opcode.substr(9, 3);
            this->rm = opcode.substr(12, 3);
            this->size = 2;
            this->getBufferInfo(buffer, true, false, true);
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
    AAA(string opcode, string buffer)
    {

        this->getBasicInfo("AAA", opcode);
        this->size = 1;
    }
};

class BAA : public Instruction
{
public:
    BAA(string opcode, string buffer)
    {

        this->getBasicInfo("BAA", opcode);
        this->size = 1;
    }
};

class SUB : public Instruction
{
public:
    SUB(string opcode, string buffer)
    {

        this->getBasicInfo("SUB", opcode);
        // reg/mem and register to either
        if (opcode.substr(0, 6) == "001010")
        {
            this->size = 2;
            this->d = opcode[6];
            this->w = opcode[7];
            this->getBufferInfo(buffer, true, true, true);
        }
        else if (opcode.substr(0, 6) == "100000")
        { // immediate from reg/mem
            this->s = opcode[6];
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
            this->getBufferInfo(buffer, true, false, true);
        }
        else
        { // immediate from acc
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);
        }
    }
};
class SBB : public Instruction
{
public:
    SBB(string opcode, string buffer)
    {

        this->getBasicInfo("SBB", opcode);
        // reg/mem and register with borrow
        if (opcode.substr(0, 6) == "000110")
        {
            this->size = 2;
            this->d = opcode[6];
            this->w = opcode[7];

            this->getBufferInfo(buffer, true, true, true);

            if (mod == "10")
            {
                this->size = 4;
            }
        }
        else if (opcode.substr(0, 6) == "100000")
        { // immediate from reg/mem with borrow
            this->s = opcode[6];
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
            this->getBufferInfo(buffer, true, false, true);
        }
        else
        { // immediate from acc with borrow (WRONG!)
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);
        }
    }
};

class DEC : public Instruction
{
public:
    DEC(string opcode, string buffer)
    {

        this->getBasicInfo("DEC", opcode);
        if (opcode.substr(0, 8) == "1111111")
        {
            this->w = opcode[8];

            this->getBufferInfo(buffer, true, false, true);
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
    NEG(string opcode, string buffer)
    {

        this->getBasicInfo("NEG", opcode);
        this->w = opcode[8];

        this->getBufferInfo(buffer, true, false, true);
        this->size = 2;
    }
};

class CMP : public Instruction
{
public:
    CMP(string opcode, string buffer)
    {

        this->getBasicInfo("CMP", opcode);
        // reg/mem and register
        if (opcode.substr(0, 6) == "001110")
        {
            this->size = 2;
            this->d = opcode[6];
            this->w = opcode[7];
            this->subtype = 0;
            this->getBufferInfo(buffer, true, true, true);
        }
        else if (opcode.substr(0, 6) == "100000")
        { // immediate with reg/mem
            this->s = opcode[6];
            this->w = opcode[7];
            this->subtype = 1;
            this->checkWSize(this->w, 4, 3);
            this->getBufferInfo(buffer, true, false, true);
        }
        else
        { // immediate with acc
            this->w = opcode[7];
            this->subtype = 2;
            this->checkWSize(this->w, 3, 2);
        }
    };
    string getPayloadInfo()
    {
        string payloadInfo = "";
        string p1, p2;
        if (this->subtype == 0)
        {
            payloadInfo = "1";
        }
        else if (this->subtype == 1)
        {
            p1 = this->regMapChooserMap[this->w]["011"];
            if (this->s == '0' && this->w == '1')
            {
                stringstream s1;
                s1 << hex << stoll(this->payload[2], NULL, 2);
                p2 = s1.str();
                if (p2.length() == 1)
                {
                    p2 = "0" + p2;
                }
            }
            stringstream s2;
            s2 << hex << stoll(this->payload[1], NULL, 2);
            p2 = p2 + s2.str();

            payloadInfo = p1 + ", " + p2;
        }
        else
        {
            payloadInfo = "2";
        }
        return payloadInfo;
    }
};

class AAS : public Instruction
{
public:
    AAS(string opcode, string buffer)
    {

        this->getBasicInfo("AAS", opcode);
        this->size = 1;
    }
};

class DAS : public Instruction
{
public:
    DAS(string opcode, string buffer)
    {

        this->getBasicInfo("DAS", opcode);
        this->size = 1;
    }
};
class MUL : public Instruction
{
public:
    MUL(string opcode, string buffer)
    {

        this->getBasicInfo("MUL", opcode);
        this->size = 2;
        this->getBufferInfo(buffer, true, false, true);
    }
};

class IMUL : public Instruction
{
public:
    IMUL(string opcode, string buffer)
    {

        this->getBasicInfo("IMUL", opcode);
        this->size = 2;
        this->getBufferInfo(buffer, true, false, true);
    }
};

class AAM : public Instruction
{
public:
    AAM(string opcode, string buffer)
    {

        this->getBasicInfo("AAM", opcode);
        this->size = 2;
    }
};

class DIV : public Instruction
{
public:
    DIV(string opcode, string buffer)
    {

        this->getBasicInfo("DIV", opcode);
        this->getBufferInfo(buffer, true, false, true);
        this->size = 2;
    }
};
class IDIV : public Instruction
{
public:
    IDIV(string opcode, string buffer)
    {

        this->getBasicInfo("IDIV", opcode);
        this->getBufferInfo(buffer, true, false, true);
        this->size = 2;
    }
};

class AAD : public Instruction
{
public:
    AAD(string opcode, string buffer)
    {

        this->getBasicInfo("AAD", opcode);
        this->size = 2;
    }
};

class CBW : public Instruction
{
public:
    CBW(string opcode, string buffer)
    {

        this->getBasicInfo("CBW", opcode);
        this->size = 1;
    }
};

class CWD : public Instruction
{
public:
    CWD(string opcode, string buffer)
    {

        this->getBasicInfo("CWD", opcode);
        this->size = 1;
    }
};

// logic instructions
class NOT : public Instruction
{
public:
    NOT(string opcode, string buffer)
    {

        this->getBasicInfo("NOT", opcode);
        this->getBufferInfo(buffer, true, false, true);
        this->size = 2;
    }
};
class SHIFT : public Instruction
{
public:
    SHIFT(){};
    SHIFT(string opcode, string buffer)
    {

        this->getBasicInfo("SHIFT", opcode);
        this->size = 2;
    }
    void getArgs(string buffer)
    {
        this->v = opcode[6];
        this->w = opcode[7];
        this->getBufferInfo(buffer, true, false, true);
    }

    virtual ~SHIFT(){};
};
class SHL : public SHIFT
{
public:
    SHL(string opcode, string buffer)
    {

        this->getBasicInfo("SHL", opcode);
        this->size = 2;
        this->getArgs(buffer);
    }
};
// create classes for all the shift instructions: SHR,SAR,ROL,ROR.RCL and RCR
class SHR : public SHIFT
{
public:
    SHR(string opcode, string buffer)
    {

        this->getBasicInfo("SHR", opcode);
        this->size = 2;
        this->getArgs(buffer);
    }
};
class SAR : public SHIFT
{
public:
    SAR(string opcode, string buffer)
    {

        this->getBasicInfo("SAR", opcode);
        this->size = 2;
        this->getArgs(buffer);
    }
};
class ROL : public SHIFT
{
public:
    ROL(string opcode, string buffer)
    {

        this->getBasicInfo("ROL", opcode);
        this->size = 2;
        this->getArgs(buffer);
    }
};
class ROR : public SHIFT
{
public:
    ROR(string opcode, string buffer)
    {

        this->getBasicInfo("ROR", opcode);
        this->size = 2;
        this->getArgs(buffer);
    }
};
class RCL : public SHIFT
{
public:
    RCL(string opcode, string buffer)
    {

        this->getBasicInfo("RCL", opcode);
        this->size = 2;
        this->getArgs(buffer);
    }
};
class RCR : public SHIFT
{
public:
    RCR(string opcode, string buffer)
    {

        this->getBasicInfo("RCR", opcode);
        this->size = 2;
        this->getArgs(buffer);
    }
};

class AND : public Instruction
{
public:
    AND(string opcode, string buffer)
    {

        this->getBasicInfo("AND", opcode);
        if (opcode.substr(0, 6) == "001000")
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->getBufferInfo(buffer, true, true, true);
            this->size = 2;
        }
        else if (opcode.substr(0, 6) == "100000")
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
            this->getBufferInfo(buffer, true, false, true);
        }
        else
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);
        }
    };
};

class TEST : public Instruction
{
public:
    TEST(string opcode, string buffer)
    {
        this->getBasicInfo("TEST", opcode);
        if (opcode.substr(0, 7) == "1000010")
        {
            this->w = opcode[7];
            this->getBufferInfo(buffer, true, true, true);
            this->size = 2;
        }
        else if (opcode.substr(0, 7) == "1111011")
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
            this->getBufferInfo(buffer, true, false, true);
        }
        else
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);
        }
    }
};

class OR : public Instruction
{
public:
    OR(string opcode, string buffer)
    {

        this->getBasicInfo("OR", opcode);
        if (opcode.substr(0, 6) == "000010")
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
            this->getBufferInfo(buffer, true, true, true);
        }
        else if (opcode.substr(0, 7) == "1000000")
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
            this->getBufferInfo(buffer, true, false, true);
        }
        else
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);
        }
    }
};

class XOR : public Instruction
{
public:
    XOR(string opcode, string buffer)
    {

        this->getBasicInfo("XOR", opcode);
        if (opcode.substr(0, 6) == "001100")
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->getBufferInfo(buffer, true, true, true);
            this->size = 2;
            this->subtype = 0;
        }
        else if (opcode.substr(0, 7) == "1000000")
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
            this->getBufferInfo(buffer, true, false, true);
            this->subtype = 1;
        }
        else
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);

            this->subtype = 3;
        }
    };

    string getPayloadInfo()
    {

        string payloadInfo = "";
        string p1, p2;

        if (this->subtype == 0)
        {
            p1 = this->regMapChooserMap[this->w][this->reg];
            p2 = this->regMapChooserMap[this->w][this->rm];
            payloadInfo = p1 + ", " + p2;
        }
        return payloadInfo;
    }
};

class REP : public Instruction
{
public:
    REP(string opcode, string buffer)
    {

        this->getBasicInfo("REP", opcode);
        this->size = 2;
        this->z = opcode[7];
    }
};

class MOVS : public Instruction
{
public:
    MOVS(string opcode, string buffer)
    {

        this->getBasicInfo("MOVS", opcode);
        this->size = 2;
        this->w = opcode[7];
    }
};

class CMPS : public Instruction
{
public:
    CMPS(string opcode, string buffer)
    {

        this->getBasicInfo("CMPS", opcode);
        this->size = 2;
        this->w = opcode[7];
    }
};

class SCAS : public Instruction
{
public:
    SCAS(string opcode, string buffer)
    {

        this->getBasicInfo("SCAS", opcode);
        this->size = 2;
        this->w = opcode[7];
    }
};

class LODS : public Instruction
{
public:
    LODS(string opcode, string buffer)
    {

        this->getBasicInfo("LODS", opcode);
        this->size = 2;
        this->w = opcode[7];
    }
};

class STOS : public Instruction
{
public:
    STOS(string opcode, string buffer)
    {

        this->getBasicInfo("STOS", opcode);
        this->size = 2;
        this->w = opcode[7];
    }
};

class CALL : public Instruction
{
public:
    CALL(string opcode, string buffer)
    {
        this->getBasicInfo("CALL", opcode);
        if (opcode == "11111111")
        {
            this->getBufferInfo(buffer, true, false, true);
        }
        if (opcode == "11101000" || opcode == "10011010" || (opcode == "11111111" && buffer.substr(2, 3) == "011"))
        {
            this->size = 3;
        }
        else
        {
            this->size = 2;
        }
    }
};

class JMP : public Instruction
{
public:
    JMP(string opcode, string buffer)
    {
        this->getBasicInfo("JMP", opcode);
        if (opcode == "11111111")
        {
            this->getBufferInfo(buffer, true, false, true);
        }
        if (opcode == "11101001" || opcode == " 11101010" || opcode == "11111111")
        {
            this->size = 3;
        }
        else
        {
            this->size = 2;
        }
    }
};

class RET : public Instruction
{
public:
    RET(string opcode, string buffer)
    {
        this->getBasicInfo("RET", opcode);
        if (opcode == "11000011" || opcode == "11001011")
        {
            this->size = 1;
        }
        else
        {
            this->size = 3;
        }
    }
};

class JE : public Instruction
{
public:
    JE(string opcode, string buffer)
    {
        this->getBasicInfo("JE", opcode);
        this->size = 2;
    }
};

class JL : public Instruction
{
public:
    JL(string opcode, string buffer)
    {
        this->getBasicInfo("JL", opcode);
        this->size = 2;
    }
};

class JLE : public Instruction
{
public:
    JLE(string opcode, string buffer)
    {
        this->getBasicInfo("JLE", opcode);
        this->size = 2;
    }
};

class JB : public Instruction
{
public:
    JB(string opcode, string buffer)
    {
        this->getBasicInfo("JB", opcode);
        this->size = 2;
    }
};

class JBE : public Instruction
{
public:
    JBE(string opcode, string buffer)
    {
        this->getBasicInfo("JBE", opcode);
        this->size = 2;
    }
};

class JP : public Instruction
{
public:
    JP(string opcode, string buffer)
    {
        this->getBasicInfo("JP", opcode);
        this->size = 2;
    }
};

class JO : public Instruction
{
public:
    JO(string opcode, string buffer)
    {
        this->getBasicInfo("JO", opcode);
        this->size = 2;
    }
};

class JS : public Instruction
{
public:
    JS(string opcode, string buffer)
    {
        this->getBasicInfo("JS", opcode);
        this->size = 2;
    }
};

class JNE : public Instruction
{
public:
    JNE(string opcode, string buffer)
    {
        this->getBasicInfo("JNE", opcode);
        this->size = 2;
    }
};

class JNL : public Instruction
{
public:
    JNL(string opcode, string buffer)
    {
        this->getBasicInfo("JNL", opcode);
        this->size = 2;
    }
};

class JNLE : public Instruction
{
public:
    JNLE(string opcode, string buffer)
    {
        this->getBasicInfo("JNLE", opcode);
        this->size = 2;
    }
};

class JNB : public Instruction
{
public:
    JNB(string opcode, string buffer)
    {
        this->getBasicInfo("JNB", opcode);
        this->size = 2;
    }
};

class JNBE : public Instruction
{
public:
    JNBE(string opcode, string buffer)
    {
        this->getBasicInfo("JNBE", opcode);
        this->size = 2;
    }
};

class JNP : public Instruction
{
public:
    JNP(string opcode, string buffer)
    {
        this->getBasicInfo("JNP", opcode);
        this->size = 2;
    }
};

class JNO : public Instruction
{
public:
    JNO(string opcode, string buffer)
    {
        this->getBasicInfo("JNO", opcode);
        this->size = 2;
    }
};

class JNS : public Instruction
{
public:
    JNS(string opcode, string buffer)
    {
        this->getBasicInfo("JNS", opcode);
        this->size = 2;
    }
};

class LOOP : public Instruction
{
public:
    LOOP(string opcode, string buffer)
    {
        this->getBasicInfo("LOOP", opcode);
        this->size = 2;
    }
};

class LOOPZ : public Instruction
{
public:
    LOOPZ(string opcode, string buffer)
    {
        this->getBasicInfo("LOOPZ", opcode);
        this->size = 2;
    }
};

class LOOPNZ : public Instruction
{
public:
    LOOPNZ(string opcode, string buffer)
    {
        this->getBasicInfo("LOOPNZ", opcode);
        this->size = 2;
    }
};

class JCXZ : public Instruction
{
public:
    JCXZ(string opcode, string buffer)
    {
        this->getBasicInfo("JCXZ", opcode);
        this->size = 2;
    }
};

class INT : public Instruction
{
public:
    INT(string opcode, string buffer)
    {
        this->getBasicInfo("INT", opcode);
        if (opcode == "11001101")
        {
            this->size = 2;
        }
        else
        {
            this->size = 1;
        }
    }
};

class INTO : public Instruction
{
public:
    INTO(string opcode, string buffer)
    {
        this->getBasicInfo("INTO", opcode);
        this->size = 1;
    }
};

class IRET : public Instruction
{
public:
    IRET(string opcode, string buffer)
    {
        this->getBasicInfo("IRET", opcode);
        this->size = 1;
    }
};

// processor control cmds
class CLC : public Instruction
{
public:
    CLC(string opcode, string buffer)
    {
        this->getBasicInfo("CLC", opcode);
        this->size = 1;
    }
};

class CMC : public Instruction
{
public:
    CMC(string opcode, string buffer)
    {
        this->getBasicInfo("CMC", opcode);
        this->size = 1;
    }
};

class STC : public Instruction
{
public:
    STC(string opcode, string buffer)
    {
        this->getBasicInfo("STC", opcode);
        this->size = 1;
    }
};

class CLD : public Instruction
{
public:
    CLD(string opcode, string buffer)
    {
        this->getBasicInfo("CLD", opcode);
        this->size = 1;
    }
};

class STD : public Instruction
{
public:
    STD(string opcode, string buffer)
    {
        this->getBasicInfo("STD", opcode);
        this->size = 1;
    }
};

class CLI : public Instruction
{
public:
    CLI(string opcode, string buffer)
    {
        this->getBasicInfo("CLI", opcode);
        this->size = 1;
    }
};

class STI : public Instruction
{
public:
    STI(string opcode, string buffer)
    {
        this->getBasicInfo("STI", opcode);
        this->size = 1;
    }
};

class HLT : public Instruction
{
public:
    HLT(string opcode, string buffer)
    {
        this->getBasicInfo("HLT", opcode);
        this->size = 1;
    }
};

class WAIT : public Instruction
{
public:
    WAIT(string opcode, string buffer)
    {
        this->getBasicInfo("WAIT", opcode);
        this->size = 1;
    }
};

class ESC : public Instruction
{
public:
    ESC(string opcode, string buffer)
    {
        this->getBasicInfo("ESC", opcode);
        this->getBufferInfo(buffer, true, false, true);
        this->size = 2;
    }
};

class LOCK : public Instruction
{
public:
    LOCK(string opcode, string buffer)
    {
        this->getBasicInfo("LOCK", opcode);
        this->size = 1;
    }
};

#endif