// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file buscador.cpp
* Carga un índice con un trie-especial y realiza una búsqueda sobre el mismo.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: buscador.cpp,v 1.14 2011/10/14 11:26:10 vtamara Exp $
 */


#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <set>
#include <fstream>
#include <istream>
#include <algorithm>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdint.h>


using namespace std;

#include "Pos.hpp"
#include "TrieSDisco.hpp"


/**
 * Recibe consulta, la analiza y retorna un conjunto de cadenas por buscar con la función realizaBusqueda
 */
set<string> analizaConsulta(char *consulta)
{
        string pnor;
        string petiqueta;
        set<string> ccad;
        if (consulta[0] == '\0') {
                return ccad;
        }
        int estado = 0;
        /** 0 saltando blancos
         * 1 leyendo palabra
         * 2 leyendo palabra entre comillas
         * 3 leyendo etiqueta después de :
         */
        char pal[MAXCAD];
        int npal = 0;  // Número de caracteres escritos en pal
        uint32_t p = 0;
        for (p = 0; p < strlen(consulta); p++) {
                if (estado == 0) {
                        if (isalnum(consulta[p])) {
                                estado = 1;
                                npal = 0;
                                pal[0] = consulta[p];
                        } else if (consulta[p] == '"') {
                                estado = 2;
                                npal = -1;
                                //clog << "OJO De 0 a 2" << endl;
                        }
                } else if (estado == 1) {
                        if (isspace(consulta[p])) {
                                estado = 0;
                                npal++;
                                pal[npal] = '\0';
                                pnor = normaliza(string(pal));
                                if (pnor != "") {
                                        ccad.insert(pnor);
                                }
                        } else if (consulta[p] == ':') {
                                estado = 3;
                                npal++;
                                pal[npal] = '\0';
                                petiqueta = normaliza(string(pal));
                                npal = 0;
                        } else if (npal <(int)MAXCAD-2) {
                                npal++;
                                pal[npal] = consulta[p];
                        }
                } else if (estado == 2) {
                        //clog << "OJO en 2" << endl;
                        if (consulta[p] == '"') {
                                estado = 0;
                                npal++;
                                pal[npal] = '\0';
                                //clog << "OJO fin de cadena npal=" << npal << ", pal='" << pal << "'" << endl;
                                vector<string> partes = estalla(" ", pal);
                                string sep = "", cres = "";
                                for(uint32_t np = 0; np < partes.size(); np++) {
                                        pnor = normaliza(string(partes[np]));
                                        //clog << "OJO Tras normalizar partes[" << np << "]=" << partes[np] << " dio pnor=" << pnor << endl;
                                        if (pnor != "") {
                                                cres += sep + pnor;
                                                sep = " ";
                                        }
                                }
                                //clog << "OJO se insertará a la consulta '" << cres << "'" << endl;
                                ccad.insert(cres);
                        } else if (npal < (int)MAXCAD-3  && (!isspace(consulta[p]) ||
                                                             !isspace(consulta[p-1]))) {
                                npal++;
                                pal[npal] = consulta[p];
                                //clog << "aumentando pal a " << npal << ", pal[npal]=" << pal[npal] << endl;
                        } else {
                                //clog << "OJO else" << endl;
                        }
                } else if (estado == 3) {
                        if (isspace(consulta[p])) {
                                estado = 0;
                                pal[npal] = '\0';
                                npal++;
                                pnor = normaliza(string(pal));
                                //cerr << "Tras normalizar pal=" << pal <<
                                //	" dio pnor=" << pnor << endl;
                                if (pnor != "") {
                                        ccad.insert(petiqueta+":"+pnor);
                                }
                        } else if (npal < (int)(MAXCAD-2-petiqueta.length())) {
                                pal[npal] = consulta[p];
                                npal++;
                        }
                }
        }
        if (estado == 1) {
                npal++;
                ASSERT(npal<=(int)MAXCAD-1);
                pal[npal] = '\0';
                pnor = normaliza(string(pal));
                if (pnor != "") {
                        ccad.insert(pnor);
                }
        } else if (estado == 2) {
                npal++;
                ASSERT(npal<=(int)MAXCAD-2);
                pal[npal] = '"';
                npal++;
                pal[npal] = '\0';
                pnor = normaliza(string(pal));
                if (pnor != "") {
                        ccad.insert(pnor);
                }
        } else if (estado == 3) {
                pal[npal] = '\0';
                npal++;
                pnor = normaliza(string(pal));
                if (pnor != "") {
                        //			//clog << "OJO por insertar " << petiqueta << ":" << pnor << endl;
                        ccad.insert(petiqueta+":"+pnor);
                }
        }

        return ccad;
}


