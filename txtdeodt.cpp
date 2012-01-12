// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file txtdeodt.cpp
 * Extrae texto de un archivo en formato odt de Open Document Format 
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: txtdeodt.cpp,v 1.5 2010/01/06 10:09:30 vtamara Exp $
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#include "leeODT.hpp"


void muestra(string t)
{
        fstream is(t.c_str(), ios_base::in);
        while (!is.eof()) {
                cout << (char)is.get() ;
        }
        is.close();
}

int main(int argc, char *argv[])
{

        if (argc != 2) {
                cerr << "Se esperaban un argumento, el nombre del archivo odt"
                <<endl;
                exit(1);
        }

        string dt = prepara(string(argv[1]));
        string ns = dt + string("/salida.txt");
        aplicaXSLT(dt, dt + "/content.xml", ns);
        muestra(dt + "/salida.txt");
        unlink(string(dt + "/mimetype").c_str());
        unlink(string(dt + "/content.xml").c_str());
        unlink(string(dt + "/salida.txt").c_str());
        unlink(string(dt + "/convierte.xslt").c_str());
        rmdir(dt.c_str());

        return 0;
}

