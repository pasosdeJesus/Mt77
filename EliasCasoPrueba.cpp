// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file EliasCasoPrueba.cpp
 * Pruebas de unidad a algoritmos de compresión
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: EliasCasoPrueba.cpp,v 1.7 2010/01/18 16:12:50 vtamara Exp $
 */

#include <sstream>
#include <iostream>
#include <fstream>
#include <stdint.h>

#include "Elias.hpp"
#include "EliasCasoPrueba.hpp"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(EliasCasoPrueba);


/* rep. binaria de http://www.velocityreviews.com/forums/t318127-using-binary-numbers-in-c.html */

#define HEX__(n) 0x##n##LU

/* 8-bit conversion function */
#define B8__(x) ((x&0x0000000FLU)?1:0) \
+((x&0x000000F0LU)?2:0) \
+((x&0x00000F00LU)?4:0) \
+((x&0x0000F000LU)?8:0) \
+((x&0x000F0000LU)?16:0) \
+((x&0x00F00000LU)?32:0) \
+((x&0x0F000000LU)?64:0) \
+((x&0xF0000000LU)?128:0)

/* *** user macros *** */

/* for upto 8-bit binary constants */
#define B8(d) ((unsigned char)B8__(HEX__(d)))

/* for upto 16-bit binary constants, MSB first */
#define B16(dmsb,dlsb) (((uint64_t)B8(dmsb)<< 8) + (uint64_t)B8(dlsb))


/* for upto 32-bit binary constants, MSB first */
#define B32(dmsb,db2,db3,dlsb) (((uint64_t)B8(dmsb)<<24) \
+ ((uint64_t)B8(db2)<<16) \
+ ((uint64_t)B8(db3)<< 8) \
+ B8(dlsb))

/* Sample usage:
B8(01010101) = 85
B16(10101010,01010101) = 43605
B32(10000000,11111111,10101010,01010101) = 2164238933
*/


void EliasCasoPrueba::prueba_piso_logb2()
{
        CPPUNIT_ASSERT_THROW(piso_logb2(0), std::string );

        CPPUNIT_ASSERT(piso_logb2((uint32_t)1) == 0);
        CPPUNIT_ASSERT(piso_logb2((uint32_t)2) == 1);
        CPPUNIT_ASSERT(piso_logb2((uint32_t)5) == 2);
        CPPUNIT_ASSERT(piso_logb2((uint32_t)7) == 2);
        CPPUNIT_ASSERT(piso_logb2((uint32_t)8) == 3);
}

void EliasCasoPrueba::prueba_techo_logb2()
{
        CPPUNIT_ASSERT_THROW(techo_logb2(0), std::string);

        CPPUNIT_ASSERT(techo_logb2(1) == 0);
        CPPUNIT_ASSERT(techo_logb2(2) == 1);
        CPPUNIT_ASSERT(techo_logb2(5) == 3);
        CPPUNIT_ASSERT(techo_logb2(7) == 3);
        CPPUNIT_ASSERT(techo_logb2(8) == 3);
        CPPUNIT_ASSERT(techo_logb2(9) == 4);
}

void EliasCasoPrueba::prueba_pot2()
{
        CPPUNIT_ASSERT(pot2(0) == 1);
        CPPUNIT_ASSERT(pot2(1) == 2);
        CPPUNIT_ASSERT(pot2(2) == 4);
        CPPUNIT_ASSERT(pot2(3) == 8);
}

void EliasCasoPrueba::prueba_unario()
{

        vector<bool> vb(0);

        CPPUNIT_ASSERT_THROW(codifica_unario(0, vb), std::string);

        codifica_unario(10, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "1111111110");
        //clog << "vb2ascii(vb) = " << vb2ascii(vb) << endl;
        long r = decodifica_unario(vb);
        CPPUNIT_ASSERT(r == 10);
        vb.clear();
}

