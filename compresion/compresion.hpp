
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

struct nodo_arbol_huffman {
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

    // identifica si hay cambios de la tendencia
    bool modificado;

    // evitar que aun si se hacen cambios, volver a generar el arbol.
    // Esto es importante en caso de que ya se haya empezado a
    // escribir en disco, ya que lo que se guarde quedaria comprimido
    // con arboles distintos, haciendo dificil la descompresion.
    // Se tiene en comprimir y descomprimir, para asegurar que se use
    // siempre el mismo arbol en estas funciones.
    bool bloquear;

    // nodo inicial del arbol
    std::shared_ptr<nodo_arbol_huffman> raiz;
    // caracter con su respectivo codigo binario (como un string)
    std::map<char, std::string> simbolos;
    // tendencia de cada caracter
    std::map<char, int> tendencias;

    // consigue los codigos binarios para cada caracter
    void generarCodigos(std::shared_ptr<nodo_arbol_huffman> nah, std::string codigo);

    void _imprimirPreOrden(std::shared_ptr<nodo_arbol_huffman> nah, int depth);

    // construye un arbol de huffman con base a un mapa de tendencias
    // realiza el trabajo principal
    void construirArbol();

    std::string _toString(std::shared_ptr<nodo_arbol_huffman> nah);

    std::string binarioACadena(std::string codigo);

public:

// no construyen arbol ////////////////////////////////////////////////////////
    // construye un arbol vacio
    Arbol_huffman();

    // construye un arbol de huffman con base a una cadena y un archivo
    // en el que se haya guardado un arbol de huffman
    // iniciador, llama a construirArbol
    Arbol_huffman(std::string cadena);

    // construye un arbol de huffman con base a un mapa de tendencias
    // y un archivo en el que se haya guardado un arbol de huffman
    // iniciador, llama a construirArbol
    void sumarTendencia(std::map<char, int> tendencia);

    void sumarArchivo(std::string archivo);

    // retorna el mapa de tendencias del arbol de huffman
    std::map<char, int> conseguirTendencia();

    // guarda el arbol de huffman en un archivo con nombre "nombre"
    void guardar(std::string nombre);

// construyen arbol ///////////////////////////////////////////////////////////

    // funciones bloqueantes ////////////////////////////

    // comprime la cadena que se le pasa
    std::string comprimir(std::string cadena);

    // descomprime la cadena en binCodigo y la retorna
    // iniciador, llama a _descomprimir
    std::string descomprimir(std::string binCodigo);

    // funciones no bloqueantes /////////////////////////

    // retorna true en caso de estar vacio el arbol
    bool vacio();

    // imprime los nodos del arbol en pre orden
    // iniciador, llama a _imprimirPreOrden
    void imprimirPreOrden();

    // convierte el arbol a una cadena
    // iniciador, llama a _toString
    std::string toString();

// funciones estaticas ////////////////////////////////////////////////////////

    // carga un arbol de huffman en un archivo con nombre "nombre"
    static std::map<char, int> cargar(std::string nombre);

    // agrega el segundo mapa al primer mapa
    static void sumarMapas(std::map<char, int> &map1, std::map<char, int> map2);

    // agrega el segundo mapa al primer mapa
    static bool equivalenciaMapas(
            std::map<char, int> const  &map1, std::map<char, int> const map2);

    static std::map<char, int> cadenaAMapa(std::string cadena);

    static void restarCadenaAMapa(std::map<char, int> &mapa, std::string cad);

    static void imprimirMapa(std::map<char, int> const &tendencia);

};


#endif
