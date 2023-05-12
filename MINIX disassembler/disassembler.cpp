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
                    opcode = bitset<8>(c).to_string();
                    // find the opcode in the map, if not find print "not find"
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
                    if (opcodeCount != 0)
                    {
                        inst = instructionMap.find(subOpcode)->second(opcode);
                        cout << inst->name << endl;
                        payloadSize = inst->size - 1;
                    }
                    else
                    {
                        opcode = "";
                        continue;
                    }
                }
                else
                {
                    payload.push_back(bitset<8>(c).to_string());
                    payloadSize--;
                    if (payloadSize == 0)
                    {
                        opcode = "";
                    }
                }

                // cout << bitset<8>(c) << endl;
                // if (s2 != "")
                //{
                //   s1 = "";
                //   s2 = "";
                //}

                // if (s1 == "")
                //{
                //    s1 = bitset<8>(c).to_string();
                //}
                // else
                //{
                //  s2 = bitset<8>(c).to_string();
                //  i++;
                // if (i > 15)
                // {
                //    cout << s1 << " " << s2 << " / ";
                //   stringstream ss;
                //   ss << hex << stoll(s1 + s2, NULL, 2);
                //   cout << ss.str() << endl;
                ////}
                //}
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