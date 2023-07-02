#ifndef HEADER_HPP
#define HEADER_HPP
#include "defines.hpp"
#include <string>
#include <vector>

class Header
{
public:
    int8_t header_length;
    long text_length;
    long data_length;

public:
    void readHeader(vector<int8_t> binary_file)
    {
        header_length = binary_file[4];
        // text_length is written from the 9th position (index 8) to the 12th position (index 11) but is written in reverse order
        // so we have to read it from the 12th position (index 11) to the 9th position (index 8)
        text_length = binary_file[11] << 24 | binary_file[10] << 16 | binary_file[9] << 8 | binary_file[8];
        // data_length is written from the 13th position (index 12) to the 16th position (index 15) but is written in reverse order
        // so we have to read it from the 16th position (index 15) to the 13th position (index 12)
        data_length = binary_file[15] << 24 | binary_file[14] << 16 | binary_file[13] << 8 | binary_file[12];
    };
    string getString()
    {
        string header_string = "Header length: " + to_string(header_length) + "\n";
        header_string += "Text length: " + to_string(text_length) + "\n";
        header_string += "Data length: " + to_string(data_length) + "\n";
        return header_string;
    }
};

#endif // HEADER_HPP