void EliasCasoPrueba::prueba_pone_un_entero()
{
        vector<bool> vb(0);

        pone_un_entero(1, 2, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "01");
        vb.clear();
        pone_un_entero(2, 2, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "10");
        vb.clear();
        pone_un_entero(3, 3, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "011");
}

void EliasCasoPrueba::prueba_toma_un_entero()
{
        vector<bool> vb(0);
        CPPUNIT_ASSERT_THROW(toma_un_entero(10,vb), std::string);

        vb.clear();
        vb = str2vb("0");
        CPPUNIT_ASSERT(toma_un_entero(1,vb) == 0);
        vb.clear();
        vb = str2vb("01");
        CPPUNIT_ASSERT(toma_un_entero(2,vb) == 1);
        vb.clear();
        vb = str2vb("101");
        CPPUNIT_ASSERT(toma_un_entero(3,vb) == 5);
}


void EliasCasoPrueba::prueba_codifica_elias_gama()
{
        vector<bool> vb(0);

        CPPUNIT_ASSERT_THROW(codifica_elias_gama(0,vb), std::string);

        codifica_elias_gama(1, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "0");
        vb.clear();
        codifica_elias_gama(2, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "100");
        vb.clear();
        codifica_elias_gama(3, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "101");
        vb.clear();
        codifica_elias_gama(4, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "11000");
        vb.clear();
        codifica_elias_gama(5, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "11001");
        vb.clear();
        codifica_elias_gama(6, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "11010");
        vb.clear();
        codifica_elias_gama(7, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "11011");
        vb.clear();
        codifica_elias_gama(8, vb);
        string rs = vb2str(vb);
        CPPUNIT_ASSERT(rs == "1110000");
        vb.clear();
        codifica_elias_gama(9, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "1110001");
}


void EliasCasoPrueba::prueba_decodifica_elias_gama()
{
        vector<bool> vb(0);

        CPPUNIT_ASSERT_THROW(decodifica_elias_gama(vb), std::string);

        vb.clear();
        vb = str2vb("1110001");
        CPPUNIT_ASSERT(decodifica_elias_gama(vb) == 9);
        vb.clear();
        vb = str2vb("1110000");
        CPPUNIT_ASSERT(decodifica_elias_gama(vb) == 8);
        vb.clear();
        vb = str2vb("11011");
        CPPUNIT_ASSERT(decodifica_elias_gama(vb) == 7);
        vb.clear();
        vb = str2vb("11010");
        CPPUNIT_ASSERT(decodifica_elias_gama(vb) == 6);
        vb.clear();
        vb = str2vb("11001");
        CPPUNIT_ASSERT(decodifica_elias_gama(vb) == 5);
        vb.clear();
        vb = str2vb("11000");
        CPPUNIT_ASSERT(decodifica_elias_gama(vb) == 4);
        vb.clear();
        vb = str2vb("101");
        CPPUNIT_ASSERT(decodifica_elias_gama(vb) == 3);
        vb.clear();
        vb = str2vb("100");
        CPPUNIT_ASSERT(decodifica_elias_gama(vb) == 2);
        vb.clear();
        vb = str2vb("0");
        CPPUNIT_ASSERT(decodifica_elias_gama(vb) == 1);
}


