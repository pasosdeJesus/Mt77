// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file RamDisco.cpp
 * Operaciones en RAM y Disco
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: RamDisco.cpp,v 1.17 2010/01/18 16:12:50 vtamara Exp $
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

#include "RamDisco.hpp"

#include "compresion/compresion.hpp"


/**
 * Calcula tamaño en bytes requerido para escribir nodo y hermanos
 * sin descendientes con función escribe
 */
uint32_t
precalcula_escribe_con_hermanos(NodoTrieS *n)
{
        //ASSERT(n == NULL || n->cad.length() > 0);

        uint32_t t = 0;
        if (n != NULL) {
                t = 1; // Por \n
        }
        //cerr << "OJO precalcula_escribe_con_hermanos '" << (n!=NULL ? n->cad : string("null")) << "' t=" << t ;
        while (n != NULL) {
                t += precalcula_escribe_actual(n->cad.length(),
                                               &(n->cpos));
                //cerr << " 2 t=" << t;
                n = n->hermano_mayor;
                //cerr << " 3 t=" << t;
        }
        //cerr << endl;
        return t;
}

uint32_t
precalcula_escribe(NodoTrieS *n)
{
        uint32_t t = precalcula_escribe_con_hermanos(n);
        //cerr << "OJO precalcula_escribe en '" << (n!=NULL ? n->cad : string("null")) << "' t=" << t ;

        while (n != NULL) {
                t += precalcula_escribe(n->hijo_menor);
                //cerr << "  En ciclo t=" << t ;
                n = n->hermano_mayor;
        }
        //cerr << endl;
        return t;
}


extern stringstream *depuraos ;

/**
 * Primero se escribe mayor con hermanos en una cadena:
 * MENOR { nhermano_1 nhijo_1 p1_1..p1_n} SIG { nhermano_2 nhijo_2 p2_1..p2_m} ... MAYOR { 00000 nhijo_k pk_1..pk_s}\n
 * Después hijos del menor al mayor.
 *
 * @param n Nodo
 * @param os Flujo de salida
 * @param desp Desplazamiento por agregar a todo 'apuntador'
 **/
void
escribePlanoStream(NodoTrieS *n, std::iostream &os,
                   Arbol_huffman arbolHuffman, uint32_t desp)
{

        NodoTrieS *tn;
        uint32_t nh = precalcula_escribe_con_hermanos(n);
        uint32_t dultimo = nh; // Distancia a último desde comienzo de grupo de hermanos
        uint32_t dhijo = 0; // distancia a hijo desde hermano actual

        // Escribe hermanos
        tn = n;
        uint32_t pactual = 0;  // Posición actual desde comienzo de grupo de hermanos
        //cout << "escribePlanoStream de: " << endl; n->aDotty(cerr, "");
        while (tn != NULL && tn->cad.length() > 0) {
                if (tn->hijo_menor != NULL) {
                        dhijo = dultimo - pactual;
                        dultimo += precalcula_escribe(tn->hijo_menor);
                } else {
                        dhijo = 0;
                }
                //cout << "dultimo=" << dultimo << ", pactual=" << pactual << ", os.tellp=" << os.tellp() << ". Por llamar escribeNodo(ss, " << tn->cad << ", " << (tn->cpos).size() << ", " << dhijo << ")" << endl;
                escribeNodo(os, tn->cad, &(tn->cpos), dhijo, arbolHuffman, desp);
                //cout << "ret = " << r << endl;
                if (depuraos != NULL) {
                        cout << depuraos->str() << endl;
                }
                pactual += precalcula_escribe_actual((tn->cad).length(),
                                                     &(tn->cpos));
                tn = tn->hermano_mayor;
        }
        os << "\n";
        //cout << "Escribe hijos de cada hermano" << endl;
        tn = n;
        while (tn != NULL) {
                if (tn->hijo_menor != NULL) {
                        escribePlanoStream(tn->hijo_menor, os, arbolHuffman, desp);
                }
                tn = tn->hermano_mayor;
        }
}



NodoTrieS *
leePlanoStream(std::istream &is) throw(string)
{
        string cad;
        set<Pos> *cpos;
        uint32_t hijo, hermano;
        int c = is.get();
        if (c == '\n' || c == EOF) {
                return NULL;
        }
        while (c != FINCADENA && c != EOF) {
                cad += c;
                c = is.get();
        }
        //cout << "leePlanoStream cad="<<cad<<endl;
        if (c != FINCADENA) {
                throw errorFormato(is, string("Se esperaba ") + FINCADENA);
        }
        hermano = leeNDesp(is);
        hijo = leeNDesp(is);
        //cout << "hermano ="<< hermano << ", hijo=" << hijo <<endl;

        cpos = leePos(is, NULL);
        if ((uint32_t)is.tellg() != hermano) {
                stringstream ss;
                ss << "Cursor y hermano difieren (" << is.tellg() << ", " << hermano << ")";
                throw errorFormato(is, ss.str());
        }

        NodoTrieS *hermanomayor = leePlanoStream(is);
        NodoTrieS *hijomenor = NULL;
        if (hijo > 0) {
                is.seekg(hijo);
                hijomenor = leePlanoStream(is);
        }
        NodoTrieS *r = new NodoTrieS(cad, hijomenor,
                                     hermanomayor, *cpos);

        return r;
}


void
escribePlano(NodoTrieS &t, vector<Doc> &docs, const char *na, const char *nrel,
             Arbol_huffman &arbolHuffman)
{

        ASSERT(na!=NULL && na[0] != '\0' && strlen(na)<FILENAME_MAX);
        ASSERT(nrel!=NULL && na[0] != '\0' && strlen(nrel)<FILENAME_MAX);

        fstream os(na, ios_base::out);
        os << MARCAIND << endl;
        escribePlanoStream(&t, os, arbolHuffman, 0);
        os.close();

        escribeRelacion(nrel, docs, NULL);
}



