// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file Elias.cpp
 * Algoritmos de compresión para números 
 *
 * Tomados de:
 * Compression and coding algorithms
 * Escrito por Alistair Moffat, Andrew Turpin
 *
 * http://books.google.com.co/books?id=czwqegW_NCAC&pg=PA42&lpg=PA42&dq=Binary+Interpolative+Coding&source=bl&ots=SJ0D8VHszl&sig=nkk2CIg4rpskJkCSh6NnRimC3eU&hl=es&ei=vYqWSteBLoyvtgfXiOy-Dg&sa=X&oi=book_result&ct=result&resnum=7#v=onepage&q=Binary%20Interpolative%20Coding&f=false
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: Elias.cpp,v 1.6 2010/01/18 17:12:57 vtamara Exp $
 */

#include <vector>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <stdint.h>
#include "comun.hpp"
#include "Elias.hpp"

using namespace std;

uint32_t techo_logb2(uint64_t x)
{
        ASSERT(x>=1);

        int r = 0;
        int u = 0;
        while (x>1) {
                r++;
                if (x % 2 == 1) {
                        u = 1;
                }
                x = x / 2;
        }
        return r + u;
}


/*
uint32_t piso_logb2(uint64_t x) {
	ASSERT(x>=1);
 
	int r = 0;
	while (x>1) {
		r++;
		x = x / 2;
	}
	return r;
} */

/** http://en.wikipedia.org/wiki/Binary_logarithm
 * */
uint32_t piso_logb2(uint32_t n)
{
        ASSERT(n>=1);
        int pos = 0;
        if (n >= 1<<16) {
                n >>= 16;
                pos += 16;
        }
        if (n >= 1<< 8) {
                n >>=  8;
                pos +=  8;
        }
        if (n >= 1<< 4) {
                n >>=  4;
                pos +=  4;
        }
        if (n >= 1<< 2) {
                n >>=  2;
                pos +=  2;
        }
        if (n >= 1<< 1) {
                pos +=  1;
        }
        return ((n == 0) ? (-1) : pos);
}

uint64_t pot2(uint32_t e)
{
        ASSERT(e>=0);

        uint64_t r=1;
        while (e>0) {
                r = r * 2;
                e--;
        }
        return r;
}

/*string ulong2ascii(uint64_t n, uint32_t l = 64)
{
	stringstream ss("");
	uint64_t p2 = 1 << (l-1);
	while (p2 > 0) {
		if ((uint64_t)(n & p2) > 0) {
			ss << "1";
		} else {
			ss << "0";
		}
		p2 >>= 1;
	}
 
	return ss.str();
}
 
string bin2ascii(string bs)
{
	uint32_t i;
	stringstream ss("");
	string sep = "";
	ss << bs.length() << "[" ;
	for(i = 0; i < bs.length(); i++) {
		ss << sep << (int)((unsigned char)bs[i]) ;
		sep = " ";
	}
	ss << "]" ;
	return ss.str();
}
 
 
string vb2ascii(vector<bool> &vb) 
{
	stringstream ss("");
	ss << vb.size() << "[" ;
	uint32_t i, p2 = 128;
	short int c = 0;
	string sep = "";
	for(i = 0; i< vb.size(); i++) {
		c = c | p2 * vb[i];
		p2 >>= 1;
		if (p2 == 0) {
			ss << sep << (int)((unsigned char)c);
			sep = " ";
			p2 = 128;
			c = 0;
		}
	}
	if (c != 0) {
		ss << sep << (int)((unsigned char)c);
	}
	ss << "]" ;
	return ss.str();
 
}
 
 
string vb2bin(vector<bool> &vb) 
{
	stringstream ss("");
	//clog << vb.size() << endl;
	uint32_t i, p2 = 128;
	short int c = 0;
	for(i = 0; i< vb.size(); i++) {
		c = c | p2 * vb[i];
		p2 >>= 1;
		if (p2 == 0) {
			ss << (char)c;
			p2 = 128;
			c = 0;
		}
	}
	if (c != 0) {
		ss << (char)c;
	}
	return ss.str();
}
*/

string vb2str(vector<bool> &vb)
{
        uint32_t i;
        stringstream ss;
        ss.clear();
        ss.str("");
        for(i = 0; i < vb.size(); i++) {
                ss << (char)(vb[i] + '0');
        }
        return ss.str();
}


vector<bool> str2vb(string s)
{
        vector<bool> vb(0);
        stringstream ss(s);
        do {
                char c;
                ss >> c;
                if (ss.good()) {
                        vb.push_back(c-'0');
                }
        } while (!ss.eof());

        return vb;
}


