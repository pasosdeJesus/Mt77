// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file NodoTrieS.cpp
 * TrieS en RAM.
 * Basado en http://www.pasosdejesus.org/vtamara/estinf2006-1/proy-s/index.html
 * Dominio público. 2008. vtamara@pasosdejesus.org
 *
 * Diseño:
 * Se intenta por sugerencia de ... que en eficiencia para indices invertidos
 * es mejor tablas de hashing que tries normales, excepto tries especiales.
 *
 * Estrategia para búsquedas tras ver doc. lucene es mantener varios indices 
 * en disco con facilidad para mezclarlos en disco y cuyas búsquedas en 
 * disco sean muy rápidas.
 *
 * Construir índices se facilita entonces porque se hace en memoria hasta
 * que quepa, cuando no se agota memoria RAM se escribe índice en disco y
 * se comienza uno nuevo y así sucesivamente hasta que todos los documentos
 * son indexados.  Entonces se mezclan todos los índices producidos.
 *
 * Para disminuir problemas de bloqueos, los indices no se sobreescriben 
 * (excepto cuando esté terminado un procesamiento).
 *
 * 2 índices grandes por sitio: local, todo.  El local es enviado a los demás
 * nodos. El de todo se construye localmente después de traer de todos los
 * nodos.
 *
 * Tras revisar ... mejor mantener posicion(es) dentro del documento de cada 
 * ocurrencia para poder buscar varias palabras consecutivas.
 *
 * Implementación:
 * 4 fases imp: memoria, disco en formato extra-simple, disco en formato binario , disco en formato binario comprimido
 *
 *
 * Suponemos que las cadenas ya viene en un formato estándar, por ejemplo
 * mayúsculas sin signos de puntuación.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: NodoTrieS.cpp,v 1.19 2010/01/18 16:12:50 vtamara Exp $
 */

#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <set>
#include <fstream>
#include <istream>
#include <sstream>
#include <stdint.h>

using namespace std;

#include "Pos.hpp"
#include "NodoTrieS.hpp"


NodoTrieS::NodoTrieS(string cad, NodoTrieS *hijo_menor,
                     NodoTrieS *hermano_mayor, set<Pos> cpos)
{
        ASSERT(cad.size() <= MAXCAD);
        this->cad = cad;
        this->hijo_menor = hijo_menor;
        this->hermano_mayor = hermano_mayor;
        this->cpos = cpos;
}


/**
 * p es una posición por insertar
 * Convención: Si p tien algún componente negativo no lo inserta en 
 * conjunto cpos 
 **/
NodoTrieS::NodoTrieS(string cad, NodoTrieS *hijo_menor,
                     NodoTrieS *hermano_mayor, Pos p)
{
        ASSERT(cad.size() <= MAXCAD);
        this->cad = cad;
        this->hijo_menor = hijo_menor;
        this->hermano_mayor = hermano_mayor;
        if (p.numd >= 0 && p.numb >= 0) {
                cpos.insert(p);
        }
}


NodoTrieS::~NodoTrieS()
{
        //cerr << "Elimina cad=" << cad <<endl;
        cad.clear();
        delete hijo_menor;
        delete hermano_mayor;
        cpos.clear();
}


/** Apuntador retornado es a información del árbol.
 * OJO al borrar.
 **/
set<Pos>
        NodoTrieS::busca(string pal)
{

        if (cad.size()==pal.size() && cad == pal) {
                return cpos;
        } else if (cad.size()<pal.size() &&
                        pal.substr(0, cad.size())==cad
                        && hijo_menor!=NULL) {
                return hijo_menor->busca(pal.substr(cad.size()));
        } else if (cad < pal && hermano_mayor!=NULL) {
                return hermano_mayor->busca(pal);
        }
        return set<Pos>();
}


/** Retorna nueva raíz.
 * Pensando en siguiente arbol
 * 	BUEN -> PODEROSO
 * 	 |
 * 	 O
 * 	 |
 * 	 S
 * Se han considerado estos casos:
 * En lista de hermanos:
 * insertar A
 * insertar C
 * Igual:
 * insertar BUEN
 * Cambiando hijos:
 * insertar BUENA
 * insertar BUENO
 * insertar BUENZ
 * insertar BU
 * insertar BUS
 */
