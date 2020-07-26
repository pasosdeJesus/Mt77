#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>

static int text_is_ascii(u_char c)
{
    const char cc[] = "\007\010\011\012\014\015\033";

    if (c == '\0')
        return (0);
    if (strchr(cc, c) != NULL)
        return (1);
    return (c > 31 && c < 127);
}

static int text_is_latin1(u_char c)
{
    if (c >= 160)
        return (1);
    return (text_is_ascii(c));
}

static int text_is_extended(u_char c)
{
    if (c >= 128)
        return (1);
    return (text_is_ascii(c));
}

    static int
text_try_test(const std::string data, int (*f)(u_char))
{
    for (char c: data)
    {
        if (!f(c))
            return (0);
    }
    return (1);
}

// const std::string
// los valores de retorno son temporales para las pruebas
int text_get_type(const std::string base)
{
    if (text_try_test(base, text_is_ascii))
        return 0;
    // return ("ASCII");
    if (text_try_test(base, text_is_latin1))
        return 1;
    // return ("ISO-8859");
    if (text_try_test(base, text_is_extended))
        return 2;
    // return ("Non-ISO extended-ASCII");
    return 0;
}

// en linux string ya reconoce automaticamente el uso de utf8
std::string latin1_to_utf8(std::string &str)
{
    // if utf8? return str
    // int tipo = text_get_type(str);
    // if (tipo != 1)
    if (!text_try_test(str, text_is_latin1))
        return str;

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


    std::cout << text_get_type(argv[1]) << std::endl; // Ã en UTF8
    exit(0);
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
                std::cout << line << " \t==> " << latin1_to_utf8( line ) << std::endl;


                // std::cout << line << " -> ";
                file >> line;
            }
            file.close();
        }
    }
}
