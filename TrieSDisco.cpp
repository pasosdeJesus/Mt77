// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file TrieSDisco.cpp
 * Representación en disco de un TrieS
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: TrieSDisco.cpp,v 1.24 2010/03/08 12:02:40 vtamara Exp $
 */


#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>
#include <stdint.h>


using namespace std;


#include "TrieSDisco.hpp"

stringstream *depuraos = NULL;


uint32_t
precalcula_escribe_actual(uint32_t longcad, set<Pos> *cpos)
{
        uint32_t t = longcad;
        t += 1; 	 // {
        t += longNDesp(0); // pos hermano mayor
        t += longNDesp(0); // pos hijo menor
        t += longPos(cpos);
        //clog << "OJO precalcular_escribe_actual(" << longcad <<
        //                ", " << cpos << ") da " << t << endl;
        return t;
}


/*
 * cadena
 * {
 * pos a siguiente hermano (el hermano final se marca con \n)
 * pos a hijo mayor o 0 si no hay
 * (p1,d1),...(p_k_,d_k)
 * }
 * @param desp Desplazamiento inicial en stream
 * @param dhijo Distancia al hijo menor
 * @return Posición donde comienza ''apuntador'' a hijo menor
 */
uint32_t
escribeNodo(iostream &os, string c, set<Pos> *cpos,
            uint32_t dhijos, uint32_t desp,
            Arbol_huffman &arbolHuffman )
{
        ASSERT(c.length() > 0);
        ASSERT(c.length() <= MAXCAD);
        ASSERT(dhijos >= 0);

        //clog << "escribeNodo(os, " << c << ", " << (cpos!=NULL ? cpos->size() : 0) << ", " << dhijos << ", " << desp << ")" << endl;
        uint32_t pih = 0;

        int64_t pini = os.tellp();  // Es  posible que sea -1
        //clog << "pini=" << pini << endl;
        os << c << FINCADENA;
        uint32_t dhermano = (pini >= 0 ? pini : 0) + desp +
                        precalcula_escribe_actual(c.length(), cpos); //+ 1;
        //clog << "dhermano=" << dhermano << endl;
        escribeNDesp(os, dhermano);
        uint32_t phijo = 0;
        if (dhijos > 0) {
                phijo = ((pini >= 0) ? pini : 0) + desp + dhijos;
        }
        //clog << "phijo=" << phijo << endl;
        pih = os.tellp(); // No puede ser -1
        //clog << "pih=" << pih << endl;
        escribeNDesp(os, phijo);
        escribePos(os, cpos);

        if (depuraos != NULL) {
                //clog << "escribeNodo: '" << depuraos->str() << "'" << endl;
        }
        return pih;
}


uint32_t escribeCopiaNodo(iostream &os, istream &is, uint32_t &phijo,
                          vector<int64_t>* renum,
                          Arbol_huffman &arbolHuffman )
{

        uint32_t ret = 0;

        int64_t pini = is.tellg();
        //clog << "\nescribeCopiaNodo is.tellg() = " << is.tellg();
        string cad = leeCad(is);
        //clog << " cad='" << cad << "'";
        if (cad != "") {
#if defined(NDEBUG)
                (void)leeNDesp(is); // Si depura comentar y descomentar ant.
#else
                uint32_t phermano = leeNDesp(is); 
#endif
                //clog << " phermano=" << phermano;
                phijo = leeNDesp(is);
                uint32_t dhijo = 0;
                if (phijo > 0) {
                        dhijo = phijo - pini;
                }
                //clog << " phijo=" << phijo;
                set<Pos> *cpos = leePos(is, renum);
                //clog << " cpos=" << *cpos;
                ASSERT(phijo == 0 || is.tellg() <= phijo);
                //clog << " is.tellg() = " << is.tellg();
                ASSERT(phermano == 0 || is.tellg() <= phermano);
                ret = escribeNodo(os, cad, cpos, dhijo, arbolHuffman);
                delete cpos;
                cpos = NULL;
        }

        //clog << " ret=" << ret << endl;
        return ret;
}



