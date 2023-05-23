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

map<string, Instruction *(*)(string)> shiftConflictMap = {
    {"100", &createSHL},
    {"101", &createSHR},
    {"111", &createSAR},
    {"000", &createROL},
    {"001", &createROR},
    {"010", &createRCL},
    {"011", &createRCR},
};

map<string, Instruction *(*)(string)> bit7_1ConflictMap = {
    {"000", &createADD},
    {"010", &createADC},
    {"101", &createSUB},
    {"011", &createSSB},
    {"111", &createCMP},
    {"100", &createAND},
    {"001", &createOR},
    {"110", &createXOR},
};

map<string, Instruction *(*)(string)> all1ConflictMap = {
    {"000", &createPUSH},
    {"010", &createCALL},
    {"011", &createCALL},
    {"100", &createJMP},
    {"101", &createJMP},
};

map<string, Instruction *(*)(string)> testMulConflictMap = {
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
                inst = shiftConflictMap[p1.substr(2, 3)](opcode);
            }
        }
        else if (conflict->type == ConflictTypesEnum::BIT7_1)
        {
            string p1 = buffer;
            free(inst);
            // search in map
            if (bit7_1ConflictMap.find(p1.substr(2, 3)) != bit7_1ConflictMap.end())
            {
                inst = bit7_1ConflictMap[p1.substr(2, 3)](opcode);
            }
        }
        else if (conflict->type == ConflictTypesEnum::ALL_1)
        {
            string p1 = buffer;

            free(inst);
            // search in map
            if (all1ConflictMap.find(p1.substr(2, 3)) != all1ConflictMap.end())
            {
                inst = all1ConflictMap[p1.substr(2, 3)](opcode);
            }
        }
        else if (conflict->type == ConflictTypesEnum::TEST_MUL_OTHERS)
        {
            string p1 = buffer;
            // search in map
            if (testMulConflictMap.find(p1.substr(2, 3)) != testMulConflictMap.end())
            {
                inst = testMulConflictMap[p1.substr(2, 3)](opcode);
            }
        }
    }
    return inst;
}

vector<int> fileReader(string name)
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
    vector<int> instructions;
    vector<string> payload;

    Instruction *inst;
    int payloadSize;
    while (address < lines.size() - 1)
    {
        if (opcode == "")
        {

            opcode = lines.at(address);
            buffer = lines.at(address + 1);

            cout << endl;
            // cout << "opcode: " << hex << stoll(opcode, NULL, 2)
            //      << " | buffer: " << hex << stoll(buffer, NULL, 2) << endl;

            payload.clear();

            int opcodeCount = 8;
            string subOpcode = opcode.substr(0, opcodeCount);

            while (instructionMap.find(subOpcode) == instructionMap.end())
            {
                opcodeCount--;
                if (opcodeCount == 0)
                {
                    cout << "not find" << endl;
                    break;
                }
                subOpcode = opcode.substr(0, opcodeCount);
            }
            // cout << "terminei de procurar com o opcode: " << subOpcode << endl;
            if (opcodeCount != 0)
            {
                inst = instructionMap.find(subOpcode)->second(opcode);
                inst = checkConflict(inst, buffer);
                inst->setAddress(address);
                payloadSize = inst->size - 1;
                if (payloadSize == 0)
                {
                    opcode = "";
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

                opcode = "";
                string out = inst->getHexAddr() + ": " + inst->getHex();
                // make the out have 20 chars (complete with " ")
                while (out.size() < 20)
                {
                    out += " ";
                }
                cout << out << inst->name;
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
    vector<int> instructions = fileReader(name);

    // for (int i = 0; i < instructions.size(); i++)
    //{
    //     cout << instructions[i] << endl;
    // }

    return 0;
}