/**
 *@file RamDisco.hpp
 * Operaciones en RAM y Disco
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: RamDisco.hpp,v 1.11 2010/01/18 16:12:50 vtamara Exp $
 */

#if !defined(RamDisco_hpp)

#define RamDisco_hpp


#include <vector>
#include "comun.hpp"
#include "Pos.hpp"
#include "Doc.hpp"
#include "NodoTrieS.hpp"
#include "TrieSDisco.hpp"
#include <stdint.h>

#include "compresion/compresion.hpp"

using namespace std;

/**
 * Calcula tamaño en bytes requerido para escribir nodo y hermanos
 * sin descendientes con función escribe 
 */
uint32_t
precalcula_escribe_con_hermanos(NodoTrieS *n);

/**
 * Calcula cuantos bytes requeriría el procedimiento escribe
 * para representar el nodo, sus hermanos y descendientes
 */
uint32_t
precalcula_escribe(NodoTrieS *n);


/**
 * Escribe en formato plano en un stream
 * Primero se escribe mayor con hermanos en una cadena:
 * MENOR nhermano_1 nhijo_1 {p1_1..p1_n} SIG {p2_1..p2_m} nhermano_2 nhijo_2... MAYOR 00000 nhijo_k {pk_1..pk_s}\n
 * Después hijos del menor al mayor.
 **/
void
escribePlanoStream(NodoTrieS *n, std::ostream &os,
                   Arbol_huffman &arbolHuffman, uint32_t desp = 0);


/** Lee un trieS de un stream donde está almacenado en formato
 * extra-simple.
 * @param is Flujo de entrada
 * @return trieS que construye
 */
NodoTrieS *
leePlanoStream(std::istream &is) throw(string);

/**
 * Escribe en formato plano en un archivo
 * @param t TrieS por escribir
 * @param docs Documentos referenciados en t
 * @param na nombre del archivo por generar con índice
 * @param nrel Nombre del archivo por generar con relación de documentos
 */
void
escribePlano(NodoTrieS &t, vector<Doc> &docs, const char *na, const char *nrel,
             Arbol_huffman &arbolHuffman);

/** Lee un trieS de un archivo de nombre na donde está almacenado.
 * Retorna vector de documentos indexados en idocs.
 * @param na Nombre del archivo con índice
 * @param nrel Nombre del archivo con relación de documentos
 * @param docs Retorna en este relación de documentos indexados leido de nrel
 * @return TrieS leido de na
 */
NodoTrieS *
leePlano(char *na, char *nrel, vector<Doc> &docs);


/** Elimina un documento de un índice.
 * Tanto en índice como en relación de documentos.
 *
 * @param na Nombre del índice
 * @param nrel Nombre de la relación
 * @param nd Número de documento por eliminar (>=1)
 *
 * @return Cantidad de documentos que quedan indexados
uint32_t
eliminaUnDoc(char *na, char *nrel, uint64_t nd);
 */
/**
 * Renumera último documento indexado y los que le siguen.
 *
 * @param na Nombre del índice
 * @param nrel Nombre de la relación
 * @param nd Nuevo índice para el último documento indexado.
 *
 * @return Cantidad de documentos que quedan indexados
uint32_t
renumeraUltimoDoc(char *na, char *nrel, uint64_t nd);
 */

/**
 * Mezcla is1 con a2 y deja resultado en os.
 * @param is1 1er archivo de entrada, su cursor se espera sobre cadena del 
 * 	hermano menor
 * @param a2 2do trieS de entrada
 * @param saltacad Cuanto de la cadena de a2 debe saltarse
 * @param os salida, su cursor se espera al final --allí escribirá mezcla.
 * @param conHermanos1 procesar hermanos de is1
 * @param conHermanos2 procesar hermanos de a2
 * @param renum1 Renumeración a documentos de is1
 * @param renum2 Renumeración a documentos de a2
 *
 * Es un algoritmo similar al de escribeCopiaSubarbol:
 * 1. Escribe todos los hermanos del menor al mayor.  Para esto
 * 	recorre los hermanos de is1 y a2.
 * 2. Escribe todos los subárboles del menor al mayor. Para esto
 * 	recorre los subárboles necesarios  de is1 y a2
 */
uint32_t
mezclaDiscoRam(istream &is1, NodoTrieS *a2, int saltacad, iostream &os,
               bool conHermanos1, bool conHermanos2,
               vector<int64_t> *renum1, vector<int64_t> *renum2,
               Arbol_huffman &arbolHuffmanLectura, Arbol_huffman &arbolHuffmanEscritura);


/**
 * Extrae subíndice correspondiente a un documento nd del índice en
 * disco is y lo agrega al índice en RAM t
 * @param is Indice en disco de entrada
 * @param t Indice en ram donde se agrega 
 * @param nd Número de documento por extraer de is
 * @param pcad Prefijo (en la primera llamada no recursiva debe ser "")
 */
void
subindiceDiscoaRAM(std::istream &is, NodoTrieS *t, uint32_t nd, 
		string pcad = "") 
        throw(string);

#endif
