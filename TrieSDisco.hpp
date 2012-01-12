/**
 * TrieS en disco.
 *
 * El formato en disco consta de 2 archivos el primero con el índice es:
 * marca\n
 * nodoshermanos\n
 * nodoshermanos\n
 * ...
 * nodoshermanos\n
 * 
 * El segundo con la relación de documentos es
 * marca\n
 * url\n
 * url\n
 * ...
 * url\n
 
 * El prime/r documento tiene indice 1
 *
 * nodoshermanos consta de una secuencia de nodos cada uno es
 * cadena posiciones phijos
 * donde cadena es la cadena del nodo
 * posiciones es lista de posiciones en formato
 * n id_1 p_1 id_2 p_2 ... id_n p_n
 * donde n es cantidad de posiciones, id_i es indice del documento de
 * la i-esima ocurrencia y p_i es posición de la cadena en ese documento
 * phijos es desplazamiento en el archivo a un nodoshermanos que
 * corresponden a los hijos del nodo.  Si es 0 no tiene hijos.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdejesus.org
 * Dominio público.  2008. Sin Garantias.
 * http://creativecommons.org/licenses/publicdomain/ 
 * @version  $Id: TrieSDisco.hpp,v 1.17 2010/03/08 12:02:40 vtamara Exp $
 */

#if !defined(TrieSDisco_hpp)
#define TrieSDisco_hpp

#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <stdint.h>

#include "NodoTrieS.hpp"

#include "sha256.hpp"

using namespace std;

#include "Pos.hpp"
#include "Doc.hpp"

/**
 * Escribe un nodo, con cadena y posiciones donde aparece.
 * @param os Flujo de salido donde escribir, donde esté el "cursor"
 * @param c Cadena no vacía de máximo MAXCAD caracteres
 * @param cpos Conjunto de posiciones por escribir, NULL corresponde a vacío.
 * @param dhijos Posición en el archivo dond estará nodo con hijos, no negativa.
 *        Por convención una posición 0 significa que no tiene hijos.
 * @param desp Desplazamiento inicial
 * @return Posición donde escribe desplazamiento a hijos
 */
uint32_t
escribeNodo(iostream &os, string c, set<Pos> *cpos,
                    uint32_t dhijos, uint32_t desp = 0) ;

/**
 * Calcula cuantos bytes requeriría almacenar un nodo con cadena
 * de longitud [longdad] y con un número [numpos] de posiciones
 * @param longcad Longitud de la cadena del nodo
 * @param cpos Posiciones
 *
 * @return Cantidad de bytes que requeriría su almacenamiento
 */
uint32_t
precalcula_escribe_actual(uint32_t longcad, set<Pos> *cpos);


/**
 * En flujo de salida escribe copia del primer nodo (sin hijos) de un flujo
 * de entrada.
 * Verifica que la posición del hijo sea posterior.
 * @param os flujo de salida, se espera que su "cursor" apunte al final 
 * @param is flujo de entrada, su "cursor" sobre cadena del nodo por copiar.
 * @param phijo posición del nodo hijo registrada en el flujo de entrada
 * @param renum Vector para renumerar
 *  	renum[0] tiene nuevo número menos 1 para documento 1, ... 
 *  	renum[n-1] nuevo número menos 1 para n-esimo documento.
 *	Si renum[i] es -1 no se incluirá en la respuesta el documento i+1 esimo
 *	que es como un borrado del subindice de ese documento, pero que deja
 *	todas las palabras del mismo en el TrieS --requeriría una
 *	optimización posterior.
 
 *
 * @return Posición donde escribe desplazamiento a hijos o 0 si no hay
 * 	nodo en flujo de entrada.
 */
uint32_t escribeCopiaNodo(iostream &os, istream &is, uint32_t &phijo,
                      vector<int64_t>* renum);


/**
 * En flujo de salida escribe copia de subárbol apuntado en el flujo de entrada.
 * A medida que copia realiza algunas verificaciones.
 * @param os flujo de salida, se espera que su "cursor" apunte al final 
 * @param is flujo de entrada, su "cursor" sobre cadena del nodo por copiar.
 * @param conHermanos si es true también copia hermanos.
 * @param renum Vector para renumerar. @see escribeCopiaNodo
 *
 * @return retorna la posición inicial de escritura en os 
 */
uint32_t escribeCopiaSubarbol(iostream &os, istream &is, bool conHermanos,
                          vector<int64_t> *renum = NULL);