void codifica_unario(uint64_t x, vector<bool> &vb)
{
        ASSERT(x >= 1);

        while (x > 1) {
                vb.push_back(1);
                x--;
        }
        vb.push_back(0);
}


uint64_t decodifica_unario(vector<bool> &vb)
{
        ASSERT(vb.size() > 0);

        uint64_t x = 1;
        vector<bool>::iterator cv = vb.begin();
        short int c = *cv;
        vb.erase(cv);

        while (c == 1) {
                x++;
                cv = vb.begin();
                c = *cv;
                vb.erase(cv);
        }
        return x;
}


void pone_un_entero(uint64_t x, uint32_t nbits, vector<bool> &vb)
{
        ASSERT(x >= 0);
        ASSERT(nbits >= 0);

        int i;
        uint64_t p2 = 0;
        if (nbits>0) {
                p2 = pot2(nbits-1);
        }
        for(i = nbits - 1; i >= 0; i--) {
                unsigned char b = (x / p2) % 2;
                vb.push_back(b);
                p2 = p2 / 2;
        }
}


uint64_t toma_un_entero(uint32_t nbits, vector<bool> &vb)
{
        ASSERT(nbits >= 0);
        ASSERT(vb.size() >= nbits);

        uint64_t x = 0;
        int i;
        vector<bool>::iterator cv;
        for(i = nbits-1; i >= 0; i--) {
                cv = vb.begin();
                short int b = *cv;
                vb.erase(cv);
                x = 2 * x + b;
        }
        return x;
}


/*void codifica_elias_gama_os(std::ostream &os, uint64_t x)
{
	ASSERT(x >= 1);
 
	int b = 1 + piso_logb2(x);
	codifica_unario_os(os, b, vb);
	pone_un_entero(x - pot2(b - 1), b - 1, vb);
}
*/

void codifica_elias_gama(uint32_t x, vector<bool> &vb)
{
        ASSERT(x >= 1);

        int b = 1 + piso_logb2(x);
        codifica_unario(b, vb);
        pone_un_entero(x - pot2(b - 1), b - 1, vb);
}


uint64_t decodifica_elias_gama(vector<bool> &vb)
{
        ASSERT(vb.size() > 0);

        uint32_t b = decodifica_unario(vb);
        uint64_t x = toma_un_entero(b - 1, vb);
        return pot2(b - 1) + x;
}

/**
 * Lee de un stream un número en codificación gama de elias,
 * Lee hasta el último caracter que tenga bits del número, si hay más
 * se pierden porque esta función no los retorna.  
 * Es decir sirve para codificar de a un número en uno o más bytes,
 * Una secuencia de números quedará cada numero comenzando en un byte.
 */
uint64_t
lee_elias_gama(std::istream &is) throw (string)
{
        vector<bool> vb(0);
        vb.clear();

        //clog << "OJO lee_elias_gama(is)" << endl;
        uint32_t p2;
        int c;
        short bit;
        do {
                c = is.get();
                //clog << "lei c=" << c << endl;
                if (c == EOF) {
                        throw errorFormato(is,
                                           "Fin de archivo mientras esperaba unario");
                } else if (c == 255) {
                        vb.push_back(1);
                        vb.push_back(1);
                        vb.push_back(1);
                        vb.push_back(1);
                        vb.push_back(1);
                        vb.push_back(1);
                        vb.push_back(1);
                        vb.push_back(1);
                }
        } while (c == 255);
        p2 = 256;
        do {
                p2 >>= 1;
                //clog << "c= " << c << ", p2 =" << p2 << endl;
                bit = (c & (short)p2) > 0 ? 1 : 0;
                vb.push_back(bit);
                //clog << "empujó =" << bit << ", c= " << c << endl;
        } while (bit == 1 && p2 > 1);

        uint32_t nb = vb.size();
        //clog << "nb=" << nb << ", c= " << c << endl;
        do {
                //clog << "ac p2=" << p2 << endl;
                while (p2 > 1 && vb.size() < 1+ 2 * (nb-1)) {
                        p2 >>= 1;
                        //clog << "c=" << c << ", p2 =" << p2 << endl;
                        bit = (c & (short)p2) > 0 ? 1 : 0;
                        vb.push_back(bit);
                        //clog << "empujo " << bit << endl;
                }
                if (vb.size() < 1 + 2 * (nb-1)) {
                        c = is.get();
                        //clog << "lee c=" << c << endl;
                        if (c == EOF) {
                                stringstream ss("");
                                ss << "Fin de archivo, se esperaba binario ("
                                << vb.size() << " < " <<
                                1+ 2*(nb-1)<< ")";
                                throw errorFormato(is, ss.str());
                        }
                        p2 = 256;
                }
                //clog << "vb.size() = " << vb.size() << ", 2*nb = " << 2*nb << endl;
        } while (vb.size() < 1+ 2 * (nb-1));


        uint64_t lr = decodifica_elias_gama(vb);
        //clog << "lr = " << lr << endl;
        return lr;
}

