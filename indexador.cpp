// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file indexador.cpp
 * A partir de un texto, crea un trie-s y lo salva en un formato
 * propio.
 * 
 * Convenciòn para un ìndice vacio:
 * archivo.indice tiene cabecera con fin de linea
 * archivo.relacion tiene cabecer sin fin de lina
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: indexador.cpp,v 1.21 2010/11/01 23:31:31 vtamara Exp $
 */

#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <fstream>
#include <istream>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>

#include "sha256.hpp"
#include "leeODT.hpp"
#include "leeXML.hpp"
#include "leeHTML.hpp"
#include "leePDF.hpp"



using namespace std;

#include "Operaciones.hpp"

/** Prefijo  por elimnar al armar URL */
string ELIMPREF = "../";

/*
 * Indexa contenido del archivo noma en RAM
 *
 * @param nom Nombre de archivo por indexar
 * @param metainformacion Agregar metainformación
 * @param pref Prefijo del URL
 * @param t Arbol con índice por completar
 * @param tipo Tipo determinado
 * @param formato Formato determinado
 */
void
indexa(Doc &d, long numdoc, bool metainformacion, const char *pref,
       NodoTrieS &t, string &tipo, string &formato) 
{
        //clog << "OJO indexa(d.URL=" << d.URL << ", " << numdoc << ", " << metainformacion << ", " << pref << ", t," << tipo << "," << formato << ")" << endl;
        ASSERT(numdoc >= 1);
        string sitio = prefijoASitio(pref);
        //clog << "OJO sitio=" << sitio << endl;
        tipo = "otro";
        char nom[MAXLURL];
        snprintf(nom, MAXLURL, "%s", d.URL.c_str());
        //Si es XML y/o relato emplear otra función
        formato = determinaFormato(nom);
        //clog << "OJO formato= "<< formato << endl;
        try {
                if (formato == "texto") {
                        leeTexto(nom, numdoc, t, metainformacion);
                        tipo = "documento";
                } else if (formato == "xml") {
                        leeXML(nom, numdoc, t);
                        tipo = "documento";
                } else if (formato == "relato") {
                        leeXML(nom, numdoc, t);
                        tipo = "relato";
                } else if (formato == "odf") {
                        leeODT(nom, numdoc, t, metainformacion);
                        tipo = "documento";
                } else if (formato == "html") {
                        leeHTML(nom, numdoc, t, metainformacion);
                        tipo = "documento";
                } else if (formato == "pdf") {
                        leePDF(nom, numdoc, t, metainformacion);
                        tipo = "documento";
                } else if (formato == "ogv" || formato == "flv") {
                        tipo = "vídeo";
                } else if (formato == "ogg" || formato == "mp3") {
                        tipo = "audio";
                } else if (formato == "jpg" || formato == "png" || formato == "gif") {
                        tipo = "imagen";
                }
                //clog << "OJO tipo= "<< tipo<< endl;
                if (metainformacion) {
                        // Insertamos titulo también como palabra
                        t.insertaConEtiqueta(nom, "titulo", numdoc, 1);
                        string tt = normaliza(nom);
                        if (tt != "") {
                                t.inserta(tt, Pos(numdoc, 1));
                        }
                        // Insertamos la extensión como titulo
                        vector<string> bext = estalla(".", nom);
                        if (bext.size() > 1) {
                                //clog << "OJO Insertando extensión " << bext[bext.size() -1] << endl;
                                t.insertaConEtiqueta(bext[bext.size() - 1],
                                                     "titulo", numdoc, 1);
                        }

                        t.insertaConEtiqueta(sitio, "sitio", numdoc, 1);
                        t.insertaConEtiqueta(tipo, "tipo", numdoc, 1);
                }

                struct tm* rel;
                struct stat atr;
                stat(nom, &atr);
                rel = gmtime(&(atr.st_mtime));
                char fecha[11];
                snprintf(fecha, 11, "%04i-%02i-%02i",
                         1900 + rel->tm_year,
                         rel->tm_mon + 1, rel->tm_mday);
                string hash = sha256archivo(string(nom));
                //clog << "OJO hash= " << hash << endl;
                d.cond = hash;
                d.fecha = string(fecha);
                if (d.URL.substr(0, ELIMPREF.size()) == ELIMPREF) {
                        d.URL = pref + d.URL.substr(ELIMPREF.size());
                } else {
                        d.URL = pref + d.URL;
                }
        } catch (string m) {
                cerr << m << endl;
        }

}


/**
 * Determina si un archivo existe.
 * http://pages.cs.wisc.edu/~colbster/cs302/examples/fileopen/fileopen.cpp
 */
bool existe_archivo(string filename) {
  ifstream fin;
  fin.open (filename.c_str());
  if (fin.fail()) return false;
  fin.close();
  return true;
}

uint32_t MAXG = 50000000;

bool metainformacion=true;

