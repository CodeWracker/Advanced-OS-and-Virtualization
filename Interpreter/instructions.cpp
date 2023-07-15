#include "processor.hpp"
#include "defines.hpp"
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

void Processor::handle_operators_for_algebra(string op1, string op2, uint16_t *addr1, uint16_t *word1, bool *dest_is_addr, uint16_t *addr2, uint16_t *word2, bool *src_is_addr)
{
    if (op1[0] == '[')
    {
        *(dest_is_addr) = true;
        // pega o endereço
        string s_address1 = (op1.substr(1, op1.size() - 2));
        // verifica se tem um offset (um + ou -)

        // verifica se tem +
        if (s_address1.find('+') != string::npos)
        {
            // pega o offset
            string s_offset = s_address1.substr(s_address1.find('+') + 1);
            // pega o endereço
            s_address1 = s_address1.substr(0, s_address1.find('+'));

            // pega o valor do endereço registrador + offset
            *(word1) = getRegisterValue(s_address1) + stoi(s_offset, nullptr, 16);
        }
        else if (s_address1.find('-') != string::npos)
        {
            // pega o offset
            string s_offset = s_address1.substr(s_address1.find('-') + 1);
            // pega o endereço
            s_address1 = s_address1.substr(0, s_address1.find('-'));

            // pega o valor do endereço registrador - offset
            // pega o valor em hexadecimal do offset
            *(word1) = getRegisterValue(s_address1) - stoi(s_offset, nullptr, 16);
        }
        else
        {
            // pega o valor do endereço
            // verifica se é um registrador ou um numero em hexadecimal
            // se o tamanho for 2 é um registrador
            if (s_address1.size() == 2)
            {
                // pega o valor do registrador

                *(addr1) = getRegisterValue(s_address1);
                *(word1) = this->physical_memory[*(addr1) + 1] << 8 | this->physical_memory[*(addr1)];
            }
            else
            {
                // converte o valor do endereço para inteiro e coloca em *(addr1)
                *(addr1) = stoi(s_address1, nullptr, 16);
                *(word1) = this->physical_memory[*(addr1) + 1] << 8 | this->physical_memory[*(addr1)];
            }
        }
    }
    else
    {
        // pega o valor do endereço só convertendo para inteiro
        *(word1) = getRegisterValue(op1);
    }

    if (op2[0] == '[')
    {
        *(src_is_addr) = true;
        // pega o endereço
        string s_address2 = (op2.substr(1, op2.size() - 2));
        // verifica se tem um offset (um + ou -)

        // verifica se tem +
        if (s_address2.find('+') != string::npos)
        {
            // pega o offset
            string s_offset = s_address2.substr(s_address2.find('+') + 1);
            // pega o endereço
            s_address2 = s_address2.substr(0, s_address2.find('+'));

            // pega o valor do endereço registrador + offset
            *(word2) = getRegisterValue(s_address2) + stoi(s_offset, nullptr, 16);
        }
        else if (s_address2.find('-') != string::npos)
        {
            // pega o offset
            string s_offset = s_address2.substr(s_address2.find('-') + 1);
            // pega o endereço
            s_address2 = s_address2.substr(0, s_address2.find('-'));

            // pega o valor do endereço registrador - offset
            // pega o valor em hexadecimal do offset
            *(word2) = getRegisterValue(s_address2) - stoi(s_offset, nullptr, 16);
        }
        else
        {
            // pega o valor do endereço
            // verifica se é um registrador ou um numero em hexadecimal
            // se o tamanho for 2 é um registrador
            if (s_address2.size() == 2)
            {
                // pega o valor do registrador
                *(addr2) = getRegisterValue(s_address2);
                *(word2) = this->physical_memory[*(addr2) + 1] << 8 | this->physical_memory[*(addr2)];
            }
            else
            {
                // converte o valor do endereço para inteiro e coloca em addr1
                *(addr2) = stoi(s_address2, nullptr, 16);
                *(word2) = this->physical_memory[*(addr2) + 1] << 8 | this->physical_memory[*(addr2)];
            }
        }
    }
    else
    {
        // verify if is a register
        if (op2.size() == 2)
        {
            *(word2) = getRegisterValue(op2);
        }
        else
        {
            *(word2) = stoi(op2, nullptr, 16);
        }
    }
}

