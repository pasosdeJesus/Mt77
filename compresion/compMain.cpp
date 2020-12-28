#include <iostream>
#include "compresion.hpp"

int main() {

    // prueba(); return 0;

    std::string cadenas[] = {
        "hola, que hacee:ee\\\\\\eeeeeeeeeeeeeeeeeeeeeeeeee" //\n"
        , "algÈÞesoµ½esto×"
        , "abcdefghijklmnopqrstuvwxyz"
    };

    std::string cadena = "";

    std::string archivo = "huffman_prueba.txt";

    for (std::string c : cadenas)
    {
        // un mismo arbol solo puede encriptar a un unico archivo
        std::cout << c << std::endl;
        cadena += c;
        Arbol_huffman ah(cadena, archivo);

        std::string comp = ah.comprimir(cadena);
        std::cout << comp << "  ->" << comp.length() << std::endl;
        std::string desc = ah.descomprimir(comp);
        std::cout << desc << "  ->" << desc.length() << std::endl;

        ah.guardar(archivo);
        // ah.cargar(archivo);
        std::cout << std::endl << std::endl;
    }

    return 0;
}
