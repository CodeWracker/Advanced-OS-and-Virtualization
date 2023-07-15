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

void Processor::run()
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
        uint16_t i_address = this->getRegisterValue("ip");
        // cout << "IP: " << i_address << endl;
        stringstream ss;
        ss << hex << i_address;
        string s_address_hex = ss.str();

        // se o numero tiver menos de 4 caracteres, completa com 0 na esquerda
        while (s_address_hex.size() < 4)
        {
            s_address_hex = "0" + s_address_hex;
        }

        // cout << s_address_hex << endl;

        instruction = this->work_memory.getWorkMemorySlot(s_address_hex).assembly_code;

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

        if (DEBUG)
        {
            cout << this->getState() << endl;
        }
        // executa a instrução
        this->execute(instruction);

        // atualiza o IP
        // pega o valor da instrução e soma com o IP e depois soma 1
        uint16_t size_instruction = instruction.hex_code.size() / 2;
        // cout << size_instruction << endl;
        // cout << this->IP.getRegister() << endl;
        if (!this->flags.jump)
        {
            this->IP.add(size_instruction);
        }
        this->flags.jump = false;
    }
}

void Processor::execute(AssemblyCode instruction)
{

    // pega o mnemonico
    string mnemonic = instruction.mnemonic;

    // pega os operandos
    string operand1 = instruction.operand1;
    string operand2 = instruction.operand2;

    // lida com os mnemonicos com o switch
    if (mnemonic == "mov")
    {
        this->mov_(operand1, operand2);
    }
    else if (mnemonic == "int")
    {
        this->int_();
    }
    else if (mnemonic == "xor")
    {
        this->xor_(operand1, operand2);
    }
    else if (mnemonic == "sub")
    {
        this->sub_(operand1, operand2);
    }
    else if (mnemonic == "pop")
    {
        this->pop_(operand1);
    }
    else if (mnemonic == "dec")
    {
        this->dec_(operand1);
    }
    else if (mnemonic == "add")
    {
        this->add_(operand1, operand2);
    }
    else if (mnemonic == "hlt")
    {
        this->hlt_();
    }
    else if (mnemonic == "jne")
    {
        this->jne_(operand1);
    }
    else if (mnemonic == "lea")
    {
        this->lea_(operand1, operand2);
    }
    else if (mnemonic == "cmp")
    {
        this->cmp_(operand1, operand2);
    }
    else if (mnemonic == "jnb")
    {
        this->jnb_(operand2);
    }
    else if (mnemonic == "test")
    {
        this->test_(operand1, operand2);
    }
    else
    {
        cout << "ainda não implementei o mnemonico " << mnemonic << endl;
    }
}