#include <iostream>
#include "compresion.hpp"

int main() {

    // prueba(); return 0;


    // antes de guardar -- reconstruir arbol huffman

    std::string cadena = "";
    std::string archivo = "huffman_prueba.txt";

    Arbol_huffman ah;
    if(!ah.vacio()) {
        std::cerr << "el arbol de huffman no esta vacio" << std::endl;
        return 1;
    }
    ah.guardar("archivo_vacio.txt");

    for (std::string c : cadenas)
    {
        // un mismo arbol solo puede comprimir a un unico archivo
        std::cout << c << std::endl;
        cadena += c;
        Arbol_huffman ah(cadena, archivo);

        if(ah.vacio()) {
            std::cerr << "el arbol de huffman esta vacio" << std::endl;
            return 1;
        }
        std::string comp = ah.comprimir(cadena);
        std::cout << comp << " - longitud ->" << comp.length() << std::endl;
        std::string desc = ah.descomprimir(comp);
        std::cout << desc << " - longitud ->" << desc.length() << std::endl;

        ah.guardar(archivo);
        // ah.cargar(archivo);
        std::cout << std::endl << std::endl;
    }

    return 0;
}
