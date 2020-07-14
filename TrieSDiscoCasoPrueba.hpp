/** @file TrieSDiscoCasoPrueba.hpp
 * Pruebas a TrieSDisco
 *
 * @package Mt77
 * @author Vladimir Támara Patiño vtamara@pasosdeJesus.org
 * Dominio público. 2008. Sin garantías.
 * http://creativecommons.org/licenses/publicdomain/ 
 * @version  $Id: TrieSDiscoCasoPrueba.hpp,v 1.9 2010/01/06 04:19:25 vtamara Exp $
 */

#if !defined(CPP_UNIT_TRIESDISCOCASOPRUEBA_HPP)
#define CPP_UNIT_TRIESDISCOCASOPRUEBA_HPP

#include <cppunit/extensions/HelperMacros.h>


// De NodoTrieS usa Pos

class TrieSDiscoCasoPrueba : public CPPUNIT_NS::TestFixture
{
                CPPUNIT_TEST_SUITE(TrieSDiscoCasoPrueba);
                CPPUNIT_TEST(prueba_escribeNodo);
                CPPUNIT_TEST(prueba_escribeCopiaNodo);
                //	CPPUNIT_TEST(prueba_escribePlanoStream);
                CPPUNIT_TEST(prueba_escribeCopiaSubarbol);
                CPPUNIT_TEST(prueba_mezclarec);
                CPPUNIT_TEST(prueba_leeRelacion);
                CPPUNIT_TEST(prueba_escribeRelacion);
                CPPUNIT_TEST(prueba_buscaPlanoStream);
                CPPUNIT_TEST(prueba_buscaPlano);
                CPPUNIT_TEST_SUITE_END();

        public:
                void setUp();
        protected:
                void prueba_escribeNodo();
                void prueba_escribeCopiaNodo();
                //	void prueba_escribePlanoStream();
                void prueba_escribeCopiaSubarbol();
                void prueba_mezclarec();
                void prueba_leeRelacion();
                void prueba_escribeRelacion();
                void prueba_buscaPlanoStream();
                void prueba_buscaPlano();


};


#endif
