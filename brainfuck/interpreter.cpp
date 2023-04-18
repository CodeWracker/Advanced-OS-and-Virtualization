#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

// this program implements an interpreter for the brainfuck language

vector<char> fileReader(string name)
{
    // read the file in the tests folder
    ifstream file("tests/" + name);
    string line;
    vector<char> instructions;
    getline(file, line);
    // cout << line << endl;

    for (char ch : line)
    {
        // cout << ch << endl;
        instructions.push_back(ch);
    }

    return instructions;
}

void handleInstruction(int &cellPosition, vector<char> instructions, vector<int> &mem, int &instPosition)
{
    // cout << "cellPos:" << cellPosition << " / instPos:" << instPosition << endl;
    // cout << "intruction:" << instructions[instPosition] << endl;
    // cout << "memHead:" << mem[cellPosition] << endl;
    if (instructions[instPosition] == '+')
    {
        mem[cellPosition] = mem[cellPosition] + 1;
        return;
    }
    if (instructions[instPosition] == '-')
    {
        mem[cellPosition] = mem[cellPosition] - 1;
        return;
    }
    if (instructions[instPosition] == '>')
    {
        cellPosition++;
    }
    if (instructions[instPosition] == '<')
    {
        cellPosition--;
    }
    if (instructions[instPosition] == '[')
    {
        if (mem[cellPosition] == 0)
        {
            // jump to the matching closing bracket
            int nesting_level = 1;
            while (nesting_level > 0)
            {
                instPosition++;
                if (instructions[instPosition] == '[')
                {
                    nesting_level++;
                }
                else if (instructions[instPosition] == ']')
                {
                    nesting_level--;
                }
            }
        }
        return;
    }
    if (instructions[instPosition] == ']')
    {
        if (mem[cellPosition] != 0)
        {
            // jump back to the matching opening bracket
            int nesting_level = 1;
            while (nesting_level > 0)
            {
                instPosition--;
                if (instructions[instPosition] == '[')
                {
                    nesting_level--;
                }
                else if (instructions[instPosition] == ']')
                {
                    nesting_level++;
                }
            }
        }
        return;
    }
    if (instructions[instPosition] == '.')
    {
        char out = mem[cellPosition];
        cout << out;
        return;
    }
    if (instructions[instPosition] == ',')
    {
        char input;
        cin >> input;
        mem[cellPosition] = input;
        return;
    }
}

// main function receives the file name as an arg
int main(int argc, char *argv[])
{
    // check the length of the argv
    // if()

    string name = argv[1];
    vector<char> bfInstructions = fileReader(name);
    vector<int> mem;
    for (int i = 0; i < 1000; i++)
    {
        mem.push_back(0);
    }
    int memCellPosition = 0;
    int instPosition = 0;

    while (instPosition < bfInstructions.size())
    {
        handleInstruction(memCellPosition, bfInstructions, mem, instPosition);
        instPosition++;
    }

    return 0;
}