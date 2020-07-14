// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file PosCasoPrueba.cpp
 * Pruebas de regresión a Pos
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: PosCasoPrueba.cpp,v 1.10 2010/01/18 16:12:50 vtamara Exp $
 */

#include <sstream>
#include <iostream>
#include <fstream>
#include <stdint.h>

#include "PosCasoPrueba.hpp"
#include "Elias.hpp"
#include "Pos.hpp"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(PosCasoPrueba);


string posAstringYBorra(set<Pos> *n)
{
        ASSERT(n!=NULL);
        std::stringstream sso;
        sso << *n;
        delete n;
        //	cout << "OJO posAstringYborra retornará " << sso.str() << endl;
        return sso.str();
}


/**
 * Convierte número a cadena en codificacio usada en archivos para posiciones
 * Números y posiciones
 */
string numC(long n)
{
        std::stringstream ss;
        escribe_elias_gama(ss, (uint32_t)n);
        return ss.str();
}


/**
 * Convierte cadena con codificacion para números usada en archivos a long
 */
uint64_t cNum(string s)
{
        std::stringstream ss;
        ss.str(s);
        uint64_t n = lee_elias_gama(ss);
        return n;
}


void PosCasoPrueba::prueba_escribePos()
{
        std::stringstream ss;
        ss.clear();
        escribePos(ss, NULL);
        CPPUNIT_ASSERT(ss.str() == "}");
        //clog << "ss=" << ss.str() << endl;
        set<Pos> cp;
        ss.clear();
        ss.str("");
        escribePos(ss, &cp);
        CPPUNIT_ASSERT(ss.str() == "}");
        //clog << "ss=" << ss.str() << endl;
        cp.insert(Pos(2,1));
        ss.clear();
        ss.str("");
        escribePos(ss, &cp);
        CPPUNIT_ASSERT(ss.str() == numC(2) + numC(1) + "}");
        //clog << "ss=" << ss.str() << endl;
        cp.insert(Pos(2,2));
        ss.clear();
        ss.str("");
        escribePos(ss, &cp);
        //clog << "ss=" << ss.str() << endl;
        CPPUNIT_ASSERT(ss.str() == numC(2) + numC(1) + numC(1) + "}");
        cp.insert(Pos(1,2));
        ss.clear();
        ss.str("");
        escribePos(ss, &cp);
        //clog << "ss=" << ss.str() << endl;
        CPPUNIT_ASSERT(ss.str() == numC(1) + numC(2) + "(" + numC(2) + numC(1)
                       + numC(1) + "}");
        cp.insert(Pos(1,5));
        ss.clear();
        ss.str("");
        escribePos(ss, &cp);
        //clog << "ss=" << ss.str() << endl;
        CPPUNIT_ASSERT(ss.str() == numC(1) + numC(2) + numC(3) + "(" +
                       numC(2) + numC(1) + numC(1) + "}");


        //CPPUNIT_ASSERT(posAstringYBorra(leePos(ss)) == "{}");

}

void PosCasoPrueba::prueba_longPos()
{
        long r = longPos(NULL);
        //clog << "1 r=" << r << endl;
        CPPUNIT_ASSERT(r == 1);

        set<Pos> cp;
        r = longPos(&cp);
        //clog << "2 r=" << r << endl;
        CPPUNIT_ASSERT(r == 1);

        cp.insert(Pos(2,1));
        r = longPos(&cp);
        //clog << "3 r=" << r << endl;
        CPPUNIT_ASSERT(r == 3);

        cp.insert(Pos(2,2));
        r = longPos(&cp);
        //clog << "4 r=" << r << endl;
        CPPUNIT_ASSERT(r == 4);

        cp.insert(Pos(1,2));
        r = longPos(&cp);
        //clog << "5 r=" << r << endl;
        CPPUNIT_ASSERT(r == 7);

        cp.insert(Pos(1,5));
        r = longPos(&cp);
        //clog << "5 r=" << r << endl;
        CPPUNIT_ASSERT(r == 8);

        //CPPUNIT_ASSERT(posAstringYBorra(leePos(ss)) == "{}");

}


