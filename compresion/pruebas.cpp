
#include <cppunit/config/SourcePrefix.h>
#include <iostream>
#include <string>

#include "pruebas.hpp"

#define ARCHIVO_PRUEBA "compresion_archivo_prueba.txt"


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( CompresionTest );

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

// void CompresionTest::testBinarioACadena() {
//
// }


void CompresionTest::equalString(const std::string &str1, const std::string &str2) {
    // std::cout << std::endl << str1 << " == " << str2 << std::endl;
    CPPUNIT_ASSERT(str1 == str2);
}

void CompresionTest::setUp() {
    this->cadenas = {
        "hola, que hacee:ee\\\\\\eeeeeeeeeeeeeeeeeeeeeeeeee\n"
            , "algÈÞesoµ½esto×"
            , "abcdefghijklmnopqrstuvwxyz"
            };

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

    // std::cout << std::endl << desc << " == " << cadena << std::endl;
    equalString(desc, cadena);
}

void CompresionTest::pruebaCadenasCompresion() {
    for (std::string c : this->cadenas)
    {
        Arbol_huffman ah(c);
        CPPUNIT_ASSERT(!ah.vacio());
        pruebaComprimirDescomprimir(ah,c);
    }
}

void CompresionTest::pruebaAgregarCadenasACompresion() {
    std::string cadena = "";
    for (std::string c : this->cadenas)
    {
        cadena += c;
        // un mismo arbol solo puede comprimir a un unico archivo
        Arbol_huffman ah(cadena);
        CPPUNIT_ASSERT(!ah.vacio());
        pruebaComprimirDescomprimir(ah, cadena);
    }
}

void CompresionTest::pruebaCargarArchivo() {
    std::string cadena = this->cadenas[0];
    Arbol_huffman ah(cadena);
    ah.guardar(ARCHIVO_PRUEBA);

    Arbol_huffman ah2("", ARCHIVO_PRUEBA);

    equalString(ah.toString(), ah2.toString());
}

void CompresionTest::pruebaDescompresionArbolVacio() {
    Arbol_huffman ah;
    for (std::string c : this->cadenas)
    {
        // std::cout << c << std::endl;
        pruebaComprimirDescomprimir(ah,c);
    }
}

void CompresionTest::pruebaCadenaAMapa() {

    std::map<char, int> mapaCaracteres {
        {'a', 2},
        {'b', 3},
        {'c', 4}
    };
    std::string cadena = "aabbbcccc";
    CPPUNIT_ASSERT(
        Arbol_huffman::cadenaAMapa(cadena) == mapaCaracteres
        );

}

void CompresionTest::pruebaSumarMapas() {
    std::map<char, int> mapaCaracteresResultado {
        {'a', 2},
        {'b', 3},
        {'c', 4}
    };

    std::map<char, int> mc1 {
        {'a', 1},
        {'b', 1},
        {'c', 2}
    };
    std::map<char, int> mc2 {
        {'a', 1},
        {'b', 2},
        {'c', 2}
    };

    Arbol_huffman::sumarMapas(mc1, mc2);

    CPPUNIT_ASSERT(
            Arbol_huffman::equivalenciaMapas(mapaCaracteresResultado, mc1));

}

void CompresionTest::pruebaRestarCadenaAMapa() {
    std::map<char, int> mapa {
        {'a', 2},
        {'b', 3},
        {'c', 4}
    };

    Arbol_huffman::restarCadenaAMapa(mapa, "aacb");

    std::map<char, int> mapaResultado {
        {'b', 2},
        {'c', 3}
    };

    CPPUNIT_ASSERT(
            Arbol_huffman::equivalenciaMapas(mapa, mapaResultado));

}

void CompresionTest::pruebaConseguirTendencias() {}