void EliasCasoPrueba::prueba_long_elias_gama()
{

        CPPUNIT_ASSERT_THROW((void)long_elias_gama(0), std::string);

        CPPUNIT_ASSERT(long_elias_gama(1) == 1);
        CPPUNIT_ASSERT(long_elias_gama(2) == 1);
        CPPUNIT_ASSERT(long_elias_gama(3) == 1);
        CPPUNIT_ASSERT(long_elias_gama(4) == 1);
        CPPUNIT_ASSERT(long_elias_gama(5) == 1);
        CPPUNIT_ASSERT(long_elias_gama(6) == 1);
        CPPUNIT_ASSERT(long_elias_gama(7) == 1);
        CPPUNIT_ASSERT(long_elias_gama(8) == 1);
        CPPUNIT_ASSERT(long_elias_gama(9) == 1);
        CPPUNIT_ASSERT(long_elias_gama(15) == 1);
        CPPUNIT_ASSERT(long_elias_gama(16) == 2);
        CPPUNIT_ASSERT(long_elias_gama(17) == 2);
        CPPUNIT_ASSERT(long_elias_gama(31) == 2);
        CPPUNIT_ASSERT(long_elias_gama(32) == 2);
        CPPUNIT_ASSERT(long_elias_gama(33) == 2);
        CPPUNIT_ASSERT(long_elias_gama(63) == 2);
        CPPUNIT_ASSERT(long_elias_gama(64) == 2);
        CPPUNIT_ASSERT(long_elias_gama(65) == 2);
        CPPUNIT_ASSERT(long_elias_gama(127) == 2);
        CPPUNIT_ASSERT(long_elias_gama(128) == 2);
        CPPUNIT_ASSERT(long_elias_gama(129) == 2);
        CPPUNIT_ASSERT(long_elias_gama(255) == 2);
        CPPUNIT_ASSERT(long_elias_gama(256) == 3);
        CPPUNIT_ASSERT(long_elias_gama(257) == 3);
        CPPUNIT_ASSERT(long_elias_gama(4095) == 3);
        CPPUNIT_ASSERT(long_elias_gama(4096) == 4);
        CPPUNIT_ASSERT(long_elias_gama(4097) == 4);

        CPPUNIT_ASSERT(long_elias_gama(pot2(16)-1) == 4);
        CPPUNIT_ASSERT(long_elias_gama(pot2(16)) == 5);
        CPPUNIT_ASSERT(long_elias_gama(pot2(16)+1) == 5);

        CPPUNIT_ASSERT(long_elias_gama(pot2(32)-1) == 8);
        /*	CPPUNIT_ASSERT(long_elias_gama(pot2(32)) == 9);
        	CPPUNIT_ASSERT(long_elias_gama(pot2(32)+1) == 9); */


        // Teo: Si k es entero no negativo, el entero 2^4k requiere
        // k+1 bytes en codificación gama de Elías.
        // Dem: Este número requiere 4k bits binarios para representarse
        // Por tanto la codificación unaria toma 4k bits, un 0 y otros 4k
        // bits en 0 para un total de 8k+1 bits, los cuales en bytes
        // requerirían techo(8k+1/8) k bytes.
        // 2^0=1 requiere 1
        // 2^4=16 requiere 2
        // 2^8=256 requiere 3
        // 2^12=4096 requiere 4
        // 2^16=65536 requiere 5
        // ...
        // 2^32 requiere 9

        // Asi que este esquema es muy bueno para codificar números menores a
        // 65535, para números mayores requiere más bytes que un long típico.
        //
        // Esto se traduce en recomendación para usuarios, los documentos pueden
        // ser de cualquier tamaño, pero para facilitar compresión se recomienda
        // que sean de máximo 64K


}



