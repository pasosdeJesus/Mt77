// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file operaindice.cpp
 * Realiza operaciones en un índice:
 * - Listar palabras de un índice con la frecuencia
 *    lista i
 * - Generar árbol en formato dotty
 *    grafo i
 * - Genera condensado de índice
 *    condensado i
 * - Mezcla en ram los índices i1 a i_n y deja el resultado en is
 *    mezclaram is i1 i2 ... i_n
 * - Mezcla en disco el índice i1 con el índice i2 y deja el resultado en is
 *    mezcladisco is i1 i2
 * - Indexar y añadir textos a indice i con dominio d y dejar resultado en o.
 *    indexa i d a -salida o
 * - Mezclar con otro índice y dejar resultado en un tercer archivo.
 *   o Añadir un índice
 *     i1 d -aumenta i2 -salida i3
 * - Eliminar uno o más documentos
 *     i d -elimina d1 -elimina d2 ... -elimina d_n
 * - Renumerar referencias a documento
 *     i -renumera d1 d2 -renumera d3 d4 ...
 * - Generar el subíndice correspondiente a un documento
 *     subindice i d
 * - Mezclar subíndice en índice manteniendo numeración
 *     i -mezcla i2 d2
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: operaindice.cpp,v 1.16 2010/03/08 12:02:40 vtamara Exp $
 */

/**
 * Dominio público. 2009. vtamara@pasosdejesus.org
 */

#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <fstream>
#include <istream>

using namespace std;

#include "RamDisco.hpp"
#include "Operaciones.hpp"

#include "compresion/compresion.hpp"

bool metainformacion=true;