int compDoc(Pos p1, Pos p2)
{
        return (p1.numd < p2.numd);
}



/**
 * Realiza búsqueda en el índice especificado.
 * Las cadenas por buscar ya están normalizadas.
 */
set<uint32_t> *realizaBusqueda(char *indice, set<string> &consulta,
                                                 vector<Doc> &docs)
        {
                set<uint32_t> *res = new set<uint32_t>();
                set<Pos> *cp = NULL, *cp2 = NULL;
                char relacion[MAXLURL];

                verificaNombre(indice, relacion);

                int64_t mira = -1;
                set<Pos>::iterator cpi, cpi2;
                set<uint32_t>::iterator resi;
                set<string>::iterator i;

                leeRelacion(relacion, docs);
                for (i = consulta.begin(); i != consulta.end(); i++) {
                        vector<string> partes = estalla(" ", *i);
                        if (mira != -1) {
                                //clog << "OJO al comienzo del ciclo mira docs[" << mira << "].numoc=" << docs[mira].numoc << endl;
                        }
                        for(uint32_t np = 0; np < partes.size(); np++) {
                                try {
                                        //clog << "OJO buscando '" << partes[np] << "'" << endl;
                                        fstream is(indice, ios_base::in);
                                        verificaIndice(is);
                                        try {
                                                cp2 = buscaPlanoStream(is, partes[np]);
                                        } catch (string m) {
                                                throw errorFormato(is, m);
                                        }
                                        is.close();
                                        //cp2 = buscaPlano(indice, relacion, partes[np], docs);
                                } catch (string m) {
                                        cerr << indice << ":" << m << endl;
                                        exit(1);
                                }
                                if (np == 0) {
                                        cp = cp2;
                                } else {
                                        set<Pos> *cp3 = new set<Pos>();

                                        //clog << "OJO intersectando resultados con palabra anterior en cadena" << endl;
                                        cpi = cp->begin();
                                        cpi2 = cp2->begin();
                                        while (cpi != cp->end() && cpi2 != cp2->end()) {
                                                // Hay palabra entre cpi y cpi2 sería mejor criterio
                                                if ((cpi->numd < cpi2->numd)) {
                                                        cpi++;
                                                } else if ((cpi->numd == cpi2->numd) &&
                                                                ((uint32_t)(cpi->numb + partes[np-1].size() + 5) < (uint32_t)(cpi2->numb))) {
                                                        cpi++;
                                                } else if (cpi2->numd < cpi->numd) {
                                                        cpi2++;
                                                } else if ((cpi2->numd == cpi->numd) &&
                                                                ((uint32_t)cpi2->numb < (uint32_t)(cpi->numb + partes[np-1].size() + 1))) {
                                                        cpi2++;
                                                } else {
                                                        ASSERT(cpi2->numd == cpi->numd);
                                                        ASSERT((uint32_t)cpi->numb + partes[np-1].size() + 1 <= (uint32_t)cpi2->numb);
                                                        ASSERT((uint32_t)cpi2->numb <= (uint32_t)cpi->numb + partes[np-1].size() + 5);

                                                        cp3->insert(*cpi2);
                                                        cpi++;
                                                        cpi2++;
                                                }
                                        }
                                        if (cp == cp2) {
                                                delete cp;
                                        } else {
                                                delete cp;
                                                delete cp2;
                                        }
                                        cp = cp3;
                                }
                        }

                        if (cp != NULL) {
                                if (i == consulta.begin()) {
                                        ASSERT(res->size() == 0);
                                        for (cpi = cp->begin(); cpi != cp->end(); cpi++) {
                                                //clog << "cpi->numd=" << cpi->numd << ", ";
                                                res->insert(cpi->numd);
                                                docs[cpi->numd - 1].numoc++;
                                                if (mira == -1) {
                                                        mira = cpi->numd - 1;
                                                        //clog << "activando mira en " << mira << endl;
                                                } else if (mira == cpi->numd - 1) {
                                                        //clog << "mirando que aumentó en primera iteración docs[" << mira << "].numoc a " << docs[mira].numoc << endl;
                                                }
                                        }
                                        //clog << endl;
                                } else {
                                        set<uint32_t> *res2 =
                                                        new set<uint32_t>();
                                        if (mira != -1) {
                                                //clog << "OJO en i que no es inicial mira docs[" << mira << "].numoc=" << docs[mira].numoc << endl;
                                        }

                                        cpi = cp->begin();
                                        resi = res->begin();
                                        while (cpi != cp->end() && resi != res->end()) {
                                                if ((uint32_t)cpi->numd < *resi) {
                                                        cpi++;
                                                } else if (*resi < (uint32_t)cpi->numd) {
                                                        resi++;
                                                } else {
                                                        res2->insert(cpi->numd);
                                                        //clog << "Agregando doc donde también esta siguiente palabra, docs[cpi->numd] es " << docs[cpi->numd - 1].numoc << endl;
                                                        //clog << "*resi=" << *resi << ", cpi->numd=" << (uint32_t)cpi->numd << endl;
                                                        while (cpi != cp->end() && *resi == (uint32_t)cpi->numd) {
                                                                docs[cpi->numd - 1].numoc++;
                                                                cpi++;
                                                        }
                                                        //clog << "*resi = " << *resi << endl;
                                                        if (*resi == (uint32_t)mira) {
                                                                //clog << "OJO mirando que aumento la mirada " << mira << " a " << docs[mira].numoc << endl;
                                                        }
                                                        resi++;
                                                }
                                        }

                                        //clog << "Intersección dió res2.size()=" << res2->size() << endl;
                                        //clog << *res2 << endl;
                                        delete res;
                                        res = res2;
                                }
                                delete cp;
                        } else { // Si un conjunto es vacío la intersección es vacía.
                                delete res;
                                res = new set<uint32_t>();
                                break;
                        }

                }
                //clog << "OJO res->size=" << res->size() << endl;
                //clog << *res << endl;
                return res;
        }