int main(int argc, char *argv[])
{
        if (argc < 5) {
                cerr<<"Se esperaban al menos 4 argumentos, el primero indice por crear o aumentar, indice temporal, dominio por agregar como prefijo y los siguientes textos por leer"<<endl;
                exit(1);
        }
        vector<Doc> idocs;
        vector<Doc> sdocs;
        char noma[1024], nomt[1024], nomrel[1024], pref[1024];

        verificaNombre(argv[1], nomrel);
        snprintf(noma, 1000, "%s", argv[1]);
        snprintf(nomt, 1000, "%s", argv[2]);
        snprintf(pref, 1024, "%s", argv[3]);
        //cerr<<"noma="<<noma<<endl;

        char *cm = getenv("MT77MAXG");
        if (cm != NULL) {
                MAXG = atol(cm);
        }
        if (MAXG <= 10) {
                cerr<<"El valor de MT77MAXG no es aceptable" << endl;
                exit(1);
        }
        cm = getenv("MT77ELIMPREF");
        if (cm != NULL) {
                ELIMPREF = cm;
        }
 
        if (strcmp(nomt, noma) == 0) {
                cerr<<"Indice por crear o aumentar y temporal deben ser diferentes" << endl;
                exit(1);
        }

        if (!existe_archivo(noma)) {
                // Lo creamos en blanco y sdocs comienza vacío
                fstream os(noma, ios_base::out);
                os << MARCAIND << endl;
                os << endl;
                os.close();
        } else if (!existe_archivo(nomrel)) {
                cerr << "Debería existir " << nomrel << endl;
                exit(1);
        } else {
                leeRelacion(nomrel, sdocs);
        }
        long tamsdocsini = sdocs.size();
        // clog << "OJO Por aumentar indice con " << tamsdocsini << " documentos" << endl;
 
        vector<uint32_t> grupo; // Índice donde termina cada grupo en idocs
        uint32_t tg = 0 ; // Tamaño del grupo actual
        for (int i = 4; i < argc; i++) {
                FILE *f = NULL;
                if ((f = fopen (argv[i],  "rb")) == NULL) {
                        cerr << "No existe el archivo '" << argv[i] <<
                        "'" << endl;
                } else  {
                        fseek(f, 0L, SEEK_END);
                        uint32_t sz = ftell(f);
                        fclose(f);
                        //clog << "i=" << i << ", argv[i]=" << argv[i] << ", sz=" << sz << endl;
                        if (sz > MAXG) {
                                cerr << "El archivo " << argv[i] << " es demasiado grande (" << sz << " bytes), no se indexará." << endl;
                        } else {
                                idocs.push_back(Doc(argv[i], "", "1960-01-01"));
                                if (tg + sz > MAXG) {
                                        grupo.push_back(idocs.size()-2);
                                        tg = sz;
                                } else {
                                        tg += sz;
                                }
                        }
                        //clog << "tg=" << tg << endl;
                }
        }
        if (idocs.size() > 0) {
                grupo.push_back(idocs.size() - 1);
        }

        char *nomi[2];  // Nombres de temporal e indice final
/*        if (grupo.size() % 2 == 1) {
                nomi[0] = noma;
                nomi[1] = nomt;
        } else { */
        nomi[0] = nomt;
        nomi[1] = noma;
        //        }
        //

        string tipo = "otro";
        string formato;
        NodoTrieS *t;
        uint32_t nd = 0; // indice del documento procesado
        try {
                for (uint32_t g = 0; g < grupo.size(); g++) {
                        //clog << "OJO indexando grupo g=" << g << " que termina en " << grupo[g] << endl;
                        t = new NodoTrieS();
                        while (nd <= grupo[g]) {
                                //clog << "OJO indexando nd=" << nd << " como " << tamsdocsini + nd + 1 << endl;
                                indexa(idocs[nd], tamsdocsini + nd + 1, 
                                                metainformacion, pref,
                                                *t, tipo, formato);
                                sdocs.push_back(idocs[nd]);
                                nd++;
                        }
                        //cerr<<"idocs.size="<<idocs.size()<<endl;

/*                        if (g == 0) {
                                //clog << "OJO escribiendo primer grupo en " << nomi[g % 2] << endl;
                                fstream os(nomi[g % 2], ios_base::out);
                                os << MARCAIND << endl;
                                escribePlanoStream(t, os);
                                os.close();
                        } else { */
                        //clog << "OJO escribiendo grupo g=" << g << " en " << nomi[g % 2] << endl;
                        fstream os(nomi[g % 2], ios_base::out);
                        fstream is1(nomi[(g + 1) % 2], ios_base::in);

                        verificaIndice(is1);
                        is1.clear();

                        os << MARCAIND << endl;
                        mezclaDiscoRam(is1, t, 0, os, true, true,
                                        NULL, NULL);
                        is1.close();
                        os.close();
                        //}
                        delete t;
                }
                if (grupo.size() % 2 == 1) {
                        remove(noma);
                        rename(nomt, noma);
                }
                if (grupo.size() >= 0) {
                        //clog << "OJO escribiendo relacion de documentos" << endl;
                        escribeRelacion(nomrel, sdocs, NULL);
                }

        } catch (string m) {
                cerr << noma << ":" << m;
                exit(1);
        }

        return 0;
}