void
NodoTrieS::inserta(string pal, Pos p)
{
        ASSERT(pal != "");
        ASSERT(pal.size() <= MAXCAD);
        ASSERT(p.numd >= 0 && p.numb >= 0);
        
        set<Pos> *npos = new set<Pos>();
        npos->insert(p);
        inserta(pal, npos);
        delete npos;
}

/** Retorna nueva raíz.
 * Pensando en siguiente arbol
 * 	BUEN -> PODEROSO
 * 	 |
 * 	 O
 * 	 |
 * 	 S
 * Se han considerado estos casos:
 * En lista de hermanos:
 * insertar A
 * insertar C
 * Igual:
 * insertar BUEN
 * Cambiando hijos:
 * insertar BUENA
 * insertar BUENO
 * insertar BUENZ
 * insertar BU
 * insertar BUS
 */
void
NodoTrieS::inserta(string pal, set<Pos> *npos)
{
        //cerr << "OJO " << "inserta("<< pal << ", "<< p << ")" << endl;
        if (pal == cad) {
                //cerr << "  OJO pal == cad"<<endl;
                set<Pos>::iterator si;
                for(si = npos->begin(); si != npos->end(); si++) {
                        cpos.insert(*si);
                }
                return;
        }
        string pcml = prefijo_comun_mas_largo(pal, cad);
        if (pcml != "") {
                string rcad = cad.substr(pcml.size());
                string rpal = pal.substr(pcml.size());
                //cerr << "  OJO pcml=" << pcml << ", rcad=" << rcad << ", rpal=" << rpal << endl;
                if (rcad == "") {
                        //cerr << "  OJO rcad == ''" << endl;
                        if (hijo_menor==NULL) {
                                hijo_menor = new
                                             NodoTrieS(rpal, NULL, NULL, *npos);
                        } else {
                                hijo_menor->inserta(rpal, npos);
                        }
                        return;
                } else if (rpal == "") {
                        //cerr << "    OJO rpal == ''" << endl;

                        NodoTrieS *nhijo = new NodoTrieS(rcad, hijo_menor,
                                                         NULL, cpos);
                        cad = pcml;
                        hijo_menor = nhijo;
                        /* hermano_mayor igual */
                        cpos.clear();
                        set<Pos>::iterator si;
                        for(si = npos->begin(); si != npos->end(); si++) {
                                cpos.insert(*si);
                        }
                        return;
                } else  if (rcad < rpal) {
                        //cerr << "    OJO rcad<rpal" << endl;
                        NodoTrieS *nh2=new NodoTrieS(rpal, NULL, NULL, *npos);
                        NodoTrieS *nh1=new NodoTrieS(rcad, hijo_menor,
                                                     nh2, cpos);
                        cad = pcml;
                        hijo_menor = nh1;
                        /* hermano_mayor igual */
                        cpos.clear();
                        return;
                } else { /* rpal<rcad */
                        //cerr << "    OJO rpal<rcad" << endl;
                        NodoTrieS *nh2=new NodoTrieS(rcad, hijo_menor,
                                                     NULL, cpos);
                        NodoTrieS *nh1=new NodoTrieS(rpal, NULL, nh2, *npos);

                        cad = pcml;
                        hijo_menor = nh1;
                        /* hermano_mayor igual */
                        cpos.clear();
                        return;
                }
        } else { /* pcml == "" */
                //cerr << "  OJO pcml == ''" << endl;
                if (pal < cad) {
                        //cerr << "    OJO pal<cad" << endl;
                        NodoTrieS *nh = new NodoTrieS(cad, hijo_menor,
                                                      hermano_mayor, cpos);
                        cad = pal;
                        hijo_menor = NULL;
                        hermano_mayor = nh;
                        cpos.clear();
                        set<Pos>::iterator si;
                        for(si = npos->begin(); si != npos->end(); si++) {
                                cpos.insert(*si);
                        }
                        return;
                }
                /* cad<pal */
                else if (hermano_mayor==NULL) {
                        //cerr << "    OJO cad<pal y h_m==NULL" << endl;
                        if (cad == "") {
                                ASSERT(hijo_menor==NULL);
                                /* Primer nodo del árbol después de crear
                                 * con constructora por defecto.
                                 */
                                //cerr << "    OJO estaba vacío" << endl;
                                cad = pal;
                                set<Pos>::iterator si;
                                for(si = npos->begin(); si != npos->end(); si++) {
                                        cpos.insert(*si);
                                }
                                return;
                        }
                        NodoTrieS *nh = new NodoTrieS(pal, NULL, NULL, *npos);
                        hermano_mayor = nh;
                        return;
                } else { /* cad<pal y hermano_mayor!=NULL */
                        //cerr << "    OJO cad<pal y h_m!=NULL" << endl;
                        hermano_mayor->inserta(pal, npos);
                        return;
                }
        }
}