/* Otra versión de la función anterior, pero que se espera sea rápida.
 
  codifica_elias_gama(1, vb) da 0   ->     00000000 -> 000000
  codifica_elias_gama(2, vb) da 100 ->     10000000 -> 000000
  codifica_elias_gama(3, vb) da 101
  codifica_elias_gama(4, vb) da 11000
  codifica_elias_gama(5, vb) da 11001 ->   11001000 -> 0...01
  codifica_elias_gama(6, vb) da 11010 ->   11010000 -> 0...10
  codifica_elias_gama(7, vb) da 11011 ->   11011000 -> 0...11
  codifica_elias_gama(8, vb) da 1110000 -> 11100000 -> 0..000
  codifica_elias_gama(9, vb) da 1110001 -> 11100010 -> 0..001
 
	16 es 
	CPPUNIT_ASSERT(rs[0] == (char)B8(11110000));
	CPPUNIT_ASSERT(rs[1] == (char)B8(00000000));
	
	32  es
	CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111000));
	CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));
 
	128
	CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111110));
	CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));
 
	256
	CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
	CPPUNIT_ASSERT(ss.str()[1] == (char)B8(00000000));
	CPPUNIT_ASSERT(ss.str()[2] == (char)B8(00000000));
 
	65536
	CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
	CPPUNIT_ASSERT(ss.str()[1] == (char)B8(11111111)); 
	CPPUNIT_ASSERT(ss.str()[2] == (char)B8(00000000));
	CPPUNIT_ASSERT(ss.str()[3] == (char)B8(00000000));
	CPPUNIT_ASSERT(ss.str()[4] == (char)B8(00000000));
 
  Máximo es representable en 31 bits i.e
  escribe_elias_gama(ss, (uint32_t)(pot2(32)-1));
  que da
	CPPUNIT_ASSERT(ss.str()[0] == (char)B8(11111111));
	CPPUNIT_ASSERT(ss.str()[1] == (char)B8(11111111)); 
	CPPUNIT_ASSERT(ss.str()[2] == (char)B8(11111111));
	CPPUNIT_ASSERT(ss.str()[3] == (char)B8(11111110));
	CPPUNIT_ASSERT(ss.str()[4] == (char)B8(11111111));
	CPPUNIT_ASSERT(ss.str()[5] == (char)B8(11111111));
	CPPUNIT_ASSERT(ss.str()[6] == (char)B8(11111111));
	CPPUNIT_ASSERT(ss.str()[7] == (char)B8(11111110));
 
	Así que se leen entre 1 y 8 bytes.
	1 si n está entre 2^0=1 y 2^4-1=15
	2 si n está entre 2^4=16 y 2^8-1=255
	3 si n está entre 2^8=256 y 2^12-1=4095
	4 si n está entre 2^12=4096 y 2^16-1=65535
	5 si n está entre 2^16=65536 y 2^20-1
	6 si n está entre 2^20 y 2^24-1
	7 si n está entre 2^24 y 2^28-1
	8 si n está entre 2^28 y 2^32-1
 
  */
