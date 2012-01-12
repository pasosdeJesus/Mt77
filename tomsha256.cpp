// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file tomsha256.cpp
 * Calcula hash sha256 de un archivo.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: tomsha256.cpp,v 1.3 2010/01/06 10:09:30 vtamara Exp $
 */


#include "sha256.hpp"
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
        if (argc != 2) {
                cerr << "Se esperaban un argumento, el nombre del archivo"
                <<endl;
                exit(1);
        }

        string hash = sha256archivo(string(argv[1]));

        cout << hash << endl;

        return 0;
}

