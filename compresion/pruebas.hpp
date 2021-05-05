
#ifndef TestCompresion
#define TestCompresion

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include "compresion.hpp"

class CompresionTest : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( CompresionTest );

    CPPUNIT_TEST(pruebaVacia);
    CPPUNIT_TEST(pruebaCadenasCompresion);
    CPPUNIT_TEST(pruebaAgregarCadenasACompresion);
    CPPUNIT_TEST(pruebaCargarArchivo);
    CPPUNIT_TEST(pruebaCadenaAMapa);
    CPPUNIT_TEST(pruebaSumarMapas);
    CPPUNIT_TEST(pruebaRestarCadenaAMapa);
    CPPUNIT_TEST(pruebaConseguirTendencias);
    CPPUNIT_TEST(pruebaCadenaAMapa);
    CPPUNIT_TEST(pruebaDescompresionArbolVacio);
    CPPUNIT_TEST(pruebaSumarMapas);
    CPPUNIT_TEST(pruebaRestarCadenaAMapa);

    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
private:

    std::vector<std::string> cadenas;

    // void setUp();
    // void tearDown();

    // method to test the constructor
    // void testConstructor();

    // // method to test the assigning and retrieval of grades
    // void testAssignAndRetrieveGrades();

    void pruebaVacia();

    void pruebaCadenasCompresion();

    void pruebaAgregarCadenasACompresion();

    void pruebaCargarArchivo();

    void pruebaCadenaAMapa();

    void pruebaSumarMapas();

    void pruebaRestarCadenaAMapa();

    void pruebaConseguirTendencias();

    void pruebaDescompresionArbolVacio();

    // utilidades generales

    void pruebaComprimirDescomprimir(Arbol_huffman ah, std::string cadena);

    void equalString(const std::string &str1, const std::string &str2);

};
#endif
