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

void Processor::mov(string operand1, string operand2, vector<uint8_t> *data_memory)
{

    // pega o valor do operando 2
    // verifica se o valor é um endereço ou não
    // se estiver entre [] é um endereço
    uint16_t word1;
    uint16_t word2;
    uint16_t address2;
    if (operand2[0] == '[')
    {
        // pega o endereço
        string s_address = operand2.substr(1, operand2.size() - 2);
        // verifica se tem um offset (um + ou -)
        if (s_address.find('+') != string::npos)
        {
            // pega o offset
            string s_offset = s_address.substr(s_address.find('+') + 1);
            // pega o endereço
            s_address = s_address.substr(0, s_address.find('+'));
            // pega o valor do endereço registrador + offset
            address2 = getRegisterValue(s_address) + stoi(s_offset, nullptr, 16);
        }
        else if (s_address.find('-') != string::npos)
        {
            // pega o offset
            string s_offset = s_address.substr(s_address.find('-') + 1);
            // pega o endereço
            s_address = s_address.substr(0, s_address.find('-'));

            // pega o valor do endereço registrador + offset
            address2 = getRegisterValue(s_address) - stoi(s_offset, nullptr, 16);
        }
        else
        {
            // pega o valor do endereço
            address2 = getRegisterValue(s_address);
        }

        // pega o valor que está no endereço
        word2 = (*data_memory)[address2];
    }
    else
    {
        // verifica se é um registrador ou um numero em hexadecimal
        // se o tamanho for 2 é um registrador
        if (operand2.size() == 2)
        {
            // pega o valor do registrador
            word2 = getRegisterValue(operand2);
        }
        else
        {
            // pega o valor do numero em hexadecimal
            word2 = stoi(operand2, nullptr, 16);
        }
    }

    // lida com o operando 1
    // verifica se o valor é um endereço ou não
    // se estiver entre [] é um endereço
    bool is_address = false;
    // cout << operand1 << endl;
    if (operand1[0] == '[')
    {
        is_address = true;
        // pega o endereço
        string s_address1 = (operand1.substr(1, operand1.size() - 2));
        // verifica se tem um offset (um + ou -)
        if (s_address1.find('+') != string::npos)
        {
            // cout << operand1 << endl;
            // pega o offset
            string s_offset = s_address1.substr(s_address1.find('+') + 1);
            // pega o endereço
            s_address1 = s_address1.substr(0, s_address1.find('+'));

            // pega o valor do endereço registrador + offset
            word1 = getRegisterValue(s_address1) + stoi(s_offset, nullptr, 16);
        }
        else if (s_address1.find('-') != string::npos)
        {
            // pega o offset
            string s_offset = s_address1.substr(s_address1.find('-') + 1);
            // cout << s_offset << endl;
            // pega o endereço
            s_address1 = s_address1.substr(0, s_address1.find('-'));

            // pega o valor do endereço registrador - offset
            // pega o valor em hexadecimal do offset
            word1 = getRegisterValue(s_address1) - stoi(s_offset, nullptr, 16);
        }
        else
        {
            // pega o valor do endereço
            // verifica se é um registrador ou um numero em hexadecimal
            // se o tamanho for 2 é um registrador
            if (s_address1.size() == 2)
            {
                // pega o valor do registrador
                word1 = getRegisterValue(s_address1);
            }
            else
            {
                // pega o valor do endereço só convertendo para inteiro
                word1 = stoi(s_address1);
            }
        }
    }

    if (is_address)
    {
        // atualiza o valor do endereço
        (*data_memory)[word1] = (word2 >> 8) & 0xFF;
        (*data_memory)[word1 + 1] = word2 & 0xFF;
    }
    else
    {
        //  AX;
        //  BX;
        //  CX;
        //  DX;
        //  SP;
        //  BP;
        //  SI;
        //  DI;
        //  CS;
        //  DS;
        //  ES;
        //  SS;
        //  IP;
        if (operand1 == "ax")
        {
            // atualiza o valor do registrador ax
            AX.high = (word2 >> 8) & 0xFF;
            AX.low = word2 & 0xFF;
        }
        else if (operand1 == "bx")
        {
            // atualiza o valor do registrador bx
            BX.high = (word2 >> 8) & 0xFF;
            BX.low = word2 & 0xFF;
        }
        else if (operand1 == "cx")
        {
            // atualiza o valor do registrador cx
            CX.high = (word2 >> 8) & 0xFF;
            CX.low = word2 & 0xFF;
        }
        else if (operand1 == "dx")
        {
            // atualiza o valor do registrador dx
            DX.high = (word2 >> 8) & 0xFF;
            DX.low = word2 & 0xFF;
        }
        else if (operand1 == "sp")
        {
            // atualiza o valor do registrador sp
            SP.high = (word2 >> 8) & 0xFF;
            SP.low = word2 & 0xFF;
        }
        else if (operand1 == "bp")
        {
            // atualiza o valor do registrador bp
            BP.high = (word2 >> 8) & 0xFF;
            BP.low = word2 & 0xFF;
        }
        else if (operand1 == "si")
        {
            // atualiza o valor do registrador si
            SI.high = (word2 >> 8) & 0xFF;
            SI.low = word2 & 0xFF;
        }
        else if (operand1 == "di")
        {
            // atualiza o valor do registrador di
            DI.high = (word2 >> 8) & 0xFF;
            DI.low = word2 & 0xFF;
        }
        else if (operand1 == "cs")
        {
            // atualiza o valor do registrador cs
            CS.high = (word2 >> 8) & 0xFF;
            CS.low = word2 & 0xFF;
        }
        else if (operand1 == "ds")
        {
            // atualiza o valor do registrador ds
            DS.high = (word2 >> 8) & 0xFF;
            DS.low = word2 & 0xFF;
        }
        else if (operand1 == "es")
        {
            // atualiza o valor do registrador es
            ES.high = (word2 >> 8) & 0xFF;
            ES.low = word2 & 0xFF;
        }
        else if (operand1 == "ss")
        {
            // atualiza o valor do registrador ss
            SS.high = (word2 >> 8) & 0xFF;
            SS.low = word2 & 0xFF;
        }
        else if (operand1 == "ip")
        {
            // atualiza o valor do registrador ip
            IP.high = (word2 >> 8) & 0xFF;
            IP.low = word2 & 0xFF;
        }
    }
};

