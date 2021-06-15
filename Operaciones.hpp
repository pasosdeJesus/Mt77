/** @file Operaciones.hpp
 * Realiza operaciones en un índice.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009. Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: Operaciones.hpp,v 1.7 2010/01/18 16:12:50 vtamara Exp $
 */

#if !defined(Operaciones_hpp)
#define Operaciones_hpp

using namespace std;

#include "RamDisco.hpp"

#include "compresion/compresion.hpp"

/**
 * Muestra palabras y frecuencias contenidas en un índice.
 * @param is Flujo de entrada
 * @param pre Prefijo para indentar
 */
void
muestraStream(std::istream &is, string pre, Arbol_huffman &arbolHuffman); //throw(char *)

/**
 * Lista por salida estándar palabras y frecuencias contenidas en índice.
 * @param noma Nombre de archivo con índice
 * @param nrel Nombre de archivo con relación  de documentos
 */
void listaPalabras(char *noma, char *nrel, Arbol_huffman &arbolHuffman) throw(string);

/**
 * Elimina del indice nomind el documento nd y deja resultado en noma
 * @param noma Nombre del índice de salida
 * @param nomind Nombre del índice de entrada
 * @param nd Número de documento a eliminar de nomind
 */
void
eliminaDoc(char *noma, char *nomind, uint32_t nd, Arbol_huffman &arbolHuffmanEscritura, Arbol_huffman &arbolHuffmanLectura) throw(string);


/**
 * De un prefijo convierte a sitio
 * @param pref Prefijo
 * @return sitio
 */
string prefijoASitio(const char *pref);


/** 
 * Determina el formato de un archivo
 * @param narch Nombre del archivo
 * @return Formato
 */
string determinaFormato(string narch);


/**
 * Mezcla en disco dos índices
 * @param indsal Nombre del índice de salida
 * @param ind1 Nombre del primer índice por mezclar
 * @param ind2 Nombre del segundo índice por mezclar
 * @param nd Posición dentro de ind1 a partir del cual incluir
 * los de ind2.  Por convención 0 es al final.
 */
void
mezclaDosDisco(const char *indsal, const char *ind1, const char *ind2,
               Arbol_huffman &arbol_huffman1,
               Arbol_huffman &arbol_huffman2,
               Arbol_huffman &arbol_huffmanSalida,
               uint32_t nd = 0) throw(string);

/**
 * Agrega un documento a un índice
 *
 * @param indsal Indice de salida
 * @param inden Indice de entrada
 * @param nom Nombre de documento por agregar
 * @param metainformacion decide si se agrega o no
 * @param pref Prefijo para URL en nombre de documento
 * @param nd Número de documento en medio del índice existente en el que se pondrá nuevo documento o 0 para agregar al final
 */
void
agregaDoc(const char *indsal, const char *inden, const char *nom,
          bool metainformacion, const char *pref, uint32_t nd);


/**
 * Extrae del indice ind, el subindice del documento nd y lo pone en el
 * indice salida
 * @param ind Indice de entrada
 * @param salida indice de salida
 * @param nd Numero de documento por extraer del indice de entrada
 */
void
subindice(const char *ind, const char *salida, uint32_t nd, Arbol_huffman &arbolHuffman);


#endif
