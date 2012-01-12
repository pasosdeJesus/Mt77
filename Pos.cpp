// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file Pos.cpp
 * Posición
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: Pos.cpp,v 1.13 2010/01/18 16:12:50 vtamara Exp $
 */

#include <set>
#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include <stdint.h>


// using namespace std;
// La anterior comentada por poco portable

#include "comun.hpp"
#include "Pos.hpp"
#include "Elias.hpp"


bool
operator<(Pos p1, Pos p2)
{
        return (p1.numd < p2.numd ||
                (p1.numd == p2.numd && p1.numb < p2.numb));
}


std::ostream &
operator<<(std::ostream &os, Pos p)
{
        os << "(" << p.numd << "," << p.numb << ")";
        return os;
}


void escribePos(iostream &os, set<Pos> *cpos)
{
        //clog << "OJO escribePos os.tellp()=" << os.tellp() << endl;
        if (cpos != NULL) {
                string sep = "";
                set<Pos>::iterator i;
                long l = -1;
                long unumb = 0;
                for (i = cpos->begin(); i != cpos->end(); i++) {
                        long nd = i->numd;
                        ASSERT(nd >= 1);
                        //clog << "nd=" << nd << ", numb=" << i->numb << endl;
                        if (nd != l) {
                                os << sep;
                                escribe_elias_gama2(os, (uint32_t)nd);
                                //clog << "escribió nd" << endl;
                                unumb = 0;
                                sep = "(";
                                l = nd;
                        }
                        ASSERT(i->numb - unumb >= 1);
                        escribe_elias_gama2(os, i->numb - unumb);
                        //clog << "escribió i->numb-unumb=" << i->numb-unumb << endl;
                        unumb = i->numb;
                }
        }
        os << "}";
        /*	long pih = os.tellp();
        	
        	return pih; */
}


uint32_t longPos(set<Pos> *cpos)
{

        uint32_t r=0;
        if (cpos != NULL) {
                uint32_t ls = 0;
                set<Pos>::iterator i;
                int64_t l = -1;
                uint32_t unumb = 0;
                for (i = cpos->begin(); i != cpos->end(); i++) {
                        if (i->numd != l) {
                                r += ls;
                                ASSERT(i->numd >= 1);
                                r += long_elias_gama(i->numd);
                                ls = 1;
                                l = i->numd;
                                unumb = 0;
                        }
                        ASSERT(i->numb - unumb >= 1);
                        r += long_elias_gama(i->numb - unumb);
                        unumb = i->numb;
                }
        }
        r++;
        return r;
}


set<Pos> *copiaPos(set<Pos> &p, vector<int64_t> *renum)
{
        //clog << "OJO copiaPos(p="<< p<< ", renum=" << renum << ")" << endl;
        set<Pos> *cpos = new set<Pos>();
        ASSERT(cpos != NULL);
        string sep = "";
        set<Pos>::iterator i;
        int c=0;
        for (i = p.begin(), c=0; i != p.end(); i++, c++) {
                //clog << "OJO c="<< c << ", i->numd=" << i->numd << "i->numb=" << i->numb << endl;
                int64_t nd = i->numd;
                ASSERT(nd >= 1);
                if (renum != NULL) {
                        //clog << "OJO renum->size=" << renum->size() << ", nd=" << nd << ", (*renum)[nd-1]=" << (*renum)[nd-1] << endl;
                        ASSERT(nd <= (long)renum->size());
                        ASSERT((*renum)[nd-1] >= -1);
                        if ((*renum)[nd-1] >= 0) {
                                nd = (*renum)[nd-1] + 1;
                        } else {
                                nd = -1;  // eliminar
                                //clog << "OJO por eliminar" << endl;
                        }
                }
                if (nd >= 1) {
                        ////clog << "OJO en ciclo primer hex leido v1="<<v1<<endl;
                        cpos->insert(Pos(nd, i->numb));
                }
        }

        return cpos;
}



set<Pos> *leePos(istream &is, vector<int64_t> *renum)
{
        //clog << "OJO leePos is.tellg=" << is.tellg() << endl;
        set<Pos> *cpos = new set<Pos>();
        long np = 0;
        int c = is.peek();
        //clog << "OJO c="<<c<<endl;
        while (c != '}' && c != EOF) {
                //clog << "OJO en ciclo c="<<c<<" "<<(char)c<<endl;
                long v1,v2, uv2;
                v1 = lee_elias_gama2(is);
                //clog << "OJO en ciclo c="<<c<<" "<<(char)c<<", v1=" << v1 <<endl;
                ASSERT(v1 >= 1);
                if (renum != NULL) {
                        //clog << "OJO renum->size=" << renum->size() << ", v1=" << v1 << ", (*renum)[v1-1]=" << (*renum)[v1-1] << endl;
                        ASSERT(v1 <= (long)renum->size());
                        ASSERT((*renum)[v1-1] >= -1);
                        if ((*renum)[v1-1] >= 0) {
                                v1 = (*renum)[v1-1] + 1;
                        } else {
                                v1 = -1;  // eliminar
                        }
                }
                //clog << "OJO en ciclo primer hex leido v1="<<v1<<endl;
                uv2=0;
                do {
                        v2 = lee_elias_gama2(is);
                        //clog << "OJO en ciclo segundo hex leido v2="<<v2<<endl;
                        if (v1 >= 0) {
                                uv2 += v2;
                                cpos->insert(Pos(v1, uv2));
                                //clog << "OJO insertado" <<endl;
                        }
                        c = is.peek();
                        //clog << "OJO c=" << (int)c << ", como car=" << (char)c << endl;
                        if ((c & 128) == 0 && c != 0 && c != '}'
                                        && c != '(') {
                                throw errorFormato(is, "Se esperaba numero en codificación de elias o { o (");
                        }

                        np++;
                } while (c != '}' && c != '(' && c != EOF) ;
                // } ascii 125 ( ascii 40, son buenos porque no corresponden
                // a códigos elias ---los ve como uno porque su rep. binaria
                // comienza con 0
                c = is.get();
        }
        if (c != '}') {
                throw errorFormato(is, "Se esperaba }");
        }
        if (np == 0) {
                c = is.get();
        }
        return cpos;
}

