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

void Processor::mov(string operand1, string operand2, vector<int8_t> *data_memory)
{
    // pega o valor do operando 2
    // verifica se o valor é um endereço ou não
    // se estiver entre [] é um endereço
    int16_t word1;
    int16_t word2;
    int16_t address2;
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
            address2 = getRegisterValue(s_address) + stoi(s_offset);
        }
        else if (s_address.find('-') != string::npos)
        {
            // pega o offset
            string s_offset = s_address.substr(s_address.find('-') + 1);
            // pega o endereço
            s_address = s_address.substr(0, s_address.find('-'));

            // pega o valor do endereço registrador + offset
            address2 = getRegisterValue(s_address) - stoi(s_offset);
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
        // pega o valor que está em hexadecimal
        word2 = stoi(operand2, nullptr, 16);
    }

    // lida com o operando 1
    // verifica se o valor é um endereço ou não
    // se estiver entre [] é um endereço
    bool is_address = false;
    if (operand1[0] == '[')
    {
        is_address = true;
        // pega o endereço
        string s_address1 = (operand1.substr(1, operand1.size() - 2));
        // verifica se tem um offset (um + ou -)
        if (s_address1.find('+') != string::npos)
        {
            // pega o offset
            string s_offset = s_address1.substr(s_address1.find('+') + 1);
            // pega o endereço
            s_address1 = s_address1.substr(0, s_address1.find('+'));

            // pega o valor do endereço registrador + offset
            word1 = getRegisterValue(s_address1) + stoi(s_offset);
        }
        else if (s_address1.find('-') != string::npos)
        {
            // pega o offset
            string s_offset = s_address1.substr(s_address1.find('-') + 1);
            // pega o endereço
            s_address1 = s_address1.substr(0, s_address1.find('-'));

            // pega o valor do endereço registrador - offset
            word1 = getRegisterValue(s_address1) - stoi(s_offset);
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
}