#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <sstream>
using namespace std;

vector<int> fileReader(string name)
{
    ifstream file(name);
    string line;
    vector<int> instructions;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            // printa o binário da linha
            string s1, s2;
            int i = 0;
            for (char c : line)
            {
                // cout << bitset<8>(c) << endl;
                if (s2 != "")
                {
                    s1 = "";
                    s2 = "";
                }

                if (s1 == "")
                {
                    s1 = bitset<8>(c).to_string();
                }
                else
                {
                    s2 = bitset<8>(c).to_string();
                    i++;
                    if (i > 15)
                    {
                        cout << s1 << " " << s2 << " / ";
                        stringstream ss;
                        ss << hex << stoll(s1 + s2, NULL, 2);
                        cout << ss.str() << endl;
                    }
                }
            }
        }
        file.close();
    }
    else
    {
        cout << "Unable to open file";
    }
    return instructions;
}

int main(int argc, char *argv[])
{

    string name = argv[1];
    cout << "lendo o arquivo: " << name << endl;
    vector<int> instructions = fileReader(name);

    // for (int i = 0; i < instructions.size(); i++)
    //{
    //     cout << instructions[i] << endl;
    // }

    return 0;
}