uint32_t
escribeCopiaSubarbol(iostream &os, istream &is, bool conHermanos,
                     vector<int64_t>* renum,
                     Arbol_huffman &arbolHuffman ) ;
{

        int64_t prini = os.tellp(); //puede ser -1
        //clog << "OJO escribeCopiaSubarbol prini=" << prini << endl;
        //clog << "OJO is.tellg()=" << is.tellg() << endl;

        string cad;
        uint32_t n; // Cantidad de nodos hermanos
        vector<uint32_t> dhijo;
        vector<uint32_t> pih;

        cad = leeCad(is);
        //clog << "OJO cad=" << cad << endl;
        /* INV: cad es cadena leida del nodo por copiar
         * "cursor" de is está a continuación de la cadena leída 
         * n es cantidad de nodos hermanos ya leidos
         * dhijo tiene lista de apuntadores a hijos 
         * pih es vector de posiciones de apuntadores a hijos escritos en os
         */
        for (n = 0; cad != "" && (conHermanos || n == 0); n++) {
                //clog << "OJO is.tellg()=" << is.tellg() << endl;
#if defined(NDEBUG)
                (void)leeNDesp(is); 
#else
                uint32_t phermano = leeNDesp(is); 
                //clog << "OJO prini=" << prini << " phermano=" << phermano << endl;
#endif
                uint32_t h = leeNDesp(is);
                //clog << "OJO prini=" << prini << " h=" << h << endl;
                set<Pos> *cpos = leePos(is, renum);
                //clog << "OJO prini=" << prini << " cpos=" << *cpos << endl;
                //clog << "OJO prini=" << prini << " is.tellg=" << is.tellg() << endl;
                ASSERT(h == 0 || is.tellg()<=h);
                ASSERT(phermano == 0 || is.tellg()<=phermano);
                //clog << "OJO paso ASSERT" << endl;
                dhijo.push_back(h);
                uint32_t tp = escribeNodo(os, cad, cpos, 0, arbolHuffman);
                //clog << "OJO prini=" << prini << " tp=" << tp << endl;
                pih.push_back(tp);
                //clog << "OJO 2 prini=" << prini << ", os.tellp=" << os.tellp() << endl;
                delete cpos;
                cpos = NULL;
                if (depuraos!=NULL) {
                        //clog << depuraos->str() << endl;
                }
                if (conHermanos) {
                        cad = leeCad(is);
                        //clog << "OJO conHermanos cad=" << cad << endl;
                }
        }
        if (n > 0) {
                os << endl;
                //clog << "OJO dhijo[0]=" << dhijo[0] << " " << dhijo.size() << endl;
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
                if (dhijo[i] > 0) {
                        //clog << "OJO en hijos prini=" << prini << " dhijo[" << i << "]=" << dhijo[i] << endl;
                        is.seekg(dhijo[i]);
                        pini = escribeCopiaSubarbol(os, is, true, renum);
                        //clog << "OJO prini=" << prini << " pini=" << pini << endl;
                        pfin = os.tellp(); // No puede ser -1
                        //clog << "OJO pfin=" << pfin <<endl;
                        os.seekp(pih[i]);
                        //clog << "OJO pih[i]=" << pih[i] <<endl;
                        escribeNDesp(os, pini);
                        //clog << "OJO escribiendo pini=" << pini <<endl;
                        os.seekp(pfin);
                }
        }

        return prini;
}