uint64_t
lee_elias_gama2(std::istream &is) throw (string)
{
        //clog << "OJO lee_elias_gama2(is)" << endl;
        uint32_t p2;
        int c;
        short bit;
        uint32_t nbits = 0;
        uint64_t p2f = 1;

        // Primero leemos en unario en nbits
        // Al tiempo vamos calculando 2^nbits en p2f
        //clog << "OJO Por leer unario y calcular p2f" << endl;
        do {
                c = is.get();
                //clog << "OJO leido c=" << c << " = " << ulong2ascii(c, 8) << endl;
                //clog.clear();

                if (c == EOF) {
                        throw errorFormato(is,
                                           "Fin de archivo mientras esperaba unario");
                } else if (c == 255) {
                        nbits += 8;
                        p2f <<= 8;

                }
        } while (c == 255);
        //clog << "OJO Tras primer ciclo nbits=" << nbits << " y p2f=" << p2f << endl;
        p2 = 256;
        do {
                p2 >>= 1;
                bit = (c & (short)p2) > 0 ? 1 : 0;
                //clog << "OJO en segundo ciclo c= " << c << ", p2 =" << p2 << ", bit=" << bit << endl;
                if (bit == 1) {
                        nbits++;
                        p2f <<= 1;
                }
        } while (bit == 1 && p2 > 1);
        //clog << "OJO Tras segundo ciclo nbits=" << nbits << " y p2f=" << p2f << endl;

        ASSERT(nbits<=31);

        // codifica_elias_gama(7, vb) da 11011 se representa con caracter 11011000 debe quedar 0...11
        // nbits=2,  nbits%8=2

        // Ahora leemos nbits bits en binario en ui

        /* nbits%8 = 2^p2 */

        //clog << "OJO Por leer " << nbits << " bits binarios en ui" << endl;
        uint32_t bl;
        if (nbits <= 3) {
                bl = nbits;
                /*	} else if (nbits >=7) {
                		bl = 0; */
        } else {
                bl = 7 - (nbits % 8);
        }
        //clog << "OJO Bits binarios ya extraidos bl=" << bl << endl;
        uint32_t  ui = 0;
        //ss <<  (char)B8(11111001);
        //ss <<  (char)B8(00000000);
        ui = (unsigned char)(c << ((nbits % 8) + 1)); // Quitamos lo que quedaba de unario en c  0...11000000
        //clog << "OJO ui tras quitar unario corriendo es " << ui << " = " << ulong2ascii(ui, 32) << endl;
        if (bl == nbits) {
                //clog << "OJO cupo en un byte" << endl;
                ui >>= 8 - bl;    		         // Y dejamos lo que quede de binario en parte baja de ui 0...00000011
        } else {
                //clog << "No cupo en un byte" << endl;
                ui >>= (nbits % 8) + 1;
        }

        //clog << "OJO ui con lo que quedaba de binario es " << ui << " = " << ulong2ascii(ui, 32) << endl;
        while (bl < nbits) {
                c = is.get();
                //clog << "OJO en tercer ciclo bl = " << bl << ", c=" << c << "=" << ulong2ascii(c, 8) << endl;
                if ((nbits > 8) && (bl < nbits - 8)) {
                        // Quedarán más
                        ui <<= 8;
                        ui |= c;
                        bl += 8;
                        //clog << "OJO quedarán más ui = " << ui << ", bl=" << bl << endl;
                } else {
                        // Ultimo byte puede no estar totalmente lleno
                        ui <<= nbits - bl;
                        unsigned char cui = ((unsigned char)c) >> 8 - nbits + bl;
                        //clog << "OJO último nbits=" << nbits << ", bl=" << bl << ", nbits-bl=" << nbits-bl << ", cui=" << (uint32_t)cui << " = " << ulong2ascii(cui, 8) << endl;
                        ui |= cui;
                        bl = nbits;
                        //clog << "OJO último ui=" << ui << endl;
                }
        }

        uint32_t lr = p2f | ui;
        //clog << "OJO lr = " << lr << endl;
        return lr;
}

/**
 * Escribe al flujo os un entero en codificación gama de Elias.
 *
 * @param os Flujo de salida
 * @param n Número por escribir 
 **/
void
escribe_elias_gama(std::ostream &os, uint32_t n)
{
        ASSERT(n >= 1);

        //clog << "escribe_elias_gama(os, " << n << ")" << endl;
        vector<bool> vb(0);
        codifica_elias_gama(n, vb);
        //clog << "codificado es '" << vb2str(vb) << "'" << endl;
        uint32_t i, p2 = 128;
        unsigned char c = 0;
        //clog << "vb.size=" << vb.size() << endl;
        uint32_t t = vb.size();
        for(i = 0; i < t; i++) {
                if (vb[i]) {
                        c = c | p2;
                }
                //clog << "i=" << i << ", p2=" << p2 << ", vb[i]= " << vb[i] << ", c=" << c << endl;
                p2 >>= 1;
                if (p2 == 0) {
                        os << c ; //os.write((char *)&c, 1);
                        //clog << "Poniendo " << (uint32_t)c << ", sizeof(c)=" << sizeof(c) << endl;
                        p2 = 128;
                        c = 0;
                }
        }
        if (p2 != 128) {
                //clog << "typide(os)=" << typeid(os).name() << endl;
                os << c ; //os.write((char *)&c, 1);
                //clog << "Poniendo " << (uint32_t)c << endl;
        }
}


