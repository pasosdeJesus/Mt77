
#ifndef __COMPRESION_HUFFMAN
#define __COMPRESION_HUFFMAN

#include <iostream>
#include <map>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <vector>
#include <queue>
#include <bitset>
#include <sstream>
#include <fstream>

// es mas facil que sean dos caracteres, ya que no es posible que uno de los nodos
// guarde mas de un unico caracter
#define SEPARADOR "\\\\"

class nodo_arbol_huffman {
public:
    char valor;
    std::shared_ptr<nodo_arbol_huffman>  hijo_i;
    std::shared_ptr<nodo_arbol_huffman>  hijo_d;
    int tendencia;

    nodo_arbol_huffman(char valor, int tendencia);

    nodo_arbol_huffman(nodo_arbol_huffman &nah1, nodo_arbol_huffman &nah2);

    void prt();

    std::string toString();

    friend bool operator>(const nodo_arbol_huffman &nah,
                          const nodo_arbol_huffman &nah2) {
        return nah.tendencia > nah2.tendencia;
    }

    friend bool operator<(const nodo_arbol_huffman &nah,
                          const nodo_arbol_huffman &nah2) {
        return nah.tendencia < nah2.tendencia;
    }
};

class Arbol_huffman {

    // nodo inicial del arbol
    std::shared_ptr<nodo_arbol_huffman> raiz;
    // caracter con su respectivo codigo binario (como un string)
    std::map<char, std::string> simbolos;
    // tendencia de cada caracter
    std::map<char, int> tendencias;

    void _conseguirCodigos(std::shared_ptr<nodo_arbol_huffman> nah, std::string codigo);

    char _descomprimir(std::shared_ptr<nodo_arbol_huffman> nah,
            std::string codigo,
            int &posicion
            );

    void _imprimirPreOrden(std::shared_ptr<nodo_arbol_huffman> nah, int depth);

    // construye un arbol de huffman con base a un mapa de tendencias
    // realiza el trabajo principal
    void construirArbol(std::map<char, int> tendencia, std::string archivo);

    std::string _toString(std::shared_ptr<nodo_arbol_huffman> nah);

    // consigue los codigos binarios para cada caracter
    void conseguirCodigos();

public:


    std::string binarioACadena(std::string binario);

    // construye un arbol vacio
    Arbol_huffman();

    // construye un arbol de huffman con base a una cadena y un archivo
    // en el que se haya guardado un arbol de huffman
    // iniciador, llama a construirArbol
    Arbol_huffman(std::string cadena, std::string archivo="");

    // construye un arbol de huffman con base a un mapa de tendencias
    // y un archivo en el que se haya guardado un arbol de huffman
    // iniciador, llama a construirArbol
    Arbol_huffman(std::map<char, int> tendencia, std::string archivo="");

    // retorna el mapa de tendencias del arbol de huffman
    std::map<char, int> conseguirTendencias();

    // descomprime la cadena en binCodigo y la retorna
    // iniciador, llama a _descomprimir
	std::string descomprimir(std::string binCodigo);

    // imprime los nodos del arbol en pre orden
    // iniciador, llama a _imprimirPreOrden
	void imprimirPreOrden();

    // comprime la cadena que se le pasa
	std::string comprimir(std::string cadena);

    // convierte el arbol a una cadena
    // iniciador, llama a _toString
    std::string toString();

    // guarda el arbol de huffman en un archivo con nombre "nombre"
    void guardar(std::string nombre);

    // carga un arbol de huffman en un archivo con nombre "nombre"
    static std::map<char, int> cargar(std::string nombre);

    // retorna true en caso de estar vacio el arbol
    bool vacio();

    // agrega el segundo mapa al primer mapa
    static void sumarMapas(std::map<char, int> &map1, std::map<char, int> map2);

    static std::map<char, int> cadenaAMapa(std::string cadena);

    static void restarCadenaAMapa(std::map<char, int> &mapa, std::string cad);

};


#endif
