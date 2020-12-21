// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file comun.cpp
 * Funciones comunes
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: comun.cpp,v 1.17 2011/03/29 23:07:26 vtamara Exp $
 */

#include <set>
#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <stdint.h>

#include <stdio.h>
#include <string.h>
#include <map>

// using namespace std;
// La anterior comentada por poco portable

#include "comun.hpp"


/**
 * Divide una cadena de acuerdo a un delimitador
 * http://www.zedwood.com/article/106/cpp-explode-function
 */
vector<string> estalla(const string &delimitador, const string &cad)
{
        vector<string> arr;

        int cadlong = cad.length();
        int dellong = delimitador.length();
        if (dellong == 0)
                return arr;

        int i = 0;
        int k = 0;
        while( i < cadlong ) {
                int j=0;
                while (i + j < cadlong && j < dellong && cad[i + j] == delimitador[j]) {
                        j++;
                }
                if (j == dellong) {
                        arr.push_back( cad.substr(k, i-k) );
                        i += dellong;
                        k = i;
                } else {
                        i++;
                }
        }
        arr.push_back(cad.substr(k, i-k) );

        return arr;
}

void verificaNombre(const char *na, char *nrel) throw(string)
{
        ASSERT(na != NULL);
        ASSERT(nrel != NULL);
        /*	nrel[MAXLURL - 1] = '\0';
        	ASSERT(nrel[MAXLURL - 1] == '\0'); */

        if (strlen(na) < 7 || strcmp(na + (strlen(na) - 7), ".indice") != 0) {
                stringstream ss;
                ss << "Se esperaba extensión .indice de '" << na << "'" ;
                throw ss.str();
        }
        if (strlen(na) > MAXLURL - 3) {
                stringstream ss;
                ss << "Nombre de archivo excede "
                << MAXLURL - 3 << " caracteres" ;
                throw ss.str();
        }
        strlcpy(nrel, na, min((size_t)MAXLURL, (size_t)(strlen(na) - 6)));
        strlcat(nrel, ".relacion", MAXLURL);
}

/** Retorna el prefijo común más largo de dos cadenas */
string
prefijo_comun_mas_largo(string a, string b)
{
        uint32_t c;
        uint32_t m = min(min(a.size(), b.size()), (size_t)MAXCAD);
        for (c = 0 ; c < m && a[c] == b[c] ; c++) {}
        return a.substr(0, c);
}

std::string errorFormato(std::istream &is, string m)
{
        stringstream r;
        r << max(0L, (long)is.tellg() - 1) << ": " << m;
        return r.str();
}

/** Lee de un stream una cadena vacía (si el stream no puede leerse) o
 * terminada con { o de MAXCAD caracteres*/
string leeCad(std::istream &is, Arbol_huffman &arbolHuffman) throw(string)
{
        int c = is.get();
        string cad = "";
        if (c == '\n' || c == EOF) {
                return "";
        }
        while (c != FINCADENA && c != EOF && cad.size() < MAXCAD) {
                cad += c;
                c = is.get();
        }
        if (cad.size() >= MAXCAD) {
                cerr << "No se indexó completa palabra larga: " << cad << endl;
                while (c != FINCADENA && c != EOF) {
                        c =  is.get();
                }
        }
        if (c != FINCADENA) {

                throw errorFormato(is, string("Se esperaba ") + FINCADENA);
        }
        return cad;
}

/** Lee un número hexadecimal */
long
leeHex(std::istream &is) throw(std::string)
{
        uint32_t i;
        long valor = 0;
        int c = 0;

        //clog << "OJO leeHex..." << endl;
        for (i = 0; i < (int)MAXLHEX; i++) {
                c = is.get();
                //clog << "OJO leeHex i=" << i << ", c=" << (char)c << endl;
                if (c == EOF) {
                        std::stringstream ss;
                        ss << "Se esperaban " << MAXLHEX << " digitos no " << i;
                        throw errorFormato(is, ss.str());
                }
                if ((c < '0' || c > '9') && (c < 'a' || c > 'f')) {
                        throw errorFormato(is, "Se esperaba digito hexadecimal");
                }
                valor = valor * 16 + ((c >= '0' && c <= '9') ?
                                      c - '0' : c - 'a' + 10);
                //clog << "OJO leeHex valor=" << valor << endl;
        }

        return valor;
}