void
escribe_elias_gama2(std::ostream &os, uint32_t n)
{
        ASSERT(n >= 1);

        uint64_t r = 0;
        //clog << "OJO escribe_elias_gama2(os, " << n << ")" << endl;
        int b = piso_logb2(n);
        //clog << "OJO b=" << b << endl;

        ASSERT(b <= 31);
        //clog << "OJO codifica_unario(b)" << endl;
        for (int i = 0; i < b; i++) {
                r |= 1;
                r <<= 1;
                //clog << "OJO final de ciclo i=" << i << ", r=" << r << endl;
        }
        r <<= b;
        //clog << "OJO tras corrimient r=" << r << "=" << ulong2ascii(r) << endl;

        //clog << "pone_un_entero(x - pot2(b - 1), b - 1)" << endl;
        uint64_t m = 0;
        for (int i = 0; i < b; i++) {
                m <<= 1;
                m |= 1;
        }
        //clog << "OJO mascara m=" << m << "=" << ulong2ascii(m) << endl;
        r |= n & m;
        //clog << "n & m = " << (n & m) << ", r=" << r << "=" << ulong2ascii(r) << endl;

        int nb = 1+b/4;
        r <<= (nb*8) - (2*b + 1);
        //clog << "OJO nb=" << nb << " bytes r=" << r << "=" << ulong2ascii(r) << endl;
        m = (uint64_t)0xFF00000000000000LL;
        unsigned char c = 0;
        for (int i=0; i < 8; i++) {
                if (8-i <= nb) {
                        uint64_t ct = (uint64_t)r & (uint64_t)m;
                        //clog << " OJO en ciclo final i=" << i << ", ct1=0x" << setbase(16) << (uint64_t)ct << setbase(10) << endl;
                        ct >>= 8*(7-i);
                        //clog << " OJO en ciclo final i=" << i << ", ct2=0x" << setbase(16) << (uint64_t)ct << setbase(10) << endl;
                        c = (unsigned char)ct;
                        //clog << " OJO en ciclo final i=" << i << ", c=" <<  ulong2ascii(c, 8) << ", ct=0x" << setbase(16) << (uint64_t)ct << ", r=0x" << r << ", m=0x" << m << setbase(10) << "=" << ulong2ascii(m, 64) << endl;
                        os.write((char *)&c, 1);
                        //clog << " OJO escrito " << (uint32_t)*((char *)&c) << endl;
                } else {
                        //clog << " OJO en ciclo final i=" << i << ", m=" << setbase(16) << m << setbase(10) << " = " << ulong2ascii((uint64_t)m, 64) << endl;
                }
                m >>= 8;
        }

        /*
        	if (b<=3) { // 1 byte
        		r <<= 8 - (2*b + 1);
        		clog << "1 byte r=" << r << " p[4]=" << (int)p[4] << endl;
        		os.write(p+7, 1);
        	} else if (b<=7) { // 2 bytes
        		r <<= 16 - (2*b + 1);
        		clog << "2 bytes r=" << r << "p[3]=" << (int)p[3] << ", p[4]=" << (int)p[4] << endl;
        		os.write(p+6, 2);
        	} else if (b<=11) { // 3 bytes
        		r <<= 24 - (2*b + 1);
        		clog << "3 bytes r=" << r << "p[2]=" << (int)p[2] << ", p[3]=" << (int)p[3] << ", p[4]=" << (int)p[4] << endl;
        		os.write(p+1, 3);
        	} */

}



/**
 * Retorna espacio que requeriría en un flujo codificar un número en 
 * codificación gama de Elías
 * @param n número
 *
 * @return longitud
 */
uint32_t
long_elias_gama(uint64_t n)
{
        ASSERT(n >= 1);

        int b = 1 + piso_logb2(n);
        int nb = (b-1)*2 + 1;

        return (uint64_t)ceil((double)nb/(double)8);
}


void codifica_binaria_minima(uint64_t x, uint32_t n, vector<bool> &vb)
{
        ASSERT(x >= 0);
        ASSERT(n >= 1);

        int b = techo_logb2(n);
        uint64_t d = pot2(b) - n;
        if (x > d) {
                pone_un_entero(x - 1 + d, b, vb);
        } else {
                pone_un_entero(x - 1 , b - 1, vb);
        }
}


uint64_t decodifica_binaria_minima(uint32_t n, vector<bool> &vb)
{
        ASSERT(n >= 1);

        //clog << "decodifica_binaria_minima(" << n << ", vb)" << endl;
        uint32_t b = techo_logb2(n);

        ASSERT(vb.size() >= b - 1);

        int64_t d = pot2(b) - n;
        uint64_t x = toma_un_entero(b - 1, vb);
        if ((int64_t)(x + 1) > d) {
                vector<bool>::iterator cv = vb.begin();
                char c = *cv;
                vb.erase(cv);
                x = 2 * x + c;
                x = x - d;
        }
        return x + 1;
}