void Processor::hlt_()
{
    if (DEBUG)
    {
        cout << "hlt - exiting" << endl;
    }
    exit(0);
}
void Processor::jne_(string operand1)
{

    if (this->flags.ZF == 0)
    {

        uint16_t addr = stoi(operand1, nullptr, 16);
        this->IP.high = addr >> 8;
        this->IP.low = addr & 0xFF;
        // cout << this->IP.getRegister() << endl;
        this->flags.jump = true;
        this->flags.ZF = 0;
    }
}
void Processor::jnb_(string op2)
{
    if (this->flags.CF == 0)
    {
        uint16_t addr = stoi(op2, nullptr, 16);
        this->IP.high = addr >> 8;
        this->IP.low = addr & 0xFF;
        // cout << this->IP.getRegister() << endl;
        this->flags.jump = true;
        this->flags.CF = 0;
    }
}
void Processor::test_(string op1, string op2)
{

    // set flags to 0
    this->flags.CF = 0;
    this->flags.SF = 0;
    this->flags.ZF = 0;

    // verifica se o destino é um endereço ou um registrador
    bool dest_is_addr = false;
    uint16_t addr1;
    uint16_t word1;
    // verifica se a origem é um endereço ou um registrador
    uint16_t addr2;
    uint16_t word2;
    bool src_is_addr = false;

    handle_operators_for_algebra(op1, op2, &addr1, &word1, &dest_is_addr, &addr2, &word2, &src_is_addr);

    // cout << "word1 = " << word1 << endl;
    // cout << "word2 = " << word2 << endl;
    // cout << "addr1 = " << addr1 << endl;
    // cout << "addr2 = " << addr2 << endl;
    // cout << "dest_is_addr = " << dest_is_addr << endl;
    // cout << "src_is_addr = " << src_is_addr << endl;

    uint16_t result = word1 & word2;

    if (result == 0)
    {
        this->flags.ZF = 1;
    }
    else
    {
        this->flags.ZF = 0;
    }
}
void Processor::lea_(string op1, string op2)
{
    // op1 sempre é um registrador
    // op 2 sempre é um registrador com offst de endereço[reg+-num]
    // verifica se o destino é um endereço ou um registrador

    // pega o endereço
    uint16_t addr;
    // pega o offset
    string offset;
    if (op2.find('+') != string::npos)
    {
        // pega a posição do +
        // faz uma substring do proximo caracter até o final - 1, pq ignoramos o ]
        offset = op2.substr(op2.find('+') + 1, op2.size() - op2.find(']'));
        string reg = op2.substr(1, op2.find('+') - 1);
        uint16_t value = this->getRegisterValue(reg);
        addr = value + stoi(offset, nullptr, 16);
    }
    else
    {
        // pega a posição do -
        // faz uma substring do proximo caracter até o final - 1, pq ignoramos o ]
        offset = op2.substr(op2.find('-') + 1, op2.size() - op2.find(']'));
        string reg = op2.substr(1, op2.find('-') - 1);
        uint16_t value = this->getRegisterValue(reg);
        addr = value - stoi(offset, nullptr, 16);
    }

    // poe no registrador o endereço
    this->setRegisterValue(op1, addr);
}

void Processor::cmp_(string op1, string op2)
{
    // verifica se o destino é um endereço ou um registrador
    bool dest_is_addr = false;
    uint16_t addr1;
    uint16_t word1;
    // verifica se a origem é um endereço ou um registrador
    uint16_t addr2;
    uint16_t word2;
    bool src_is_addr = false;

    handle_operators_for_algebra(op1, op2, &addr1, &word1, &dest_is_addr, &addr2, &word2, &src_is_addr);

    if (word1 == word2)
    {
        this->flags.ZF = 1;
        return;
    }
    if (word1 > word2)
    {
        return;
    }
    if (word1 < word2)
    {
        this->flags.CF = 1;
        this->flags.SF = 1;
    }
}

