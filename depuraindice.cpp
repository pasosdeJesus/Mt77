// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file depuraindice.cpp
 * Depura un índice con un trie-especial 
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: depuraindice.cpp,v 1.5 2010/03/08 12:02:40 vtamara Exp $
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
#include <getopt.h>

#include "sha256.hpp"

using namespace std;

#include "Pos.hpp"
#include "TrieSDisco.hpp"

void uso()
{
        cerr << "depuraindice [-cd] indice" << endl;
        cerr << "       -c indica que calcularse condensado" << endl;
        cerr << "       -d imprime representación a la que se calcula condensado" << endl;
        exit(1);
}

int examina(istream &is, long p)
{
        string cad;
        long hermano = 0, hijo = 0;
        set<Pos> *cpos = NULL;
        string cmd = "";
        int sigue = 1;

        do {
                is.seekg(p);
                cout << "Posición: " << p << endl;
                cad = leeCad(is, arbolHuffman);
                if (cad.size() > 0) {
                        hermano = leeNDesp(is);
                        hijo = leeNDesp(is);
                        cpos = leePos(is);
                        cout << "Cadena: " << cad << 
                                "(" << cad.length() << ")" << endl;
                        cout << "Hijo menor: " << hijo << endl;
                        cout << "Hermano mayor: " << hermano << endl;
                        cout << "Posiciones: " << *cpos << endl;
                        if (cpos != NULL) {
                                delete cpos;
                                cpos = NULL;
                        }
                } else {
                        cout << "No hay nodo" << endl;
                }
                cin >> cmd;
                if ((cmd == "hijo" || cmd == "i") && hijo > 0) {
                        sigue = examina(is, hijo);
                } else if ((cmd == "hermano" || cmd == "e") && hermano > 0) {
                        sigue = examina(is, hermano);
                } else if ((cmd == "anterior" || cmd == "a")) {
                        return 1;
                }
        } while (sigue == 1 && cmd != "salir");
        return 0;
}


int main(int argc, char *argv[])
{

        bool calcondensado = false;
        int ch;
        while ((ch = getopt(argc, argv, "cd")) != -1) {
                switch (ch) {
                        case 'c':
                                calcondensado = true;
                                break;
 
                        default:
                                uso();
                }
        }
        argc -= optind;
        argv += optind;
        if (argc != 1) {
                uso();
        }

        //cerr << "argv[0]" << argv[0] << endl;
        vector<Doc> docs;
        char relacion[MAXLURL];
        clog << "Verificando archivo " << argv[0] << endl;
        verificaNombre(argv[0], relacion);

        if (calcondensado) {
                string c = condensado(argv[0], arbolHuffman, true);
                cout << c << endl;
        } else {
                fstream is(argv[0], ios_base::in);
                clog << "Verificando que es indice" << endl;
                verificaIndice(is);
                clog << "Examinando" << endl;
                examina(is, is.tellg());
                is.close();
        }

        return 0;
}

