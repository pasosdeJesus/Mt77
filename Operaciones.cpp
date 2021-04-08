// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file Operaciones.cpp
 * Realiza operaciones en un índice.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: Operaciones.cpp,v 1.9 2010/01/18 16:12:50 vtamara Exp $
 */

#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <fstream>
#include <istream>

#include <sys/stat.h>
#include <time.h>

#include "RamDisco.hpp"
#include "Operaciones.hpp"
using namespace std;


string prefijoASitio(const char *pref)
{
        ASSERT(pref != NULL);
        //cout << "OJO prefijoASitio(" << pref << ")" << endl;
        string o = "";
        if (strlen(pref)>7 && (strncmp(pref, "https://", 8) == 0
                               || strncmp(pref, "http://", 7) == 0)) {
                uint32_t i = strncmp(pref, "https://", 8) == 0 ? 8 : 7;
                for (; pref[i] != '\0' && i < (MAXCAD - 10) &&
                                //pref[i] != '.' &&
                                pref[i] != '/' && pref[i] != ':'; i++) {
                        o += pref[i];
                        //cout << "OJO " << i << o << endl;
                }

        }
        if (o == "") {
                o = "localhost";
        }
        return o;
}

string determinaFormato(string narch)
{
        string f = "";
        if (narch.substr(narch.length() - 4) == ".txt") {
                f = "texto";
        } else if (narch.substr(narch.length() - 4) == ".odt") {
                f = "odf";
        } else if (narch.substr(narch.length() - 6) == ".xrlat") {
                f = "relato";
        } else if (narch.substr(narch.length() - 4) == ".xml") {
                f = "xml";
        } else if (narch.substr(narch.length() - 4) == ".ogv") { // Video Theora
                f = "ogv";
        } else if (narch.substr(narch.length() - 4) == ".flv") { 
                f = "flv";
        } else if (narch.substr(narch.length() - 4) == ".ogg") { // Audio OGG
                f = "ogg";
        } else if (narch.substr(narch.length() - 4) == ".mp3") { 
                f = "mp3";
        } else if (narch.substr(narch.length() - 4) == ".jpg") {
                f = "jpg";
        } else if (narch.substr(narch.length() - 4) == ".png") {
                f = "png";
        } else if (narch.substr(narch.length() - 4) == ".gif") {
                f = "gif";
        } else if (narch.substr(narch.length() - 4) == ".pdf") {
                f = "pdf";
        } else if (narch.substr(narch.length() - 5) == ".html" ||
                        narch.substr(narch.length() - 4) == ".htm") {
                f = "html";
        } else {
                f = "otro";
        }

        return f;
}




void
muestraStream(std::istream &is, string pre, Arbol_huffman &arbolHuffman)
{
        string cad, npre;
        set<Pos> *cpos;
        uint32_t hijo, her;

        // std::cout << arbolHuffman.toString() << std::endl;

        cad = leeCad(is, arbolHuffman);
        if (cad != "")
        {
                ASSERT(!arbolHuffman.vacio());
                her = lee128b(is);
                hijo = lee128b(is);
                cpos = leePos(is, NULL);
                if (cpos->size()>0) {
                        cout << pre << cad << " " << cpos->size() << endl;
                }
                uint32_t pac = is.tellg();
                if (hijo > 0) {
                        is.seekg(hijo);
                        npre = pre;
                        npre += cad;
                        muestraStream(is, npre, arbolHuffman);
                }
                is.seekg(pac);
                muestraStream(is, pre, arbolHuffman);
        }
}


/* Presenta listado de palabras del Trie almacenado en el indice noma
 *
 * @param noma Nombre del indice
 * @param nrel Nombre de la relación de archivos indexados en indice noma (si
 *   es vacio no se imprime)
 * @param arbolHuffman Arbol con tendencia para compresión
 */
void listaPalabras(char *noma, char *nrel, Arbol_huffman &arbolHuffman) throw(string)
{
        ASSERT(noma != NULL);
        ASSERT(nrel != NULL);


        // std::cout << arbolHuffman.toString() << std::endl;


        // std::cout << arbolHuffman.toString() << std::endl;

        vector<Doc> idocs;
        if (strlen(nrel) > 0) {
                leeRelacion(nrel, idocs);

                for (uint32_t i = 0; i< idocs.size(); i++) {
                        cout << (i+1) << " " << idocs[i].URL << endl;
                }
        }

        fstream is(noma, ios_base::in);
        verificaIndice(is);
        muestraStream(is, "", arbolHuffman);
        is.close();
}