void Processor::mov_(string op1, string op2)
{

    // verifica se o destino é um endereço ou um registrador
    bool dest_is_addr = false;
    uint16_t addr1;
    uint16_t word1;
    // verifica se a origem é um endereço ou um registrador
    uint16_t addr2;
    uint16_t word2;
    bool src_is_addr = false;

    handle_operators_for_algebra(op1, op2, &addr1, &word1, &dest_is_addr, &addr2, &word2, &src_is_addr);

    if (dest_is_addr)
    {
        // atualiza o valor do endereço
        this->physical_memory[word1] = (word2 >> 8) & 0xFF;
        this->physical_memory[word1 + 1] = word2 & 0xFF;
    }
    else
    {
        // atualiza o valor do registrador
        this->setRegisterValue(op1, word2);
    }
};

void Processor::int_()
{
    // pega o valor no registrador BX
    uint16_t address = BX.high << 8 | BX.low;

    // pega os 2 primeiros parametros para o INT (cada parametro tem 2 bytes na memoria de dados) e eles estão escritos em LOW HIGH
    uint16_t param1 = this->physical_memory[address + 1] << 8 | this->physical_memory[address];
    uint16_t param2 = this->physical_memory[address + 3] << 8 | this->physical_memory[address + 2];

    // verifica se é um WRITE ou um EXIT

    // WRITE
    if (param2 == 4)
    {
        // 3o parametro
        uint16_t param3 = this->physical_memory[address + 5] << 8 | this->physical_memory[address + 4];

        // 4o parametro
        // o 4o parametro é o tamanho da string que vai ser printads
        uint16_t param4 = this->physical_memory[address + 7] << 8 | this->physical_memory[address + 6];

        // 5o parametro
        uint16_t param5 = this->physical_memory[address + 9] << 8 | this->physical_memory[address + 8];

        // 6o parametro
        // este é o endereço de onde começa o texto em ASCII
        uint16_t param6 = this->physical_memory[address + 11] << 8 | this->physical_memory[address + 10];

        // loop na memoria de dados para printar o texto em ASCII
        for (int i = 0; i < param4; i++)
        {
            // pega o valor do endereço
            char word = this->physical_memory[param6 + i];

            // printa o valor do endereço em ASCII
            cout << (char)word;
        }
    }

    // EXIT
    else if (param2 == 1)
    {
        if (DEBUG)
        {
            cout << "EXIT CODE " << endl;
        }

        // finaliza o programa
        exit(0);
    }
}

void Processor::xor_(string op1, string op2)
{
    // ambos os operandos são registradores

    // pega o valor que esta no registrador do operando 2
    uint16_t value_src = this->getRegisterValue(op2);

    // pega o valor que esta no registrador do operando 1
    uint16_t value_dst = this->getRegisterValue(op1);

    // faz o xor entre os valores dos registradores
    uint16_t result = value_dst ^ value_src;

    // atualiza o valor do registrador do operando 1 passando como parametro o nome do registrador
    this->setRegisterValue(op1, result);
}

void Processor::sub_(string op1, string op2)
{
    // os operandos podem ser registradores ou endereços de memória

    // verifica se o destino é um endereço ou um registrador
    bool dest_is_addr = false;
    uint16_t addr1;
    uint16_t word1;

    // verifica se a fonte é um endereço ou um registrador
    bool src_is_addr = false;
    uint16_t addr2;
    uint16_t word2;

    // verifica se o destino é um endereço ou um registrador
    handle_operators_for_algebra(op1, op2, &addr1, &word1, &dest_is_addr, &addr2, &word2, &src_is_addr);

    uint8_t result;
    result = word1 - word2;

    // coloca o resultado no destino
    if (dest_is_addr)
    {
        // se o destino for um endereço
        // coloca o resultado no endereço
        this->physical_memory[addr1] = result & 0x00FF;
        this->physical_memory[addr1 + 1] = (result & 0xFF00) >> 8;
    }
    else
    {
        // se o destino for um registrador
        // coloca o resultado no registrador
        this->setRegisterValue(op1, result);
    }
}

