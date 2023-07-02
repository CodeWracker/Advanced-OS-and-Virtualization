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

void Processor::run(WorkMemoryTape work_memory, vector<uint8_t> *data_memory)
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
        uint16_t i_address = this->getRegisterValue("IP");
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

        // checa se encontrou a instrução
        if (instruction.mnemonic == "NULL")
        {
            if (DEBUG)
            {
                cout << "Instruction not found" << endl;
            }
            break;
        }

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
        // pega o valor da instrução e soma com o IP e depois soma 1
        uint16_t size_instruction = instruction.hex_code.size() / 2;
        // cout << size_instruction << endl;
        // cout << this->IP.getRegister() << endl;
        this->IP.add(size_instruction);
        // cout << this->IP.getRegister() << endl;
        // cout << "IP: " << this->IP.getRegister() << endl;
        // if (this->IP.getRegister() > 140)
        // {
        //     break;
        // }
    }
}

void Processor::execute(AssemblyCode instruction, vector<uint8_t> *data_memory)
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
    else if (mnemonic == "int")
    {
        this->interrupt(data_memory);
    }
}