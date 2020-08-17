// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file comunCasoPrueba.cpp
 * Pruebas de regresión a comun.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: comunCasoPrueba.cpp,v 1.10 2010/01/18 16:12:50 vtamara Exp $
 */

#include <sstream>
#include <iostream>

#include "comunCasoPrueba.hpp"
#include "comun.hpp"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(comunCasoPrueba);

void comunCasoPrueba::prueba_estalla()
{
        vector<string> r;
        r = estalla("", "a,b,c");
        CPPUNIT_ASSERT(r.size() == 0);

        r = estalla(",", "");
        CPPUNIT_ASSERT(r.size() == 1);
        CPPUNIT_ASSERT(r[0] == "");

        r = estalla(",", "a,b,c");
        CPPUNIT_ASSERT(r.size() == 3);
        CPPUNIT_ASSERT(r[0] == "a");
        CPPUNIT_ASSERT(r[1] == "b");
        CPPUNIT_ASSERT(r[2] == "c");
}


void comunCasoPrueba::prueba_verificaNombre()
{
        char nrel[MAXLURL];
        nrel[0] = '\0';
        CPPUNIT_ASSERT_THROW(verificaNombre(NULL, nrel),
                             std::string);
        CPPUNIT_ASSERT_THROW(verificaNombre("indice.indice", NULL),
                             std::string);
        CPPUNIT_ASSERT_THROW(verificaNombre("indice", nrel), std::string);
        stringstream nomar;
        for(uint32_t i = 0; i < MAXLURL + 10; i++) {
                nomar << "a";
        }
        nomar << ".indice";

        CPPUNIT_ASSERT_THROW(verificaNombre(nomar.str().c_str(), nrel),
                             std::string);
        verificaNombre("indice.indice", nrel);
        return;

        //clog << "nrel='" << nrel << "'" << endl;
        CPPUNIT_ASSERT(nrel == string("indice.relacion"));
}


void comunCasoPrueba::prueba_errorFormato()
{
        stringstream is;
        CPPUNIT_ASSERT(errorFormato(is, "a") == "0: a");
}


void comunCasoPrueba::prueba_prefijo_comun_mas_largo()
{
        CPPUNIT_ASSERT(prefijo_comun_mas_largo("", "") == "");
        CPPUNIT_ASSERT(prefijo_comun_mas_largo("a", "") == "");
        CPPUNIT_ASSERT(prefijo_comun_mas_largo("", "a") == "");
        CPPUNIT_ASSERT(prefijo_comun_mas_largo("a", "b") == "");
        CPPUNIT_ASSERT(prefijo_comun_mas_largo("a", "A") == "");
        CPPUNIT_ASSERT(prefijo_comun_mas_largo("a", "a") == "a");
        CPPUNIT_ASSERT(prefijo_comun_mas_largo("aa", "a") == "a");
        CPPUNIT_ASSERT(prefijo_comun_mas_largo("a", "aa") == "a");
        CPPUNIT_ASSERT(prefijo_comun_mas_largo("ab", "aa") == "a");
        CPPUNIT_ASSERT(prefijo_comun_mas_largo("abc", "aa") == "a");
        CPPUNIT_ASSERT(prefijo_comun_mas_largo("aac", "aa") == "aa");
}


void comunCasoPrueba::prueba_leeCad()
{
        std::stringstream ss;
        ss.clear();
        ss << "";
        CPPUNIT_ASSERT(leeCad(ss)=="" );
        ss.clear();
        ss << "{";
        CPPUNIT_ASSERT(leeCad(ss) == "");
        ss.clear();
        ss << "a";
        CPPUNIT_ASSERT_THROW(leeCad(ss), std::string);
        ss.clear();
        ss << "a{";
        CPPUNIT_ASSERT(leeCad(ss) == "a");
        ss.clear();
        ss << "a{b";
        CPPUNIT_ASSERT(leeCad(ss) == "a");
        ss.str("");  // Este es necesario para que el clear siguiente funcione.
        // clear solo borra flags de error.
        ss.clear();
        ss << "123456789{";
        CPPUNIT_ASSERT(leeCad(ss) == "123456789");
        ss.clear();
        ss << "12345678901234567890123456789012{";
        CPPUNIT_ASSERT(leeCad(ss) == "12345678901234567890123456789012");
        ss.clear();
        ss << "123456789012345678901234567890123";
        CPPUNIT_ASSERT_THROW(leeCad(ss), std::string);
        ss.clear();
        ss << "a\0b{";
        CPPUNIT_ASSERT_THROW(leeCad(ss), std::string);
}


void comunCasoPrueba::prueba_leeHex()
{
        std::stringstream ss;
        ss.clear();
        ss << "";
        CPPUNIT_ASSERT_THROW(leeHex(ss), std::string);
        ss.clear();
        ss << "1{";
        CPPUNIT_ASSERT_THROW(leeHex(ss), std::string);
        ss.clear();
        ss << "1234";
        CPPUNIT_ASSERT_THROW(leeHex(ss), std::string);
        ss.clear();
        ss << "00000001";
        CPPUNIT_ASSERT(leeHex(ss) == 1);
        ss.clear();
        ss << "0000000a";
        CPPUNIT_ASSERT(leeHex(ss) == 10);
        ss.clear();
        ss << "0000000abcd";
        CPPUNIT_ASSERT(leeHex(ss) == 10);
        string x;
        ss >> x;
        CPPUNIT_ASSERT(x  == "bcd");
        ss.clear();
        ss << "000000ff";
        CPPUNIT_ASSERT(leeHex(ss) == 255);
        ss.clear();
        ss << "0000ffff";
        CPPUNIT_ASSERT(leeHex(ss) == 0xffff);
        ss.clear();
        ss << "ffffffff";
        CPPUNIT_ASSERT(leeHex(ss) == 0xffffffff);
}