string nombra_resconsulta(set<string> &cons, char *ind)
{
        string r("../indices/consultas/");
        for(char *p = ind; *p != '\0'; p++) {
                if (isalnum(*p)) {
                        r += *p;
                } else {
                        r += '_';
                }
        }
        set<string>::iterator i;
        for(i = cons.begin(); i != cons.end(); i++) {
                r += "_" + (*i);
        }
        return r;
}

bool resconsulta_reciente(string nc)
{
        FILE *f = fopen(nc.c_str(), "r");
        if (f == NULL) {
                return false;
        }
        fclose(f);

        struct tm* rel;
        struct stat atr;
        stat(nc.c_str(), &atr);
        rel = gmtime(&(atr.st_mtime));
        time_t o = mktime(rel);
        time_t actual = time_t();
        double d = difftime(actual, o);

        if (d>(24*60*60)) {  // 1 día
                return false;
        }

        return true;
}


void guarda_resconsulta(string nc, vector<uint32_t> *vpos)
{
        ASSERT(nc != "");
        ASSERT(nc.length() < MAXLURL);
        ASSERT(vpos != NULL);

        fstream os(nc.c_str(), ios_base::out);
        escribeNDesp(os, vpos->size());
        for(uint32_t i = 0; i < vpos->size(); i++) {
                escribeNDesp(os, (*vpos)[i]);
        }
        os.close();
}

