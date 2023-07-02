#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "workMemory.hpp"
#include "processor.hpp"
#include "header.hpp"

#include "functions.hpp"

using namespace std;

int main(int argc, char *argv[])
{

    string name = argv[1];
    // Chame o programa externo usando o comando desejado
    const string command = "mmvm -d " + name;

    // le o arquivo
    vector<int8_t> file_bytes = readBinaryFile(name);

    // create a header
    Header header;
    header.readHeader(file_bytes);

    // cria a fita de memoria de dados (int8_t)
    vector<int8_t> data_memory_tape = createMemoryTape(file_bytes, header);

    // [address, hex_code, assembly_code]
    vector<vector<string>> parseredCode = parseAssemblyFromMMVM(command);

    // Crie um objeto MemoryTape
    WorkMemoryTape work_memory_tape;

    // Adicione os MemorySlots na Memory_tape
    work_memory_tape = loadMemoryFromParser(parseredCode);

    // Crie um objeto Processor
    Processor processor;

    return 0;
}