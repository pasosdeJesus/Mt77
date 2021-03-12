/** @file EliasCasoPrueba.hpp
 * Pruebas de unidad a algoritmos de compresión
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2008. Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: EliasCasoPrueba.hpp,v 1.4 2010/01/06 04:19:25 vtamara Exp $
 */

#if !defined(CPP_UNIT_ELIASCASOPRUEBA_HPP)
#define CPP_UNIT_ELIASCASOPRUEBA_HPP

#include <cppunit/extensions/HelperMacros.h>
#include "compresion/compresion.hpp"

class EliasCasoPrueba : public CPPUNIT_NS::TestFixture
{
                CPPUNIT_TEST_SUITE(EliasCasoPrueba);
                CPPUNIT_TEST(prueba_piso_logb2);
                CPPUNIT_TEST(prueba_techo_logb2);
                CPPUNIT_TEST(prueba_pot2);
                CPPUNIT_TEST(prueba_unario);
                CPPUNIT_TEST(prueba_pone_un_entero);
                CPPUNIT_TEST(prueba_toma_un_entero);
                CPPUNIT_TEST(prueba_codifica_elias_gama);
                CPPUNIT_TEST(prueba_decodifica_elias_gama);
                CPPUNIT_TEST(prueba_lee_elias_gama);
                CPPUNIT_TEST(prueba_lee_elias_gama2);
                CPPUNIT_TEST(prueba_escribe_elias_gama);
                CPPUNIT_TEST(prueba_escribe_elias_gama2);
                CPPUNIT_TEST(prueba_long_elias_gama);
                CPPUNIT_TEST(prueba_codifica_binaria_minima);
                CPPUNIT_TEST(prueba_decodifica_binaria_minima);
                CPPUNIT_TEST_SUITE_END();

        public:
                void prueba_piso_logb2();
                void prueba_techo_logb2();
                void prueba_pot2();
                void prueba_unario();
                void prueba_pone_un_entero();
                void prueba_toma_un_entero();
                void prueba_codifica_elias_gama();
                void prueba_decodifica_elias_gama();
                void prueba_lee_elias_gama();
                void prueba_lee_elias_gama2();
                void prueba_escribe_elias_gama();
                void prueba_escribe_elias_gama2();
                void prueba_long_elias_gama();
                void prueba_codifica_binaria_minima();
                void prueba_decodifica_binaria_minima();
};


#endif
