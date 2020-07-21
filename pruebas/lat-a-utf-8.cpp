#include <iostream>
#include <string>
#include <fstream>

// en linux string ya reconoce automaticamente el uso de utf8
std::string latin1_to_utf8(std::string &str)
{
    // if utf8? return str

    std::string strOut;
    for (std::string::iterator it = str.begin(); it != str.end(); ++it)
    {
        uint8_t ch = *it;
        if (ch < 0x80) {
            strOut.push_back(ch);
        }
        else {
            strOut.push_back(0xc0 | ch >> 6);
            strOut.push_back(0x80 | (ch & 0x3f));
        }
    }
    return strOut;
}


int main(int argc, char *argv[])
{
    std::fstream file;
    std::string line;
    for (int i = 1 ; i < argc ; i++ )
    {
        file.open(argv[i]);
        std::cout << "----------------------\n";
        std::cout << argv[i] << ":\n\n";
        if (file.is_open()) {
            file >> line;
            while(!file.eof())
            {
                std::cout << line << " -> ";
                std::cout << latin1_to_utf8( line ) << std::endl << std::endl;
                file >> line;
            }
            file.close();
        }
    }
}
