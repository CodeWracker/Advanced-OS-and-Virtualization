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

vector<int> fileReader(string name)
{
    ifstream file(name);
    string line;
    vector<int> instructions;
    int address = 0;
    if (file.is_open())
    {
        while (getline(file, line))
        {

            string opcode;
            Instruction *inst;

            // printa o binário da linha
            vector<string> payload;
            int byteCount = 0;
            int payloadSize;
            for (char c : line)
            {
                if (byteCount < 32)
                {
                    byteCount++;
                    continue;
                }

                if (opcode == "")
                {
                    cout << endl;
                    opcode = bitset<8>(c).to_string();

                    // rest the payload vector
                    payload.clear();

                    // stringstream s1;
                    //  s1 << hex << stoll(opcode, NULL, 2);
                    //  cout << s1.str() << " ";
                    //   find the opcode in the map, if not find print "not find"
                    int opcodeCount = 8;
                    string subOpcode = opcode.substr(0, opcodeCount);
                    // cout << "procurando: " << subOpcode << endl;
                    while (instructionMap.find(subOpcode) == instructionMap.end())
                    {
                        opcodeCount--;
                        if (opcodeCount == 0)
                        {
                            cout << "not find" << endl;
                            break;
                        }
                        subOpcode = opcode.substr(0, opcodeCount);
                        // cout << "procurando: " << subOpcode << endl;
                    }
                    if (opcodeCount != 0)
                    {
                        inst = instructionMap.find(subOpcode)->second(opcode);
                        // cout << inst->name << "(" << inst->size << ") ";
                        payloadSize = inst->size - 1;
                        if (payloadSize == 0)
                        {
                            opcode = "";
                            continue;
                        }
                    }
                    else
                    {
                        opcode = "";
                        continue;
                    }
                }
                else
                {
                    string cmdBinary = bitset<8>(c).to_string();
                    payload.push_back(cmdBinary);
                    // printa o hexadecimal do cmdBinary
                    // stringstream ss;
                    // ss << hex << stoll(cmdBinary, NULL, 2);
                    // cout << ss.str() << " ";
                    payloadSize--;
                    if (payloadSize == 0)
                    {
                        inst->setAddress(address);
                        address += inst->size;

                        inst->setPayload(payload);

                        // check for conflicts
                        // verify if inst is of the Instruction type
                        if ((dynamic_cast<Conflict *>(inst)))
                        {

                            Conflict *conflict = dynamic_cast<Conflict *>(inst);
                            if (conflict->type == ConflictTypesEnum::SHIFT)
                            {
                                string p1 = payload[0];
                                free(inst);
                                if (p1.substr(2, 3) == "100")
                                {
                                    inst = createSHL(opcode);
                                }
                                else if (p1.substr(2, 3) == "101")
                                {
                                    inst = createSHR(opcode);
                                }
                                else if (p1.substr(2, 3) == "111")
                                {
                                    inst = createSAR(opcode);
                                }
                                else if (p1.substr(2, 3) == "000")
                                {
                                    inst = createROL(opcode);
                                }
                                else if (p1.substr(2, 3) == "001")
                                {
                                    inst = createROR(opcode);
                                }
                                else if (p1.substr(2, 3) == "010")
                                {
                                    inst = createRCL(opcode);
                                }
                                else // 011
                                {
                                    inst = createRCR(opcode);
                                }

                                inst->setPayload(payload);
                            }
                            else if (conflict->type == ConflictTypesEnum::BIT7_1)
                            {
                                string p1 = payload[0];

                                free(inst);
                                if (p1.substr(2, 3) == "000")
                                {
                                    inst = createADD(opcode);
                                }
                                else if (p1.substr(2, 3) == "010")
                                {
                                    inst = createADC(opcode);
                                }
                                else if (p1.substr(2, 3) == "101")
                                {
                                    inst = createSUB(opcode);
                                }
                                else if (p1.substr(2, 3) == "011")
                                {
                                    inst = createSSB(opcode);
                                }
                                else if (p1.substr(2, 3) == "111")
                                {
                                    inst = createCMP(opcode);
                                }
                                else if (p1.substr(2, 3) == "100")
                                {
                                    inst = createAND(opcode);
                                }
                                else if (p1.substr(2, 3) == "001")
                                {
                                    inst = createOR(opcode);
                                    inst->setPayload(payload);
                                }
                                else // 110
                                {
                                    inst = createXOR(opcode);
                                }

                                inst->setPayload(payload);
                            }
                            else if (conflict->type == ConflictTypesEnum::ALL_1)
                            {
                                string p1 = payload[0];

                                free(inst);
                                if (p1.substr(2, 3) == "110")
                                {
                                    inst = createPUSH(opcode);
                                }
                                else if (p1.substr(2, 3) == "010")
                                {
                                    inst = createCALL(opcode);
                                }
                                else if (p1.substr(2, 3) == "011")
                                {
                                    inst = createCALL(opcode);
                                }
                                else if (p1.substr(2, 3) == "100")
                                {
                                    inst = createJMP(opcode);
                                }
                                else // 101
                                {
                                    inst = createJMP(opcode);
                                }

                                inst->setPayload(payload);
                            }
                            else if (conflict->type == ConflictTypesEnum::ALL_1)
                            {

                                string p1 = payload[0];

                                if (p1.substr(2, 3) == "110")
                                {
                                    inst = createNEG(opcode);
                                }
                                else if (p1.substr(2, 3) == "110")
                                {
                                    inst = createMUL(opcode);
                                }
                                else if (p1.substr(2, 3) == "101")
                                {
                                    inst = createIMUL(opcode);
                                }
                                else if (p1.substr(2, 3) == "110")
                                {
                                    inst = createDIV(opcode);
                                }
                                else if (p1.substr(2, 3) == "111")
                                {
                                    inst = createIDIV(opcode);
                                }
                                else if (p1.substr(2, 3) == "010")
                                {
                                    inst = createNOT(opcode);
                                }
                                else if (p1.substr(2, 3) == "000")
                                {
                                    inst = createTEST(opcode);
                                }

                                inst->setPayload(payload);
                            }
                        }

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
            }
        }
        file.close();
    }
    else
    {
        cout << "Unable to open file";
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