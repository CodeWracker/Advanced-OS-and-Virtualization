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
        this->getBasicInfo(name, opcode);
        this->size = size;
    };
    virtual void getBasicInfo(string name, string opcode)
    {
        this->name = name;
        this->opcode = opcode;
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
    Conflict(string opcode)
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
    MOV(string opcode)
    {

        this->getBasicInfo("MOV", opcode);
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
            this->checkWSize(this->w, 3, 2);
        }
        else if (opcode.substr(0, 6) == "100010") // register/memmory to/from register
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
            if (w == '1')
            {
                // word instruction
                size = 4;
            }
        }
        else // immediate to register memmory
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
        }
    };
    ~MOV(){};
};

class PUSH : public Instruction
{
public:
    PUSH(string opcode)
    {

        this->getBasicInfo("PUSH", opcode);
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
        }
    }
};

class XCHG : public Instruction
{
public:
    XCHG(string opcode)
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
        }
    }
};

class IN : public Instruction
{
public:
    IN(string opcode)
    {

        this->getBasicInfo("IN", opcode);
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
    XLAT(string opcode)
    {

        this->getBasicInfo("XLAT", opcode);
        this->size = 1;
    }
};

class LEA : public Instruction
{
public:
    LEA(string opcode)
    {

        this->getBasicInfo("LEA", opcode);
        this->size = 2 + 1;
    }
};

class LDS : public Instruction
{
public:
    LDS(string opcode)
    {

        this->getBasicInfo("LDS", opcode);
        this->size = 2;
    }
};

class LES : public Instruction
{
public:
    LES(string opcode)
    {

        this->getBasicInfo("LES", opcode);
        this->size = 2;
    }
};

class LAHF : public Instruction
{
public:
    LAHF(string opcode)
    {

        this->getBasicInfo("LAHF", opcode);
        this->size = 1;
    }
};

class SAHF : public Instruction
{
public:
    SAHF(string opcode)
    {

        this->getBasicInfo("SAHF", opcode);
        this->size = 1;
    }
};

class PUSHF : public Instruction
{
public:
    PUSHF(string opcode)
    {

        this->getBasicInfo("PUSHF", opcode);
        this->size = 1;
    }
};

class POPF : public Instruction
{
public:
    POPF(string opcode)
    {

        this->getBasicInfo("POPF", opcode);
        this->size = 1;
    }
};

// arithmetic commands
class ADD : public Instruction
{
public:
    ADD(string opcode)
    {

        this->getBasicInfo("ADD", opcode);
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
            this->checkWSize(this->w, 4, 3);
        }
        else
        { // immediate to acc
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);
        }
    };
};

class ADC : public Instruction
{
public:
    ADC(string opcode)
    {

        this->getBasicInfo("ADC", opcode);
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
            this->checkWSize(this->w, 4, 3);
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
    INC(string opcode)
    {

        this->getBasicInfo("INC", opcode);
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

        this->getBasicInfo("AAA", opcode);
        this->size = 1;
    }
};

class BAA : public Instruction
{
public:
    BAA(string opcode)
    {

        this->getBasicInfo("BAA", opcode);
        this->size = 1;
    }
};

class SUB : public Instruction
{
public:
    SUB(string opcode)
    {

        this->getBasicInfo("SUB", opcode);
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
            this->checkWSize(this->w, 4, 3);
        }
        else
        { // immediate from acc
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);
        }
    }
};
class SSB : public Instruction
{
public:
    SSB(string opcode)
    {

        this->getBasicInfo("SSB", opcode);
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
            this->checkWSize(this->w, 4, 3);
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
    DEC(string opcode)
    {

        this->getBasicInfo("DEC", opcode);
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

        this->getBasicInfo("NEG", opcode);
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

        this->getBasicInfo("CMP", opcode);
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
            this->checkWSize(this->w, 4, 3);
        }
        else
        { // immediate with acc
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);
        }
    }
};

class AAS : public Instruction
{
public:
    AAS(string opcode)
    {

        this->getBasicInfo("AAS", opcode);
        this->size = 1;
    }
};

class DAS : public Instruction
{
public:
    DAS(string opcode)
    {

        this->getBasicInfo("DAS", opcode);
        this->size = 1;
    }
};
class MUL : public Instruction
{
public:
    MUL(string opcode)
    {

        this->getBasicInfo("MUL", opcode);
        this->size = 2;
    }
};

class IMUL : public Instruction
{
public:
    IMUL(string opcode)
    {

        this->getBasicInfo("IMUL", opcode);
        this->size = 2;
    }
};

class AAM : public Instruction
{
public:
    AAM(string opcode)
    {

        this->getBasicInfo("AAM", opcode);
        this->size = 2;
    }
};

class DIV : public Instruction
{
public:
    DIV(string opcode)
    {

        this->getBasicInfo("DIV", opcode);
        this->size = 2;
    }
};
class IDIV : public Instruction
{
public:
    IDIV(string opcode)
    {

        this->getBasicInfo("IDIV", opcode);
        this->size = 2;
    }
};

