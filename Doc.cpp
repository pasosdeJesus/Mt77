// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file Doc.cpp
 * Documento.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: Doc.cpp,v 1.10 2010/01/18 16:12:50 vtamara Exp $
 */

#include <vector>
#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include <stdint.h>
#include "comun.hpp"

using namespace std;
// La anterior comentada por poco portable

#include "Doc.hpp"


bool
operator<(Doc d1, Doc d2)
{
        return (d1.fecha < d2.fecha ||
                (d1.fecha == d2.fecha && d1.numoc < d2.numoc));
}


std::ostream &
operator<<(std::ostream &os, Doc d)
{
        int64_t es = 0;
        for (uint32_t j = 0; es < MAXLURL && j < d.URL.length(); j++) {
                int c = d.URL[j];
                if (c <= ' ' || c >= 127) {
                        os << "%" << setfill('0') << setw(2) << setbase(16) << c;
                        es += 3;
                } else {
                        os << (char)c;
                        es ++;
                }
        }
        os << " " << d.cond;
        os << " " << d.fecha;
        os << endl;
        if (es >= MAXLURL) {
                throw std::string(string("El URL del documento '") + d.URL +
                                  string("' requeriría demasiado espacio"));
        }
        return os;
}


void escribeDocs(iostream &os, vector<Doc> &vdoc, vector<int64_t> *reord)
{
        ASSERT(reord == NULL || reord->size() == vdoc.size());
        //clog << "OJO escribeDoc os.tellp()=" << os.tellp() << endl;
        uint32_t i;
        for (i = 0; i < vdoc.size(); i++) {
                if (reord == NULL) {
                        os << vdoc[i];
                } else {
                        ASSERT((*reord)[i] >= 0);
                        ASSERT((*reord)[i] < (int)vdoc.size());
                        os << vdoc[(*reord)[i]];
                }
        }
}


vector<Doc> leeDocs(istream &is)
{
        vector<Doc> vdoc;
        string u, con, f;

        vdoc.clear();
        int c = is.peek();
        while (c != EOF && !is.eof()) {
                c = is.get();
                //clog << "leeDocs c=" << c << endl;
                u = "";
                for (uint32_t i = 0; i < MAXLURL && c != EOF && c != ' ';
                                i++) {
                        if (c < ' ') {
                                throw errorFormato(is,
                                                   "Se esperaba caracter imprimible");
                        }
                        u += c;
                        c = is.get();
                }
                //clog << "leeDocs u=" << u << endl;
                if (c != ' ') {
                        throw errorFormato(is, "Se esperaba espacio tras URL");
                }
                c = is.get();
                con = "";
                for (uint32_t i = 0; i < MAXLCONDENSADO &&
                                c != EOF && c != ' '; i++) {
                        if (c < ' ') {
                                throw errorFormato(is, "Se esperaba caracter imprimible");
                        }
                        con += c;
                        c = is.get();
                }
                //clog << "leeDocs con=" << con << endl;
                if (c != ' ') {
                        throw errorFormato(is,
                                           "Se esperaba espacio tras condensado");
                }
                c = is.get();
                f = "";
                for (uint32_t i = 0; i < 4 && c != EOF; i++) {
                        if (c < '0' || c > '9') {
                                throw errorFormato(is,
                                                   "Se esperaba digito de año");
                        }
                        f += c;
                        c = is.get();
                }
                //clog << "leeDocs f1=" << f << ", c=" << c << endl;
                if (c != '-') {
                        throw errorFormato(is, "Se esperaba - tras año");
                }
                f += c;
                c = is.get();
                for (uint32_t i = 0; i < 2 && c != EOF; i++) {
                        if (c < '0' || c > '9') {
                                throw errorFormato(is, "Se esperaba digito de mes");
                        }
                        f += c;
                        c = is.get();
                }
                //clog << "leeDocs f2=" << f << endl;
                if (c != '-') {
                        throw errorFormato(is, "Se esperaba - tras mes");
                }
                f += c;
                c = is.get();
                for (uint32_t i = 0; i < 2 && c != EOF; i++) {
                        if (c < '0' || c > '9') {
                                throw errorFormato(is,
                                                   "Se esperaba digito de dia");
                        }
                        f += c;
                        c = is.get();
                }
                //clog << "leeDocs f3=" << f << endl;
                if (c != '\n') {
                        stringstream ss;
                        ss << "Se esperaba fin de línea tras fecha, no " << (int) c;
                        //clog << ss.str();
                        throw ss.str();
                }
                vdoc.push_back(Doc(u, con, f));
                c = is.peek();
        }

        return vdoc;
}


vector<int64_t> mezclaDocs(vector<Doc> &docs1, vector<Doc> &docs2)
{
        vector<int64_t> renum(docs1.size(), -1);
        for (uint32_t i = 0; i < docs1.size(); i++) {
                for (uint32_t j = 0; j < docs2.size(); j++) {
                        if (docs1[i].URL == docs2[j].URL) {
                                renum[i] = j;
                                if (docs1[i].fecha < docs2[j].fecha) {
                                        docs2[j].cond = docs1[i].cond;
                                        docs2[j].fecha = docs1[i].fecha;
                                }
                        }
                }
                if (renum[i] == -1) {
                        renum[i] = docs2.size();
                        docs2.push_back(docs1[i]);
                }
        }
        ASSERT(renum.size() == docs1.size());
        return renum;
}