/*
 * Escribe en os un entero como hexadecimal de 8 digitos.
 * @param n Entero, debe ser menor o igual a 0xffffffff
 */
void escribeHex(std::ostream &os, uint32_t n)
{
        ASSERT(n <= 0xffffffff);
        os << setfill('0') << setw(8) << setbase(16) << n ;
}


void escribe128b(std::ostream &os, uint32_t n)
{
        // clog << "OJO escribe128b n=" << n << endl;
        ASSERT(n <= 0xffffffff);
        string res = "";
        uint32_t c = n;

        for (int i = 0; i<MAXL128B; i++) {
                char r = (c % 128) + '0';
                res = r + res;
                c = c / 128;
        }

        os << res;
}


long
lee128b(std::istream &is) throw (std::string)
{
        int i;
        long valor = 0;
        int c = 0;

        for (i = 0; i < (int)MAXL128B; i++) {
                c = is.get();
                //clog << "OJO lee128bx c=" << c << endl;
                if (c == EOF) {
                        std::stringstream ss;
                        ss << "Se esperaban  " << MAXL128B << " digitos, no " << i;
                        throw errorFormato(is, ss.str());
                }

                if (c < '0' || c > ('0' + 127)) {
                        throw errorFormato(is, "Se esperaba digito 128b");
                }
                valor = valor * 128 + (c - '0');
        }


        return valor;
}


/**
 * Convierte número a cadena usada en archivos
string n128b_c(uint64_t n) {
	std::stringstream ss;
	escribe128b(ss, n);
	return ss.str();
}
 */


/**
 * Convierte número de cadena usada en archivos a long

uint64_t c_n128b(string s) {
	std::stringstream ss;
	ss.str(s);
	uint64_t n = lee128b(ss);
	return n;
}
 */


map<string, string> normaCaracteres2bytes {
        {"Á", "A"},
        {"á", "A"},
        {"É", "E"},
        {"é", "E"},
        {"Í", "I"},
        {"í", "I"},
        {"Ñ", "Ñ"},
        {"ñ", "Ñ"},
        {"Ó", "O"},
        {"ó", "O"},
        {"Ú", "U"},
        {"ú", "U"},
        {"Ü", "U"},
        {"ü", "U"}
};

// Normaliza primer caracter del español de una cadena en UTF-8
// @param c Cadena cuyo primer caracter se normaliza
// @param inc Cuantos caracteres normalizó (0, 1  o 2 dependiendo de
//   longitud del primer caracter UTF-8)
// @return Primer caracter normalizado e inc con cantidad empleada de c
string normalizaCaracter(string c, int &inc)
{
        string o = "";
        inc = 0;
        //clog << "\n::" << c << endl;
        if (c.length() == 0) {
                return "";
        }
        inc = 1;
        if ((c[0] >= 'A' && c[0] <= 'Z') ||
                        (c[0] >= '0' && c[0] <= '9') ) { //Mayúsculas
                o += c[0];
        } else if (c[0] >= 'a' && c[0] <= 'z') { //Minúsculas pasan a mayúsculas
                o += toupper(c[0]);
        } else if (normaCaracteres2bytes[c.substr(0,2)].length() > 0) {  
                // Otros caracteres del español que ocupan 2 bytes
                o += normaCaracteres2bytes[c.substr(0,2)];
                inc = 2;
        }
        //clog << o << endl;

        return o;
}

const int tamnoagregan = 40;
string noagregan[tamnoagregan] = {

        "Y",  // Conjunciones
        "O",
        "EL", // Artículos
        "LA",
        "LOS",
        "LAS",
        "UN",
        "UNA",
        "UNOS",
        "UNAS",
        "A", // Preposiciones http://www.apoyolingua.com/LASPREPOSICIONES.htm
        "ANTE",
        "BAJO",
        "CON",
        "CONTRA",
        "DE",
        "DESDE",
        "DURANTE",
        "EN",
        "ENTRE",
        "HACIA",
        "HASTA",
        "MEDIANTE",
        "PARA",
        "POR",
        "SEGUN",
        "SIN",
        "SOBRE",
        "TRAS",
        "QUE", //Otros
        "LE",
        "LES",
        "DEL",
        "AL",
        "CUANDO",
        "SU",
        "SUS",
        "COMO",
        "O",
        "MAS"
};