class AAD : public Instruction
{
public:
    AAD(string opcode)
    {

        this->getBasicInfo("AAD", opcode);
        this->size = 2;
    }
};

class CBW : public Instruction
{
public:
    CBW(string opcode)
    {

        this->getBasicInfo("CBW", opcode);
        this->size = 1;
    }
};

class CWD : public Instruction
{
public:
    CWD(string opcode)
    {

        this->getBasicInfo("CWD", opcode);
        this->size = 1;
    }
};

// logic instructions
class NOT : public Instruction
{
public:
    NOT(string opcode)
    {

        this->getBasicInfo("NOT", opcode);
        this->size = 2;
    }
};
class SHIFT : public Instruction
{
public:
    SHIFT(){};
    SHIFT(string opcode)
    {

        this->getBasicInfo("SHIFT", opcode);
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

        this->getBasicInfo("SHL", opcode);
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

        this->getBasicInfo("SHR", opcode);
        this->size = 2;
        this->getArgs();
    }
};
class SAR : public SHIFT
{
public:
    SAR(string opcode)
    {

        this->getBasicInfo("SAR", opcode);
        this->size = 2;
        this->getArgs();
    }
};
class ROL : public SHIFT
{
public:
    ROL(string opcode)
    {

        this->getBasicInfo("ROL", opcode);
        this->size = 2;
        this->getArgs();
    }
};
class ROR : public SHIFT
{
public:
    ROR(string opcode)
    {

        this->getBasicInfo("ROR", opcode);
        this->size = 2;
        this->getArgs();
    }
};
class RCL : public SHIFT
{
public:
    RCL(string opcode)
    {

        this->getBasicInfo("RCL", opcode);
        this->size = 2;
        this->getArgs();
    }
};
class RCR : public SHIFT
{
public:
    RCR(string opcode)
    {

        this->getBasicInfo("RCR", opcode);
        this->size = 2;
        this->getArgs();
    }
};

class AND : public Instruction
{
public:
    AND(string opcode)
    {

        this->getBasicInfo("AND", opcode);
        if (opcode.substr(0, 6) == "001000")
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
        }
        else if (opcode.substr(0, 6) == "100000")
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
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
    TEST(string opcode)
    {
        this->getBasicInfo("TEST", opcode);
        if (opcode.substr(0, 7) == "1000010")
        {
            this->w = opcode[7];
            this->size = 2;
        }
        else if (opcode.substr(0, 7) == "1111011")
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
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
    OR(string opcode)
    {

        this->getBasicInfo("OR", opcode);
        if (opcode.substr(0, 6) == "000010")
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
        }
        else if (opcode.substr(0, 7) == "1000000")
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
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
    XOR(string opcode)
    {

        this->getBasicInfo("XOR", opcode);
        if (opcode.substr(0, 6) == "001100")
        {
            this->d = opcode[6];
            this->w = opcode[7];
            this->size = 2;
        }
        else if (opcode.substr(0, 7) == "1000000")
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 4, 3);
        }
        else
        {
            this->w = opcode[7];
            this->checkWSize(this->w, 3, 2);
        }
    }
};

class REP : public Instruction
{
public:
    REP(string opcode)
    {

        this->getBasicInfo("REP", opcode);
        this->size = 2;
        this->z = opcode[7];
    }
};

class MOVS : public Instruction
{
public:
    MOVS(string opcode)
    {

        this->getBasicInfo("MOVS", opcode);
        this->size = 2;
        this->w = opcode[7];
    }
};

class CMPS : public Instruction
{
public:
    CMPS(string opcode)
    {

        this->getBasicInfo("CMPS", opcode);
        this->size = 2;
        this->w = opcode[7];
    }
};

class SCAS : public Instruction
{
public:
    SCAS(string opcode)
    {

        this->getBasicInfo("SCAS", opcode);
        this->size = 2;
        this->w = opcode[7];
    }
};

class LODS : public Instruction
{
public:
    LODS(string opcode)
    {

        this->getBasicInfo("LODS", opcode);
        this->size = 2;
        this->w = opcode[7];
    }
};

class STOS : public Instruction
{
public:
    STOS(string opcode)
    {

        this->getBasicInfo("STOS", opcode);
        this->size = 2;
        this->w = opcode[7];
    }
};

class CALL : public Instruction
{
public:
    CALL(string opcode)
    {

        this->getBasicInfo("CALL", opcode);
        this->size = 2;
        this->w = opcode[7];
    }
};

