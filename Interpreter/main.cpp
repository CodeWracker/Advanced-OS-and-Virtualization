#include "defines.hpp"

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
    cout << argc << endl;

    // get the last argument
    string name = argv[argc - 1];
    // Chame o programa externo usando o comando desejado
    const string command = "mmvm -d " + name;

    // le o arquivo
    vector<uint8_t> file_bytes = readBinaryFile(name);

    // create a header
    Header header;
    header.readHeader(file_bytes);

    // cria a fita de memoria de dados (uint8_t)
    vector<uint8_t> data_memory_tape = createMemoryTape(file_bytes, header);

    // [address, hex_code, assembly_code]
    vector<vector<string>> parseredCode = parseAssemblyFromMMVM(command);

    // Crie um objeto MemoryTape
    WorkMemoryTape work_memory_tape;

    // Adicione os MemorySlots na Memory_tape
    work_memory_tape = loadMemoryFromParser(parseredCode);

    // Crie um objeto Processor
    Processor processor;
    int qtd_of_args = argc - 1;
    char *new_arg_list[qtd_of_args];
    for (int i = 0; i < qtd_of_args; i++)
    {
        new_arg_list[i] = argv[i + 1];
    }

    processor.load_memories(work_memory_tape, data_memory_tape);
    processor.load_args(qtd_of_args, new_arg_list);

    // simula o processador
    processor.run();

    return 0;
}