uint32_t
mezclaRec(istream &is1, istream &is2, iostream &os,
          bool conHermanos1, bool conHermanos2,
          vector<int64_t> *renum1, vector<int64_t> *renum2,
          Arbol_huffman &arbolHuffman )
{

        string cad1;
        string cad2;
        uint32_t numhermanos; // Número de hermanos escritos en os
        uint32_t n1=0; // Número de hermanos revisados en is1
        uint32_t n2=0; // Número de hermanos revisados en is2
        uint32_t phermano1; // Posición de hermano en is1
        uint32_t phermano2; // Posición de hermano en is2

        // Posibles operaciones con hijos en is1 e is2 para producir hijo(s)
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
        vector<int64_t> dhijo2(0); // apuntadores a hijos de nodos en is2
        vector<string> resto1(0); // Cadenas con las que comenazarán hijos
        vector<string> resto2(0); // Cadenas con las que comenzarían hijos
        vector<int64_t> opera(0); // operación por realizar a hijos en 2da parte
        vector<int64_t> pih(0); // posiciones de apuntadores a hijos en os

        int64_t prini = os.tellp(); //podría ser -1 
        //clog << "OJO mezclaRec prini=" << prini << ", is1.tellg()=" << is1.tellg() << ", is2.tellg()=" << is2.tellg() << endl;
        //clog << "peek1=" << is1.peek() << endl;
        //clog << "peek2=" << is2.peek() << endl;
        for (cad1=leeCad(is1), cad2=leeCad(is2), numhermanos = 0;
                        (cad1!="" && (conHermanos1 || n1==0)) ||
                        (cad2!="" && (conHermanos2 || n2==0));
                        numhermanos++) {
                //clog << "OJO mezclaRec prini=" << prini << ", cad1=" << cad1 << ", cad2= "<< cad2 << endl;
                dhijo1.push_back(-1);
                dhijo2.push_back(-1);
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
                 * cad2 es cadena en nodo de is2
                 * cursor de is1 está a continuación de cad1 
                 * 	(sobre inicio de posiciones)
                 * cursor de is2 está a continuación de cad2 
                 * conHermanos1 es true si debe continuar con hermanos en is1
                 * conHermanos2 es true si debe seguir con hermanos en is2
                 */
                string c = "";
                if (cad1 != "" && cad2 != "") {
                        c = prefijo_comun_mas_largo(cad1, cad2);
                }
                //clog << " prefijo c="<<c<<endl;
                if (c == "") { // No hay prefijo comun
                        resto1[numhermanos] = "";
                        resto2[numhermanos] = "";
                        if (cad2 == "" || (cad1 != "" && cad1 < cad2)) {
                                // e.g AMOR y BUENO  o
                                //     ""   y BUENO
                                //clog<< "se copia nodo de is1" <<endl;
                                phermano1 = leeNDesp(is1);
                                dhijo1[numhermanos] = leeNDesp(is1);
                                dhijo2[numhermanos] = -1;
                                set<Pos> *cpos = leePos(is1, renum1);
                                opera[numhermanos] = O_COPIA1;
                                pih[numhermanos] =
                                        escribeNodo(os, cad1, cpos, 0, arbolHuffman);
                                delete cpos;
                                cpos = NULL;
                                cad1 = leeCad(is1);
                                n1++;
                                // cad2 quieto
                                // en la recursión hijos de cad1
                                //quedamos en el siguiente hermano mayor en is1
                        } else { /*cad2!="" && (cad1=="" || cad1 >= cad2) */
                                // e.g BUENO y AMOR o
                                //      ""   y AMOR
                                ASSERT(cad1 == "" || (cad1 > cad2));
                                //clog << "se copia nodo de is2" <<endl;
                                dhijo1[numhermanos] = -1;
                                phermano2 = leeNDesp(is2);
                                dhijo2[numhermanos] = leeNDesp(is2);
                                //clog << "phermano2=" << phermano2 << ", phijo=" << dhijo2[numhermanos] << endl;
                                set<Pos> *cpos = leePos(is2, renum2);
                                opera[numhermanos] = O_COPIA2;
                                pih[numhermanos] =
                                        escribeNodo(os, cad2, cpos, 0, arbolHuffman);
                                delete cpos;
                                cpos = NULL;
                                cad2 = leeCad(is2);
                                n2++;
                        }
                } else if (cad1 == cad2) { // Hay prefijo c != "" && cad1!="" && cad2!=""
                        phermano1 = leeNDesp(is1);
                        phermano2 = leeNDesp(is2);
                        dhijo1[numhermanos] = leeNDesp(is1);
                        dhijo2[numhermanos] = leeNDesp(is2);
                        //clog << "iguales" <<endl;
                        set<Pos> cpos;
                        insert_iterator<set<Pos> >
                        cpos_ins(cpos, cpos.begin());
                        set<Pos> *cpos1 = leePos(is1, renum1);
                        set<Pos> *cpos2 = leePos(is2, renum2);

                        set_union(cpos1->begin(), cpos1->end(),
                                  cpos2->begin(), cpos2->end(),
                                  cpos_ins);
                        pih[numhermanos] = escribeNodo(os, cad2, &cpos, 0, arbolHuffman);
                        delete cpos1;
                        cpos1=NULL;
                        delete cpos2;
                        cpos2=NULL;
                        //En hijos NodoTrieS *m=mezcla(a1->hijo_menor, a2->hijo_menor);
                        resto1[numhermanos] = "";
                        resto2[numhermanos] = "";
                        opera[numhermanos] = O_MEZCLA_H1H2;
                        cad1 = leeCad(is1);
                        n1++;
                        cad2 = leeCad(is2);
                        n2++;
                } else { // hay prefijo c != "" && cad1!="" && cad2!="" && cad1!=cad2
                        string r1 = cad1.substr(c.size());
                        string r2 = cad2.substr(c.size());
                        resto1[numhermanos] = r1;
                        resto2[numhermanos] = r2;
                        ASSERT(r1 != "" || r2 != "");
                        //clog << "hay posfijo r1="<<r1<<" r2="<<r2<<endl;
                        if (r1=="") {
                                // e.g BUENO BUENOS
                                ASSERT(r2 != "");
                                //clog << "r1 vacio"<<endl;
                                // debe mezclar hijo de is1 con un sólo
                                // nodo de is2 pero is2 comenzando en el
                                // posfijo r2

                                phermano1 = leeNDesp(is1);
                                phermano2 = leeNDesp(is2);
                                dhijo1[numhermanos] = leeNDesp(is1);
                                dhijo2[numhermanos] = (uint32_t)is2.tellg() -
                                                      MAXLNUMERO - 1 - (uint32_t)r2.size();
                                /*				(void)leeNDesp(is2);
                                				saltaPos(is2); */
                                is2.seekg(phermano2); // Salta al sig en is2
                                set<Pos> *cpos1 = leePos(is1, renum1);
                                //clog << "OJO is2.tellg()="<< is2.tellg() << endl;
                                pih[numhermanos] = escribeNodo(os, c, cpos1, 0, arbolHuffman);
                                opera[numhermanos] = O_MEZCLA_H1;
                                delete cpos1;
                                cpos1=NULL;
                        } else if (r2 == "") {
                                // e.g BUENOS BUENO
                                ASSERT(r1 != "");
                                //clog << "r2 vacio"<<endl;
                                //
                                phermano1 = leeNDesp(is1);
                                dhijo1[numhermanos] = (uint32_t)is1.tellg() -
                                                      MAXLNUMERO - 1 - (uint32_t)r1.size();
                                phermano2 = leeNDesp(is2);
                                dhijo2[numhermanos] = leeNDesp(is2);
                                is1.seekg(phermano1);
                                /*saltaPos(is1);
                                leeNDesp(is1); */
                                set<Pos> *cpos2 = leePos(is2, renum2);
                                //clog << "OJO en nodo dhijo2[numhermanos]=" << dhijo2[numhermanos] << endl;
                                pih[numhermanos] = escribeNodo(os, c, cpos2, 0, arbolHuffman);
                                // Recursion NodoTrieS *m=mezcla(n1, a2->hijo_menor);
                                delete cpos2;
                                cpos2=NULL;
                                opera[numhermanos] = O_MEZCLA_H2;
                        } else if (r1 < r2) {
                                // e.g BUENA BUENO
                                //clog << "r1<r2"<<endl;

                                phermano1 = leeNDesp(is1);
                                //clog << "phermano1=" << phermano1 <<endl;
                                phermano2 = leeNDesp(is2);
                                //clog << "phermano2=" << phermano2 <<endl;
                                pih[numhermanos] = escribeNodo(os, c, NULL, 0, arbolHuffman);
                                //clog << "pih=" << pih[numhermanos] <<endl;
                                dhijo1[numhermanos] = (uint32_t)is1.tellg() -
                                                      MAXLNUMERO - 1 - 
                                                      (uint32_t)r1.size();
                                //clog << "dhijo1=" << dhijo1[numhermanos] <<endl;
                                dhijo2[numhermanos]=(uint32_t)is2.tellg() -
                                                    MAXLNUMERO - 1 - 
                                                    (uint32_t)r2.size();
                                //clog << "dhijo2=" << dhijo2[numhermanos] <<endl;
                                is1.seekg(phermano1);
                                is2.seekg(phermano2);
                                opera[numhermanos] = O_COPIA_PRIMERO_1;
                                //a1->cad=r1;
                                //a2->cad=r2;
                                // El hijo de este nodo debe ser
                                // a1 con hermano mayor a2
                                //*r=new NodoTrieS(c, n1, NULL, set<Pos>());
                        } else { /* r1 >= r2 */
                                // e.g BUENO BUENA
                                //clog << "r2 <= r1"<<endl;
                                phermano1 = leeNDesp(is1);
                                phermano2 = leeNDesp(is2);
                                pih[numhermanos] = escribeNodo(os, c, NULL, 0, arbolHuffman);
                                dhijo1[numhermanos] = (uint32_t)is1.tellg() -
                                                      MAXLNUMERO - 1 - 
                                                      (uint32_t)r1.size();
                                dhijo2[numhermanos] = (uint32_t)is2.tellg() -
                                                      MAXLNUMERO - 1 - 
                                                      (uint32_t)r2.size();
                                //saltaPos(is1); leeNDesp(is1);
                                is1.seekg(phermano1);
                                //saltaPos(is2); leeNDesp(is2);
                                is2.seekg(phermano2);
                                opera[numhermanos] = O_COPIA_PRIMERO_2;
                        }
                        cad1=leeCad(is1);
                        n1++;
                        //clog << "lee cad1=" << cad1 << endl;
                        cad2=leeCad(is2);
                        n2++;
                        //clog << "lee cad2=" << cad2 << endl;
                }
        }
        os << endl;
        //clog << "Termina hermanos, pasa a hijos" << endl;

        // Desplazamientos en os
        uint32_t pini, pfin;
        uint32_t hijo1=0, hijo2=0;
        uint32_t ph1, ph2, pnh1, pnh2;
        for (uint32_t  n = 0; n < numhermanos; n++) {
                pini = 0; // Posición donde queda hijo en os
                switch (opera[n]) {
                case O_COPIA1:
                        //clog << "COPIA1" <<endl;
                        if (dhijo1[n] > 0) {
                                is1.seekg(dhijo1[n]);
                                pini = escribeCopiaSubarbol(os, is1,
                                                            true, renum1);
                        }
                        break;

                case O_COPIA2:
                        //clog << "COPIA2" <<endl;
                        if (dhijo2[n] > 0) {
                                //clog << "dhijo2[" << n << "]=" <<
                                //dhijo2[n] << endl;
                                is2.seekg(dhijo2[n]);
                                pini = escribeCopiaSubarbol(os, is2,
                                                            true, renum2);
                        }
                        break;

                case O_MEZCLA_H1H2:
                        //clog << "MEZCLA_H1H2" <<endl;
                        if (depuraos!=NULL) {
                                //clog << depuraos->str() << endl;
                        }


                        if (dhijo1[n] > 0 && dhijo2[n] > 0) {
                                is1.seekg(dhijo1[n]);
                                is2.seekg(dhijo2[n]);
                                pini = mezclaRec(is1, is2, os,
                                                 true, true, renum1, renum2) ;
                        } else if (dhijo1[n] > 0) {
                                ASSERT(dhijo2[n] == 0);
                                is1.seekg(dhijo1[n]);
                                pini = escribeCopiaSubarbol(os, is1,
                                                            true, renum1);
                        } else if (dhijo2[n] > 0) {
                                //clog << "OJO dhijo2[n]=" <<dhijo2[n]<< endl;
                                ASSERT(dhijo1[n] == 0);
                                is2.seekg(dhijo2[n]);
                                //clog << "OJO is2.peek=" << (char)is2.peek()<< endl;
                                pini = escribeCopiaSubarbol(os, is2,
                                                            true, renum2);
                                //clog << "OJO pini=" << pini << endl;
                        }
                        break;
                case O_MEZCLA_H1:
                        //clog << "MEZCLA_H1" <<endl;
                        ASSERT(dhijo2[n] != 0);

                        //clog << "OJO dhijo2[n]=" << dhijo2[n] << endl;
                        is2.seekg(dhijo2[n]);
                        if (dhijo1[n] > 0) {
                                is1.seekg(dhijo1[n]);
                                pini = mezclaRec(is1, is2, os,
                                                 true, false, renum1, renum2) ;
                        } else {
                                pini = escribeCopiaSubarbol(os, is2,
                                                            false, renum2);
                        }
                        break;

                case O_MEZCLA_H2:
                        //clog << "MEZCLA_H2" <<endl;
                        ASSERT(dhijo1[n] != 0);

                        is1.seekg(dhijo1[n]);
                        //clog << "dhijo2[n]=" << dhijo2[n] <<  endl;
                        if (dhijo2[n] > 0) {
                                is2.seekg(dhijo2[n]);
                                pini = mezclaRec(is1, is2, os,
                                                 false, true, renum1, renum2) ;
                        } else {
                                pini = escribeCopiaSubarbol(os, is1,
                                                            false, renum1);
                        }
                        break;

                case O_COPIA_PRIMERO_1:
                        //clog << "COPIA_PRIMERO_1" <<endl;
                        ASSERT(dhijo1[n] != 0);
                        ASSERT(dhijo2[n] != 0);

                        is1.seekg(dhijo1[n]);
                        is2.seekg(dhijo2[n]);
                        pini = os.tellp();  // no puede ser -1

                        // pos no es NULL en los siguientes,
                        // más bien copiar de is1 e is2 e hijos
                        // después.
                        pnh1=escribeCopiaNodo(os, is1, hijo1, renum1);
                        pnh2=escribeCopiaNodo(os, is2, hijo2, renum2);
                        os << endl;
                        ph1=os.tellp(); // no puede ser -1
                        if (hijo1 > 0) {
                                is1.seekg(hijo1);
                                (void)escribeCopiaSubarbol(os, is1,
                                                           true, renum1);
                        }
                        ph2=os.tellp(); // no puede ser -1
                        if (hijo2 > 0) {
                                is2.seekg(hijo2);
                                escribeCopiaSubarbol(os, is2,
                                                     true, renum2);
                        }
                        pfin = os.tellp(); // no puede ser -1
                        if (hijo1 > 0) {
                                os.seekp(pnh1);
                                escribeNDesp(os, ph1);
                        }
                        if (hijo2 > 0) {
                                os.seekp(pnh2);
                                escribeNDesp(os, ph2);
                        }
                        os.seekp(pfin);

                        break;

                case O_COPIA_PRIMERO_2:
                        //clog << "COPIA_PRIMERO_2" <<endl;
                        ASSERT(dhijo1[n] != 0);
                        ASSERT(dhijo2[n] != 0);

                        is1.seekg(dhijo1[n]);
                        is2.seekg(dhijo2[n]);
                        pini = os.tellp(); // no -1

                        pnh2=escribeCopiaNodo(os, is2, hijo2, renum2);
                        pnh1=escribeCopiaNodo(os, is1, hijo1, renum1);
                        os << endl;
                        ph2=os.tellp(); // no -1
                        if (hijo2 > 0) {
                                is2.seekg(hijo2);
                                escribeCopiaSubarbol(os, is2,
                                                     true, renum2);
                        }
                        ph1=os.tellp(); // no -1
                        if (hijo1 > 0) {
                                is1.seekg(hijo1);
                                escribeCopiaSubarbol(os, is1,
                                                     true, renum1);
                        }
                        pfin = os.tellp(); 
                        if (hijo2 > 0) {
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
                pfin = os.tellp(); // no -1
                //clog << "OJO pfin=" << pfin << endl;
                //clog << "OJO n=" << n << ", pih[n]=" << pih[n]<< endl;
                os.seekp(pih[n]);
                escribeNDesp(os, pini);
                os.seekp(pfin);
        }
        //clog << "Termina hijos" << endl;
        return prini;
}



/** Busca una palabra en un trieS mantenido en un steam en
 * formato extra-simple
 */
set<Pos> *
buscaPlanoStream(std::istream &is, string pal) throw(string)
{
        string cad;
        set<Pos> *cpos;

        cad = leeCad(is);
        //clog<<"OJO cad = "<<cad <<endl;
        if (cad.size() == pal.size() && cad.compare(pal) == 0) {
                //clog<<"OJO encontrado";
                (void)leeNDesp(is);
                (void)leeNDesp(is);
                cpos = leePos(is, NULL);
                return cpos;
        } else if (cad.size() > 0 &&
                        cad.size() < pal.size() &&
                        cad.compare(pal.substr(0, cad.size())) == 0) {
                leeNDesp(is); // hermano
                uint32_t hijo = leeNDesp(is);
                /*hijo = 0;
                saltaPos(is); */
                //clog<<"OJO por buscar entre hijos a partir de " << hijo << endl;
                //is>>hijo;
                if (hijo > 0) {
                        is.seekg(hijo);
                        return buscaPlanoStream(is, pal.substr(cad.size()));
                }
        } else if (cad.size() > 0 && cad.compare(pal) < 0) {
                //clog<<"OJO pasar a hermano mayor"<<endl;
                // salta cpos e hijo
                uint32_t hermano = leeNDesp(is);
                is.seekg(hermano);
                //saltaPos(is);

                return buscaPlanoStream(is, pal);
        }
        //clog<<"OJO no se encontró" << endl;
        return NULL;
}



void verificaIndice(istream &is)
{
        char rec[MAXLURL];
        is.seekg(0);
        is.getline(rec, MAXLURL);
        if (MARCAIND.compare(rec) != 0) {
                throw errorFormato(is, string("Se esperaba `") + MARCAIND + string("' pero se encontró ") + rec);
        }
}

set<Pos> *
buscaPlano(const char *na, const char *nrel, string pal, vector<Doc> &docs)
{
        set<Pos> *r;
        string enc;


        fstream is(na, ios_base::in);
        verificaIndice(is);
        try {
                r = buscaPlanoStream(is, pal);
                //clog << "OJO buscado r=" << r << endl;
                //throw errorFormato(is, "OJO paso 2");
        } catch (string m) {
                throw errorFormato(is, m);
        }
        is.close();

        leeRelacion(nrel, docs);
        /*	for (int i=0; i<idocs.size(); i++) {
        		clog << i << " " << idocs[i] << endl;
        	}  */

        return r;
}


uint32_t
leeRelacion(const char *nrel,  vector<Doc> &docs) throw(string)
{
        ASSERT(nrel != NULL);
        ASSERT(nrel[0] != '\0');

        string s = "", m = "";
        char rec[MAXLURL];

        fstream is(nrel, ios_base::in);
        if (!is) {
                stringstream ss;
                ss << "No pudo abrir relación " << nrel;
                throw ss.str();
        }
        is.seekg(0);
        is.getline(rec, 100);
        if (rec != MARCAREL) {
                throw errorFormato(is,
                                   string("Se esperaba marca de relación ") + MARCAREL + " pero se encontró " + rec);
        }
        docs = leeDocs(is);
        is.close();
        //clog << "OJO docs.size()=" << docs.size() << endl;
        return docs.size();
}


void escribeRelacion(const char *nrel, vector<Doc> &docs, 
                vector<int64_t> *reord)
{
        ASSERT(nrel != NULL);
        ASSERT(strlen(nrel) > 0 && strlen(nrel) < MAXLURL);
        ASSERT(reord == NULL || reord->size() == docs.size());

        fstream os(nrel, ios_base::out);
        os << MARCAREL << endl ;
        escribeDocs(os, docs, reord);
        os.close();
}


/**
 * Calcula condensado de un conjunto de posiciones
 * @param cpos conjunto de posiciones
 * @param md   estado del condensado que es modificado 
 *
 * @return void 
 */
void condensadoPos(set<Pos> *cpos, hash_state *md, bool imprime = false)
{
        ASSERT(cpos != NULL);
        stringstream ss;
        set<Pos>::
        set<Pos>::iterator i;
        for (i = cpos->begin(); i != cpos->end(); i++) {
                stringstream c;
                c << i->numd << "," << i->numb;
                sha256_process(md, (const unsigned char*)c.str().c_str(), 
                                c.str().size());
                if (imprime) {
                        cout << " " << c.str();
                }
        }
}


/**
 * Calcula condensando de un flujo de índice teniendo en cuenta palabras
 * completas y las posiciones donde aparece.
 *
 * @param is       archivo que se lee ubicado al comienzo de un nodo
 * @param md       estado del condensado que es modificado 
 * @param precad   prefijo para la cadena del nodo actual
 * 
 * @return void 
 */
void condensadoFlujo(istream &is,  hash_state *md, string precad, bool imprime)
{
 //       cerr << "OJO condensado(is, md, " << precad << ", " << imprime << ")" << endl;
        string cad;
        long hermano, hijo;
        set<Pos> *cpos = NULL;

        string lc = leeCad(is);
        if (lc.size() > 0) {
                cad = precad + lc;
                hermano = leeNDesp(is);
                hijo = leeNDesp(is);
                cpos = leePos(is);
                if (cpos != NULL && cpos->size() > 0) {
                        sha256_process(md, (const unsigned char *)cad.c_str(), 
                                        cad.size());
                        if (imprime) {
                                cout << cad;
                        }
                        condensadoPos(cpos, md, imprime);
                        delete cpos;
                        cpos = NULL;
                        if (imprime) {
                                cout << endl;
                        }
                }

                if (hijo > 0) {
                        is.seekg(hijo);
                        condensadoFlujo(is, md, cad, imprime);
                }
                if (hermano > 0) {
                        is.seekg(hermano);
                        condensadoFlujo(is, md, precad, imprime);
                }
        }
}


/**
 * Calcula condensando de un índice teniendo en cuenta palabras
 * completas y las posiciones donde aparece.
 *
 * @param indice   Índice
 * @param depura   Si debe presentar datos a los que calcula SHA256
 * 
 * @return string 32 bytes hexadececimales
 */
string condensado(char *indice, bool depura)
{
        ASSERT(indice != NULL);

        stringstream ss;

        char relacion[MAXLURL];
        verificaNombre(indice, relacion);

        fstream is(indice, ios_base::in);
        verificaIndice(is);

        hash_state md;
        unsigned char hash[33];
        sha256_init(&md);
        condensadoFlujo(is, &md, "", depura);
        sha256_done(&md, hash);
        is.close();

        ss << hexaDeHash(hash, 32) << endl;
        return ss.str();
}

