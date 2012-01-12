/**
 * Pruebas a RamDisco
 *
 * @author Vladimir Támara Patiño vtamara@pasosdeJesus.org
 * Dominio público. 2008. Sin garantías.
 * http://creativecommons.org/licenses/publicdomain/ Dominio Público. Sin garantías.
 * @version $Id: RamDiscoCasoPrueba.hpp,v 1.8 2010/01/09 19:28:04 vtamara Exp $
 */

#if !defined(CPP_UNIT_RAMDISCOPRUEBA_HPP)
#define CPP_UNIT_RAMDISCOPRUEBA_HPP

#include <cppunit/extensions/HelperMacros.h>


// De NodoTrieS usa Pos

class RamDiscoCasoPrueba : public CPPUNIT_NS::TestFixture
{
                CPPUNIT_TEST_SUITE(RamDiscoCasoPrueba);
                CPPUNIT_TEST(prueba_precalcula_escribe_con_hermanos);
                CPPUNIT_TEST(prueba_precalcula_escribe);
                // Se prueba en TrieSDisco prueba_escribePlanoStream;
                CPPUNIT_TEST(prueba_leePlano);
                CPPUNIT_TEST(prueba_leeTexto);
                CPPUNIT_TEST(prueba_escribePlano);
                CPPUNIT_TEST(prueba_mezclaDiscoRam);
                CPPUNIT_TEST(prueba_subindiceDiscoaRAM);
                CPPUNIT_TEST_SUITE_END();

        public:
                void setUp();
        protected:
                void prueba_precalcula_escribe_con_hermanos();
                void prueba_precalcula_escribe();
                void prueba_leePlano();
                void prueba_leeTexto();
                void prueba_escribePlano();
                void prueba_mezclaDiscoRam();
                void prueba_subindiceDiscoaRAM();

};


#endif