int main(int argc, char *argv[])
{
        if (argc < 2) {
                cerr<<"Se esperaba operación y parámetros" << endl;
                cerr<<"Las operaciones posibles y sus parámetros son:" << endl;
                cerr << endl;
                cerr << "eliminadoc is i n" << endl;
                cerr << "	Elimina el n-esimo documento del índice i (n>=1) produciendo is" << endl;

                cerr << "grafo i" << endl;
                cerr << "	Generar árbol en formato dotty" << endl;
                cerr << "condensado i" << endl;
                cerr << "	Generar condensado" << endl;
                cerr << "lista i" << endl;
                cerr << "	Lista palabras de un índice con frecuencia" << endl;
                cerr << "mezclaram is i1 i2 ... i_n" << endl;
                cerr << "	Mezcla en ram índices i1 e i2 produciendo is" << endl;
                cerr << "mezcladisco is i1 i2 [n]" << endl;
                cerr << "	Mezcla en disco. Análogo a mezclaram, aunque el parametro opcional n " << endl;
                cerr << "	índica la posición en medio de documentos referenciados por i1, " << endl;
                cerr << "	en la que se insertarán documentos referenciados " << endl;
                cerr << "	por i2. Si no se especifica se insertan después de los de i1. " << endl;
                cerr << "	Puede ser entre 1 y el total de documentos de i1 más 1"  << endl;
/* Esto lo hace indexador
 * cerr << "agregadoc is i1 pref d [n]" << endl;
                cerr << "	Usando como prefijo para URL pref, indexa el documento d en RAM y " << endl;
                cerr << "       lo mezcla con el índice i1, dejando el índice resultante en is." << endl;
                cerr << "	El parámetro n es como en mezcladisco " << endl;
                */
                cerr << "subindice is i n" << endl;
                cerr << "	Saca del índice i el subíndice que corresponde al documento n " << endl;
                cerr << "       y lo escribe en is" << endl;


                exit(1);
        }
        char noma[1000];
        char nrel[MAXLURL];

        if (((strcmp(argv[1], "lista") == 0 ||
                strcmp(argv[1], "grafo") == 0 ||
                strcmp(argv[1], "condensado") == 0) && argc != 3) ||
                (strcmp(argv[1], "subindice") == 0 && argc != 5) ||
                (strcmp(argv[1], "eliminadoc") == 0 && argc != 5) ||
                (strcmp(argv[1], "mezclaram") == 0 && argc < 5) ||
//              (strcmp(argv[1], "agregadoc") == 0 && argc != 6 && argc != 7) ||
                (strcmp(argv[1], "mezcladisco") == 0 &&
                (argc < 5 || argc>6))) {
                cerr << "Cantidad de parámetros incorrecta" << endl;
                exit(1);
        }
        try {
                if (strcmp(argv[1], "eliminadoc") == 0) {
                        snprintf(noma, 1000, "%s", argv[2]);
                        long nd = atoi(argv[4]);
                        if (nd <= 0) {
                                cerr << "n debe ser mayor o igual a 1" << endl;
                                exit(1);
                        }
                        Arbol_huffman arbolHuffman("", std::string(argv[2]) + ".tendencia");
                        eliminaDoc(noma, argv[3], (long)nd, arbolHuffman);
                } else if (strcmp(argv[1], "grafo") == 0) {
                        snprintf(noma, 1000, "%s", argv[2]);
                        verificaNombre(argv[2], nrel);
                        vector<Doc> docs1;
                        Arbol_huffman arbolHuffman("", std::string(argv[2]) + ".tendencia");
                        NodoTrieS *r = leePlano(noma, nrel, docs1, arbolHuffman);
                        r->aDotty(cout);
                } else if (strcmp(argv[1], "condensado") == 0) {
                        snprintf(noma, 1000, "%s", argv[2]);
                        verificaNombre(argv[2], nrel);
                        Arbol_huffman arbolHuffman("", std::string(argv[2]) + ".tendencia");
                        cout << condensado(argv[2], arbolHuffman, false) << endl;
                } else if (strcmp(argv[1], "lista") == 0) {
                        snprintf(noma, 1000, "%s", argv[2]);
                        verificaNombre(argv[2], nrel);

                        // std::clog << "archivo tendencia" << nombre_tendencia << std::endl;
                        // TODO: aqui solo se imprime la primera palabra, se deberia imprimir toda la lista

                        Arbol_huffman arbolHuffman("", std::string(argv[2]) + ".tendencia");
                        listaPalabras(noma, nrel, arbolHuffman);
                } else if (strcmp(argv[1], "mezclaram") == 0) {

                        // aqui se van acumulando los demas archivos
                        NodoTrieS *t = NULL;

                        snprintf(noma, 1000, "%s", argv[2]);
                        //cerr<<"noma="<<noma<<endl;

                        vector<Doc> docs2;
                        for (int i = 3; i < argc; i++) {
                                vector<Doc> docs1; // Documentos en i-esimo índ.
                                verificaNombre(argv[i], nrel);
                                Arbol_huffman arbolHuffman("", std::string(argv[i]) + ".tendencia");
                                NodoTrieS *r = leePlano(argv[i], nrel, docs1, arbolHuffman);
                                // Renumera los del r
                                // Mezcla docs1 en docs2
                                vector<int64_t> renum =
                                        mezclaDocs(docs1, docs2);
                                if (r != NULL) {
                                  r->renumeraDocs(renum);
                                }
                                NodoTrieS *t2=mezcla(t, r);
                                t = t2;
                                r = NULL;
                        }

                        std::map<char, int> tendencia;
                        if (t != NULL) {
                          tendencia = t->conseguirTendencia();
                        }
                        Arbol_huffman arbolHuffman(tendencia);
                        arbolHuffman.guardar( string(argv[2]) + ".tendencia");
                        //cerr<<"idocs.size="<<idocs.size()<<endl;
                        verificaNombre(noma, nrel);
                        escribePlano(*t, docs2, noma, nrel, arbolHuffman);
                } else if (strcmp(argv[1], "mezcladisco") == 0) {

                        /* Mezcla 2 tries en disco */
                        long nd = 0;
                        if (argc == 6) {
                                nd = atoi(argv[5]);
                                if (nd < 1) {
                                        cerr << "n debe ser mayor o igual a 1" << endl;
                                        exit(1);
                                }
                        }

                        Arbol_huffman arbolHuffman1("", std::string(argv[3]) + ".tendencia");
                        Arbol_huffman arbolHuffman2("", std::string(argv[4]) + ".tendencia");
                        Arbol_huffman arbolHuffmanOut("", std::string(argv[2]) + ".tendencia");
                        // TODO: esto por alguna razon solo retorna el primer valor
                        mezclaDosDisco(argv[2], argv[3], argv[4],
                                       arbolHuffman1,
                                       arbolHuffman2,
                                       arbolHuffmanOut,
                                       nd);

                        /*        } else if (strcmp(argv[1], "agregadoc") == 0) {
                                  long nd = 0;
                                  if (argc == 7) {
                                  nd = atoi(argv[6]);
                                  if (nd < 1) {
                                  cerr << "n debe ser mayor o igual a 1" << endl;
                                  exit(1);
                                  }
                                  }

                                  try {
                                  agregaDoc(argv[2], argv[3], argv[5],
                                  metainformacion, argv[4], nd);
                                  } catch (std::string s) {
                                  cerr << s << endl;
                                  exit(1);
                                  } */
                } else if (strcmp(argv[1], "subindice") == 0) {
                        long nd = atoi(argv[4]);
                        if (nd < 1) {
                                cerr << "n debe ser mayor o igual a 1" << endl;
                                exit(1);
                        }

                        // TODO: preguntar esto
                        Arbol_huffman arbolHuffman("", std::string(argv[2]) + ".tendencia");
                        subindice(argv[3], argv[2], nd, arbolHuffman);
                } else {
                        cerr << "operación desconocida " << argv[1] << endl;
                        return 1;
                }
        } catch (string s) {
                cerr << s << endl;
                return 1;
        }
        return 0;
}



