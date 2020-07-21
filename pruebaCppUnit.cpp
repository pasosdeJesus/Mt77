// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file pruebaCppUnit.cpp
 * Pruebas con cppunit, efectua las pruebas de los archivos objeto que se
 * encadenen con el de este.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: pruebaCppUnit.cpp,v 1.4 2010/01/06 10:09:30 vtamara Exp $
 */

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>


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