void EliasCasoPrueba::prueba_escribe_elias_gama()
{
        vector<bool> vb(0);

        stringstream ss;
        ss.clear();
        ss.str("");

        CPPUNIT_ASSERT_THROW(escribe_elias_gama(ss, 0), std::string);

        escribe_elias_gama(ss, 1);
        //clog << "1 es '" << ss.str() << "'" << endl;
        CPPUNIT_ASSERT(ss.str()[0] == (char)0);
        ss.clear();
        ss.str("");
        escribe_elias_gama(ss, 2);
        //clog << "2 es '" << ss.str() << "'" << endl;
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(10000000));
        ss.clear();
        ss.str("");
        escribe_elias_gama(ss, 3);
        //clog << "3 es '" << ss.str() << "'" << endl;
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(10100000));
        ss.clear();
        ss.str("");
        escribe_elias_gama(ss, 4);
        //clog << "4 es '" << ss.str() << "'" << endl;
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11000000));
        ss.clear();
        ss.str("");
        escribe_elias_gama(ss, 5);
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11001000));
        ss.clear();
        ss.str("");
        escribe_elias_gama(ss, 6);
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11010000));
        ss.clear();
        ss.str("");
        escribe_elias_gama(ss, 7);
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11011000));
        ss.clear();
        ss.str("");
        escribe_elias_gama(ss, 8);
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11100000));
        ss.clear();
        ss.str("");
        escribe_elias_gama(ss, 9);
        string rs = ss.str();
        CPPUNIT_ASSERT(rs[0] == (char)B8(11100010));
        ofstream os("gama.elias", ios::out | ios::binary);
        escribe_elias_gama(os, 16);
        os.close();

        ss.str("");
        ss.clear();
        escribe_elias_gama(ss, 16);
        rs = ss.str();
        CPPUNIT_ASSERT(rs[0] == (char)B8(11110000));
        CPPUNIT_ASSERT(rs[1] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama(ss, 32);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111000));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama(ss, 64);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111100));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama(ss, 128);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111110));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama(ss, 256);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));
        CPPUNIT_ASSERT(ss.str()[2] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama(ss, 512);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(10000000));
        CPPUNIT_ASSERT(ss.str()[2] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama(ss, 1024);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(11000000));
        CPPUNIT_ASSERT(ss.str()[2] == (char)B8(00000000));
        // Si puede representar entero tipicamente de n bits, requiere 2n+1 bits para
        // indicarlo
        // n bits	bytes
        // 1 3		1
        // 2 5		1
        // 3 7		1
        // 4 9		2
        // 5 11
        // 6 13
        // 7 15
        // 8 17		techo(17/8) 3
        //		techo(bits/8) = 4 en bits=25  que da n=12
        //		techo(bits/8) = 5 en bits=33  que da n=piso(bits/2)=16,
        //		techo(bits/8) = 6 en bits=41  que da n=piso(bits/2)=20,
        //
        //		es decir n=4k  2^4k ocupa k+1 bytes
        //		piso(techo(2*piso(log_2(k))+1/8)/2)>=6
        //
        //
        ss.str("");
        ss.clear();
        escribe_elias_gama(ss, 65536);
        rs = ss.str();
        /*cout << 65536 << " " << rs << ", length=" << rs.length() << endl;
        cout << "rs[0]=" << rs[0] <<" o " << (int)((unsigned char)rs[0])<< endl;
        cout << "rs[1]=" << rs[1] <<" o " << (int)((unsigned char)rs[1])<< endl;
        cout << "rs[2]=" << rs[2] <<" o " << (int)((unsigned char)rs[2])<< endl;
        cout << "rs[3]=" << rs[3] <<" o " << (int)((unsigned char)rs[3])<< endl;
        cout << "rs[4]=" << rs[4] <<" o " << (int)((unsigned char)rs[4])<< endl;*/

        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[2] == (char)B8(00000000));
        CPPUNIT_ASSERT(ss.str()[3] == (char)B8(00000000));
        CPPUNIT_ASSERT(ss.str()[4] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama(ss, (uint32_t)(pot2(32)-1));
        rs = ss.str();
        /*cout << "2^32 - 1 " << " " << rs << ", length=" << rs.length() << endl;
        cout << "rs[0]=" << rs[0] <<" o " << (int)((unsigned char)rs[0])<< endl;
        cout << "rs[1]=" << rs[1] <<" o " << (int)((unsigned char)rs[1])<< endl;
        cout << "rs[2]=" << rs[2] <<" o " << (int)((unsigned char)rs[2])<< endl;
        cout << "rs[3]=" << rs[3] <<" o " << (int)((unsigned char)rs[3])<< endl;
        cout << "rs[4]=" << rs[4] <<" o " << (int)((unsigned char)rs[4])<< endl;
        cout << "rs[5]=" << rs[5] <<" o " << (int)((unsigned char)rs[5])<< endl;
        cout << "rs[6]=" << rs[6] <<" o " << (int)((unsigned char)rs[6])<< endl;
        cout << "rs[7]=" << rs[7] <<" o " << (int)((unsigned char)rs[7])<< endl; */

        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[2] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[3] == (char)B8(11111110));
        CPPUNIT_ASSERT(ss.str()[4] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[5] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[6] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[7] == (char)B8(11111110));

}

