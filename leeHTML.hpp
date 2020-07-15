/**@file leeHTML.hpp
 * Lee un archivo en formato HTML e incluye su 
 * información a un índice.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: leeHTML.hpp,v 1.4 2010/01/06 11:48:45 vtamara Exp $
 */

#if !defined(LEEHTML_HPP)
#define LEEHTML_HPP

#include <string>
#include "NodoTrieS.hpp"

void insertaNormalizada(string pal, long ndoc, long p, NodoTrieS &t,
                        bool normalizaPal);

/** Construye un trieS a partir de un HTML */
void leeHTML(const char *na, long ndoc, NodoTrieS &t, bool normalizaPal);

#endif
