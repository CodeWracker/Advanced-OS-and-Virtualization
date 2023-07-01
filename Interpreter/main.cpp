#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "memory.hpp"
#include "functions.hpp"

using namespace std;

int main(int argc, char *argv[])
{

    string name = argv[1];
    // Chame o programa externo usando o comando desejado
    const string command = "mmvm -d " + name;
    // [address, hex_code, assembly_code]
    vector<vector<string>> parseredCode = parseAssemblyFromMMVM(command);

    // Crie um objeto MemoryTape
    MemoryTape memory_tape;

    // Adicione os MemorySlots na Memory_tape
    memory_tape = loadMemoryFromParser(parseredCode);

    return 0;
}