/**
 * Mezcla is1 con is2 y deja resultado en os.
 * @param is1 1er archivo de entrada, su cursor se espera sobre cadena del 
 * 	hermano menor
 * @param is2 2do archivo de entrada, su cursor se espera sobre cadena del 
 * 	hermano menor
 * @param os salida, su cursor se espera al final --allí escribirá mezcla.
 * @param conHermanos1 Procesar hermanos de is1
 * @param conHermanos2 Procesar hermanos de is2
 * @param renum1 Especifica cambio en índices en is1. A diferencia de 
 * posiciones, los documentos se numeran comenzando en 0 --como renum1.
 * @param renum2 Especifica cambio en índices en is2. A diferencia de 
 * posiciones, los documentos se numeran comenzando en 0 --como renum2.
 *
 * @return retorna la posición inicial de escritura en os 
 *
 *
 * Es un algoritmo similar al de escribeCopiaSubarbol:
 * 1. Escribe todos los hermanos del menor al mayor.  Para esto
 * 	recorre los hermanos de is1 e is2.
 * 2. Escribe todos los subárboles del menor al mayor. Para esto
 * 	recorre los subárboles necesarios  de is1 e is2
 */
uint32_t mezclaRec(istream &is1, istream &is2, iostream &os,
               bool conHermanos1, bool conHermanos2,
               vector<int64_t> *renum1, vector<int64_t> *renum2);

/**
 * Elimina n-esimo documento de is1 dejando resultado en os
 * @param is archivo de entrada, su cursor se espera sobre cadena del 
 * 	hermano menor
 * @param os salida, su cursor se espera al final -
 * @param n Número de documento a eliminar
 * @param renum Vector que retorna con renumración por efectuar a
 * documentos referenciados
 *
 * @return retorna la posición inicial de escritura en os 
 * 	recorre los subárboles necesarios  de is1 e is2
 */
uint32_t eliminaDoc(istream &is, iostream &os, uint32_t n, 
		vector<int64_t> *renum);



/**
 * Verifica marca de índice en is y deja listo curso
 * @param is Flujo de entrada a un índice
 * Si no logra verifica lanza excepción con mensaje de error
 */
void verificaIndice(istream &is);

/** Buscan una palabra en un índice
 * @param na Archivo con índice
 * @param nrel Archivo con relación de documentos referenciadso en índice
 * @param pal Palabra por buscar
 * @param docs En esta retorna la relación de documentos indexados
 * @return Conjunto de posiciones donde aparece la palabra
 */
set<Pos> *buscaPlano(const char *na, const char *nrel, string pal, 
		vector<Doc> &docs);


/** Busca una palabra en un índice ya abierto.
 * @param is Flujo de entrada con cursor sobre cadena
 * @param pal Palabra por buscar
 * @return Conjunto de posiciones donde aparece la palabra
 **/
set<Pos> *buscaPlanoStream(std::istream &is, string pal) throw(string);



/**
 * Lee relación de documentos indexados en un archivo
 * @param nrel Nombre de archivo con relación
 * @param docs Vector que desocupa y usa para almacenar vector de documentos
 *
 * @return Cantidad de documentos leidos
 * Lanza excepción si no puede leer archivo.
 */
uint32_t 
leeRelacion(const char *nrel,  vector<Doc> &docs) throw(string);


/**
 * Escribe relación de documentos indexados en un flujo.
 * @param nrel Nombre de archivo por crear o sobreescribir.
 * @param docs Vector de documentos
 * @param reord Si no es NULL es reordenamiento para vdoc que debe
 * 	quedar en archivo
 */
void escribeRelacion(const char *nrel,  vector<Doc> &docs,
                     vector<int64_t> *reord = NULL);


/**
 * Calcula condensando de un flujo de índice teniendo en cuenta palabras
 * completas y las posiciones donde aparece.
 *
 * @param is       archivo que se lee ubicado al comienzo de un nodo
 * @param md       estado del condensado que es modificado 
 * @param precad   prefijo para la cadena del nodo actual
 * 
 * @return void 
 */
void condensadoFlujo(istream &is,  hash_state *md, string precad = "", 
                bool imprime = false);


/**
 * Calcula condensando de un índice teniendo en cuenta palabras
 * completas y las posiciones donde aparece.
 *
 * @param indice   Índice
 * @param depura   Si debe presentar datos a los que calcula SHA256
 * 
 * @return string 32 bytes hexadececimales
 */
string condensado(char *indice, bool depura = false);


#endif
