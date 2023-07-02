#include "processor.hpp"
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "workMemory.hpp"
#include "processor.hpp"
#include "header.hpp"
using namespace std;

void Processor::run(WorkMemoryTape work_memory, vector<int8_t> *data_memory)
{
    if (DEBUG)
    {
        cout << this->getStateHeader() << endl;
    }
    while (1)
    {
        // pega a instrução
        AssemblyCode instruction;

        // pega o valor do endereço usando o getRegisterValue e converte para hexadecimal em string
        int16_t i_address = this->getRegisterValue("IP");
        stringstream ss;
        ss << hex << i_address;
        string s_address_hex = ss.str();

        // se o numero tiver menos de 4 caracteres, completa com 0 na esquerda
        while (s_address_hex.size() < 4)
        {
            s_address_hex = "0" + s_address_hex;
        }

        // cout << s_address_hex << endl;

        instruction = work_memory.getWorkMemorySlot(s_address_hex).assembly_code;

        // cout << instruction.getString() << endl;

        // atualiza a ultima instrução
        lastInstruction = instruction;

        // executa a instrução
        this->execute(instruction, data_memory);
        if (DEBUG)
        {
            cout << this->getState() << endl;
        }

        // atualiza o IP
        IP.add(1);
        break;
    }
}

void Processor::execute(AssemblyCode instruction, vector<int8_t> *data_memory)
{

    // pega o mnemonico
    string mnemonic = instruction.mnemonic;

    // pega os operandos
    string operand1 = instruction.operand1;
    string operand2 = instruction.operand2;

    // lida com os mnemonicos com o switch
    if (mnemonic == "mov")
    {
        this->mov(operand1, operand2, data_memory);
    }
}