void PosCasoPrueba::prueba_leePos()
{
        std::stringstream ss;
        ss.clear();
        ss << "";
        CPPUNIT_ASSERT_THROW(leePos(ss), std::string);
        ss.str("");
        ss.clear();
        ss << "adsfa";
        CPPUNIT_ASSERT_THROW(leePos(ss), std::string);
        ss.str("");
        ss.clear();
        ss << "{}";
        CPPUNIT_ASSERT_THROW(leePos(ss), std::string);
        ss.str("");
        ss.clear();
        ss << "}";
        CPPUNIT_ASSERT(posAstringYBorra(leePos(ss)) == "{}");
        ss.str("");
        ss.clear();
        ss << "(}";
        CPPUNIT_ASSERT_THROW(leePos(ss), std::string);
        ss.str("");
        ss.clear();
        ss << numC(1) << numC(1) << "(}";
        CPPUNIT_ASSERT_THROW(leePos(ss), std::string);
        ss.str("");
        ss.clear();
        ss << numC(1) << numC(1) << "}";
        //clog << "11 ss=" << ss.str() << endl;
        string r = posAstringYBorra(leePos(ss));
        //clog << "r=" << r << endl;
        CPPUNIT_ASSERT(r == "{(1,1)}");

        ss.str("");
        ss.clear();
        ss << numC(1) << numC(1) << ";}";
        //clog << "11; ss=" << ss.str() << endl;
        CPPUNIT_ASSERT_THROW(leePos(ss), std::string);

        /* 0 no representable
          	s.str(""); ss.clear(); 
        	ss << numC(1) << numC(1) << numC(0) << "}";
        	//clog << "antes ss=" << ss.str() << endl;
        	r = posAstringYBorra(leePos(ss));
        	//clog << "r=" << r << endl;
        	CPPUNIT_ASSERT(r == "{(1,1)}");  // set es conjunto (sin repetidos)
        	*/
        set<Pos> cp;
        cp.insert(Pos(1,2));
        cp.insert(Pos(1,1));
        ss.str("");
        ss.clear();
        escribePos(ss, &cp);
        //clog << "ss=" << ss.str() << endl;
        CPPUNIT_ASSERT(posAstringYBorra(leePos(ss)) == "{(1,1), (1,2)}");  // ordenado
        cp.clear();
        cp.insert(Pos(2,1));
        cp.insert(Pos(1,2));
        ss.str("");
        ss.clear();
        escribePos(ss, &cp);
        CPPUNIT_ASSERT(posAstringYBorra(leePos(ss)) == "{(1,2), (2,1)}");  // ordenado
        // En un AMD64 con 1GB en RAM operando como usuario puede localizarse un conjunto de pos de n=10818812 elementos

        vector<int64_t> renum;
        renum.push_back(-1);
        cp.clear();
        cp.insert(Pos(1,2));
        cp.insert(Pos(1,1));
        ss.clear();
        ss.str("");
        ss.clear();
        ss.seekp(-1);
        ss.seekg(0);
        ss.clear();
        escribePos(ss, &cp);
        //cout << "OJO antes ss=" << ss.str() << endl;
        string sr = posAstringYBorra(leePos(ss, &renum));
        //cout << "OJO leePos(ss, &renum)=" << sr << endl;
        CPPUNIT_ASSERT( sr == "{}");  // ordenado
        cp.clear();
        renum.push_back(0);
        cp.insert(Pos(1,1));
        cp.insert(Pos(2,1));
        ss.str("");
        ss.clear();
        escribePos(ss, &cp);
        CPPUNIT_ASSERT(posAstringYBorra(leePos(ss, &renum)) == "{(1,1)}");  // ordenado

}


void PosCasoPrueba::prueba_copiaPos()
{
        std::stringstream ss;
        ss.clear();
        set<Pos> cp;
        cp.insert(Pos(2,1));
        set<Pos> *cp2 = copiaPos(cp, NULL);
        //clog << "1" << cp2->size() << endl;
        CPPUNIT_ASSERT(cp2->size() == 1);
        string r = posAstringYBorra(cp2);
        //clog << "2 r=" << r << endl;
        CPPUNIT_ASSERT(r == "{(2,1)}");

        cp.insert(Pos(2,2));
        cp2 = copiaPos(cp, NULL);
        CPPUNIT_ASSERT(cp2->size() == 2);
        r = posAstringYBorra(cp2);
        //clog << "3 r=" << r << endl;
        CPPUNIT_ASSERT(r == "{(2,1), (2,2)}");

        cp.insert(Pos(1,2));
        cp2 = copiaPos(cp, NULL);
        CPPUNIT_ASSERT(cp2->size() == 3);
        r = posAstringYBorra(cp2);
        //clog << "4 r=" << r << endl;
        CPPUNIT_ASSERT(r == "{(1,2), (2,1), (2,2)}");

        cp.insert(Pos(1,5));
        cp2 = copiaPos(cp, NULL);
        CPPUNIT_ASSERT(cp2->size() == 4);
        r = posAstringYBorra(cp2);
        //clog << "5 r=" << r << endl;
        CPPUNIT_ASSERT(r == "{(1,2), (1,5), (2,1), (2,2)}");

        vector<int64_t> renum;
        renum.push_back(1);
        renum.push_back(0);
        cp2 = copiaPos(cp, &renum);
        CPPUNIT_ASSERT(cp2->size() == 4);
        r = posAstringYBorra(cp2);
        //clog << "6 r=" << r << endl;
        CPPUNIT_ASSERT(r == "{(1,1), (1,2), (2,2), (2,5)}");

        renum.clear();
        renum.push_back(-1);
        renum.push_back(0);
        cp2 = copiaPos(cp, &renum);
        CPPUNIT_ASSERT(cp2->size() == 2);
        r = posAstringYBorra(cp2);
        //clog << "7 r=" << r << endl;
        CPPUNIT_ASSERT(r == "{(1,1), (1,2)}");


}


void PosCasoPrueba::setUp()
{
        return;
}
