#include <iostream>
#include "compresion.hpp"

int main() {

    // prueba(); return 0;

    std::string cadenas[] = {
        "hola que hace"
        , "algÈÞesoµ½esto×"
        , "abcdefghijklmnopqrstuvwxyz"
    };

    for (std::string c : cadenas)
    {
        std::cout << c << std::endl;
        Arbol_huffman ah(c);
        // ah.imprimirPreOrden();

        std::string bla = ah.comprimir(c);
        std::cout << bla << std::endl;
        // std::cout << bla.length()% 8 << std::endl;

        std::cout << ">" << ah.descomprimir(bla) << "<" << std::endl;
        // ah.prtTendencia();
        std::cout << std::endl << std::endl;
    }

    return 0;
}
