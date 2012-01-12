// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file DocCasoPrueba.cpp
 *
 * Pruebas de regresión a Doc
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: DocCasoPrueba.cpp,v 1.9 2010/01/18 16:12:50 vtamara Exp $
 */

#include <sstream>
#include <iostream>
#include <vector>

#include "DocCasoPrueba.hpp"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(DocCasoPrueba);

void DocCasoPrueba::prueba_base()
{
        Doc d("u", "c", "2009-09-30");
        CPPUNIT_ASSERT(d.URL == "u");
        CPPUNIT_ASSERT(d.cond == "c");
        CPPUNIT_ASSERT(d.fecha == "2009-09-30");

        std::stringstream ss;
        ss.clear();
        ss.str("");

        ss << d ;
        CPPUNIT_ASSERT(ss.str() == "u c 2009-09-30\n");

        Doc g("con\nnoimp", "c", "2010-01-05");
        ss.clear();
        ss.str("");
        ss << g ;
        //clog << "ss es " << ss.str() << endl;
        string cc = "con\%0anoimp c 2010-01-05\n";
        //clog << "cc es " << cc << endl;
        CPPUNIT_ASSERT(ss.str() == cc);

        ss.clear();
        ss.str("");
        for (uint32_t i = 0; i < MAXLURL; i++) {
                ss << "aa";
        }

        g.URL = ss.str();
        CPPUNIT_ASSERT_THROW(ss << g, std::string);


        d.numoc = 1;
        Doc e("u", "c", "2009-09-30");
        e.numoc = 2;
        CPPUNIT_ASSERT(d < e);
        Doc f("u", "c", "2009-09-29");
        CPPUNIT_ASSERT(f < d);
        CPPUNIT_ASSERT(f < e);

}

void DocCasoPrueba::prueba_escribeDocs()
{

        vector<Doc> vdoc;

        vdoc.push_back(Doc("x","y","2009-09-30"));

        std::stringstream sos;

        sos.clear();
        sos << "";
        escribeDocs(sos, vdoc);
        CPPUNIT_ASSERT(sos.str() == "x y 2009-09-30\n" );

        vdoc.push_back(Doc("u","v","2008-09-30"));
        sos.clear();
        sos.str("");
        escribeDocs(sos, vdoc);
        CPPUNIT_ASSERT(sos.str() == "x y 2009-09-30\nu v 2008-09-30\n" );

        sos.clear();
        sos.str("");
        vector<int64_t> reord;
        reord.push_back(1);
        reord.push_back(0);
        escribeDocs(sos, vdoc, &reord);
        CPPUNIT_ASSERT(sos.str() == "u v 2008-09-30\nx y 2009-09-30\n" );
}

void DocCasoPrueba::prueba_leeDocs()
{
        std::stringstream ss;
        ss.clear();
        ss.str("");
        vector<Doc> vdoc;
        vdoc = leeDocs(ss);
        CPPUNIT_ASSERT(vdoc.size() == 0);
        ss.clear();
        ss.str("a");
        CPPUNIT_ASSERT_THROW(leeDocs(ss), std::string);
        ss.clear();
        ss.str("a b");
        CPPUNIT_ASSERT_THROW(leeDocs(ss), std::string);
        ss.clear();
        ss.str("a b c");
        CPPUNIT_ASSERT_THROW(leeDocs(ss), std::string);
        ss.clear();
        ss.str("a b 2009-01");
        CPPUNIT_ASSERT_THROW(leeDocs(ss), std::string);
        //clog << "1" << endl;
        ss.clear();
        ss.str("a\n b 2009-01");
        CPPUNIT_ASSERT_THROW(leeDocs(ss), std::string);
        //clog << "2" << endl;
        ss.clear();
        ss.str("a b 2009-01-\n");
        CPPUNIT_ASSERT_THROW(leeDocs(ss), std::string);
        //clog << "3" << endl;
        ss.clear();
        ss.str("a b 2009-01-01");
        CPPUNIT_ASSERT_THROW(leeDocs(ss), std::string);

        //clog << "4" << endl;

        ss.clear();
        ss.str("a b 2009-01-01\n");
        vdoc = leeDocs(ss);
        //clog << "vdoc.size=" << vdoc.size() << endl;
        CPPUNIT_ASSERT(vdoc.size() == 1);
        CPPUNIT_ASSERT(vdoc[0].URL == "a");
        CPPUNIT_ASSERT(vdoc[0].cond == "b");
        CPPUNIT_ASSERT(vdoc[0].fecha == "2009-01-01");
}


void DocCasoPrueba::prueba_mezclaDocs()
{
        std::stringstream ss;
        ss.clear();
        ss << "";

        vector<Doc> docs1;
        docs1.push_back(Doc("a", "d", "2001-01-01"));
        vector<Doc> docs2;
        docs2.push_back(Doc("a", "b", "2009-01-01"));
        docs1.push_back(Doc("b", "b", "2009-01-01"));

        vector<int64_t> renum = mezclaDocs(docs1, docs2);
        CPPUNIT_ASSERT(renum.size() == 2);
        CPPUNIT_ASSERT(renum[0] == 0);
        CPPUNIT_ASSERT(docs1.size() == 2);
        CPPUNIT_ASSERT(docs2.size() == 2);
        CPPUNIT_ASSERT(docs2[0].URL == "a");
        CPPUNIT_ASSERT(docs2[0].cond == "d");
        CPPUNIT_ASSERT(docs2[0].fecha == "2001-01-01");
}