void EliasCasoPrueba::prueba_escribe_elias_gama2()
{
        stringstream ss;
        ss.clear();
        ss.str("");

        CPPUNIT_ASSERT_THROW(escribe_elias_gama2(ss, 0), std::string);

        escribe_elias_gama2(ss, 1);
        //clog << "1 es '" << ss.str() << "'" << endl;
        CPPUNIT_ASSERT(ss.str()[0] == (char)0);
        ss.clear();
        ss.str("");
        escribe_elias_gama2(ss, 2);
        //clog << "2 es '" << ss.str() << "'" << endl;
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(10000000));
        ss.clear();
        ss.str("");
        escribe_elias_gama2(ss, 3);
        //clog << "3 es '" << ss.str() << "'" << endl;
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(10100000));
        ss.clear();
        ss.str("");
        escribe_elias_gama2(ss, 4);
        //clog << "4 es '" << ss.str() << "'" << endl;
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11000000));
        ss.clear();
        ss.str("");
        escribe_elias_gama2(ss, 5);
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11001000));
        ss.clear();
        ss.str("");
        escribe_elias_gama2(ss, 6);
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11010000));
        ss.clear();
        ss.str("");
        escribe_elias_gama2(ss, 7);
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11011000));
        ss.clear();
        ss.str("");
        escribe_elias_gama2(ss, 8);
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11100000));
        ss.clear();
        ss.str("");
        escribe_elias_gama2(ss, 9);
        string rs = ss.str();
        CPPUNIT_ASSERT(rs[0] == (char)B8(11100010));
        ofstream os("gama.elias", ios::out | ios::binary);
        escribe_elias_gama2(os, 16);
        os.close();

        ss.str("");
        ss.clear();
        escribe_elias_gama2(ss, 16);
        rs = ss.str();
        CPPUNIT_ASSERT(rs[0] == (char)B8(11110000));
        CPPUNIT_ASSERT(rs[1] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama2(ss, 32);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111000));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama2(ss, 64);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111100));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama2(ss, 128);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111110));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama2(ss, 256);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));
        CPPUNIT_ASSERT(ss.str()[2] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama2(ss, 512);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(10000000));
        CPPUNIT_ASSERT(ss.str()[2] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama2(ss, 1024);
        rs = ss.str();
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(11000000));
        CPPUNIT_ASSERT(ss.str()[2] == (char)B8(00000000));

        ss.str("");
        ss.clear();
        escribe_elias_gama2(ss, 65536);
        rs = ss.str();

        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[2] == (char)B8(00000000));
        CPPUNIT_ASSERT(ss.str()[3] == (char)B8(00000000));
        CPPUNIT_ASSERT(ss.str()[4] == (char)B8(00000000));

        ss.clear();
        ss.str("");
        escribe_elias_gama2(ss, 40);
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111001));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));

        ss.clear();
        ss.str("");
        escribe_elias_gama2(ss, 41);
        cout << "41 ss=" << ss.str() << endl;
        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111001));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00100000));


        ss.str("");
        ss.clear();
        escribe_elias_gama2(ss, (uint32_t)(pot2(32)-1));
        rs = ss.str();

        CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[1] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[2] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[3] == (char)B8(11111110));
        CPPUNIT_ASSERT(ss.str()[4] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[5] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[6] == (char)B8(11111111));
        CPPUNIT_ASSERT(ss.str()[7] == (char)B8(11111110));

}



