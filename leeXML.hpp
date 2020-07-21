/** @file leeXML.hpp
 *
 * Lee un XML y agrega datos con etiquetas.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdejesus.org
 * Dominio público. 2009. Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: leeXML.hpp,v 1.2 2010/01/06 10:09:30 vtamara Exp $
 */

#include <set>
#include <string>
#include <vector>

#if !defined(leeXML_hpp)
#define leeXML_hpp

using namespace std;

#include "NodoTrieS.hpp"

/** Construye un trieS a partir de un XML
 * @param na Nombre del archivo
 * @param ndoc Número de documento
 * @param t TrieS
 */
void leeXML(const char *na, long ndoc, NodoTrieS &t);

#endif