void
NodoTrieS::aDotty(std::ostream &os, string pref, bool primero, bool mayor)
        throw(string)
{
        static int numcluster = 0;
        if (primero) {
                os << "digraph \"rdsgc\" {" << endl << endl;
                os << "    rankdir=LR;" << endl;
        }
        if (cad != "") {
                if (mayor) {
                        NodoTrieS *n = this;
                        while (n!=NULL) {
                                os << '"' << pref << n->cad
                                << "\" [label=\"" << n->cad
                                << " " << n->cpos << "\"]" << endl;
                                n = n->hermano_mayor;
                        }
                        n = this->hermano_mayor;
                        if (n!=NULL) {
                                numcluster++;
                                os << "subgraph cluster" << numcluster
                                << " {"<<endl;
                                NodoTrieS *a = this;
                                while (n!=NULL) {
                                        os << '"' << pref << a->cad <<
                                        "\" -> \"" << pref
                                        << n->cad << "\" [color=\"red\"]"
                                        << endl;
                                        a = n;
                                        n = n->hermano_mayor;
                                }
                                os << "}" << endl << endl;
                        }
                }
                if (hijo_menor!=NULL) {
                        os << '"' << pref << cad << "\" -> \""
                        << pref << cad << hijo_menor->cad
                        << '"' << endl;
                        hijo_menor->aDotty(os, pref+cad, false, true);
                }
                if (hermano_mayor!=NULL) {
                        hermano_mayor->aDotty(os, pref, false, false);
                }
        } else {
                throw string("  # Nodo con cadena vacía no procesado");
        }
        if (primero) {
                os << "}" << endl;
        }
}

string
NodoTrieS::preorden()
{
        string r = cad;

        if (hijo_menor!=NULL) {
                r+=hijo_menor->preorden();
        }
        if (hermano_mayor!=NULL) {
                r+=hermano_mayor->preorden();
        }
        return r;
}


/**
 * Mezcla dos tries retornando un tercero con el resultado.
 * memoria: a1 y a2 son destruidos, la memoria que debían liberar
 * ahora es responsabilidad del nuevo árbol retornado.
 * Debería llamarse así: 
 * 	r = mezcla(a1, a2); a1=NULL; a2=NULL;
 */