void EliasCasoPrueba::prueba_lee_elias_gama()
{
        vector<bool> vb(0);

        std::stringstream ss;
        CPPUNIT_ASSERT_THROW(lee_elias_gama(ss), std::string);

        ss.clear();
        ss.str("");
        ss << (char)B8(11111110);
        CPPUNIT_ASSERT_THROW(lee_elias_gama(ss), std::string);

        ss.clear();
        ss.str("");
        ss << (char)B8(11110000) << (char) 0;
        CPPUNIT_ASSERT(lee_elias_gama(ss) == 16);
        ss.clear();
        ss.str("");
        ss << (char)B8(11100010);
        CPPUNIT_ASSERT(lee_elias_gama(ss) == 9);
        ss.clear();
        ss.str("");
        ss << (char)B8(11100000);
        CPPUNIT_ASSERT(lee_elias_gama(ss) == 8);
        ss.clear();
        ss.str("");
        ss << (char)B8(11011000);
        CPPUNIT_ASSERT(lee_elias_gama(ss) == 7);
        ss.clear();
        ss.str("");
        ss << (char)B8(11010000);
        CPPUNIT_ASSERT(lee_elias_gama(ss) == 6);
        ss.clear();
        ss.str("");
        ss << (char)B8(11001000);
        CPPUNIT_ASSERT(lee_elias_gama(ss) == 5);
        ss.clear();
        ss.str("");
        ss << (char)B8(11000000);
        CPPUNIT_ASSERT(lee_elias_gama(ss) == 4);

        ss.clear();
        ss.str("");
        ss << (char)B8(10100000);
        CPPUNIT_ASSERT(lee_elias_gama(ss) == 3);
        ss.clear();
        ss.str("");
        ss << (char)B8(10000000);
        CPPUNIT_ASSERT(lee_elias_gama(ss) == 2);
        ss.clear();
        ss.str("");
        ss << (char)0;
        CPPUNIT_ASSERT(lee_elias_gama(ss) == 1);

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11110000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama(ss) == 16);


        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111110);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111110);
        CPPUNIT_ASSERT(lee_elias_gama(ss) == pot2(32)-1);

}

void EliasCasoPrueba::prueba_lee_elias_gama2()
{
        vector<bool> vb(0);

        std::stringstream ss;
        CPPUNIT_ASSERT_THROW(lee_elias_gama2(ss), std::string);

        ss.clear();
        ss.str("");
        ss << (char)B8(11110000) << (char) 0;
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 16);
        ss.clear();
        ss.str("");
        ss << (char)B8(11100010);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 9);
        ss.clear();
        ss.str("");
        ss << (char)B8(11100000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 8);
        ss.clear();
        ss.str("");
        ss << (char)B8(11011000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 7);
        ss.clear();
        ss.str("");
        ss << (char)B8(11010000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 6);
        ss.clear();
        ss.str("");
        ss << (char)B8(11001000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 5);
        ss.clear();
        ss.str("");
        ss << (char)B8(11000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 4);

        ss.clear();
        ss.str("");
        ss << (char)B8(10100000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 3);
        ss.clear();
        ss.str("");
        ss << (char)B8(10000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 2);
        ss.clear();
        ss.str("");
        ss << (char)0;
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 1);

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11110000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 16);

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111001);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 40);

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111001);
        ss <<  (char)B8(00100000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 41);

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111101);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 96);

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111101);
        ss <<  (char)B8(00001000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 97);

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111110);
        ss <<  (char)B8(10000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 192);

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111110);
        ss <<  (char)B8(10000010);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 193);

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(01000000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 384);

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(01000000);
        ss <<  (char)B8(10000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == 385);

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(10100000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (512+256));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(10100000);
        ss <<  (char)B8(00100000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (512+256+1));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11010000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (1024+512));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11010000);
        ss <<  (char)B8(00001000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (1024+512+1));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11101000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (2048+1024));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11101000);
        ss <<  (char)B8(00000010);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (2048+1024+1));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11110100);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (4096+2048));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11110100);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(10000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (4096+2048+1));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111010);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (8192+4096));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111010);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00100000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (8192+4096+1));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111110);
        ss <<  (char)B8(10000000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (pot2(15)+pot2(14)));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111110);
        ss <<  (char)B8(10000000);
        ss <<  (char)B8(00000010);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (pot2(15)+pot2(14)+1));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(01000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (pot2(16)+pot2(15)));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(01000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(10000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (pot2(16)+pot2(15)+1));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11110100);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (pot2(20)+pot2(19)));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11110100);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(10000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (pot2(20)+pot2(19)+1));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(01000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (pot2(24)+pot2(23)));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(01000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(10000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (pot2(24)+pot2(23)+1));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11110100);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (pot2(28)+pot2(27)));

        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11110100);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(00000000);
        ss <<  (char)B8(10000000);
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == (pot2(28)+pot2(27)+1));



        ss.clear();
        ss.str("");
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111110);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111111);
        ss <<  (char)B8(11111110);
        //cout << "pot2(32) - 1 es " << pot2(32) - 1 << endl;
        CPPUNIT_ASSERT(lee_elias_gama2(ss) == pot2(32) - 1);

}