void Processor::add_(string op1, string op2)
{
    // verifica se o destino é um endereço ou um registrador
    bool dest_is_addr = false;
    uint16_t addr1;
    uint16_t word1;
    // verifica se a origem é um endereço ou um registrador
    uint16_t addr2;
    uint16_t word2;
    bool src_is_addr = false;

    handle_operators_for_algebra(op1, op2, &addr1, &word1, &dest_is_addr, &addr2, &word2, &src_is_addr);
    // cout << "addr1 = " << addr1 << endl;
    // cout << "word1 = " << word1 << endl;
    // cout << "addr2 = " << addr2 << endl;
    // cout << "word2 = " << word2 << endl;
    // cout << "dest_is_addr = " << dest_is_addr << endl;
    // cout << "src_is_addr = " << src_is_addr << endl;

    uint8_t result;
    result = word1 + word2;

    // coloca o resultado no destino
    if (dest_is_addr)
    {
        // se o destino for um endereço
        // coloca o resultado no endereço
        this->physical_memory[addr1] = result & 0x00FF;
        this->physical_memory[addr1 + 1] = (result & 0xFF00) >> 8;
    }
    else
    {
        // se o destino for um registrador
        // coloca o resultado no registrador
        this->setRegisterValue(op1, result);
    }
}

void Processor::pop_(string op1)
{
    // get the value of the stack pointer
    uint16_t sp = this->getRegisterValue("sp");
    // get the value of the stack
    // word = mem[sp]mem[sp+1]
    // cout << "sp = " << (int)sp << endl;
    // cout << "mem[sp] = " << (int)this->physical_memory[sp] << endl;
    // cout << "mem[sp+1] = " << (int)this->physical_memory[sp + 1] << endl;
    uint16_t word = this->physical_memory[sp] << 8 | this->physical_memory[sp + 1];
    // increment the stack pointer
    SP.add(2);
    // put the value of the stack in the destination
    this->setRegisterValue(op1, word);
}
void Processor::push_(string op2)
{
    // o op2 pode serum endereço ou um registrador
    uint16_t value;
    if (op2[0] == '[')
    {
        // verifica se tem um offset (um + ou -)
        // pega o endereço
        string s_address = (op2.substr(1, op2.size() - 2));
        // verifica se tem +
        if (s_address.find('+') != string::npos)
        {
            // pega o offset, td que vem depois do + no s_address
            string s_offset = s_address.substr(s_address.find('+') + 1);
            // pega o reg
            string reg = s_address.substr(0, s_address.find('+'));
            // pega o valor do endereço registrador + offset
            value = getRegisterValue(reg) + stoi(s_offset, nullptr, 16);
            cout << "address = " << s_address << endl;
            cout << "offset = " << s_offset << endl;
        }
        else
        {
            // pega o offset, td que vem depois do - no s_address
            string s_offset = s_address.substr(s_address.find('-') + 1);
            // pega o reg
            string reg = s_address.substr(0, s_address.find('-'));
            // pega o valor do endereço registrador - offset
            value = getRegisterValue(reg) - stoi(s_offset, nullptr, 16);
            cout << "address = " << s_address << endl;
            cout << "offset = " << s_offset << endl;
        }
    }
    else
    {
        value = getRegisterValue(op2);
    }
    // cout << "value = " << value << endl;
    this->put_on_stack(value);
}

void Processor::dec_(string op1)
{
    // get the value of the destination
    uint16_t value = this->getRegisterValue(op1);
    // decrement the value
    value--;
    if (value == 0)
    {
        this->flags.ZF = 1;
    }
    // put the value in the destination
    this->setRegisterValue(op1, value);
}