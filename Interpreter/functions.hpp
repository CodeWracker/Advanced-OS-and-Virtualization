#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include "workMemory.hpp"

vector<vector<string>> parseAssemblyFromMMVM(const string &command)
{
    // Defina um nome para o arquivo temporário
    const string tmpFileName = "output.tmp";

    // Construa o comando para redirecionar a saída para o arquivo temporário
    string commandWithOutputRedirect = command + " > " + tmpFileName;

    // Execute o comando usando system
    system((commandWithOutputRedirect + "> ./" + tmpFileName + " 2>&1").c_str());
    // system("clear");

    // Abra o arquivo temporário para leitura
    ifstream tmpFile(tmpFileName);
    if (!tmpFile)
    {
        throw runtime_error("Falha ao abrir o arquivo temporário");
    }

    // Leia o conteúdo do arquivo temporário e armazene cada linha em um vetor
    vector<vector<string>> outputLines;
    string line;
    while (getline(tmpFile, line))
    {
        string address;
        string hex_code;
        string assembly_code;
        // split on ':' and get the address and hex_code
        int pos = line.find(':');
        address = line.substr(0, pos);
        hex_code = line.substr(line.find(' ') + 1, line.length() - 1);
        hex_code = hex_code.substr(0, hex_code.find(' '));

        string aux;
        aux = line.substr(line.find(' ') + 1, line.length() - 1);
        aux = aux.substr(aux.find(' ') + 1, aux.length() - 1);
        // como tem um monte de " " no aux, tem que tirar eles
        for (int i = 0; i < aux.length(); i++)
        {
            if (aux[i] == ' ')
            {
                continue;
            }
            else
            {
                assembly_code = aux.substr(i, aux.length() - 1);
                break;
            }
        }

        if (assembly_code == "(undefined)")
        {
            continue;
        }
        vector<string> line;
        line.push_back(address);
        line.push_back(hex_code);
        line.push_back(assembly_code);
        outputLines.push_back(line);
    }

    // Feche e remova o arquivo temporário
    tmpFile.close();
    remove(tmpFileName.c_str());

    // Retorne o vetor com as linhas de saída
    return outputLines;
}

WorkMemoryTape loadMemoryFromParser(vector<vector<string>> parseredCode)
{
    // Crie uma fita de memória
    WorkMemoryTape work_memory_tape;
    for (const auto &code : parseredCode)
    {
        WorkMemorySlot work_memory_slot;
        work_memory_slot.address = code[0];
        work_memory_slot.hex_code = code[1];
        AsseblyCode assembly_code;
        assembly_code.setCode(code[2]);
        // printf("%s\n", assembly_code.getString().c_str());

        work_memory_slot.assembly_code = assembly_code;
        work_memory_tape.addWorkMemorySlot(work_memory_slot);
    }

    // Retorne a fita de memória
    return work_memory_tape;
}

vector<int8_t> readBinaryFile(string path)
{
    // opens the file on the path
    // creates a vector of int8_t
    // load the bytes into the vector

    vector<int8_t> binary_file;

    ifstream file(path, ios::binary);
    if (!file)
    {
        throw runtime_error("Falha ao abrir o arquivo");
    }
    if (file.is_open())
    {
        int8_t byte;
        while (file.read((char *)&byte, sizeof(byte)))
        {
            binary_file.push_back(byte);
        }
    }
    file.close();
    return binary_file;
}

#endif // FUNCTIONS_HPP