void EliasCasoPrueba::prueba_codifica_binaria_minima()
{
        vector<bool> vb(0);

        CPPUNIT_ASSERT_THROW(codifica_binaria_minima(10, 0, vb), std::string);

        vb.clear();
        codifica_binaria_minima(1, 10, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "000");
        vb.clear();
        codifica_binaria_minima(2, 10, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "001");
        vb.clear();
        codifica_binaria_minima(3, 10, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "010");
        vb.clear();
        codifica_binaria_minima(4, 10, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "011");
        vb.clear();
        codifica_binaria_minima(5, 10, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "100");
        vb.clear();
        codifica_binaria_minima(6, 10, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "101");
        vb.clear();
        codifica_binaria_minima(7, 10, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "1100");
        vb.clear();
        codifica_binaria_minima(8, 10, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "1101");
        vb.clear();
        codifica_binaria_minima(9, 10, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "1110");
        vb.clear();
        codifica_binaria_minima(10, 10, vb);
        CPPUNIT_ASSERT(vb2str(vb) == "1111");
}


void EliasCasoPrueba::prueba_decodifica_binaria_minima()
{
        vector<bool> vb(0);

        CPPUNIT_ASSERT_THROW(decodifica_binaria_minima(0, vb), std::string);
        CPPUNIT_ASSERT_THROW(decodifica_binaria_minima(10, vb), std::string);

        vb.clear();
        vb = str2vb("000");
        uint64_t r = decodifica_binaria_minima(10, vb);
        CPPUNIT_ASSERT(r == 1);
        vb.clear();
        vb = str2vb("001");
        r = decodifica_binaria_minima(10, vb);
        CPPUNIT_ASSERT(r == 2);
        vb.clear();
        vb = str2vb("010");
        CPPUNIT_ASSERT(decodifica_binaria_minima(10, vb) == 3);
        vb.clear();
        vb = str2vb("011");
        CPPUNIT_ASSERT(decodifica_binaria_minima(10, vb) == 4);
        vb.clear();
        vb = str2vb("100");
        CPPUNIT_ASSERT(decodifica_binaria_minima(10, vb) == 5);
        vb.clear();
        vb = str2vb("101");
        CPPUNIT_ASSERT(decodifica_binaria_minima(10, vb) == 6);
        vb.clear();
        vb = str2vb("1100");
        CPPUNIT_ASSERT(decodifica_binaria_minima(10, vb) == 7);
        vb.clear();
        vb = str2vb("1101");
        CPPUNIT_ASSERT(decodifica_binaria_minima(10, vb) == 8);
        vb.clear();
        vb = str2vb("1110");
        CPPUNIT_ASSERT(decodifica_binaria_minima(10, vb) == 9);
        vb.clear();
        vb = str2vb("1111");
        CPPUNIT_ASSERT(decodifica_binaria_minima(10, vb) == 10);

}

