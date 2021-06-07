#include <iostream>
#include "compresion.hpp"

int main() {

    std::string cadena = "esto es un ejemplo";

    Arbol_huffman ah(cadena);

    if (ah.vacio()) {
      std::cerr << "el arbol de huffman esta vacio" << std::endl;
      return 1;
    }

    Arbol_huffman::imprimirMapa(ah.conseguirTendencia());

    std::cout << "========================\n";

    std::string comp = ah.comprimir(cadena);
    std::cout << comp << " - longitud ->" << comp.length() << std::endl;
    std::string desc = ah.descomprimir(comp);
    std::cout << desc << " - longitud ->" << desc.length() << std::endl;

    return 0;
}
