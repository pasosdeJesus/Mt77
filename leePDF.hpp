/**@file leePDF.hpp
 * Lee un archivo en formato PDF e incluye su 
 * información a un índice.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: leePDF.hpp,v 1.2 2010/01/06 10:09:30 vtamara Exp $
 */

#if !defined(LEEPDF_HPP)
#define LEEPDF_HPP

#include <string>
#include "comun.hpp"
#include "NodoTrieS.hpp"


/**
 * Convierte un archivo en formato PDF  a texto plano y agrega las palabras del 
 * documento al árbol [t].
 * @param na           Nombre del archivo
 * @param ndoc         Número de documento que corresponde al archivo na
 * @param &t           Árbol al cual deben agregarse palabras y sus posiciones
 * @param normalizaPal Índica si deben normalizarse o no las
 * palabras leidas
 *
 * @return void
 */
void leePDF(const char *na, long ndoc, NodoTrieS &t, bool normalizaPal)
noexcept(false);

#endif