vector<uint32_t> *lee_resconsulta(string nc)
{
        ASSERT(nc != "");
        ASSERT(nc.length() < MAXLURL);

        fstream is(nc.c_str(), ios_base::in);
        if (!is) {
                throw std::string("No pudo abrirse" + nc);
        }
        vector<uint32_t> *vpos = new vector<uint32_t>();
        long t = leeNDesp(is);
        for (long i = 0; i < t; i++) {
                long n = leeNDesp(is);
                vpos->push_back(n);
        }
        is.close();

        return vpos;
}

class Esc
{
        public:
                vector<Doc> *docs;
                Esc(vector<Doc> *d) : docs(d)
                {
                        ASSERT(d != NULL);
                }

                bool operator() (uint32_t i, uint32_t j)
                {
                        if (i >= docs->size() || j >= docs->size()) {
                                throw "Problema buscando.  Posiblemente no coinciden índice y relación";
                        }
                        ASSERT(i>=0 && i<docs->size());
                        ASSERT(j>=0 && j<docs->size());
                        if ((*docs)[i].fecha > (*docs)[j].fecha) {
                                return true;
                        } else if ((*docs)[i].fecha == (*docs)[j].fecha) {
                                return (*docs)[i].numoc > (*docs)[j].numoc;
                        }
                        return false;
                }
};

vector<uint32_t> *escalafon(set<uint32_t> *cpos, vector<Doc> *pdocs)
{

        Esc e(pdocs);
        //clog << "escalafon(cpos->size=" << cpos->size() << ", pdocs->size=" << pdocs->size() << endl;
        set<uint32_t>::iterator cpi;
        vector<uint32_t> *vpos=new vector<uint32_t>();
        for (cpi = cpos->begin(); cpi != cpos->end(); cpi++) {
                vpos->push_back(*cpi-1);
                //clog<< " " << *cpi  ;
        }
        //clog<< endl;

        sort (vpos->begin(), vpos->end(), e);

        return vpos;
}


/* http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/ */
timespec tdiff(timespec start, timespec end)
{
        timespec temp;
        if ((end.tv_nsec-start.tv_nsec)<0) {
                temp.tv_sec = end.tv_sec-start.tv_sec-1;
                temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
        } else {
                temp.tv_sec = end.tv_sec-start.tv_sec;
                temp.tv_nsec = end.tv_nsec-start.tv_nsec;
        }
        return temp;
}

/**
 * Remplaza ocurrencias de una cadena por otra en una cadena
 * http://www.zedwood.com/article/105/cpp-strreplace-function
 */
string& cad_remplaza(const string &busca, const string &remplaza, string &cadena)
{
        string buffer;

        int sealeng = busca.length();
        int strleng = cadena.length();

        if (sealeng==0)  {
                return cadena;//no change
        }

        for(int i=0, j=0; i<strleng; j=0 ) {
                while (i+j<strleng && j<sealeng && cadena[i+j]==busca[j]) {
                        j++;
                }
                if (j==sealeng)//found 'busca'
                {
                        buffer.append(remplaza);
                        i+=sealeng;
                } else {
                        buffer.append( &cadena[i++], 1);
                }
        }
        cadena = buffer;
        return cadena;
}


string escapa(char *s)
{
        string t=s;
        cad_remplaza("\"", "\\\"", t);
        return t;
}


