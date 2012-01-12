/** @file NodoTrieSCasoPrueba.hpp
 * Pruebas de unidad a NodoTrieS
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2008. Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: NodoTrieSCasoPrueba.hpp,v 1.6 2010/03/08 12:02:40 vtamara Exp $
 */

#if !defined(CPP_UNIT_NODOTRIESCASOPRUEBA_HPP)
#define CPP_UNIT_NODOTRIESCASOPRUEBA_HPP

#include <cppunit/extensions/HelperMacros.h>


// De NodoTrieS usa Pos

class NodoTrieSCasoPrueba : public CPPUNIT_NS::TestFixture
{
                CPPUNIT_TEST_SUITE(NodoTrieSCasoPrueba);
                CPPUNIT_TEST(prueba_constructora);
                CPPUNIT_TEST(prueba_inserta);
                CPPUNIT_TEST(prueba_insertaConEtiqueta);
                CPPUNIT_TEST(prueba_mezcla);
                CPPUNIT_TEST(prueba_renumeraDocs);
                CPPUNIT_TEST(prueba_leeTexto);
                CPPUNIT_TEST(prueba_condensado);
                CPPUNIT_TEST_SUITE_END();

        public:
                void setUp();
        protected:
                void prueba_constructora();
                void prueba_inserta();
                void prueba_insertaConEtiqueta();
                void prueba_mezcla();
                void prueba_renumeraDocs();
                void prueba_leeTexto() ;
                void prueba_condensado() ;
};


#endif
