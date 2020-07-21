/** @file DocCasoPrueba.hpp
 * Pruebas a clase Doc
 *
 * @see Doc
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009. Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: DocCasoPrueba.hpp,v 1.5 2010/01/06 04:19:25 vtamara Exp $
 */

#if !defined(CPP_UNIT_DOCCASOPRUEBA_HPP)
#define CPP_UNIT_DOCCASOPRUEBA_HPP

#include <cppunit/extensions/HelperMacros.h>
#include "Doc.hpp"


class DocCasoPrueba : public CPPUNIT_NS::TestFixture
{
                CPPUNIT_TEST_SUITE(DocCasoPrueba);
                CPPUNIT_TEST(prueba_base);
                CPPUNIT_TEST(prueba_escribeDocs);
                CPPUNIT_TEST(prueba_leeDocs);
                CPPUNIT_TEST(prueba_mezclaDocs);
                CPPUNIT_TEST_SUITE_END();

        public:
                void prueba_base();
                void prueba_escribeDocs();
                void prueba_leeDocs();
                void prueba_mezclaDocs();
};



#endif
