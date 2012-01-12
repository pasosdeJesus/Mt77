/**@file comunCasoPrueba.hpp
 * Pruebas de unidad a comun
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: comunCasoPrueba.hpp,v 1.6 2010/01/18 17:17:45 vtamara Exp $
 */

#if !defined(CPP_UNIT_COMUNCASOPRUEBA_HPP)
#define CPP_UNIT_COMUNCASOPRUEBA_HPP

#include <cppunit/extensions/HelperMacros.h>

#ifdef NDEBUG
#error Estas pruebas requieren NDEBUG activo
#endif

class comunCasoPrueba : public CPPUNIT_NS::TestFixture
{
                CPPUNIT_TEST_SUITE(comunCasoPrueba);
                CPPUNIT_TEST(prueba_estalla);
                CPPUNIT_TEST(prueba_verificaNombre);
                CPPUNIT_TEST(prueba_errorFormato);
                CPPUNIT_TEST(prueba_prefijo_comun_mas_largo);
                CPPUNIT_TEST(prueba_leeCad);
                CPPUNIT_TEST(prueba_leeHex);
                CPPUNIT_TEST(prueba_lee128b);
                CPPUNIT_TEST(prueba_escribeHex);
                CPPUNIT_TEST(prueba_escribe128b);
                CPPUNIT_TEST(prueba_normalizaCaracter);
                CPPUNIT_TEST(prueba_car_utf8_a_latin1);
                CPPUNIT_TEST(prueba_utf8_a_latin1);
                CPPUNIT_TEST(prueba_normalizaCaracter);
                CPPUNIT_TEST(prueba_normaliza);
                CPPUNIT_TEST(prueba_car_utf8_a_latin1);
                CPPUNIT_TEST(prueba_utf8_a_latin1);
                CPPUNIT_TEST(prueba_directorio_temp);
                CPPUNIT_TEST_SUITE_END();

        public:
                void setUp();
        protected:
                void prueba_estalla();
                void prueba_verificaNombre();
                void prueba_errorFormato();
                void prueba_prefijo_comun_mas_largo();
                void prueba_leeCad();
                void prueba_leeHex();
                void prueba_lee128b();
                void prueba_escribeHex();
                void prueba_escribe128b();
                void prueba_normalizaCaracter();
                void prueba_normaliza();
                void prueba_car_utf8_a_latin1();
                void prueba_utf8_a_latin1();
                void prueba_directorio_temp();
};


#endif