NodoTrieS *
leePlano(char *na, char *nrel, vector<Doc> &docs)
{
        NodoTrieS *r;
        //cout << "leePlano(" << na << ", " << nrel << ", " << idocs.size() << ", " << condensados.size() << ")" << endl;
        fstream is(na, ios_base::in);
        verificaIndice(is);
        try {
                r = leePlanoStream(is);
        } catch (string m) {
                stringstream ss;
                ss << na << ":" << is.tellg() << m << endl;
                throw ss.str();
        }
        is.close();

        leeRelacion(nrel, docs);

        return r;
}


uint32_t
escribeCopiaNodoRam(iostream &os, NodoTrieS *a, int saltacad,
                    NodoTrieS **phijo, vector<int64_t>* renum,
                    Arbol_huffman &arbolHuffman)
{
        ASSERT(phijo != NULL);

        uint32_t ret = 0;

        //clog << "\nescribeCopiaNodo is.tellg() = " << is.tellg();
        string cad = (a != NULL ? a->cad.substr(saltacad) : "");
        //clog  << " cad='" << cad << "'";
        if (cad != "") {
                //clog << " phermano=" << phermano;
                *phijo = a->hijo_menor;
                uint32_t dhijo = 0;
                if (*phijo != NULL) {
                        dhijo = precalcula_escribe_con_hermanos(a);
                }
                //clog << " phijo=" << phijo;
                //clog << " cpos=" << *cpos;
                //clog << " is.tellg() = " << is.tellg();
                set<Pos> *cp = copiaPos((a->cpos), renum);
                ret = escribeNodo(os, cad, cp, dhijo, arbolHuffman);
                delete cp;

        }

        //clog << " ret=" << ret << endl;
        return ret;
}


uint32_t
escribeCopiaSubarbolRam(iostream &os, NodoTrieS *a, int saltacad,
                        bool conHermanos, vector<int64_t>* renum,
                        Arbol_huffman &arbolHuffman)
{

        int64_t prini = os.tellp(); // Podría ser -1 si no hay marca de inicio
        ASSERT(prini >= 0);
        //cout << "OJO escribeCopiaSubarbol prini=" << prini << endl;
        //cout << "is.tellg()=" << is.tellg() << endl;

        string cad;
        uint32_t n; // Cantidad de nodos hermanos
        vector<NodoTrieS *> dhijo;
        vector<int64_t> pih;

        cad = (a != NULL ? a->cad.substr(saltacad) : "");
        //cout << "OJO cad=" << cad << endl;
        /* INV: cad es cadena leida del nodo por copiar
         * "cursor" de is está a continuación de la cadena leída
         * n es cantidad de nodos hermanos ya leidos
         * dhijo tiene lista de apuntadores a hijos
         * pih es vector de posiciones de apuntadores a hijos escritos en os
         */
        for (n = 0; cad != "" && (conHermanos || n == 0); n++) {
                //uint32_t h = leeNDesp(is);
                /*set<Pos> *cpos = leePos(is, renum); */
                //cout << "OJO prini=" << prini << " cpos=" << *cpos << endl;
                //cout << "OJO prini=" << prini << " phermano=" << phermano << endl;
                //cout << "OJO prini=" << prini << " h=" << h << endl;
                //cout << "OJO prini=" << prini << " is.tellg=" << is.tellg() << endl;
                dhijo.push_back(a->hijo_menor);
                set<Pos> *cp = copiaPos((a->cpos), renum);
                uint32_t tp = escribeNodo(os, cad, cp, 0, arbolHuffman);
                delete cp;
                //cout << "OJO prini=" << prini << " tp=" << tp << endl;
                pih.push_back(tp);
                //cout << "2 prini=" << prini << ", tellp=" << os.tellp() << endl;
                if (depuraos!=NULL) {
                        cout << depuraos->str() << endl;
                }
                a = a->hermano_mayor;
                if (conHermanos) {
                        cad = (a != NULL ? a->cad : "");
                }
        }
        if (n > 0) {
                os << endl;
                //cout << "OJO dhijo[0]=" << dhijo[0] << " " << dhijo.size() << endl;
        }
        uint32_t i;
        /* INV: n cantidad de nodos hermanos por copiar
         * "cursor" de os está al final
         * dhijo[i] tiene posición  en is del hijo menor del i-esimo nodo
         * pih[i] es posición en os donde está el hexadecimal con posición
         * 	de hijo menor del i-esimo nodo.
         */
        uint32_t pini, pfin;
        for (i = 0; i < n; i++) {
                if (dhijo[i] != NULL) {
                        //cout << "en hijos prini=" << prini << " dhijo["
                        //        << i << "]=" << dhijo[i] << endl;
                        pini = escribeCopiaSubarbolRam(os, dhijo[i], 0,
                                                       true, renum, arbolHuffman);
                        //cout << "prini=" << prini << " pini=" << pini << endl;
                        pfin = os.tellp();
                        //cout << "pfin=" << pfin <<endl;
                        os.seekp(pih[i]);
                        //cout << "pih[i]=" << pih[i] <<endl;
                        escribeNDesp(os, pini);
                        //cout << "escribiendo pini=" << pini <<endl;
                        os.seekp(pfin);
                }
        }

        return prini;
}