NodoTrieS *
mezcla(NodoTrieS *a1, NodoTrieS *a2)
{
        NodoTrieS *n1, *n2, *t;
        NodoTrieS *res = NULL;
        NodoTrieS **r=&res;
        string c;

        while (a1!=NULL || a2!=NULL) {
                c = "";
                if (a1!=NULL && a2!=NULL) {
                        c = prefijo_comun_mas_largo(a1->cad, a2->cad);
                }
                //cerr << " prefijo c="<<c<<endl;
                if (c == "") {
                        if (a2==NULL || (a1!=NULL && a1->cad<a2->cad)) {
                                //cerr << "a1 primero" <<endl;
                                *r = new NodoTrieS(a1->cad, a1->hijo_menor,
                                                   NULL, a1->cpos);
                                r=&((*r)->hermano_mayor);
                                t = a1;
                                a1=a1->hermano_mayor;
                                t->hijo_menor = NULL;
                                t->hermano_mayor = NULL;
                                delete t;
                        } else { /*a2!=NULL && (a1==NULL || a2->cad < a1->cad) */
                                //cerr << "a2 primero" <<endl;
                                *r = new NodoTrieS(a2->cad, a2->hijo_menor,
                                                   NULL, a2->cpos);
                                r=&((*r)->hermano_mayor);
                                t = a2;
                                a2=a2->hermano_mayor;
                                t->hijo_menor = NULL;
                                t->hermano_mayor = NULL;
                                delete t;
                        }
                } else if (a1->cad == a2->cad) { // c != "" && a1!=NULL && a2!=NULL
                        //cerr << "iguales" <<endl;
                        set<Pos> cpos;
                        insert_iterator<set<Pos> >
                        cpos_ins(cpos, cpos.begin());

                        NodoTrieS *m = mezcla(a1->hijo_menor, a2->hijo_menor);
                        a1->hijo_menor = NULL;
                        a2->hijo_menor = NULL;
                        set_union(a1->cpos.begin(), a1->cpos.end(),
                                  a2->cpos.begin(), a2->cpos.end(),
                                  cpos_ins);
                        /* http://h30097.www3.hp.com/cplus/set_union_3c__std.htm*/
                        *r = new NodoTrieS(a1->cad, m, NULL, cpos);
                        r=&((*r)->hermano_mayor);
                        t = a1;
                        a1=a1->hermano_mayor;
                        t->hermano_mayor = NULL;
                        delete t;
                        t = a2;
                        a2=a2->hermano_mayor;
                        t->hermano_mayor = NULL;
                        delete t;
                } else { // c != "" && a1!=NULL && a2!=NULL && a1->cad != a2->cad
                        string r1=a1->cad.substr(c.size());
                        string r2=a2->cad.substr(c.size());
                        ASSERT(r1!="" || r2!="");
                        //cerr << "hay posfijo r1="<<r1<<" r2="<<r2<<endl;
                        if (r1=="") {
                                //cerr << "r1 vacio"<<endl;
                                a2->cad = r2;
                                n2=a2;
                                a2=a2->hermano_mayor;
                                n2->hermano_mayor = NULL;
                                NodoTrieS *m = mezcla(a1->hijo_menor, n2);
                                a1->hijo_menor = NULL;
                                n2=NULL;
                                *r = new NodoTrieS(c, m, NULL, a1->cpos);
                                r=&((*r)->hermano_mayor);
                                t = a1;
                                a1=a1->hermano_mayor;
                                t->hermano_mayor = NULL;
                                t->hijo_menor = NULL;
                                delete t;
                        } else if (r2=="") {

                                //cerr << "r2 vacio"<<endl;
                                n1=a1;
                                a1=a1->hermano_mayor;
                                n1->cad = r1;
                                n1->hermano_mayor = NULL;
                                NodoTrieS *m = mezcla(n1, a2->hijo_menor);
                                n1=NULL;
                                a2->hijo_menor = NULL;
                                *r = new NodoTrieS(c, m, NULL, a2->cpos);
                                r=&((*r)->hermano_mayor);
                                t = a2;
                                a2=a2->hermano_mayor;
                                t->hermano_mayor = NULL;
                                t->hijo_menor = NULL;
                                delete t;
                        } else if (r1 < r2) {
                                //cerr << "r1<r2"<<endl;
                                a1->cad = r1;
                                a2->cad = r2;
                                n1=a1;
                                a1=a1->hermano_mayor;
                                n2=a2;
                                a2=a2->hermano_mayor;
                                n2->hermano_mayor = NULL;
                                n1->hermano_mayor = n2;
                                *r = new NodoTrieS(c, n1, NULL, set<Pos>());
                                r=&((*r)->hermano_mayor);
                        } else { /* r2<r1 */
                                //cerr << "r2<r1"<<endl;
                                a1->cad = r1;
                                a2->cad = r2;
                                n1=a1;
                                a1=a1->hermano_mayor;
                                n2=a2;
                                a2=a2->hermano_mayor;
                                n2->hermano_mayor = n1;
                                n1->hermano_mayor = NULL;
                                *r = new NodoTrieS(c, n2, NULL, set<Pos>());
                                r=&((*r)->hermano_mayor);
                        }
                }
        }
        /*cerr << "Sale de función con trie iniciado con ";
        if (res!=NULL) {
        	cerr <<"res="<<res<<"("<<res->cad<<")"<<endl;
        	long p = 0;
        	res->escribePlanoStream(cerr, p) ;
        	cerr << endl;
        } */
        return res;
}



void
NodoTrieS::renumeraDocs(vector<int64_t> renum)
{
        set<Pos>::iterator i;
        set<Pos> opos(cpos);

        cpos.clear();
        for (i = opos.begin(); i != opos.end(); i++) {
                ASSERT(i->numd > 0);
                ASSERT((uint32_t)i->numd <= renum.size());
                cpos.insert(Pos(renum[i->numd - 1] + 1, i->numb));
        }
        if (hermano_mayor != NULL) {
                hermano_mayor->renumeraDocs(renum);
        }
        if (hijo_menor != NULL) {
                hijo_menor->renumeraDocs(renum);
        }
}



