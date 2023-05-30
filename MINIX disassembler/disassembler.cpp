#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <sstream>
#include "Instruction.hpp"
#include "InstructionMap.hpp"
using namespace std;

// a map of string to fucntions tact return Instruction*
// this will be available in the main file

map<string, Instruction *(*)(string, string)> shiftConflictMap = {
    {"100", &createSHL},
    {"101", &createSHR},
    {"111", &createSAR},
    {"000", &createROL},
    {"001", &createROR},
    {"010", &createRCL},
    {"011", &createRCR},
};

map<string, Instruction *(*)(string, string)> bit7_1ConflictMap = {
    {"000", &createADD},
    {"010", &createADC},
    {"101", &createSUB},
    {"011", &createSBB},
    {"111", &createCMP},
    {"100", &createAND},
    {"001", &createOR},
    {"110", &createXOR},
};

map<string, Instruction *(*)(string, string)> all1ConflictMap = {
    {"110", &createPUSH},
    {"010", &createCALL},
    {"011", &createCALL},
    {"100", &createJMP},
    {"101", &createJMP},
};

map<string, Instruction *(*)(string, string)> testMulConflictMap = {
    {"011", &createNEG},
    {"100", &createMUL},
    {"101", &createIMUL},
    {"110", &createDIV},
    {"111", &createIDIV},
    {"010", &createNOT},
    {"000", &createTEST},
};

Instruction *checkConflict(Instruction *inst, string buffer)
{
    string opcode = inst->opcode;
    if ((dynamic_cast<Conflict *>(inst)))
    {
        Conflict *conflict = dynamic_cast<Conflict *>(inst);
        if (conflict->type == ConflictTypesEnum::SHIFT)
        {
            string p1 = buffer;
            free(inst);
            // search in map
            if (shiftConflictMap.find(p1.substr(2, 3)) != shiftConflictMap.end())
            {
                inst = shiftConflictMap[p1.substr(2, 3)](opcode, buffer);
            }
        }
        else if (conflict->type == ConflictTypesEnum::BIT7_1)
        {
            string p1 = buffer;
            free(inst);
            // search in map
            if (bit7_1ConflictMap.find(p1.substr(2, 3)) != bit7_1ConflictMap.end())
            {
                inst = bit7_1ConflictMap[p1.substr(2, 3)](opcode, buffer);
            }
        }
        else if (conflict->type == ConflictTypesEnum::ALL_1)
        {
            string p1 = buffer;
            cout << "error all 1" << endl;
            free(inst);
            // search in map
            if (all1ConflictMap.find(p1.substr(2, 3)) != all1ConflictMap.end())
            {
                inst = all1ConflictMap[p1.substr(2, 3)](opcode, buffer);
            }
            else
            {
                cout << "error all 1 not find" << endl;
            }
        }
        else if (conflict->type == ConflictTypesEnum::TEST_MUL_OTHERS)
        {
            string p1 = buffer;
            // search in map
            if (testMulConflictMap.find(p1.substr(2, 3)) != testMulConflictMap.end())
            {
                inst = testMulConflictMap[p1.substr(2, 3)](opcode, buffer);
            }
        }
    }
    return inst;
}
void printInst(Instruction *inst)
{

    string out = inst->getHexAddr() + ": " + inst->getHex();

    // make the out have 20 chars (complete with " ")
    while (out.size() < 20)
    {
        out += " ";
    }
    cout << out << inst->name << " " << inst->getPayloadInfo() << endl;
}
vector<Instruction *> fileReader(string name)
{
    string line;
    int byteCount = 0;
    vector<string> lines;
    ifstream file(name, ios::binary);
    if (file.is_open())
    {

        char byte;
        while (file.get(byte))
        {
            if (byteCount < 32)
            {
                byteCount++;
                continue;
            }
            bitset<8> bits(byte);
            stringstream ss;
            ss << bits;
            lines.push_back(ss.str());
        }
        file.close();
    }
    else
    {
        cout << "Unable to open file";
    }

    string buffer = "";
    int address = 0;
    string opcode = "";
    vector<Instruction *> instructions;
    vector<string> payload;

    Instruction *inst;
    int payloadSize;
    cout << endl;
    while (address < lines.size() - 1)
    {
        if (opcode == "")
        {

            opcode = lines.at(address);
            buffer = lines.at(address + 1);

            payload.clear();

            int opcodeCount = 8;
            string subOpcode = opcode.substr(0, opcodeCount);

            while (instructionMap.find(subOpcode) == instructionMap.end())
            {
                opcodeCount--;
                if (opcodeCount == 0)
                {
                    // cout << "not find" << endl;
                    break;
                }
                subOpcode = opcode.substr(0, opcodeCount);
            }
            // cout << "terminei de procurar com o opcode: " << subOpcode << endl;
            if (opcodeCount != 0)
            {
                inst = instructionMap.find(subOpcode)->second(opcode, buffer);
                inst = checkConflict(inst, buffer);
                inst->setAddress(address);
                payloadSize = inst->size - 1;
                instructions.push_back(inst);
                if (payloadSize == 0)
                {
                    opcode = "";
                    printInst(inst);
                    address++;
                    continue;
                }
            }
            else
            {
                opcode = "";
                address++;
                continue;
            }
        }
        else
        {
            string cmdBinary = lines.at(address);
            payload.push_back(cmdBinary);
            payloadSize--;
            if (payloadSize == 0)
            {

                inst->setPayload(payload);
                printInst(inst);
                opcode = "";
            }
        }
        address++;
    }

    return instructions;
}

int main(int argc, char *argv[])
{

    string name = argv[1];
    cout << "lendo o arquivo: " << name << endl;
    vector<Instruction *> instructions = fileReader(name);

    // for (Instruction *inst : instructions)
    // {
    //     printInst(inst);
    // }

    return 0;
}