void comunCasoPrueba::prueba_lee128b()
{
        std::stringstream ss;
        ss.clear();
        ss << "";
        CPPUNIT_ASSERT_THROW(lee128b(ss), std::string);
        ss.clear();
        ss << "1\n";
        CPPUNIT_ASSERT_THROW(lee128b(ss), std::string);
        ss.clear();
        ss << "12{";
        CPPUNIT_ASSERT_THROW(lee128b(ss), std::string);
        ss.clear();
        ss << "00001";
        CPPUNIT_ASSERT(lee128b(ss) == 1);
        ss.clear();
        ss << "0000" << (char)175;
        CPPUNIT_ASSERT(lee128b(ss) == 127);
        ss.clear();
        ss << "00010";
        CPPUNIT_ASSERT(lee128b(ss) == 128);
        ss.clear();
        ss << "00020";
        CPPUNIT_ASSERT(lee128b(ss) == 256);
        ss.clear();
        ss << "?" << (char)175 << (char)175 << (char)175
        << (char)175;
        CPPUNIT_ASSERT(lee128b(ss) == 0xffffffff);
}

void comunCasoPrueba::prueba_escribeHex()
{
        std::stringstream ss;
        ss.clear();
        ss.str("");
        escribeHex(ss, 0);
        CPPUNIT_ASSERT(ss.str() == "00000000" );
        ss.str("");
        escribeHex(ss, 10);
        CPPUNIT_ASSERT(ss.str() == "0000000a" );
        ss.str("");
        escribeHex(ss, 0xffffffff);
        CPPUNIT_ASSERT(ss.str() == "ffffffff" );
        ss.str("");
// Ya no se puede porque no cabe en uint32        CPPUNIT_ASSERT_THROW(escribeHex(ss, 0xfffffffff), std::string);
}

void comunCasoPrueba::prueba_escribe128b()
{
        std::stringstream ss;
        ss.clear();
        ss.str("");
        escribe128b(ss, 0);
        CPPUNIT_ASSERT(ss.str() == "00000" );
        ss.str("");
        escribe128b(ss, 128);
        CPPUNIT_ASSERT(ss.str() == "00010" );
        ss.str("");
        escribe128b(ss, 0xffffffff);
        CPPUNIT_ASSERT(ss.str() == "?\xaf\xaf\xaf\xaf");
        ss.str("");
// ya no cabe en uint32        CPPUNIT_ASSERT_THROW(escribe128b(ss, 0xfffffffff), std::string);
}

void comunCasoPrueba::prueba_normalizaCaracter()
{
        CPPUNIT_ASSERT(normalizaCaracter('a') == string("A"));
        CPPUNIT_ASSERT(normalizaCaracter(' ') == "");
        CPPUNIT_ASSERT(normalizaCaracter(':') == "");
}

void comunCasoPrueba::prueba_normaliza()
{
        //CPPUNIT_ASSERT(normaliza("aBáÁñÉíóúü") == string("ABAAÑEIOUU"));
        CPPUNIT_ASSERT(normaliza("aB\xE1\xC1\xF1\xC9\xED\xF3\xFA\xFC") == string("ABAA\xD1" "EIOUU"));
        CPPUNIT_ASSERT(normaliza("      ") == "");
        CPPUNIT_ASSERT(normaliza("titulo:nombre") == "TITULONOMBRE");
        CPPUNIT_ASSERT(normaliza("con") == "");
        CPPUNIT_ASSERT(normaliza("con.puntointermedio") == "CON.PUNTOINTERMEDIO");
        CPPUNIT_ASSERT(normaliza("sinpuntofinal.") == "SINPUNTOFINAL");
        CPPUNIT_ASSERT(normaliza("con-gui\xF3" "n") == "CONGUION");
}

void comunCasoPrueba::prueba_car_utf8_a_latin1()
{
        char l;

        int n = car_utf8_a_latin1("a", &l);
        //cout << "n = " << n << "l=" << l << endl;
        CPPUNIT_ASSERT(n == 1 && l == 'a');
        n = car_utf8_a_latin1("\xc3\xb3", &l) ;
        //cout << "n = " << n << "l=" << l << endl;
        CPPUNIT_ASSERT(n == 2 && l == '\xf3');
}


void comunCasoPrueba::prueba_utf8_a_latin1()
{
        string r = utf8_a_latin1("a", 1);
        //cout << r << endl;
        CPPUNIT_ASSERT(r == "a");
        r = utf8_a_latin1("ac", 2);
        CPPUNIT_ASSERT(r == "ac");
        r = utf8_a_latin1("acc", 3);
        CPPUNIT_ASSERT(r == "acc");
        r = utf8_a_latin1("acci", 4);
        CPPUNIT_ASSERT(r == "acci");
        r = utf8_a_latin1("acci\xc3\xb3n", 7);
        CPPUNIT_ASSERT(r == "acci\xf3n");
}



void comunCasoPrueba::prueba_directorio_temp()
{
        string r = directorio_temp();
        cout << r << endl;
        CPPUNIT_ASSERT(r.size() > 0);
}



void comunCasoPrueba::setUp()
{
        return;
}
