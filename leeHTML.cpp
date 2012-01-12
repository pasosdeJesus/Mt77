// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file leeHTML.cpp
 * Lee de HTML.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: leeHTML.cpp,v 1.4 2010/01/06 10:09:30 vtamara Exp $
 */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "NodoTrieS.hpp"

using namespace std;

void insertaNormalizada(string pal, long ndoc, long p, NodoTrieS &t,
                        bool normalizaPal)
{
        if (pal.size() >= MAXCAD) {
                pal = pal.substr(0, MAXCAD);
        }
        //clog<<"Leida "<<pal<<endl;
        if (p >= 0) {
                string r;
                if (normalizaPal) {
                        r = normaliza(pal);
                } else {
                        r = pal;
                }
                if (r.size()>0) {
                        t.inserta(r, Pos(ndoc, p));
                        //clog<<"  Insertada"<<endl;
                }
        }
}

/** Construye un trieS a partir de un HTML */
void leeHTML(const char *na, long ndoc, NodoTrieS &t, bool normalizaPal)
{
        ASSERT(na!=NULL && na[0] != '\0' && strlen(na)<FILENAME_MAX);
        ASSERT(ndoc >= 0);

        string pal = "";
        //clog<<"Leyendo de "<<na<<endl;
        // Error si no existe
        ifstream fs(na);

        long p;
        p = fs.tellg();
        int estado = 0;
        /** 0 palabra en dato
         * 1 espacio en dato
         * 2 etiqueta
         */
        do {
                char c[2];
                fs.read(&c[0], 1);
                c[1] = '\0';
                //clog << "OJO c=" << c << " estado=" << estado << endl;
                if (estado == 0 && *c == '<') {
                        estado = 2;
                        insertaNormalizada(pal, ndoc, p + 1, t, normalizaPal);
                        //clog << "De 0 a 2" << endl;
                } else if (estado == 2 && *c == '>') {
                        estado = 0;
                        p = fs.tellg();
                        pal = "";
                        //clog << "De 2 a 0" << endl;
                } else if (estado == 0 && isspace(*c)) {
                        estado = 1;
                        insertaNormalizada(pal, ndoc, p, t, normalizaPal);
                        pal = "";
                        //clog << "De 0 a 1" << endl;
                } else if (estado == 1 && *c == '<') {
                        estado = 2;
                        //clog << "De 1 a 2" << endl;
                } else if (estado == 1 && !isspace(*c)) {
                        estado = 0;
                        p = fs.tellg();
                        pal = (char *)c;
                        //clog << "De 1 a 0" << endl;
                } else if (estado == 0) {
                        pal = pal + string((const char *)c);
                }
        } while (!fs.eof());
        fs.close();
        insertaNormalizada(pal, ndoc, p, t, normalizaPal);
}