uint32_t
mezclaDiscoRam(istream &is1, NodoTrieS *a2, int saltacad, iostream &os,
               bool conHermanos1, bool conHermanos2,
               vector<int64_t> *renum1, vector<int64_t> *renum2,
               Arbol_huffman &arbolHuffman )
{

        string cad1;
        string cad2;
        uint32_t numhermanos; // Número de hermanos escritos en os
        uint32_t n1=0; // Número de hermanos revisados en is1
        uint32_t n2=0; // Número de hermanos revisados en a2
        uint32_t phermano1; // Posición de hermano en is1

        // Posibles operaciones con hijos en is1 y a2 para producir hijo(s)
        // en os
        const int O_COPIA1=1,
                O_COPIA2=2,
                O_MEZCLA_H1H2=3,
                O_MEZCLA_H1=4,
                O_MEZCLA_H2=5,
                O_COPIA_PRIMERO_1=6,
                O_COPIA_PRIMERO_2=7;

        ;
        vector<int64_t> dhijo1(0); // apuntadores a hijos de nodos en is1
        vector<int> dhijo2_saltacad(0); // Cuanto saltar de cad de hijos
        vector<NodoTrieS *> dhijo2(0); // apuntadores a hijos de nodos en a2
        vector<string> resto1(0); // Cadenas con las que comenazarán hijos
        vector<string> resto2(0); // Cadenas con las que comenzarían hijos
        vector<int> opera(0); // operación por realizar a hijos en 2da parte
        vector<int64_t> pih(0); // posiciones de apuntadores a hijos en os

        int64_t prini = os.tellp();  // Podría ser -1 si no hubiera marca de inicio
        ASSERT(depuraos != NULL  || prini >= 0);
        //clog << "OJO mezclaDiscoRam prini=" << prini << ", is1.tellg()=" << is1.tellg() << ", a2=" << (long)a2 << endl;
        //clog << "OJO peek1=" << is1.peek() << endl;
        if (a2 != NULL) {
                //clog << "OJO a2->cad.substr(saltacad)=" << a2->cad.substr(saltacad) << endl;
        }
        for (cad1=leeCad(is1, arbolHuffman), cad2=(a2 != NULL ? a2->cad.substr(saltacad) : ""),
                        numhermanos = 0;
                        (cad1!="" && (conHermanos1 || n1==0)) ||
                        (cad2!="" && (conHermanos2 || n2==0));
                        numhermanos++) {
                //clog << "OJO mezclaDiscoRam prini=" << prini << "cad1=" << cad1 << ", cad2= "<< cad2 << endl;
                dhijo1.push_back(-1);
                dhijo2_saltacad.push_back(0);
                dhijo2.push_back(NULL);
                resto1.push_back("");
                resto2.push_back("");
                opera.push_back(-1);
                pih.push_back(-1);

                if (!conHermanos1 && n1 >= 1) {
                        cad1="";
                }
                if (!conHermanos2 && n2 >= 1) {
                        cad2="";
                }
                /* INV:
                 * cad1 es cadena en nodo de is1
                 * cad2 es cadena en nodo de  a2
                 * cursor de is1 está a continuación de cad1
                 * 	(sobre inicio de posiciones)
                 * a2 está en nodo de cad2
                 * conHermanos1 es true si debe continuar con hermanos en is1
                 * conHermanos2 es true si debe seguir con hermanos en a2
                 */
                string c = "";
                if (cad1 != "" && cad2 != "") {
                        c = prefijo_comun_mas_largo(cad1, cad2);
                }
                //clog << "OJO  prefijo c="<<c<<endl;
                if (c == "") { // No hay prefijo comun
                        resto1[numhermanos] = "";
                        resto2[numhermanos] = "";
                        if (cad2=="" || (cad1!="" && cad1<cad2)) {
                                // e.g AMOR y BUENO  o
                                //     ""   y BUENO
                                //clog << "OJO se copia nodo de is1" <<endl;
                                phermano1 = leeNDesp(is1);
                                dhijo1[numhermanos] = leeNDesp(is1);
                                dhijo2[numhermanos] = NULL;
                                set<Pos> *cpos = leePos(is1, renum1);
                                opera[numhermanos] = O_COPIA1;
                                pih[numhermanos] =
                                        escribeNodo(os, cad1, cpos, 0, arbolHuffman);
                                delete cpos;
                                cpos = NULL;
                                cad1 = leeCad(is1, arbolHuffman);
                                n1++;
                                // cad2 quieto
                                // en la recursión hijos de cad1
                                //quedamos en el siguiente hermano mayor en is1
                        } else { /*cad2!="" && (cad1=="" || cad1 >= cad2) */
                                // e.g BUENO y AMOR o
                                //      ""   y AMOR
                                ASSERT(cad1 == "" || (cad1 > cad2));
                                //clog << "OJO se copia nodo de a2" <<endl;
                                dhijo1[numhermanos] = -1;
                                dhijo2_saltacad[numhermanos] = 0;
                                dhijo2[numhermanos] = a2->hijo_menor;
                                //clog << "OJO phijo=" << dhijo2[numhermanos] << endl;
                                opera[numhermanos] = O_COPIA2;
                                set<Pos> *cp = copiaPos((a2->cpos), renum2);
                                pih[numhermanos] =
                                        escribeNodo(os, cad2, cp, 0, arbolHuffman);
                                delete cp;
                                a2 = a2->hermano_mayor;
                                cad2 = (a2 != NULL) ? a2->cad : "";
                                n2++;
                        }
                } else if (cad1 == cad2) { // Hay prefijo c != "" && cad1!="" && cad2!=""
                        phermano1 = leeNDesp(is1);
                        dhijo1[numhermanos] = leeNDesp(is1);
                        dhijo2_saltacad[numhermanos] = 0;
                        dhijo2[numhermanos] = a2->hijo_menor;
                        //clog << "OJO iguales" <<endl;
                        set<Pos> cpos;
                        insert_iterator<set<Pos> >
                        cpos_ins(cpos, cpos.begin());
                        set<Pos> *cpos1 = leePos(is1, renum1);
                        set<Pos> *cpos2 = copiaPos(a2->cpos, renum2);

                        set_union(cpos1->begin(), cpos1->end(),
                                  cpos2->begin(), cpos2->end(),
                                  cpos_ins);
                        //clog << "OJO cad2='" << cad2 << "', cpos=" << cpos << endl;
                        pih[numhermanos] = escribeNodo(os, cad2, &cpos, 0, arbolHuffman);
                        delete cpos1;
                        cpos1=NULL;
                        delete cpos2;
                        cpos1=NULL;
                        //En hijos NodoTrieS *m=mezcla(a1->hijo_menor, a2->hijo_menor);
                        resto1[numhermanos] = "";
                        resto2[numhermanos] = "";
                        opera[numhermanos] = O_MEZCLA_H1H2;
                        cad1 = leeCad(is1, arbolHuffman);
                        n1++;
                        a2 = a2->hermano_mayor;
                        cad2 = (a2 != NULL) ? a2->cad : "";
                        n2++;
                } else { // hay prefijo c != "" && cad1!="" && cad2!="" && cad1!=cad2
                        string r1 = cad1.substr(c.size());
                        string r2 = cad2.substr(c.size());
                        resto1[numhermanos] = r1;
                        resto2[numhermanos] = r2;
                        ASSERT(r1 != "" || r2 != "");
                        //cerr << "hay posfijo r1="<<r1<<" r2="<<r2<<endl;
                        if (r1=="") {
                                // e.g BUENO BUENOS
                                ASSERT(r2 != "");
                                //clog << "OJO r1 vacio"<<endl;
                                // debe mezclar hijo de is1 con un sólo
                                // nodo de a2 pero comenzando en el
                                // posfijo r2

                                phermano1 = leeNDesp(is1);
                                dhijo1[numhermanos] = leeNDesp(is1);
                                dhijo2_saltacad[numhermanos] = saltacad +
                                                               c.length();
                                dhijo2[numhermanos] = a2;
                                a2 = a2->hermano_mayor; // Salta al sig en a2
                                set<Pos> *cpos1 = leePos(is1, renum1);
                                pih[numhermanos] = escribeNodo(os, c, cpos1, 0, arbolHuffman);
                                opera[numhermanos] = O_MEZCLA_H1;
                                delete cpos1;
                                cpos1=NULL;
                        } else if (r2 == "") {
                                // e.g BUENOS BUENO,  am a
                                ASSERT(r1 != "");
                                //clog << "OJO r2 vacio"<<endl;

                                phermano1 = leeNDesp(is1);
                                dhijo1[numhermanos] = (uint32_t)is1.tellg() -
                                                      MAXLNUMERO - 1 -
                                                      (uint32_t)r1.size();
                                dhijo2_saltacad[numhermanos] = 0;
                                dhijo2[numhermanos] = a2->hijo_menor;
                                is1.seekg(phermano1);
                                set<Pos> *cpos2 = copiaPos(a2->cpos, renum2);

                                //clog << "OJO en nodo dhijo2[numhermanos]="<< dhijo2[numhermanos] << endl;
                                pih[numhermanos] = escribeNodo(os, c,
                                                               cpos2, 0, arbolHuffman);
                                // Recursion NodoTrieS *m=mezcla(n1, a2->hijo_menor);
                                delete cpos2;
                                a2 = a2->hermano_mayor;
                                opera[numhermanos] = O_MEZCLA_H2;
                        } else if (r1 < r2) {
                                // e.g BUENA BUENO
                                //clog << "OJO r1<r2"<<endl;

                                phermano1 = leeNDesp(is1);
                                pih[numhermanos] = escribeNodo(os, c, NULL, 0, arbolHuffman);
                                dhijo1[numhermanos] = (uint32_t)is1.tellg() -
                                                      MAXLNUMERO - 1 -
                                                      (uint32_t)r1.size();
                                dhijo2_saltacad[numhermanos] = saltacad +
                                                               c.length();
                                dhijo2[numhermanos] = a2;
                                is1.seekg(phermano1);
                                a2 = a2->hermano_mayor;
                                opera[numhermanos] = O_COPIA_PRIMERO_1;
                                // El hijo de este nodo debe ser
                                // a1 con hermano mayor a2
                                //*r=new NodoTrieS(c, n1, NULL, set<Pos>());
                        } else { /* r1 >= r2 */
                                // e.g BUENO BUENA
                                //clog << "OJO r2 <= r1"<<endl;
                                phermano1 = leeNDesp(is1);
                                pih[numhermanos] = escribeNodo(os, c, NULL, 0, arbolHuffman);
                                dhijo1[numhermanos] = (uint32_t)is1.tellg() -
                                                      MAXLNUMERO - 1 -
                                                      (uint32_t)r1.size();
                                dhijo2_saltacad[numhermanos] = saltacad +
                                                               c.length();
                                dhijo2[numhermanos] = a2;
                                //saltaPos(is1); leeNDesp(is1);
                                is1.seekg(phermano1);
                                a2 = a2->hermano_mayor;
                                opera[numhermanos] = O_COPIA_PRIMERO_2;
                        }
                        cad1=leeCad(is1, arbolHuffman);
                        n1++;
                        cad2=(a2 != NULL ? a2->cad : "");
                        n2++;
                }
        }
        os << endl;
        //clog << "OJO Termina hermanos, pasa a hijos" << endl;

        // Desplazamientos en os
        uint32_t pini, pfin;
        uint32_t hijo1 = 0;
        NodoTrieS *hijo2 = NULL;
        uint32_t ph1, ph2, pnh1, pnh2;
        for (uint32_t n = 0; n < numhermanos; n++) {
                pini = 0; // Posición donde queda hijo en os
                switch (opera[n]) {
                case O_COPIA1:
                        //clog << "OJO COPIA1" <<endl;
                        if (dhijo1[n] > 0) {
                                is1.seekg(dhijo1[n]);
                                pini = escribeCopiaSubarbol(os, is1,
                                                            true, arbolHuffman, renum1);
                        }
                        break;

                case O_COPIA2:
                        //clog << "OJO COPIA2" <<endl;
                        if (dhijo2[n] != NULL) {
                                //clog << "OJO dhijo2[" << n << "]=" << dhijo2[n] << endl;
                                pini = escribeCopiaSubarbolRam(os,
                                                               dhijo2[n],
                                                               0, true, renum2, arbolHuffman);
                        }
                        break;

                case O_MEZCLA_H1H2:
                        //clog << "OJO MEZCLA_H1H2" <<endl;
                        if (depuraos!=NULL) {
                                //clog << depuraos->str() << endl;
                        }


                        if (dhijo1[n] > 0 && dhijo2[n] != NULL) {
                                is1.seekg(dhijo1[n]);
                                pini = mezclaDiscoRam(is1, dhijo2[n],
                                                      dhijo2_saltacad[n], os,
                                                      true, true, renum1,
                                                      renum2, arbolHuffman) ;
                        } else if (dhijo1[n] > 0) {
                                ASSERT(dhijo2[n] == NULL);
                                is1.seekg(dhijo1[n]);
                                pini = escribeCopiaSubarbol(os, is1,
                                                            true, arbolHuffman, renum1);
                        } else if (dhijo2[n] != NULL) {
                                //clog << "OJO dhijo2[n]=" <<dhijo2[n]<< endl;
                                ASSERT(dhijo1[n] == 0);
                                pini = escribeCopiaSubarbolRam(os,
                                                               dhijo2[n],
                                                               dhijo2_saltacad[n],
                                                               true, renum2, arbolHuffman);
                                //clog << "OJO pini=" << pini << endl;
                        }
                        break;
                case O_MEZCLA_H1:
                        //clog << "OJO MEZCLA_H1" <<endl;
                        ASSERT(dhijo2[n] != NULL);

                        //clog << "OJO dhijo2[n]=" << dhijo2[n] << endl;
                        if (dhijo1[n] > 0) {
                                is1.seekg(dhijo1[n]);
                                pini = mezclaDiscoRam(is1, dhijo2[n],
                                                      dhijo2_saltacad[n],
                                                      os, true, false, renum1,
                                                      renum2, arbolHuffman) ;
                        } else {
                                pini = escribeCopiaSubarbolRam(os,
                                                               dhijo2[n],
                                                               dhijo2_saltacad[n],
                                                               false, renum2, arbolHuffman);
                        }
                        break;

                case O_MEZCLA_H2:
                        //clog << "OJO MEZCLA_H2" <<endl;
                        ASSERT(dhijo1[n] != 0);

                        is1.seekg(dhijo1[n]);
                        //clog << "OJO dhijo2[n]=" << dhijo2[n] <<  endl;
                        if (dhijo2[n] != NULL) {
                                pini = mezclaDiscoRam(is1, dhijo2[n],
                                                      dhijo2_saltacad[n],
                                                      os, false, true, renum1,
                                                      renum2, arbolHuffman) ;
                        } else {
                                pini = escribeCopiaSubarbol(os, is1,
                                                            false, arbolHuffman, renum1);
                        }
                        break;

                case O_COPIA_PRIMERO_1:
                        //clog << "OJO COPIA_PRIMERO_1" <<endl;
                        ASSERT(dhijo1[n] != 0);
                        ASSERT(dhijo2[n] != NULL);

                        is1.seekg(dhijo1[n]);
                        pini = os.tellp();

                        // pos no es NULL en los siguientes,
                        // más bien copiar de is1 y a2 e hijos
                        // después.
                        pnh1 = escribeCopiaNodo(os, is1, hijo1, renum1, arbolHuffman);
                        pnh2 = escribeCopiaNodoRam(os, dhijo2[n],
                                                   dhijo2_saltacad[n],
                                                   &hijo2, renum2,
                                                   arbolHuffman);
                        os << endl;
                        ph1=os.tellp();
                        if (hijo1 > 0) {
                                is1.seekg(hijo1);
                                (void)escribeCopiaSubarbol(os, is1,
                                                           true, arbolHuffman, renum1);
                        }
                        ph2=os.tellp();
                        if (hijo2 != NULL) {
                                escribeCopiaSubarbolRam(os, hijo2,
                                                        0, true, renum2, arbolHuffman);
                        }
                        pfin = os.tellp();
                        if (hijo1 > 0) {
                                os.seekp(pnh1);
                                escribeNDesp(os, ph1);
                        }
                        if (hijo2 != NULL) {
                                os.seekp(pnh2);
                                escribeNDesp(os, ph2);
                        }
                        os.seekp(pfin);

                        break;

                case O_COPIA_PRIMERO_2:
                        //clog << "OJO COPIA_PRIMERO_2" <<endl;
                        ASSERT(dhijo1[n] != 0);
                        ASSERT(dhijo2[n] != NULL);

                        is1.seekg(dhijo1[n]);
                        pini = os.tellp();

                        pnh2=escribeCopiaNodoRam(os, dhijo2[n],
                                                 dhijo2_saltacad[n],
                                                 &hijo2, renum2,
                                                 arbolHuffman);
                        pnh1=escribeCopiaNodo(os, is1, hijo1, renum1, arbolHuffman);
                        os << endl;
                        ph2=os.tellp();
                        if (hijo2 != NULL) {
                                escribeCopiaSubarbolRam(os, hijo2,
                                                        0, true, renum2, arbolHuffman);
                        }
                        ph1=os.tellp();
                        if (hijo1 > 0) {
                                is1.seekg(hijo1);
                                escribeCopiaSubarbol(os, is1,
                                                     true, arbolHuffman, renum1);
                        }
                        pfin = os.tellp();
                        if (hijo2 != NULL) {
                                os.seekp(pnh2);
                                escribeNDesp(os, ph2);
                        }
                        if (hijo1 > 0) {
                                os.seekp(pnh1);
                                escribeNDesp(os, ph1);
                        }
                        os.seekp(pfin);

                        break;

                default:
                        throw std::string("Falla: estado desconocido");
                        break;
                }
                pfin = os.tellp();
                //clog << "OJO pfin=" << pfin << endl;
                //clog << "OJO n=" << n << ", pih[n]=" << pih[n]<< endl;
                os.seekp(pih[n]);
                escribeNDesp(os, pini);
                os.seekp(pfin);
        }
        //clog << "OJO Termina hijos" << endl;
        return prini;
}