class JMP : public Instruction
{
public:
    JMP(string opcode)
    {
        this->getBasicInfo("JMP", opcode);
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
    RET(string opcode)
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
    JE(string opcode)
    {
        this->getBasicInfo("JE", opcode);
        this->size = 2;
    }
};

class JL : public Instruction
{
public:
    JL(string opcode)
    {
        this->getBasicInfo("JL", opcode);
        this->size = 2;
    }
};

class JLE : public Instruction
{
public:
    JLE(string opcode)
    {
        this->getBasicInfo("JLE", opcode);
        this->size = 2;
    }
};

class JB : public Instruction
{
public:
    JB(string opcode)
    {
        this->getBasicInfo("JB", opcode);
        this->size = 2;
    }
};

class JBE : public Instruction
{
public:
    JBE(string opcode)
    {
        this->getBasicInfo("JBE", opcode);
        this->size = 2;
    }
};

class JP : public Instruction
{
public:
    JP(string opcode)
    {
        this->getBasicInfo("JP", opcode);
        this->size = 2;
    }
};

class JO : public Instruction
{
public:
    JO(string opcode)
    {
        this->getBasicInfo("JO", opcode);
        this->size = 2;
    }
};

class JS : public Instruction
{
public:
    JS(string opcode)
    {
        this->getBasicInfo("JS", opcode);
        this->size = 2;
    }
};

class JNE : public Instruction
{
public:
    JNE(string opcode)
    {
        this->getBasicInfo("JNE", opcode);
        this->size = 2;
    }
};

class JNL : public Instruction
{
public:
    JNL(string opcode)
    {
        this->getBasicInfo("JNL", opcode);
        this->size = 2;
    }
};

class JNLE : public Instruction
{
public:
    JNLE(string opcode)
    {
        this->getBasicInfo("JNLE", opcode);
        this->size = 2;
    }
};

class JNB : public Instruction
{
public:
    JNB(string opcode)
    {
        this->getBasicInfo("JNB", opcode);
        this->size = 2;
    }
};

class JNBE : public Instruction
{
public:
    JNBE(string opcode)
    {
        this->getBasicInfo("JNBE", opcode);
        this->size = 2;
    }
};

class JNP : public Instruction
{
public:
    JNP(string opcode)
    {
        this->getBasicInfo("JNP", opcode);
        this->size = 2;
    }
};

class JNO : public Instruction
{
public:
    JNO(string opcode)
    {
        this->getBasicInfo("JNO", opcode);
        this->size = 2;
    }
};

class JNS : public Instruction
{
public:
    JNS(string opcode)
    {
        this->getBasicInfo("JNS", opcode);
        this->size = 2;
    }
};

class LOOP : public Instruction
{
public:
    LOOP(string opcode)
    {
        this->getBasicInfo("LOOP", opcode);
        this->size = 2;
    }
};

class LOOPZ : public Instruction
{
public:
    LOOPZ(string opcode)
    {
        this->getBasicInfo("LOOPZ", opcode);
        this->size = 2;
    }
};

class LOOPNZ : public Instruction
{
public:
    LOOPNZ(string opcode)
    {
        this->getBasicInfo("LOOPNZ", opcode);
        this->size = 2;
    }
};

class JCXZ : public Instruction
{
public:
    JCXZ(string opcode)
    {
        this->getBasicInfo("JCXZ", opcode);
        this->size = 2;
    }
};

class INT : public Instruction
{
public:
    INT(string opcode)
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
    INTO(string opcode)
    {
        this->getBasicInfo("INTO", opcode);
        this->size = 1;
    }
};

class IRET : public Instruction
{
public:
    IRET(string opcode)
    {
        this->getBasicInfo("IRET", opcode);
        this->size = 1;
    }
};

// processor control cmds
class CLC : public Instruction
{
public:
    CLC(string opcode)
    {
        this->getBasicInfo("CLC", opcode);
        this->size = 1;
    }
};

class CMC : public Instruction
{
public:
    CMC(string opcode)
    {
        this->getBasicInfo("CMC", opcode);
        this->size = 1;
    }
};

class STC : public Instruction
{
public:
    STC(string opcode)
    {
        this->getBasicInfo("STC", opcode);
        this->size = 1;
    }
};

class CLD : public Instruction
{
public:
    CLD(string opcode)
    {
        this->getBasicInfo("CLD", opcode);
        this->size = 1;
    }
};

class STD : public Instruction
{
public:
    STD(string opcode)
    {
        this->getBasicInfo("STD", opcode);
        this->size = 1;
    }
};

class CLI : public Instruction
{
public:
    CLI(string opcode)
    {
        this->getBasicInfo("CLI", opcode);
        this->size = 1;
    }
};

class STI : public Instruction
{
public:
    STI(string opcode)
    {
        this->getBasicInfo("STI", opcode);
        this->size = 1;
    }
};

class HLT : public Instruction
{
public:
    HLT(string opcode)
    {
        this->getBasicInfo("HLT", opcode);
        this->size = 1;
    }
};

class WAIT : public Instruction
{
public:
    WAIT(string opcode)
    {
        this->getBasicInfo("WAIT", opcode);
        this->size = 1;
    }
};

class ESC : public Instruction
{
public:
    ESC(string opcode)
    {
        this->getBasicInfo("ESC", opcode);
        this->size = 2;
    }
};

class LOCK : public Instruction
{
public:
    LOCK(string opcode)
    {
        this->getBasicInfo("LOCK", opcode);
        this->size = 1;
    }
};

#endif