void
eliminaDoc(char *noma, char *nomind, uint32_t nd, Arbol_huffman arbolHuffman) throw(string)
{
        ASSERT(nd > 0);
        char nrel1[MAXLURL];
        char nrel[MAXLURL];
        verificaNombre(noma, nrel);
        verificaNombre(nomind, nrel1);
        if (nd <= 0) {
                stringstream ss;
                ss << "n debe ser mayor o igual a 1";
                throw ss.str();
        }
        vector<Doc> docs;
        vector<Doc> sdocs;
        uint32_t td = leeRelacion(nrel1, docs);
        if (nd > td) {
                stringstream ss;
                ss << "n debe ser menor o igual a " << td;
                throw ss.str();
        }
        uint32_t i;
        vector<int64_t> renum;
        for(i = 0; i < nd - 1; i++) {
                renum.push_back(i);
                sdocs.push_back(docs[i]);
        }
        /* 0  1  2  3 -elim tercero -> 0 1 3
           0  1 -1  2 */
        renum.push_back(-1);
        for(i = nd; i < td; i++) {
                renum.push_back(i - 1);
                sdocs.push_back(docs[i]);
        }
        fstream os(noma, ios_base::out);
        fstream is(nomind, ios_base::in);

        verificaIndice(is);
        is.clear();

        os << MARCAIND << endl;
        escribeCopiaSubarbol(os, is, true, arbolHuffman, &renum);
        is.close();
        os.close();

        // Escribe relación sin el borrado
        escribeRelacion(nrel, sdocs);
}


void
calcRenum(uint32_t td1, uint32_t td2, uint32_t nd, vector<int64_t> *reord,
          vector<int64_t> *renum1,
          vector<int64_t> &renum2)
{
        //clog << "OJO calcRenum(td1=" << td1 << ", td2=" << td2 << ", nd=" << nd << ", reord=" << reord << ", renum1=" << renum1 << ", renum2.size=" << renum2.size() << endl;
        if (reord != NULL) {
                //clog << "OJO reord.size() =" << reord->size() << endl;
        }
        if (renum1 != NULL) {
                //clog << "OJO renum1.size() =" << renum1->size() << endl;
        }

        if (nd > 0 && nd <= td1) {

                // renum2 0 -> 2, 1 -> 3 2 -> 4 y docs1 quedará a b c d e

                // queremos renum2 en
                // 0 -> 1, 1 -> 2, 2 -> 3  i->i+(nd-1)
                // Los indices de docs1 final deben ser
                // 0 -> 0, 1 -> 4, 2 -> 1, 3 -> 2, 4 -> 3
                // que se dejan invertidos en reord
                // 0 de 0, 1 de 2, 2 de 3, 3 de 4, 4 de 1

                uint32_t j;
                for(j = 0; j < renum2.size(); j++) {
                        renum2[j] = (nd - 1) + j;
                }

                // renum1 0 -> 0, 1->3  i->i si i<nd i->i+docs.size() si i>nd
                for(j = 0; j < nd - 1; j++) {
                        renum1->push_back(j);
                }
                for(j = nd - 1 ; j < td1; j++) {
                        renum1->push_back(j + td2);
                }

                for(j = 0; j < nd - 1; j++) {
                        reord->push_back(j);
                }
                for(j = 0; j < renum2.size(); j++) {
                        reord->push_back(td1 + j);
                }
                for(j = nd - 1 ; j < td1; j++) {
                        reord->push_back(j);
                }
        }
}


void
mezclaDosDisco(const char *indsal, const char *ind1,
               const char *ind2,
               Arbol_huffman &arbolHuffman1,
               Arbol_huffman &arbolHuffman2,
               Arbol_huffman &arbolHuffmanSalida,
               uint32_t nd)  throw(string)
{
        ASSERT(nd >= 0);
        char nrel[MAXLURL], nrel1[MAXLURL], nrel2[MAXLURL];

        //clog << "OJO mezclaDosDisco(" << indsal << ", " << ind1 << ", " << ind2 << ", " << nd << ")" << endl;
        verificaNombre(indsal, nrel);
        verificaNombre(ind1, nrel1);
        verificaNombre(ind2, nrel2);

        vector<Doc> docs;
        vector<Doc> sdocs;
        // Leer y mezclar relación de documentos 1 y 2
        vector<Doc> docs1;
        vector<Doc> docs2;
        uint32_t td1;
        try {
                td1 = leeRelacion(nrel1, docs1);
        //clog << "OJO td1= " << td1 << ", docs1.size()=" << docs1.size() << endl;
        } catch (char *s) {
                throw s;
        }
        if (nd > td1 + 1) {
                stringstream ss;
                ss << "n debe ser menor o igual a " <<
                        td1 + 1 << endl;
                throw ss.str();
        }
        leeRelacion(nrel2, docs2);
        //clog << "OJO docs2.size= " << docs2.size() << endl;

        vector<int64_t> *reord = NULL;
        vector<int64_t> *renum1 = NULL;
        vector<int64_t> renum2 = mezclaDocs(docs2, docs1);
        //clog << "OJO tras mezclaDocs renum2.size= " << renum2.size() << endl;
        if (nd > 0 && nd <= td1) {
                renum1 = new vector<int64_t>();
                reord = new vector<int64_t>();
        }
        calcRenum(td1, docs2.size(), nd, reord, renum1, renum2);
        //clog << "OJO después de calcRenum" << endl;
        if (reord != NULL) {
                //clog << "OJO reord.size=" << reord->size() << ", renum1.size=" << renum1->size() << ", renum2.size=" << renum2.size() << endl;
        }
        fstream os(indsal, ios_base::out);
        fstream is1(ind1, ios_base::in);
        fstream is2(ind2, ios_base::in);

        verificaIndice(is1);
        //clog << "OJO después de verificaIndice 1" << endl;
        is1.clear();
        verificaIndice(is2);
        //clog << "OJO después de verificaIndice 2" << endl;
        is2.clear();

        //clog << "OJO después de verificar" << endl;
        os << MARCAIND << endl;
        mezclaRec(is1, is2, os, true, true, renum1, &renum2, arbolHuffman1, arbolHuffman2, arbolHuffmanSalida);
        //clog << "OJO después de mezclaRec" << endl;
        is1.close();
        is2.close();
        os.close();

        // Escribe relación de documentos de mezcla
        escribeRelacion(nrel, docs1, reord);
}

