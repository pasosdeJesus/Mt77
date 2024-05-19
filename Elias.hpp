/** @file Elias.hpp
 * Algoritmos de compresión para números 
 *
 * Tomados de: Compression and coding algorithms
 * Escrito por Alistair Moffat, Andrew Turpin
 *
 * http://books.google.com.co/books?id=czwqegW_NCAC&pg=PA42&lpg=PA42&dq=Binary+Interpolative+Coding&source=bl&ots=SJ0D8VHszl&sig=nkk2CIg4rpskJkCSh6NnRimC3eU&hl=es&ei=vYqWSteBLoyvtgfXiOy-Dg&sa=X&oi=book_result&ct=result&resnum=7#v=onepage&q=Binary%20Interpolative%20Coding&f=false
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009. Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: Elias.hpp,v 1.5 2010/01/18 16:12:50 vtamara Exp $
 */

#if !defined(Elias_hpp)
#define Elias_hpp

#include <vector>
#include <string>
#include <stdint.h>

using namespace std;

/**
 * Techo de logaritmo en base 2
 */
uint32_t techo_logb2(uint64_t x);

/**
 * Piso de logaritmo en base 2
 */
uint32_t piso_logb2(uint32_t x);

/**
 * Potencia de 2, pot2(e) es 2^e
 */
uint64_t pot2(uint32_t e);

/**
 * Representación de un binario como cadena imprimible de unos y ceros 
 * con espacios cada 8 digitos
 */
string bin2ascii(string bs);

/**
 * Representación de un vector de booleanos como cadena imprimible de unos 
 * y ceros, con espacios cada 8 digitos
 */
string vb2ascii(vector<bool> &vb);

/**
 * Convierte de vector de booelanos a binario
 */
string vb2bin(vector<bool> &vb);

/**
 * Convierte de vector de booleanos a cadena de unos y ceros
 */
string vb2str(vector<bool> &vb);

/**
 * Convierte de cadena de unos y ceros a vector de booleanos
 */
vector<bool> str2vb(string s);

/**
 * Codifica entero largo x en unario como vector de booleanos
 */
void codifica_unario(uint64_t x, vector<bool> &vb);

/**
 * Decodifica unario en vector de booleanos a entero
 */
uint64_t decodifica_unario(vector<bool> &vb);

/**
 * Convierte un entero a binario de máximo nbits digitos
 */
void pone_un_entero(uint64_t x, uint32_t nbits, vector<bool> &vb);

/**
 * Convierte un binario de máximo nbits digitos a entero
 */
uint64_t toma_un_entero(uint32_t nbits, vector<bool> &vb);

/**
 * Convierte x a su codificación Gama de Elias.
 */
void codifica_elias_gama(uint32_t x, vector<bool> &vb);

/**
 * Convierte de codificaico gamma de Elias a entero
 */
uint64_t
decodifica_elias_gama(vector<bool> &vb);

/**
 * Lee un número en codificación elias gama de un flujo.
 * @param is flujo de entrada
 **/
uint64_t
lee_elias_gama(std::istream &is) noexcept(false);

/**
 * Lee rapidamente un número en codificación elias gama de un flujo.
 * @param is flujo de entrada
 **/
uint64_t
lee_elias_gama2(std::istream &is) noexcept(false);

/**
 * Escribe al flujo os un entero en codificación gama de Elias.
 * @param os Flujo de salida
 * @param n Número por escribir 
 **/
void
escribe_elias_gama(std::ostream &os, uint32_t n);

/**
 * Escribe rapidamente al flujo os un entero en codificación gama de Elias.
 * @param os Flujo de salida
 * @param n Número por escribir 
 **/
void
escribe_elias_gama2(std::ostream &os, uint32_t n);


/**
 * Retorna espacio que requeriría en un flujo codificar un número en 
 * codificación gama de Elías
 * @param n número
 *
 * @return longitud
 */
uint32_t
long_elias_gama(uint64_t n);

/**
 * Convierte un entero menor o igual a n a su codificación binaria mínima 
 */
void codifica_binaria_minima(uint64_t x, uint32_t n, vector<bool> &vb);

/**
 * Convierte vb de codificación binaria mínima con máximo n a entero
 */
uint64_t decodifica_binaria_minima(uint32_t n, vector<bool> &vb);

#endif
