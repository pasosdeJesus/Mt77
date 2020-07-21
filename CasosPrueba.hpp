/** @file CasosPrueba.hpp
 * Ejemplos para pruebas de unidad
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version $Id: CasosPrueba.hpp,v 1.4 2010/01/06 11:48:45 vtamara Exp $
 */

#if !defined(CasosPrueba_hpp)
#define CasosPrueba_hpp

// Indices de prueba
const string ps1 = "a{0000=00000}";
const string ps2 = "a{0000?00000\x80\x80}";
const string ps3 = "a{0000@00000\x80\x80\x80}";
const string ps4 = "a{0000=0000>}\nm{0000K00000}\n";
const string ps4_1="b{0000=0000>}\nm{0000K00000}\n";
const string ps5 = "a{0000=00000}b{0000J00000}\n";
const string ps6 = "a{0000=00000}b{0000J0000K}\nm{0000X00000}\n";
const string ps7 = "a{0000=0000K}b{0000J00000}\nm{0000X00000}\n";
//                            1         2         3         4         5
//                  012345678901234567890123456789012345678901234567890123
//                            :;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcde
const string ps8 = "a{0000=00000}b{0000L00000\x80\xC0}\n";
const string ps9 = "a{0000=00000}b{0000L00000\x80\x80}\n";
const string ps10= "a{0000=00000}b{0000M00000\x80\x80\x80}\n";
const string ps11= "am{0000>00000}\n";
const string ps12= "a{0000=0000>}\nl{0000K00000}\n";
const string ps13= "a{0000=0000>}\nl{0000K00000}m{0000X00000}\n";
const string ps14= "a{0000=0000>}\nm{0000K0000L}\no{0000Y0000Z}\nr{0000g00000}\n";
const string ps15=  MARCAIND + "\n" + 
		   "J{0000K0000L}\n"
                   "ESUS{0000^00000\x80\x80}O{0000k0000l}\n"
                   "RAM{0000}00000\xA0\x80}TAM{0000\21600000\x80\x80}\n";
//13+1+ 13+1+ 18 = 46

/** Auxiliar para llenar un stringstream con la representación plana
 * de un NodoTrieS
 */
inline void llena_ss(std::stringstream &ss, NodoTrieS *n, string pref = "")
{
        ss.clear();
        ss.str("");
        ss.seekp(-1);
        ss.seekg(0);
        ss.clear();
        ss << pref;
        escribePlanoStream(n, ss, 0);
        ss.seekg(0);
}


#endif