void
subindiceDiscoaRAM(std::istream &is, NodoTrieS *t, uint32_t nd, string pcad)
        throw(string)
{
        //clog << "OJO subindiceDiscoaRAM(is, t, nd=" << nd << ", pcad=" << pcad << ")" << endl;
        string cad;
        set<Pos> *cpos;

        uint32_t hijo, hermano;

        int c = is.get();
        if (c == '\n' || c == EOF) {
                //clog << "OJO subindiceDiscoaRAM fin";
                return;
        }
        while (c != FINCADENA && c != EOF) {
                cad += c;
                c = is.get();
        }
        //clog << "OJO subindiceDiscoRAM cad=" << cad << endl;
        if (c != FINCADENA) {
                throw errorFormato(is, string("Se esperaba ") + FINCADENA);
        }
        hermano = leeNDesp(is);
        hijo = leeNDesp(is);
        //clog << "OJO hermano ="<< hermano << ", hijo=" << hijo <<endl;

        cpos = leePos(is, NULL);
        if ((uint32_t)is.tellg() != hermano) {
                stringstream ss;
                ss << "Cursor y hermano difieren (" << is.tellg() << ", " << hermano << ")";
                throw errorFormato(is, ss.str());
        }
        set<Pos>::iterator i;
        set<Pos> *opos = new set<Pos>();

        for (i = cpos->begin(); i != cpos->end(); i++) {
                ASSERT(i->numd > 0);
                if (i->numd == nd) {
                        //clog << "OJO insertando " << pcad + cad << ", i->numb=" << i->numb <<endl;
                        opos->insert(Pos(1, i->numb));
                }
        }
        delete cpos;
        if (opos->size() > 0) {
                t->inserta(pcad + cad, opos);
        }
        delete opos;

        // Pasamos al hermano mayor
        subindiceDiscoaRAM(is, t, nd, pcad);

        // Ahora  a hijo menor
        if (hijo > 0) {
                is.seekg(hijo);
                subindiceDiscoaRAM(is, t, nd, pcad + cad);
        }
}



