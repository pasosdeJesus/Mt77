// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file unzipuno.cpp
 * Ejemplo de uso de funzipuno.h
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: unzipuno.cpp,v 1.5 2010/01/06 10:09:30 vtamara Exp $
 */


#include "funzipuno.hpp"
#include <iostream>

using namespace std;

/**
 * Espera ruta a contenedor ZIP y nombre de archivo por descomprimir y
 * directorio en el cual descomprimir.
 */
int
main(int argc, char **argv)
{
        char nomout[PATH_MAX];
        FILE *in, *out;

        /* if no file argument and stdin not redirected, give the user help */
        if (argc != 4 && isatty(0))   {
                cerr << "unzipuno zip archivo dir" << endl;
                cerr << "  zip es contenedor ZIP" << endl;
                cerr << "  archivo es archivo que se espera en contenedor zip"
                << endl;
                cerr << "  dir Directorio donde se descomprimirá" << endl;
                exit(3);
        }

        if ((in = fopen(argv[1], "rb")) == (FILE *)NULL) {
                err(2, "cannot find input file");
        }
        snprintf(nomout, PATH_MAX, "%s/%s", argv[3], argv[2]);
        if ((out = fopen(nomout, "wb")) == (FILE *)NULL) {
                err(2, "cannot write to output file");
        }
        unzipUno(in, argv[2], out);
        fclose(in);
        fclose(out);

        return 0;
}