/**
 * La funcionalidad de este ahora está incluida en indexador
 
void
agregaDoc(const char *indsal, const char *inden, const char *nom,
          bool metainformacion, const char *pref, long nd)
{
        char nrel[MAXLURL], nrel1[MAXLURL];
        //clog << "agregaDoc(indsal=" << indsal << ", inden=" << inden << ", nom=" << nom << ", metainformacion=" << metainformacion << ", pref=" << pref << ", nd=" << nd << endl;

        verificaNombre(indsal, nrel);
        verificaNombre(inden, nrel1);

        // Leer y mezclar relación de documentos
        vector<Doc> docs1;
        vector<Doc> docs2;
        long td1 = 0;
        td1 = leeRelacion(nrel1, docs1);
        //clog << "OJO docs1.size()=" << docs1.size() << endl;
        if (nd > td1 + 1) {
                stringstream ss;
                ss << "n debe ser menor o igual a " <<
                td1 + 1 << endl;
                throw ss.str();
        }

        Doc d(nom, "", "1960-01-01");
        docs2.push_back(d);

        vector<long> *reord = NULL;
        vector<long> *renum1 = NULL;
        vector<long> renum2 =
        mezclaDocs(docs2, docs1);
        //clog << "OJO tras mezclaDocs docs2.size=" << docs2.size() << ", docs1.size=" << docs1.size() << ", renum2.size=" << renum2.size() << endl;
        if (nd > 0 && nd <= td1) {

                renum1 = new vector<long>();
                reord = new vector<long>();
        }
        calcRenum(td1, docs2.size(), nd, reord, renum1, renum2);
        //clog << "OJO tras calcRenum: ";
        string sep="";
        for (uint32_t i=0; i < renum2.size(); i++) {
                //clog << sep << "renum2[" << i << "]=" << renum2[i];
                sep=", ";
        }
        //clog << endl;

        NodoTrieS t;
        string tipo, formato;
        indexa(docs1[docs1.size() - 1], 1, metainformacion, pref,
                        t, tipo, formato);
        //clog << "OJO tras indexa tipo=" << tipo << ", formato=" << formato << endl;

        fstream os(indsal, ios_base::out);
        fstream is1(inden, ios_base::in);

        verificaIndice(is1);
        is1.clear();

        os << MARCAIND << endl;
        mezclaDiscoRam(is1, &t, 0, os, true, true, renum1, &renum2);
        is1.close();
        os.close();

        // Escribe relación de documentos de mezcla
        escribeRelacion(nrel, docs1, reord);
        //clog << "OJO tras escribeRelacion" << endl;
}

*/

void
subindice(const char *ind, const char *salida, uint32_t nd, Arbol_huffman &arbolHuffman)
{
        char nrel[MAXLURL];
        char nrelsal[MAXLURL];
        //clog << "subindice(ind=" << ind << ", nd=" << nd << ")" << endl;

        verificaNombre(ind, nrel);
        verificaNombre(salida, nrelsal);

        // Leer y mezclar relación de documentos
        vector<Doc> docs;
        vector<Doc> docsal;
        uint32_t td1 = 0;
        td1 = leeRelacion(nrel, docs);
        //clog << "OJO docs1.size()=" << docs1.size() << endl;
        if (nd > td1 + 1) {
                stringstream ss;
                ss << "n debe ser menor o igual a " <<
                td1 + 1 << endl;
                throw ss.str();
        }
        docsal.push_back(docs[nd - 1]);

        NodoTrieS t;
        string tipo, formato;
        fstream is1(ind, ios_base::in);

        verificaIndice(is1);
        is1.clear();

        subindiceDiscoaRAM(is1, &t, nd);
        is1.close();

        escribePlano(t, docsal, salida, nrelsal, arbolHuffman);
}