/* Polimorfico en progreso

class fTrie {

	string retCad();
	uint64_t tellp();
};


class fTrieRam: fTrie {
	NodoTrieS *a2 = NULL;

	fTrieRam(NodoTrieS *r)
	{
		a2 = r;
	}

	string retCad()
	{
		return (a2 != NULL ? a2->cad.substr(saltacad) : "");
	}
	uint64_t tellp()
	{
		return (long)a2;
	}
};

class fTrieDisco: fTrie {
	istrema is1;

	fTrieRam(istream &i)
	{
		is1 = i;
	}

	string retCad()
	{
		return leeCad(is1);
	}
	uint64_t tellp()
	{
		return is1.tellp();
	}
	uint64_t leeNDesp()
	{
		return leeNDesp(is1);
	}

};

long
mezclaPolimorfico(fTrie &is1, fTrie &a2, int saltacad, fTrie &os,
		bool conHermanos1, bool conHermanos2,
		vector<long> *renum)
{

	string cad1;
	string cad2;
	long numhermanos; // Número de hermanos escritos en os
	long n1=0; // Número de hermanos revisados en is1
	long n2=0; // Número de hermanos revisados en a2
        long phermano1; // Posición de hermano en is1
        long phermano2; // Posición de hermano en a2

	// Posibles operaciones con hijos en is1 y a2 para producir hijo(s)
	// en os
	const int O_COPIA1=1,
	      	O_COPIA2=2,
		O_MEZCLA_H1H2=3,
		O_MEZCLA_H1=4,
		O_MEZCLA_H2=5,
		O_COPIA_PRIMERO_1=6,
		O_COPIA_PRIMERO_2=7;

	      ;
	vector<long> dhijo1(0); // apuntadores a hijos de nodos en is1
	vector<int> dhijo2_saltacad(0); // Cuanto saltar de cad de hijos
	vector<NodoTrieS *> dhijo2(0); // apuntadores a hijos de nodos en a2
	vector<string> resto1(0); // Cadenas con las que comenazarán hijos
	vector<string> resto2(0); // Cadenas con las que comenzarían hijos
	vector<int> opera(0); // operación por realizar a hijos en 2da parte
	vector<long> pih(0); // posiciones de apuntadores a hijos en os

	long prini = os.tellp();
	//cout << "OJO mezclaDiscoRam prini=" << prini << ", is1.tellg()=" << is1.tellg() << ", a2=" << (long)a2 << endl;
	//cout << "peek1=" << is1.peek() << endl;
	//cout << "a2->cad.substr(saltacad)=" << a2->cad.substr(saltacad) << endl;
	for (cad1=is1.retCad(), cad2=a2.retCad),
			numhermanos = 0;
			(cad1!="" && (conHermanos1 || n1==0)) ||
			(cad2!="" && (conHermanos2 || n2==0));
			 numhermanos++) {
		//cout << "OJO mezclaDiscoRam prini=" << prini << "cad1=" << cad1 << ", cad2= "<< cad2 << endl;
		dhijo1.push_back(-1);
		dhijo2_saltacad.push_back(0);
		dhijo2.push_back(NULL);
		resto1.push_back("");
		resto2.push_back("");
		opera.push_back(-1);
		pih.push_back(-1);

		if (!conHermanos1 && n1 >= 1) {
			cad1="";
		}
		if (!conHermanos2 && n2 >= 1) {
			cad2="";
		}

		string c = "";
		if (cad1 != "" && cad2 != "") {
			c = prefijo_comun_mas_largo(cad1, cad2);
		}
		//cout << " prefijo c="<<c<<endl;
		if (c == "") { // No hay prefijo comun
			resto1[numhermanos] = "";
			resto2[numhermanos] = "";
			if (cad2=="" || (cad1!="" && cad1<cad2)) {
                                // e.g AMOR y BUENO  o
                                //     ""   y BUENO
				//cout<< "se copia nodo de is1" <<endl;
                                phermano1 = leeNDesp(is1);
				dhijo1[numhermanos] = is1.leeNDesp();
				dhijo2[numhermanos] = NULL;
				set<Pos> *cpos = leePos(is1, NULL);
				opera[numhermanos] = O_COPIA1;
				pih[numhermanos] =
                                        escribeNodo(os, cad1, cpos, 0);
				delete cpos; cpos = NULL;
				cad1 = leeCad(is1);
				n1++;
				// cad2 quieto
				// en la recursión hijos de cad1
				//quedamos en el siguiente hermano mayor en is1
			}
			else { // cad2!="" && (cad1=="" || cad1 >= cad2)
                                // e.g BUENO y AMOR o
                                //      ""   y AMOR
                                ASSERT(cad1 == "" || (cad1 > cad2));
				//cout << "se copia nodo de a2" <<endl;
				dhijo1[numhermanos] = -1;
				dhijo2_saltacad[numhermanos] = 0;
				dhijo2[numhermanos] = a2->hijo_menor;
                                //cout << "phermano2=" << phermano2 <<
                                //        ", phijo=" << dhijo2[numhermanos] << endl;
				opera[numhermanos] = O_COPIA2;
				pih[numhermanos] =
                                        escribeNodo(os, cad2, &(a2->cpos), 0);
				a2 = a2->hermano_mayor;
				cad2 = (a2 != NULL) ? a2->cad : "";
				n2++;
			}
		}
		else if (cad1 == cad2) { // Hay prefijo c != "" && cad1!="" && cad2!=""
                        phermano1 = leeNDesp(is1);
			dhijo1[numhermanos] = leeNDesp(is1);
			dhijo2_saltacad[numhermanos] = 0;
			dhijo2[numhermanos] = a2->hermano_mayor;
			//cout << "iguales" <<endl;
			set<Pos> cpos;
			insert_iterator<set<Pos> >
                  		cpos_ins(cpos, cpos.begin());
			set<Pos> *cpos1 = leePos(is1, NULL);

			set_union(cpos1->begin(), cpos1->end(),
				(a2->cpos).begin(), (a2->cpos).end(),
				cpos_ins);
			pih[numhermanos] = escribeNodo(os, cad2, &cpos, 0);
			delete cpos1; cpos1=NULL;
			//En hijos NodoTrieS *m=mezcla(a1->hijo_menor, a2->hijo_menor);
			resto1[numhermanos] = "";
			resto2[numhermanos] = "";
			opera[numhermanos] = O_MEZCLA_H1H2;
			cad1 = leeCad(is1); n1++;
			a2 = a2->hermano_mayor;
			cad2 = (a2 != NULL) ? a2->cad : "";
			n2++;
		}
		else { // hay prefijo c != "" && cad1!="" && cad2!="" && cad1!=cad2
			string r1 = cad1.substr(c.size());
			string r2 = cad2.substr(c.size());
			resto1[numhermanos] = r1;
			resto2[numhermanos] = r2;
			ASSERT(r1 != "" || r2 != "");
			//cerr << "hay posfijo r1="<<r1<<" r2="<<r2<<endl;
			if (r1=="") {
                                // e.g BUENO BUENOS
				ASSERT(r2 != "");
				//cout << "r1 vacio"<<endl;
				// debe mezclar hijo de is1 con un sólo
				// nodo de a2 pero comenzando en el
				// posfijo r2

				phermano1 = leeNDesp(is1);
				dhijo1[numhermanos] = leeNDesp(is1);
				dhijo2_saltacad[numhermanos] = c.length();
                                dhijo2[numhermanos] = a2;
                                a2 = a2->hermano_mayor; // Salta al sig en a2
                                set<Pos> *cpos1 = leePos(is1, NULL);
                                pih[numhermanos] = escribeNodo(os, c, cpos1, 0);
                                opera[numhermanos] = O_MEZCLA_H1;
                                delete cpos1; cpos1=NULL;
                        }
                        else if (r2 == "") {
                                // e.g BUENOS BUENO
                                ASSERT(r1 != "");
                                //cout << "r2 vacio"<<endl;
                                //
				phermano1 = leeNDesp(is1);
                                dhijo1[numhermanos] = (long)is1.tellg() -
                                        MAXLNUMERO - 1 - (long)r1.size();
				dhijo2_saltacad[numhermanos] = 0;
                                dhijo2[numhermanos] = a2->hermano_mayor;
                                is1.seekg(phermano1);
                                //cout << "OJO en nodo dhijo2[numhermanos]="
                                //<< dhijo2[numhermanos] << endl;
                                pih[numhermanos] = escribeNodo(os, c,
						&(a2->cpos), 0);
                                // Recursion NodoTrieS *m=mezcla(n1, a2->hijo_menor);
				a2 = a2->hermano_mayor;
                                opera[numhermanos] = O_MEZCLA_H2;
                        }
                        else if (r1 < r2) {
                                // e.g BUENA BUENO
                                //cout << "r1<r2"<<endl;

				phermano1 = leeNDesp(is1);
                                pih[numhermanos] = escribeNodo(os, c, NULL, 0);
                                dhijo1[numhermanos] = (long)is1.tellg() -
                                        MAXLNUMERO - 1 - (long)r1.size();
                                dhijo2_saltacad[numhermanos] = c.length();
                                dhijo2[numhermanos] = a2;
                                is1.seekg(phermano1);
				a2 = a2->hermano_mayor;
                                opera[numhermanos] = O_COPIA_PRIMERO_1;
                                // El hijo de este nodo debe ser
                                // a1 con hermano mayor a2
                                // r=new NodoTrieS(c, n1, NULL, set<Pos>());
                        }
                        else { // r1 >= r2
                                // e.g BUENO BUENA
                                //cout << "r2 <= r1"<<endl;
				phermano1 = leeNDesp(is1);
                                pih[numhermanos] = escribeNodo(os, c, NULL, 0);
                                dhijo1[numhermanos] = (long)is1.tellg() -
                                        MAXLNUMERO - 1 - (long)r1.size();
                                dhijo2_saltacad[numhermanos] = c.length();
                                dhijo2[numhermanos] = a2->hermano_mayor;
                                //saltaPos(is1); leeNDesp(is1);
                                is1.seekg(phermano1);
                                a2 = a2->hermano_mayor;
                                opera[numhermanos] = O_COPIA_PRIMERO_2;
                        }
                        cad1=leeCad(is1); n1++;
                        cad2=(a2 != NULL ? a2->cad : ""); n2++;
                }
        }
        os << endl;
        //cout << "Termina hermanos, pasa a hijos" << endl;

        // Desplazamientos en os
        long pini, pfin;
        long hijo1 = 0;
	NodoTrieS *hijo2 = NULL;
        long ph1, ph2, pnh1, pnh2;
        for (int n = 0; n < numhermanos; n++) {
                pini = 0; // Posición donde queda hijo en os
                switch (opera[n]) {
                        case O_COPIA1:
                                //cout << "COPIA1" <<endl;
                                if (dhijo1[n] > 0) {
                                        is1.seekg(dhijo1[n]);
					pini = escribeCopiaSubarbol(os, is1,
							true, NULL);
				}
				break;

			case O_COPIA2:
				//cout << "COPIA2" <<endl;
				if (dhijo2[n] != NULL) {
                                        //cout << "dhijo2[" << n << "]=" <<
                                                //dhijo2[n] << endl;
					pini = escribeCopiaSubarbolRam(os,
							dhijo2[n],
							0, true, renum);
				}
				break;

			case O_MEZCLA_H1H2:
				//cout << "MEZCLA_H1H2" <<endl;
		if (depuraos!=NULL) {
			//cout << depuraos->str() << endl;
		}


				if (dhijo1[n] > 0 && dhijo2[n] != NULL) {
					is1.seekg(dhijo1[n]);
					pini = mezclaDiscoRam(is1, dhijo2[n],
							dhijo2_saltacad[n], os,
							true, true, renum) ;
				}
				else if (dhijo1[n] > 0) {
					ASSERT(dhijo2[n] == NULL);
					is1.seekg(dhijo1[n]);
					pini = escribeCopiaSubarbol(os, is1,
							true, NULL);
				}
				else if (dhijo2[n] != NULL) {
					//cout << "OJO dhijo2[n]=" <<dhijo2[n]<< endl;
					ASSERT(dhijo1[n] == 0);
					pini = escribeCopiaSubarbolRam(os,
							dhijo2[n],
							dhijo2_saltacad[n],
							true, renum);
					//cout << "OJO pini=" << pini << endl;
				}
				break;
			case O_MEZCLA_H1:
				//cout << "MEZCLA_H1" <<endl;
				ASSERT(dhijo2[n] != NULL);

				//cout << "OJO dhijo2[n]=" << dhijo2[n] << endl;
				if (dhijo1[n] > 0) {
					is1.seekg(dhijo1[n]);
					pini = mezclaDiscoRam(is1, dhijo2[n],
						       dhijo2_saltacad[n],
					       	       os, true, false, renum) ;
				}
				else {
					pini = escribeCopiaSubarbolRam(os,
							dhijo2[n],
							dhijo2_saltacad[n],
							false, renum);
				}
				break;

			case O_MEZCLA_H2:
				//cout << "MEZCLA_H2" <<endl;
				ASSERT(dhijo1[n] != 0);

				is1.seekg(dhijo1[n]);
				//cout << "dhijo2[n]=" << dhijo2[n] <<  endl;
				if (dhijo2[n] != NULL) {
					pini = mezclaDiscoRam(is1, dhijo2[n],
							dhijo2_saltacad[n],
							os, false, true, renum) ;
				}
				else {
					pini = escribeCopiaSubarbol(os, is1,
							false, NULL);
				}
				break;

			case O_COPIA_PRIMERO_1:
				//cout << "COPIA_PRIMERO_1" <<endl;
				ASSERT(dhijo1[n] != 0);
				ASSERT(dhijo2[n] != NULL);

				is1.seekg(dhijo1[n]);
				pini = os.tellp();

				// pos no es NULL en los siguientes,
				// más bien copiar de is1 y a2 e hijos
				// después.
				pnh1=escribeCopiaNodo(os, is1, hijo1, NULL);
				pnh2=escribeCopiaNodoRam(os, dhijo2[n],
						dhijo2_saltacad[n],
						&hijo2, renum);
				os << endl;
				ph1=os.tellp();
				if (hijo1 > 0) {
					is1.seekg(hijo1);
					(void)escribeCopiaSubarbol(os, is1,
							true, NULL);
				}
				ph2=os.tellp();
				if (hijo2 != NULL) {
					escribeCopiaSubarbolRam(os, hijo2,
							0, true, renum);
				}
				pfin = os.tellp();
				if (hijo1 > 0) {
					os.seekp(pnh1);
					escribeNDesp(os, ph1);
				}
				if (hijo2 != NULL) {
					os.seekp(pnh2);
					escribeNDesp(os, ph2);
				}
				os.seekp(pfin);

				break;

			case O_COPIA_PRIMERO_2:
				//cout << "COPIA_PRIMERO_2" <<endl;
				ASSERT(dhijo1[n] != 0);
				ASSERT(dhijo2[n] != NULL);

				is1.seekg(dhijo1[n]);
				pini = os.tellp();

				pnh2=escribeCopiaNodoRam(os, dhijo2[n],
						dhijo2_saltacad[n],
						&hijo2, renum);
				pnh1=escribeCopiaNodo(os, is1, hijo1, NULL);
				os << endl;
				ph2=os.tellp();
				if (hijo2 != NULL) {
					escribeCopiaSubarbolRam(os, hijo2,
							0, true, renum);
				}
				ph1=os.tellp();
				if (hijo1 > 0) {
					is1.seekg(hijo1);
					escribeCopiaSubarbol(os, is1,
							true, NULL);
				}
				pfin = os.tellp();
				if (hijo2 != NULL) {
					os.seekp(pnh2);
					escribeNDesp(os, ph2);
				}
				if (hijo1 > 0) {
					os.seekp(pnh1);
					escribeNDesp(os, ph1);
				}
				os.seekp(pfin);

				break;

			default:
				throw std::string("Falla: estado desconocido");
				break;
		}
		pfin = os.tellp();
		//cout << "OJO pfin=" << pfin << endl;
		//cout << "OJO n=" << n << ", pih[n]=" << pih[n]<< endl;
		os.seekp(pih[n]);
		escribeNDesp(os, pini);
		os.seekp(pfin);
	}
	//cout << "Termina hijos" << endl;
	return prini;
}

*/