string cadena_latin1_a_utf8(string str)
{
        std::string salida;
        for (uint8_t ch: str)
        {
                if (ch < 0x80) {
                        salida.push_back(ch);
                }
                else {
                        salida.push_back(0xc0 | ch >> 6);
                        salida.push_back(0x80 | (ch & 0x3f));
                }
        }
        return salida;
}

/** Retorna cadena normalizada. i.e siguiendo convenciones de:
 * - guarda en UTF-8
 * - caracteres aceptables
 * - longitud máxima
 * - palabras excluidas
 */
string normaliza(string s, bool latin1)
{
        if (latin1) {
                s = cadena_latin1_a_utf8(s);
        }
        string resultado = "";
        for(int i = 0 ; i < s.length() && i <= (int)MAXCAD; i++)
        {
                int inc = 0;
                if (s[i] == '.' && (i+1 < s.length()) &&
                                (s[i+1] == '\303' || 
                                 normalizaCaracter(s.substr(i+1), inc) != "")) {
                        resultado += ".";
                //} else if(s[i] == '\303') { // inicio de caracteres del español
                // &&       resultado += normalizaCaracter(s.substr(i), inc);
                //        i += 1;
                } else {
                        resultado += normalizaCaracter(s.substr(i), inc);
                        i += (inc-1); // El ciclo for incrementará otro más
                }
        }
        // clog << resultado << endl;

        for (int i = 0; i < tamnoagregan; i++) {
                if (resultado == noagregan[i]) {
                        return string("");
                }
        }
        // clog << "palabra normalizada: " << resultado << " de " << s << endl;

        return resultado;
}


int car_utf8_a_latin1(const char *u8, char *latin1)
{
        ASSERT(u8 != NULL);
        ASSERT(latin1 != NULL);
        ASSERT(u8[0] != '\0');
        int num;

        /*	clog << "car_utf8_a_latin1(\"" << u8 << "\")" << endl;
        	clog << "ascii *u8=" << (uint32_t)(*u8) << endl;
        	clog << "*u8 & 0xc0=" << ((uint32_t)*u8 & 0xc0) << endl; */

        /*if (*u8 & 0x80) {
        } else*/
        if ((((uint32_t)*u8 & 0xc0) == 0xc0) && u8[1] != '\0') {
                *latin1 = u8[0] << 6 | (u8[1] & 0x3f);
                num = 2;
        } else {
                *latin1 = *u8;
                num = 1;
        }

        //clog << "car_utf8_a_latin1 por salir con num=" << num << " y *latin1 ='" << *latin1 << "'" << endl;
        return num;
}

string utf8_a_latin1(const char *u8, int len)
{
        ASSERT(u8 != NULL);
        ASSERT(len >= 0);
        string r = "";

        if (len == 0) {
                return "";
        }
        /*clog << "utf8_a_latin1(\"" << u8 << "\", "<< len << ")" << endl;*/
        char *i = (char *)u8;
        int c = 0;
        while (c<len && *i != '\0') {
                char latin1;
                int nb = car_utf8_a_latin1(i, &latin1);
                i += nb;
                r += latin1;
                c += nb;
                //clog << "OJO tras iteracion i = '" << i << "', r='" << r << "'" << endl;
        }
        r += "";
        return r;
}


string
directorio_temp()
{
        //clog << "OJO directorio_tempo()" << endl;
        char sfn[100];

        strlcpy(sfn, "/tmp/leeXXXXXX", sizeof(sfn));
        if (mkdtemp(sfn) == NULL) {
                stringstream ss;
                ss << strerror(errno);
                throw ss.str();
        }
        return string(sfn);
}





