
#include <cppunit/config/SourcePrefix.h>
#include <iostream>
#include <string>

#include "pruebas.hpp"

#define ARCHIVO_PRUEBA "compresion_archivo_prueba.txt"


int main (int argc, char* argv[])
{
    // Create the event manager and test controller
    CPPUNIT_NS::TestResult controller;

    // Add a listener that colllects test result
    CPPUNIT_NS::TestResultCollector result;
    controller.addListener(&result);

    // Add a listener that print dots as test run.
    CPPUNIT_NS::BriefTestProgressListener progress;
    controller.addListener(&progress);

    // Add the top suite to the test runner
    CPPUNIT_NS::TestRunner runner;
    runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
    runner.run(controller);

    // Print test in a compiler compatible format.
    CPPUNIT_NS::CompilerOutputter outputter(&result, std::cout);
    outputter.write();

    return result.wasSuccessful() ? 0 : 1;
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( CompresionTest );

// void CompresionTest::testBinarioACadena() {
//
// }

void CompresionTest::equalString(const std::string &str1, const std::string &str2) {
    // std::cout << std::endl << str1 << " == " << str2 << std::endl;
    CPPUNIT_ASSERT(str1 == str2);
}

void CompresionTest::setUp() {
}

// cargar varios strings al arbol
// probar compresion y descompresion de varios elementos
void CompresionTest::pruebaVacia() {
    Arbol_huffman ah;
    CPPUNIT_ASSERT(ah.vacio());
}

void CompresionTest::pruebaComprimirDescomprimir(Arbol_huffman ah, std::string cadena) {
    std::string comp = ah.comprimir(cadena);
    std::string desc = ah.descomprimir(comp);

    equalString(desc, cadena);
}

void CompresionTest::pruebaCadenasCompresion() {
    std::string cadenas[] = {
        "hola, que hacee:ee\\\\\\eeeeeeeeeeeeeeeeeeeeeeeeee\n"
        , "algÈÞesoµ½esto×"
        , "abcdefghijklmnopqrstuvwxyz"
    };
    for (std::string c : cadenas)
    {
        Arbol_huffman ah(c);
        CPPUNIT_ASSERT(!ah.vacio());
        pruebaComprimirDescomprimir(ah,c);
    }
}

void CompresionTest::pruebaAgregarCadenasACompresion() {
    std::string cadenas[] = {
        "hola, que hacee:ee\\\\\\eeeeeeeeeeeeeeeeeeeeeeeeee\n"
        , "algÈÞesoµ½esto×"
        , "abcdefghijklmnopqrstuvwxyz"
    };
    std::string cadena = "";
    for (std::string c : cadenas)
    {
        cadena += c;
        // un mismo arbol solo puede comprimir a un unico archivo
        Arbol_huffman ah(cadena);
        CPPUNIT_ASSERT(!ah.vacio());
        pruebaComprimirDescomprimir(ah, cadena);
    }
}

void CompresionTest::pruebaCargarArchivo() {
    std::string cadena = "hola, que hacee:ee\\\\\\eeeeeeeeeeeeeeeeeeeeeeeeee\n";
    Arbol_huffman ah(cadena);
    ah.guardar(ARCHIVO_PRUEBA);

    Arbol_huffman ah2("", ARCHIVO_PRUEBA);

    equalString(ah.toString(), ah2.toString());
}

void CompresionTest::pruebaCadenaAMapa() {}
void CompresionTest::pruebaSumarMapas() {}
void CompresionTest::pruebaRestarCadenaAMapa() {}
void CompresionTest::pruebaConseguirTendencias() {}
