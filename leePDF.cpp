// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file leePDF.cpp
 * Lee un archivo en formato odt de Open Document Format e incluye su 
 * información a un índice.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: leePDF.cpp,v 1.6 2010/01/20 18:54:18 vtamara Exp $
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "NodoTrieS.hpp"
#include "leePDF.hpp"

using namespace std;

string escapeshellarg(string s) 
{
	string r = "'";
	for (uint32_t i = 0; i < s.size(); i++) {
		if (s[i] == '\'') {
			r += "\\'";
		} else {
			r += s[i];
		}
	}
	r += "'";
	return r;
}

/** Construye un trieS a partir de un PDF */
void leePDF(const char *na, long ndoc, NodoTrieS &t, bool normalizaPal)
throw(std::string)
{
        ASSERT(na != NULL && na[0] != '\0' && strlen(na) < FILENAME_MAX);
        ASSERT(ndoc >= 0);

        //clog << "OJO leePDF(" << na << ", " << ndoc << ", t, " << normalizaPal << ")" << endl;

        string dt = directorio_temp();
        //clog << "OJO dt = " << dt << endl;
        string ns = dt + string("/salidapdftotext.txt");
        //clog << "OJO ns = " << ns << endl;
        string cmd = string("/usr/local/bin/pdftotext ") + escapeshellarg(na) +
                string(" ") + escapeshellarg(ns); // Para evitar problemas de permisos con  + string(" > /tmp/pdftotext.bitacora 2>&1"); o de /dev/null dentro de jaula chroot dejemos que pdftotext se queje
        //clog << "OJO cmd = " << cmd << endl;
        int rc = system(cmd.c_str());
        if (rc != 0) {
                stringstream ss;
                ss << "No pudo indexarse " << na << ", error=" << rc;
                throw ss.str();
        }
        leeTexto(ns.c_str(), ndoc, t, normalizaPal);
        unlink(ns.c_str());
        rmdir(dt.c_str());
}