void Processor::interrupt(vector<uint8_t> *data_memory)
{
    // pega o valor no registrador BX
    uint16_t address = BX.high << 8 | BX.low;

    // pega os 2 primeiros parametros para o INT (cada parametro tem 2 bytes na memoria de dados) e eles estão escritos em LOW HIGH
    uint16_t param1 = (*data_memory)[address + 1] << 8 | (*data_memory)[address];
    uint16_t param2 = (*data_memory)[address + 3] << 8 | (*data_memory)[address + 2];

    // verifica se é um WRITE ou um EXIT

    // WRITE
    if (param2 == 4)
    {
        // 3o parametro
        uint16_t param3 = (*data_memory)[address + 5] << 8 | (*data_memory)[address + 4];

        // 4o parametro
        // o 4o parametro é o tamanho da string que vai ser printads
        uint16_t param4 = (*data_memory)[address + 7] << 8 | (*data_memory)[address + 6];

        // 5o parametro
        uint16_t param5 = (*data_memory)[address + 9] << 8 | (*data_memory)[address + 8];

        // 6o parametro
        // este é o endereço de onde começa o texto em ASCII
        uint16_t param6 = (*data_memory)[address + 11] << 8 | (*data_memory)[address + 10];

        // loop na memoria de dados para printar o texto em ASCII
        for (int i = 0; i < param4; i++)
        {
            // pega o valor do endereço
            char word = (*data_memory)[param6 + i];

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