/** Construye un trieS a partir de un texto plano */
void leeTexto(const char *na, uint32_t ndoc, NodoTrieS &t, bool normalizaPal)
{
        ASSERT(na!=NULL && na[0] != '\0' && strlen(na)<FILENAME_MAX);
        ASSERT(ndoc >= 0);

        string pal;
        //clog<<"Leyendo de "<<na<<endl;
        // Error si no existe
        ifstream fs(na);

        uint32_t p;
        do {
                p = fs.tellg();
                //clog << "OJO p=" << p << endl;
                fs >> pal;
                if (pal.size() >= MAXCAD) {
                        pal = pal.substr(0, MAXCAD);
                }
                if (!fs.eof()) {
                        //clog<<"Leida "<<pal<<endl;
                        if (p >= 0) {
                                // tellg comienza en 0 toca + 1
                                t.insertaNormalizando(pal, ndoc, p + 1, 
                                                normalizaPal);
                       }
                }
        } while (!fs.eof());
        fs.close();
}


/**
 * La función estádnar ispunct considera que las tildes son signos
 * de puntuación por eso no se usa.
 */
bool es_signo_punt(unsigned char c)
{
        if (c < '0' || (c > '9' && c < 'A') || (c > 'Z' && c < 'a')) {
                //clog << c << "si es signo de puntuación" << endl;
                return true;
        }

        return false;
}

/** 
 * Inserta una palabra normalizandola antes si normaliza es true
 * @param pal Palabra por insertar 
 * @param numdoc Numero de documento en el que insertará
 * @param pini Posición inicial donde insertará
 * @param normaliza Indica si debe o no normalizar
 */
void
NodoTrieS::insertaNormalizando(string pal, uint32_t numdoc, uint32_t p, bool
                normalizaPal)
{
        string r;
        if (normalizaPal) {
                r = normaliza(pal);
        } else {
                r = pal;
        }
        if (r.size()>0) {
                inserta(r, Pos(numdoc, p));
                //clog<<"  Insertada"<<endl;
        }
}

/**
 * Divide la cadena c en palabras e inserta cada una con la etiqueta
 * dada en el árbol, referenciando el documento numdoc desde la
 * posición inicial posini.
 * @param c Cadena con palabras por insertar
 * @param etiqueta Por agregar a cada palabra
 * @param numdoc Número de documento del cual provienen
 * @param pini Posición inicial en documento de la cadena c
 */
void
NodoTrieS::insertaConEtiqueta(string c, string etiqueta,
                              uint32_t numdoc, uint32_t pini)

{
        ASSERT(c != "");
        ASSERT(etiqueta != "");
        string::iterator i;
        string o = "";
        int p, inio;
        for (p = pini, inio = 0, i = c.begin(); i != c.end(); p++ , i++) {
                //clog << "insertaConEtiqueta: " << *i << endl;
                if (*i == '.') {
                        //clog << "OJO Punto: " << *i << endl;
                        if (o.length() > 0) {
                                string ot =
                                        normaliza(etiqueta) + string(":") + o;
                                if (ot.length() >= MAXCAD) {
                                        ot = ot.substr(0, MAXCAD);
                                        //clog << "OJO Cortamos 1" << endl;
                                }
                                ////clog << o << endl;
                                inserta(ot, Pos(numdoc, p));
                        }
                        o += ".";
                        //inio = p + 1;
                        //o = "";
                } else if (isspace(*i) || es_signo_punt(*i)) {
                        //clog << "Espacio o punct: " << *i << ", o.length()=" << o.length() << endl;
                        if (o.length() > 0) {
                                o = normaliza(etiqueta) + string(":") + o;
                                //clog << "tras unidr o.length()=" << o.length() << " y MAXCAD=" << MAXCAD << endl;

                                if (o.length() >= MAXCAD) {
                                        o = o.substr(0, MAXCAD);
                                        //clog << "OJO Cortamos" << endl;
                                }
                                ////clog << o << endl;
                                inserta(o, Pos(numdoc, p));
                        }
                        inio = p + 1;
                        o = "";
                } else if (o.length() < (MAXCAD - etiqueta.length() -1)) {
                        //clog << o << endl;
                        o += normalizaCaracter(*i);
                        //clog << o << endl;
                }
        }
        if (o.length() > 0) {
                // Insertar palabra anterior si hay
                o = normaliza(etiqueta) + string(":") + o;
                if (o.length() >= MAXCAD) {
                        o = o.substr(0, MAXCAD);
                }
                //clog << o << endl;
                inserta(o, Pos(numdoc, p));
        }
}
