/**@file leeODT.hpp
 * Lee un archivo en formato odt de Open Document Format e incluye su 
 * información a un índice.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: leeODT.hpp,v 1.5 2010/01/06 04:19:25 vtamara Exp $
 */

#if !defined(LEEODT_HPP)
#define LEEODT_HPP

#include <string>
#include "NodoTrieS.hpp"



/**
 * Aplica XSLT que  extrae textos al archivo de nombre arch 
 * @param arch Nombre de archivo XML en ODT
 * @param nomsal Nombre del archivo de salida, si es vacío envía a salida est.
 * Referencias:
 * - libxslt Tutorial. John Fleck. 
 *   http://xmlsoft.org/XSLT/tutorial/libxslttutorial.html
 **/
void aplicaXSLT(string dt, string arch, string nomsal = "");


/**
 * Extrae mimetype y content.xml del contenedor zip que recibe en el 
 * directorio dt
 * @param odt Ruta al archivo ODT del cual extraer
 * @param dt Ruta al directorio tempral donde se dejarán archivos
 *
 * Ver funzipuno.c
 */
void
extraezip(string odt, string dt);


/**
 * Prepara directorio con lo extraido de un odt 
 * @param odt Ruta a archivo odt
 **/
string prepara(string odt);

/**
 * Lee un archivo en formato ODT (de ODF) y agrega las palabras del 
 * documento al árbol [t].
 * @param na           Nombre del archivo
 * @param ndoc         Número de documento que corresponde al archivo na
 * @param &t           Árbol al cual deben agregarse palabras y sus posiciones
 * @param normalizaPal Índica si deben normalizarse o no las
 * palabras leidas
 *
 * @return void
 */
void leeODT(const char *na, long ndoc, NodoTrieS &t, bool normalizaPal);


#endif
