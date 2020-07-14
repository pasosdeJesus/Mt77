/**
 * Pruebas a Pos
 *
 * @author Vladimir Támara Patiño vtamara@pasosdeJesus.org
 * Dominio público. 2008. Sin garantías.
 * http://creativecommons.org/licenses/publicdomain/ Dominio Público. Sin garantías.
 * @version $Id: PosCasoPrueba.hpp,v 1.6 2010/01/06 04:19:25 vtamara Exp $
 */

#if !defined(CPP_UNIT_POSCASOPRUEBA_HPP)
#define CPP_UNIT_POSCASOPRUEBA_HPP

#include <cppunit/extensions/HelperMacros.h>


// De NodoTrieS usa Pos

class PosCasoPrueba : public CPPUNIT_NS::TestFixture
{
                CPPUNIT_TEST_SUITE(PosCasoPrueba);
                CPPUNIT_TEST(prueba_escribePos);
                CPPUNIT_TEST(prueba_leePos);
                CPPUNIT_TEST(prueba_longPos);
                CPPUNIT_TEST(prueba_copiaPos);
                CPPUNIT_TEST_SUITE_END();

        public:
                void setUp();
        protected:
                void prueba_escribePos();
                void prueba_leePos();
                void prueba_longPos();
                void prueba_saltaPos();
                void prueba_copiaPos();
};


#endif