int main(int argc, char *argv[])
{

        uint32_t inicio = 1;
        uint32_t fin = 10;
        timespec t1;
        clock_gettime(CLOCK_REALTIME, &t1);
        if (argc < 3) {
                cerr<<"Se esperaban al menos 2 argumentos, el primero indice por leer y el segundo consulta. 3ero número de resultado inicial"<<endl;
                exit(1);
        }

        vector<Doc> docs;
        set<string> cons = analizaConsulta(argv[2]);
        //clog << "OJO analizaConsulta retornó conjunto de tamaño " << cons.size() << endl;
        /*	if (cons.size() == 0) {
        		cout << "Consulta vacía<br>" << endl;
        		exit(1);
        	} */
        char *f;
        if ((f = strstr(argv[1], ".indice")) == NULL ||
                        (uint32_t)(f - argv[1]) != (strlen(argv[1]) - 7) ) {
                cerr << "se esperaba extensión .indice. "
                << argv[1] << endl;
                exit(1);
        }

        string nc = nombra_resconsulta(cons, argv[1]);
        //clog << "nc = " << nc << endl;
        vector<uint32_t> *vpos = NULL;

        bool sincopiarec = false;  // Sin tener en cuenta copia de result. recientes
        if (argc >= 6 && strcmp(argv[5], "sincopiarecientes")) {
                sincopiarec = true;
        }

        //	if (!resconsulta_reciente(nc) || sincopiarec) {
        //cerr << "No es reciente " << argv[1] << endl;
        set<uint32_t> *cpos = realizaBusqueda(argv[1], cons, docs);
        //clog << "OJO realizaBusqueda retornó conjunto de tamaño " << cpos->size() << endl;
        vpos = escalafon(cpos, &docs);
        ASSERT(cpos->size() == vpos->size());
        /*		guarda_resconsulta(nc, vpos);
        	} else {
        		//cerr << "Si es reciente" << argv[1] << endl;
        		char relacion[MAXLURL];
        		verificaNombre(argv[1], relacion);
        		leeRelacion(relacion, docs);
        		vpos = lee_resconsulta(nc);
        	} */
        /*	for (int i=0; i<docs.size(); i++) {
        	cerr << i << " " << docs[i] << endl;
        	} 
        	exit(1); */

        /*	if (vpos==NULL || vpos->size()==0) {
        		clog <<"<p>No se encontraron coincidencias</p>"<<endl;
        		return 1;
        	} */

        if (argc >= 4) {
                inicio = atoi(argv[3]);
                if (inicio >= vpos->size() ) {
                        inicio = vpos->size();
                }
                if (inicio <= 0) {
                        inicio = 1;
                }
        }
        if (argc >= 5) {  // Convención fin en 0 significa todos
                fin = atoi(argv[4]);
                if (fin == 0) {
                        fin = vpos->size();
                }
                if (fin < inicio) {
                        fin = inicio;
                }
                if (fin > vpos->size() ) {
                        fin = vpos->size();
                }
        } else {
                fin = vpos->size() < 10 ? vpos->size() : 10;
        }




        cout << "{" << endl;
        cout << "  \"consulta\": \"" << escapa(argv[2]) << "\"," << endl;
        cout << "  \"documentos\": " << vpos->size() << "," << endl;
        cout << "  \"inicio\": " << inicio << "," << endl;
        cout << "  \"fin\": " << fin << "," << endl;
        cout << "  \"resultados\": [" << endl;

        uint32_t ns = 1;
        for (ns = inicio; ns <= fin;  ns++) {
                ASSERT((*vpos)[ns - 1] >= 0 &&
                       (uint32_t)(*vpos)[ns - 1] < docs.size());
                cout << "    { \"url\": \"" <<
                docs[(*vpos)[ns - 1]].URL << "\",\n";
                cout << "    \"frec\": \"" <<
                docs[(*vpos)[ns - 1]].numoc << "\",\n";
                cout << "    \"fecha\": \"" <<
                docs[(*vpos)[ns - 1]].fecha << "\"\n";
                cout << "    }";
                if (ns < fin) {
                        cout << ", ";
                }
                cout << endl;
        }

        timespec t2;
        clock_gettime(CLOCK_REALTIME, &t2);
        //timespec d = tdiff(t1, t2);
        //uint32_t tiempo = d.tv_sec*1000000000 + d.tv_nsec;


        cout << "  ]" << endl; // resultados
        //	cout << "  \"tiempons\": " << tiempo << endl;
        cout << "}" << endl; // objeto

        return 0;
}

