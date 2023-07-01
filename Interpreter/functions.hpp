#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include "memory.hpp"

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

MemoryTape loadMemoryFromParser(vector<vector<string>> parseredCode)
{
    // Crie uma fita de memória
    MemoryTape memoryTape;
    for (const auto &code : parseredCode)
    {
        MemorySlot memorySlot;
        memorySlot.address = code[0];
        memorySlot.hex_code = code[1];
        AsseblyCode assembly_code;
        assembly_code.setCode(code[2]);
        // printf("%s\n", assembly_code.getString().c_str());

        memorySlot.assembly_code = assembly_code;
        memoryTape.addMemorySlot(memorySlot);
    }

    // Retorne a fita de memória
    return memoryTape;
}

#endif // FUNCTIONS_HPP
