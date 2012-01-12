// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file TrieSDiscoCasoPrueba.cpp
 * Pruebas de regresión a TrieSDisco.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: TrieSDiscoCasoPrueba.cpp,v 1.21 2010/01/18 16:12:50 vtamara Exp $
 */

#include <sstream>
#include <iostream>
#include <fstream>

#include "TrieSDiscoCasoPrueba.hpp"
#include "TrieSDisco.hpp"
#include "NodoTrieS.hpp"
#include "RamDisco.hpp"
#include "CasosPrueba.hpp"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(TrieSDiscoCasoPrueba);

void TrieSDiscoCasoPrueba::prueba_escribeNodo()
{
        set<Pos> *cp =  new set<Pos>();
        CPPUNIT_ASSERT(precalcula_escribe_actual(1, cp) == 13);
        CPPUNIT_ASSERT(precalcula_escribe_actual(2, cp) == 14);
        cp->insert(Pos(1,1));
        CPPUNIT_ASSERT(precalcula_escribe_actual(1, cp) == 15);


        std::stringstream ss;
        ss.clear();
        ss.str("");
        CPPUNIT_ASSERT_THROW(escribeNodo(ss, "", NULL, 0), std::string);
        ss.clear();
        ss.str("");
        long r = escribeNodo(ss, "a", NULL, 0);
        //cout << "r=" << r << "  ss.str=" << ss.str() << " size=" << ss.str().size() << endl;
        CPPUNIT_ASSERT(r == 7 && ss.str() == ps1);
        set<Pos> p;
        p.insert(Pos(2,2));
        ss.clear();
        ss.str("");
        r = escribeNodo(ss, "a", &p, 0);
        //cout << "r=" << r << "  ss.str=" << ss.str() << " size=" << ss.str().size() << endl;
        //cout << "ps2=" << ps2 << " size=" << ps2.size() << endl;
        CPPUNIT_ASSERT(r== 7 && ss.str() == ps2);

        p.insert(Pos(2,4));
        ss.clear();
        ss.str("");
        r = escribeNodo(ss, "a", &p, 0);
        //cout << "r=" << r << "  ss.str=" << ss.str() << " size=" << ss.str().size() << endl;
        CPPUNIT_ASSERT(r == 7 &&
                       ss.str() == ps3);
        //cout << "Terminó prueba_escribeNodo" << endl;

}

void TrieSDiscoCasoPrueba::prueba_escribeCopiaNodo()
{
        uint32_t r, h;
        std::stringstream sos;
        std::stringstream sis;

        sos.clear();
        sos.str("");
        sis.clear();
        sis.str("");
        r = escribeCopiaNodo(sos, sis, h, NULL);
        CPPUNIT_ASSERT(sos.str()==sis.str() );
        CPPUNIT_ASSERT(r == 0);

        sos.clear();
        sos.str("");
        sis.clear();
        sis.str("x");
        CPPUNIT_ASSERT_THROW(escribeCopiaNodo(sos, sis, h, NULL), std::string);

        sos.clear();
        sos.str("");
        sis.clear();
        sis.str(ps1);
        r = escribeCopiaNodo(sos, sis, h, NULL);
        //cout << "r=" << r << ", h=" << h << endl;
        //cout << "sis='"<<sis.str()<<"'"<<endl;
        //cout << "sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sis.str());
        CPPUNIT_ASSERT(h == 0);
        CPPUNIT_ASSERT(r == 7);

        sos.clear();
        sos.str("");
        sis.clear();
        sis << ps2;
        sis.clear();
        sis.seekg(0);
        sis.seekp(0);
        //cout << "sis='"<<sis.str()<<"', long=" << sis.str().length() <<endl;
        r = escribeCopiaNodo(sos, sis, h, NULL);
        //cout << "r=" << r << ", h=" << h << endl;
        //cout << "sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sis.str());
        CPPUNIT_ASSERT(h == 0);
        CPPUNIT_ASSERT(r == 7);

        sos.clear();
        sos.str("");
        sis.clear();
        sis << ps3;
        sis.clear();
        sis.seekg(0);
        sis.seekp(0);
        //cout << "sis='"<<sis.str()<<"', long=" << sis.str().length() <<endl;
        r = escribeCopiaNodo(sos, sis, h, NULL);
        //cout << "r=" << r << ", h=" << h << endl;
        //cout << "sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str()==sis.str());
        CPPUNIT_ASSERT(h == 0);
        CPPUNIT_ASSERT(r == 7);

        sos.clear();
        sos.str("");
        sis.clear();
        sis.str(ps4);
        r = escribeCopiaNodo(sos, sis, h, NULL);
        //cout << "r=" << r << ", h=" << h << ", sos.str()=" << sos.str() << endl;
        CPPUNIT_ASSERT(sos.str() == "a{0000=0000>}" );
        CPPUNIT_ASSERT(r == 7);
        CPPUNIT_ASSERT(h == 14);

}


extern stringstream *depuraos;


/*void TrieSDiscoCasoPrueba::prueba_escribePlanoStream()
{	
	std::stringstream sis;
 
	NodoTrieS *nr = new NodoTrieS("r", NULL, NULL, set<Pos>());
	NodoTrieS *no = new NodoTrieS("o", nr, NULL, set<Pos>());
	NodoTrieS *nm = new NodoTrieS("m", no, NULL, set<Pos>());
	NodoTrieS *na = new NodoTrieS("a", nm, NULL, set<Pos>());
	//depuraos=&sis;
	sis.clear(); sis.str("");sis.seekp(-1);sis.seekg(0); sis.clear(); 
	escribePlanoStream(na, sis);
	sis.seekg(0);
	delete na; na = NULL;
	//cout << "sis=" << sis.str() << endl;
	CPPUNIT_ASSERT(sis.str() == ps14 );
 
	NodoTrieS *ns = new NodoTrieS("s", NULL, NULL, set<Pos>());
	nr = new NodoTrieS("r", ns, NULL, set<Pos>());
	no = new NodoTrieS("o", nr, NULL, set<Pos>());
	nm = new NodoTrieS("m", no, NULL, set<Pos>());
	na = new NodoTrieS("a", nm, NULL, set<Pos>());
	//na->aDotty(cerr, "");
	llena_ss(sis, na);
	delete na; na = NULL;
	//cout << "sis='" << sis.str() << "'" << endl;
	sis.seekg(0);
	CPPUNIT_ASSERT(sis.str() == "a{0000=0000>}\nm{0000K0000L}\no{0000Y0000Z}\nr{0000g0000h}\ns{0000u00000}\n");
 
	NodoTrieS *nb = new NodoTrieS("b", NULL, NULL, set<Pos>());
	no = new NodoTrieS("o", NULL, NULL, set<Pos>());
	nm = new NodoTrieS("m", no, NULL, set<Pos>());
	na = new NodoTrieS("a", nm, nb, set<Pos>());
 
	//depuraos = &sis;
	llena_ss(sis, na);
	delete na; na = NULL;
	//cout << "sis='" << sis.str() << "'" << endl;
	sis.seekg(0);
	CPPUNIT_ASSERT(sis.str() == "a{0000=0000K}b{0000J00000}\nm{0000X0000Y}\no{0000f00000}\n");
 
	na = new NodoTrieS();
	na->inserta("CONOCEREIS", Pos(1,1));
	na->inserta("LA", Pos(1,2));
	na->inserta("VERDAD", Pos(1,3));
	na->inserta("Y", Pos(1,4));
	na->inserta("LA", Pos(1,5));
	na->inserta("VERDAD", Pos(1,6));
	na->inserta("OS", Pos(1,7));
	na->inserta("HARA", Pos(1,8));
	na->inserta("LIBRES", Pos(1,9));
 
	//na->aDotty(cout, "");
 
	llena_ss(sis, na);
	//cout << "sis='"<<sis.str().length()<<"'"<<endl;
	//cout << "precalcula_escribe='"<<precalcula_escribe(na)<<"'"<<endl; 
	sis.seekg(0);
	CPPUNIT_ASSERT(sis.str().length() == precalcula_escribe(na));
	delete na; na = NULL;
 
	na = new NodoTrieS();
	na->inserta("CINTURA", Pos(2, 1));
	na->inserta("CONTABAN", Pos(2, 15));
	na->inserta("COMFESABAN", Pos(2, 11));
	na->inserta("COMIA", Pos(2, 8));
	na->inserta("EM", Pos(1, 1));
	//na->aDotty(cout, "");
	llena_ss(sis, na);
	//cout << sis.str() << endl;
	CPPUNIT_ASSERT(sis.str().length() == precalcula_escribe(na));
	delete na;
}
*/

void TrieSDiscoCasoPrueba::prueba_escribeCopiaSubarbol()
{
        long r;
        std::stringstream sos;
        std::stringstream sis;

        sos.clear();
        sos.str("");
        sis.clear();
        sis.str("");
        escribeCopiaSubarbol(sos, sis, false, NULL);
        CPPUNIT_ASSERT(sos.str()==sis.str() );
        sos.clear();
        sos.str("");
        sis.clear();
        sis.str("x");
        CPPUNIT_ASSERT_THROW(escribeCopiaSubarbol(sos, sis,
                             false, NULL), std::string);
        sos.clear();
        sos.str("");
        sis.clear();
        sis.clear();
        sis.str(ps3 + "\n");
        //sis << ps3 << (char)0 << (char)0 << (char)0 << "}" << "\n";
        sis.clear();
        sis.seekg(0);
        sis.seekp(0);
        //cout << "sis='"<<sis.str()<<"', long="<< sis.str().length() << endl;
        r = escribeCopiaSubarbol(sos, sis, false, NULL);
        //cout << "sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sis.str() );

        sos.clear();
        sos.str("");
        sis.clear();
        sis.str(ps4);
        //cout << "sis='"<<sis.str()<<"'"<<endl;
        r = escribeCopiaSubarbol(sos, sis, true, NULL);
        //cout << "sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str()==sis.str() );


        NodoTrieS *nr = new NodoTrieS("r", NULL, NULL, set<Pos>());
        NodoTrieS *no = new NodoTrieS("o", nr, NULL, set<Pos>());
        NodoTrieS *nm = new NodoTrieS("m", no, NULL, set<Pos>());
        NodoTrieS *na = new NodoTrieS("a", nm, NULL, set<Pos>());
        //depuraos=&sos;
        llena_ss(sis, na);
        delete na;
        na = NULL;
        //cout << "sis=" << sis.str() << endl;
        ASSERT(sis.str() == ps14);
        sis.seekg(0);
        sos.clear();
        sos.str("");
        r = escribeCopiaSubarbol(sos, sis, true, NULL);
        //cout << "sis='"<<sis.str()<<"'"<<endl;
        //cout << "sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str()==sis.str() );

        NodoTrieS *ns = new NodoTrieS("s", NULL, NULL, set<Pos>());
        nr = new NodoTrieS("r", ns, NULL, set<Pos>());
        no = new NodoTrieS("o", nr, NULL, set<Pos>());
        nm = new NodoTrieS("m", no, NULL, set<Pos>());
        na = new NodoTrieS("a", nm, NULL, set<Pos>());

        //na->aDotty(cerr, "");
        sis.clear();
        sis.str("");
        llena_ss(sis, na);
        sos.clear();
        sos.str("");
        r = escribeCopiaSubarbol(sos, sis, true, NULL);
        //cout << "sis='"<<sis.str()<<"'"<<endl;
        //cout << "sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str()==sis.str() );

        delete na;
        na = NULL;

        NodoTrieS *nb = new NodoTrieS("b", NULL, NULL, set<Pos>());
        ns = new NodoTrieS("s", NULL, NULL, set<Pos>());
        nr = new NodoTrieS("r", ns, NULL, set<Pos>());
        no = new NodoTrieS("o", nr, NULL, set<Pos>());
        nm = new NodoTrieS("m", no, NULL, set<Pos>());
        na = new NodoTrieS("a", nm, nb, set<Pos>());

        //na->aDotty(cerr, "");
        sos.clear();
        sos.str("");
        sis.clear();
        sis.str("");
        llena_ss(sis, na);
        delete na;
        na = NULL;
        //cout << "sis='"<<sis.str()<<"'"<<endl;
        sis.seekg(0);
        r = escribeCopiaSubarbol(sos, sis, true, NULL);
        //cout << "sis='"<<sis.str()<<"'"<<endl;
        //cout << "sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str()==sis.str() );


        na = new NodoTrieS();
        na->inserta("CONOCEREIS", Pos(1,1));
        na->inserta("LA", Pos(1,2));
        na->inserta("VERDAD", Pos(1,3));
        na->inserta("Y", Pos(1,4));
        na->inserta("LA", Pos(1,5));
        na->inserta("VERDAD", Pos(1,6));
        na->inserta("OS", Pos(1,7));
        na->inserta("HARA", Pos(1,8));
        na->inserta("LIBRES", Pos(1,9));

        sos.clear();
        sos.str("");
        llena_ss(sis, na);
        delete na;
        na = NULL;
        //cout << "V sis="<< sis.str() << endl;
        sis.seekg(0);
        r = escribeCopiaSubarbol(sos, sis, true, NULL);
        //cout << "sis="<< sis.str() << ", sos="<<sos.str()<<", r="<<r<<endl;
        CPPUNIT_ASSERT(sos.str()==sis.str() );

        //cout << "Probamos sin escribir hermanos";
        sos.clear();
        sos.str("");
        sis.clear();
        sis.str(ps5);
        r = escribeCopiaSubarbol(sos, sis, false, NULL);
        //cout << "sis='"<< sis.str() << "', sos='"<<sos.str()<<"', r="<<r<<endl;
        CPPUNIT_ASSERT(sos.str() == ps1 + "\n" );

        sos.clear();
        sos.str("");
        sis.clear();
        sis.str(ps6);
        r = escribeCopiaSubarbol(sos, sis, false, NULL);
        //cout << "sis='"<< sis.str() << "', sos='"<<sos.str()<<"', r="<<r<<endl;
        CPPUNIT_ASSERT(sos.str() == ps1 + "\n" );


        sos.clear();
        sos.str("");
        sis.clear();
        sis.str(ps7);
        r = escribeCopiaSubarbol(sos, sis, false, NULL);
        //cout << "sis='"<< sis.str() << "', sos='"<<sos.str()<<"', r="<<r<<endl;
        CPPUNIT_ASSERT(sos.str() == ps4 );

        //cout << "Probamos escribiendo en un sos no vacío";
        sos.clear();
        sos.str("");
        ;
        sos << ps1;
        sis.clear();
        sis.str("b{0000=00000}\n");
        r = escribeCopiaSubarbol(sos, sis, true, NULL);
        CPPUNIT_ASSERT(sos.str() == ps5 );

        sos.clear();
        sos.str("");
        ;
        sos << ps1;
        sis.clear();
        sis.str(ps4_1);
        r = escribeCopiaSubarbol(sos, sis, true, NULL);
        //cout<<"sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == ps6);

        //cout << endl << "Probamos renumerando a -1 es decir borrando" << endl;
        vector<int64_t> renum;
        renum.push_back(-1); // Borramos el primero
        renum.push_back(-1); // Borramos el segundo también

        sos.clear();
        sos.str("");
        sis.clear();
        sis.str(ps5);
        r = escribeCopiaSubarbol(sos, sis, true, &renum);
        //cout << "ps5 sis='"<< sis.str() << "', sos='"<<sos.str()<<"', r="<<r<<endl;
        CPPUNIT_ASSERT(sos.str() == ps5 );

        sos.clear();
        sos.str("");
        sis.clear();
        sis.str(ps8);
        r = escribeCopiaSubarbol(sos, sis, true, &renum);
        //cout << "ps8 sis='"<< sis.str() << "', sos='"<<sos.str()<<"', r="<<r<<endl;
        CPPUNIT_ASSERT(sos.str() == ps5 );
        //cout << "paso" << endl;

        sos.clear();
        sos.str("");
        sis.clear();
        sis.str(ps15);
        renum[1]=0; // El segundo lo convertimos en primero
        renum.push_back(0); // El tercero en primero
        //	try {
        r = escribeCopiaSubarbol(sos, sis, true, &renum);
        /*	}
        	try {
        	}
        	catch (std::string s) {
        		cerr << s << endl;
        		exit(1);
        	} */
        //cout << "ps15 sis='"<< sis.str() << "', sos='"<<sos.str()<<"', r="<<r<<endl;
        sos.seekg(MARCAIND.length() + 1);
        NodoTrieS *l2 = leePlanoStream(sos);
        //l2->aDotty(cout, "");
        //cout << "ps15 preorden " << l2->preorden() << endl;
        CPPUNIT_ASSERT(l2->preorden() == "JESUSORAMTAM" );
        set<Pos> cr=l2->busca("JESUS");
        //cout << "buscó cr.size=" << cr.size() << endl;
        CPPUNIT_ASSERT(cr.size() == 1);
        //cout << "si" << endl;
        cr.clear();
        delete l2;

        na = new NodoTrieS();
        na->inserta("CONOCEREIS", Pos(1,1));
        na->inserta("Y", Pos(1,4));
        na->inserta("LA", Pos(1,5));
        na->inserta("LA", Pos(2,2));
        na->inserta("LIBRES", Pos(2,9));

        cr=na->busca("LA");
        //cout << "busco 2" << endl;
        CPPUNIT_ASSERT(cr.size() == 2);
        cr.clear();
        cr=na->busca("Y");
        //cout << "busco 3" << endl;
        CPPUNIT_ASSERT(cr.size() == 1);
        //cout << "paso busco 3" << endl;
        cr.clear();

        sos.clear();
        sos.str("");
        llena_ss(sis, na);
        delete na;
        na = NULL;
        //cout << "V-1 sis="<< sis.str() << endl;
        sis.seekg(0);
        r = escribeCopiaSubarbol(sos, sis, true, &renum);
        //cout << "sis="<< sis.str() << ", sos="<<sos.str()<<", r="<<r<<endl;
        l2 = leePlanoStream(sos);
        //l2->aDotty(cout, "");
        //cout << "V-1 preorden " << l2->preorden() << endl;
        CPPUNIT_ASSERT(l2->preorden() == "CONOCEREISLAIBRESY" );
        cr=l2->busca("LA");
        //cout << "V-1 cr->size=" << cr.size() << endl;
        CPPUNIT_ASSERT(cr.size() == 1);
        cr.clear();
        cr=l2->busca("Y");
        CPPUNIT_ASSERT(cr.size() == 0);
        //cout << "V-1 busco 2" << endl;
        cr.clear();
        delete l2;
        l2 = NULL;

}


void TrieSDiscoCasoPrueba::prueba_mezclarec()
{
        std::stringstream sos;
        std::stringstream sis1;
        std::stringstream sis2;

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str("");
        sis2.clear();
        sis2.str("");
        mezclaRec(sis1, sis2, sos, false, false, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == "\n" );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str("\n");
        sis2.clear();
        sis2.str("\n");
        mezclaRec(sis1, sis2, sos, false, false, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == "\n" );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str("\n");
        sis2.clear();
        sis2.str("\n");
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == "\n" );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str("x");
        sis2.clear();
        sis2.str("\n");
        CPPUNIT_ASSERT_THROW(mezclaRec(sis1, sis2, sos, false, false, NULL, NULL),
                             std::string);

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps1 + "\n");
        sis2.clear();
        sis2.str("\n");
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        //cout << sos.str() << endl;
        CPPUNIT_ASSERT(sos.str() == (ps1 + "\n") );

        //cout << "Comenzamos" << endl;
        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps4);
        sis2.clear();
        sis2.str("\n");
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        //cout << "1 sos='" << sos.str() << "'" << endl;
        CPPUNIT_ASSERT(sos.str() == ps4 );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str("\n");
        sis2.clear();
        sis2.str(ps1 + "\n");
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == (ps1 + "\n") );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str("\n");
        sis2.clear();
        sis2.str(ps4);
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        //cout << "2 sos='" << sos.str() << "'" << endl;
        CPPUNIT_ASSERT(sos.str() == ps4);


        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps2 + "\n");
        sis2.clear();
        sis2.str("\n");
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == (ps2 + "\n") );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps1 + "\n");
        sis2.clear();
        sis2.str(ps1 + "\n");
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == (ps1 + "\n") );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps4);
        sis2.clear();
        sis2.str(ps4);
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == ps4 );


        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps2 + "\n");
        sis2.clear();
        sis2.str(ps2 + "\n");
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == (ps2 + "\n") );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps2 + "\n");
        sis2.clear();
        sis2.str(ps3 + "\n");
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == (ps3 + "\n") );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps5);
        sis2.clear();
        sis2.str(ps5);
        mezclaRec(sis1, sis2, sos, false, false, NULL, NULL);
        //cout << "sos='" << sos.str() << "'" << endl;
        CPPUNIT_ASSERT(sos.str() == (ps1 + "\n") );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps8);
        sis2.clear();
        sis2.str(ps9);
        mezclaRec(sis1, sis2, sos, true, false, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == ps8 );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps8);
        sis2.clear();
        sis2.str(ps9);
        mezclaRec(sis1, sis2, sos, false, true, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == ps9);

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps8);
        sis2.clear();
        sis2.str(ps9);
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        //cout << "sos=" << sos.str() << endl;
        CPPUNIT_ASSERT(sos.str() == ps10);

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps9);
        sis2.clear();
        sis2.str(ps8);
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == ps10);

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps1 + "\n");
        sis2.clear();
        sis2.str(ps11);
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        //cout<<"ps4 sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == ps4 );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps11);
        sis2.clear();
        sis2.str(ps1 + "\n");
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        //cout<<"sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == ps4 );


        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps12);
        sis2.clear();
        sis2.str(ps11);
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        //cout<<"ps13 sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == ps13 );

        sos.clear();
        sos.str("");
        sis1.clear();
        sis1.str(ps11);
        sis2.clear();
        sis2.str(ps12);
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        //cout<<"sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == ps13 );


        NodoTrieS *nam = new NodoTrieS("am", NULL, NULL, set<Pos>());
        NodoTrieS *na = new NodoTrieS();
        na->inserta("a", Pos(1,1));


        sos.clear();
        sos.str("");
        llena_ss(sis1, nam);
        llena_ss(sis2, na);
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);

        NodoTrieS *nm = new NodoTrieS("m", NULL, NULL, set<Pos>());
        na->hijo_menor = nm;
        std::stringstream sist;
        llena_ss(sist, na);

        //cout<<"sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sist.str());
        delete na;  // Borra a nm también
        delete nam;


        sos.clear();
        sos.str("");
        nam = new NodoTrieS("am", NULL, NULL, set<Pos>());
        llena_ss(sis1, nam);
        NodoTrieS *no = new NodoTrieS("o", NULL, NULL, set<Pos>());
        na = new NodoTrieS("a", no, NULL, set<Pos>());
        llena_ss(sis2, na);

        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        delete na; // Borra también no
        delete nam;

        no = new NodoTrieS("o", NULL, NULL, set<Pos>());
        nm = new NodoTrieS("m", NULL, no , set<Pos>());
        na = new NodoTrieS("a", nm, NULL, set<Pos>());
        llena_ss(sist, na);
        //cout<<"amo sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sist.str() );

        delete na;


        NodoTrieS *naa = new NodoTrieS("aa", NULL, NULL, set<Pos>());
        NodoTrieS *nab = new NodoTrieS();
        nab->inserta("ab", Pos(1,1));
        llena_ss(sis1, naa);
        llena_ss(sis2, nab);
        sos.clear();
        sos.str("");
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        delete naa;
        delete nab;
        NodoTrieS *nb = new NodoTrieS();
        nb->inserta("b", Pos(1,1));
        NodoTrieS *na2 = new NodoTrieS("a", NULL, nb, set<Pos>());
        na = new NodoTrieS("a", na2, NULL, set<Pos>());
        llena_ss(sist,na);
        //cout<<"aaab sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sist.str() );
        delete na;


        sos.clear();
        sos.str("");
        nm = new NodoTrieS("m", NULL, NULL, set<Pos>());
        naa = new NodoTrieS("aa", nm, NULL, set<Pos>());
        llena_ss(sis1, naa);
        delete naa;
        nm = new NodoTrieS("m", NULL, NULL, set<Pos>());
        nab = new NodoTrieS("ab", nm, NULL, set<Pos>());
        llena_ss(sis2, nab);
        delete nab;
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        NodoTrieS *nm2 = new NodoTrieS("m", NULL, NULL, set<Pos>());
        nm = new NodoTrieS("m", NULL, NULL, set<Pos>());
        nab = new NodoTrieS("b", nm, NULL, set<Pos>());
        naa = new NodoTrieS("a", nm2, nab, set<Pos>());
        na = new NodoTrieS("a", naa, NULL, set<Pos>());
        llena_ss(sist, na);
        delete na;
        //cout<<"mbaa sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sist.str() );


        sos.clear();
        sos.str("");
        nab = new NodoTrieS("ab", NULL, NULL, set<Pos>());
        llena_ss(sis1, nab);
        delete nab;
        naa = new NodoTrieS();
        naa->inserta("aa", Pos(1,1));
        llena_ss(sis2, naa);
        delete naa;
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        nb = new NodoTrieS("b");
        naa = new NodoTrieS("a",NULL, nb, Pos(1,1));
        na = new NodoTrieS("a", naa, NULL);
        llena_ss(sist, na);
        delete na;
        //cout<<"baa sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sist.str() );

        sos.clear();
        sos.str("");
        nm = new NodoTrieS("m");
        nab = new NodoTrieS("ab", nm, NULL);
        llena_ss(sis1, nab);
        delete nab;
        nm = new NodoTrieS("m");
        naa = new NodoTrieS("aa", nm, NULL);
        llena_ss(sis2, naa);
        delete naa;
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);

        nm = new NodoTrieS("m");
        nab = new NodoTrieS("b", nm);
        nm2 = new NodoTrieS("m");
        naa = new NodoTrieS("a", nm2, nab);
        na = new NodoTrieS("a", naa);
        llena_ss(sist, na);
        delete na;
        //cout<<"mbmaa sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sist.str());

        sos.clear();
        sos.str("");
        na = new NodoTrieS("a");
        llena_ss(sis1, na);
        delete na;
        nb = new NodoTrieS("b", NULL, NULL, Pos(1,1));
        llena_ss(sis2, nb);
        delete nb;
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        nb = new NodoTrieS("b", NULL, NULL, Pos(1,1));
        na = new NodoTrieS("a", NULL, nb);
        llena_ss(sist, na);
        delete na;
        //cout<<"abba sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sist.str() );

        sos.clear();
        sos.str("");
        NodoTrieS *nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        na = new NodoTrieS("a", nm);
        llena_ss(sis1, na);
        delete na;
        na = new NodoTrieS();
        na->inserta("a", Pos(1,1));
        llena_ss(sis2, na);
        delete na;
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        na = new NodoTrieS("a", nm);
        na->inserta("a", Pos(1,1));
        llena_ss(sist, na);
        delete na;
        //cout<<"amor sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sist.str());

        sos.clear();
        sos.str("");
        na = new NodoTrieS();
        na->inserta("a", Pos(1,1));
        llena_ss(sis1, na);
        delete na;
        nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        na = new NodoTrieS("a", nm);
        llena_ss(sis2, na);
        delete na;
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        na = new NodoTrieS("a", nm);
        na->inserta("a", Pos(1,1));
        llena_ss(sist, na);
        delete na;
        //cout<<"amora sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sist.str());

        //cout << "Sin escribir hermanos" << endl;
        sos.clear();
        sos.str("");
        nb = new NodoTrieS("b");
        na = new NodoTrieS("a", NULL, nb);
        llena_ss(sis1, na);
        delete na;
        na = new NodoTrieS("a");
        llena_ss(sis2, na);
        delete na;
        mezclaRec(sis1, sis2, sos, false, true, NULL, NULL);
        CPPUNIT_ASSERT(sos.str() == sis2.str() );

        sos.clear();
        sos.str("");
        na = new NodoTrieS("a");
        llena_ss(sis1, na);
        delete na;
        nb = new NodoTrieS("b");
        na = new NodoTrieS("a", NULL, nb);
        llena_ss(sis2, na);
        delete na;
        mezclaRec(sis1, sis2, sos, true, false, NULL, NULL);
        //cout<<"sinh aba sos1='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sis1.str() );
        /*
         *        1         2         3         4         5         6         7
        01234567890123456789012345678901234567890123456789012345678901234567890123456789
        a{}0000000c\a{}00000023b{}0000002F\m{}00000000\m{}00000000
        */

        sos.clear();
        sos.str("");
        na = new NodoTrieS("a");
        llena_ss(sis1, na);
        delete na;
        nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        na = new NodoTrieS("a", nm);
        na->inserta("a", Pos(1,1));
        llena_ss(sis2, na);
        delete na;
        mezclaRec(sis1, sis2, sos, true, false, NULL, NULL);
        //cout<<"sinh amor sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str()== sis2.str());

        sos.clear();
        sos.str("");
        //depuraos=&sos;
        nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        na = new NodoTrieS("a", nm);
        na->inserta("a", Pos(1,1));
        llena_ss(sis1, na);
        delete na;
        na = new NodoTrieS("a");
        llena_ss(sis2, na);
        delete na;
        mezclaRec(sis1, sis2, sos, false, false, NULL, NULL);
        //cout<<"sinh amor2 sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sis1.str());


        // En un sos no vacío

        sos.clear();
        sos.str("");
        sos << "a{0000=00000}";
        NodoTrieS *nc = new NodoTrieS("c");
        nb = new NodoTrieS("b", NULL, nc);
        llena_ss(sis1, nb);
        delete nb;
        nb = new NodoTrieS("b");
        llena_ss(sis2, nb);
        delete nb;
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        nc = new NodoTrieS("c");
        nb = new NodoTrieS("b", NULL, nc);
        na = new NodoTrieS("a", NULL, nb);
        llena_ss(sist, na);
        delete na;
        CPPUNIT_ASSERT(sos.str() == sist.str());

        sos.clear();
        sos.str("");
        sos << "a{0000=00000}";
        nc = new NodoTrieS("c");
        nb = new NodoTrieS("b", nc);
        llena_ss(sis1, nb);
        delete nb;
        nb = new NodoTrieS("b");
        llena_ss(sis2, nb);
        delete nb;
        mezclaRec(sis1, sis2, sos, true, true, NULL, NULL);
        nc = new NodoTrieS("c");
        nb = new NodoTrieS("b", nc);
        na = new NodoTrieS("a", NULL, nb);
        llena_ss(sist, na);
        delete na;
        CPPUNIT_ASSERT(sos.str() == sist.str() );

        sos.clear();
        sos.str("");
        na = new NodoTrieS();
        na->inserta("a", Pos(1,1));
        llena_ss(sis1, na);
        delete na;
        nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        na = new NodoTrieS("a", nm);
        llena_ss(sis2, na);
        delete na;
        vector<int64_t> renum;
        renum.push_back(0);
        mezclaRec(sis1, sis2, sos, true, true, NULL, &renum);
        //cout<<"renum sos='"<<sos.str()<<"'"<<endl;
        nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        na = new NodoTrieS("a", nm);
        na->inserta("a", Pos(1,1));
        llena_ss(sist, na);
        delete na;
        CPPUNIT_ASSERT(sos.str() == sist.str());

        sos.clear();
        sos.str("");
        renum.clear();
        renum.push_back(1);
        nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        na = new NodoTrieS("a", nm);
        llena_ss(sis1, na);
        delete na;
        na = new NodoTrieS();
        na->inserta("a", Pos(1,1));
        llena_ss(sis2, na);
        delete na;
        //cout<<"renum amor por Mezclar"<<endl;
        mezclaRec(sis1, sis2, sos, true, true, NULL, &renum);
        nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        na = new NodoTrieS("a", nm);
        na->inserta("a", Pos(2,1));
        llena_ss(sist, na);
        delete na;
        //cout<<"renum sist='"<<sist.str()<<"'"<<endl;
        //cout<<"sos='"<<sos.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sist.str());

        sos.clear();
        sos.str("");
        renum.clear();
        renum.push_back(0);
        nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        na = new NodoTrieS("a", nm);
        llena_ss(sis1, na);
        delete na;
        nm = new NodoTrieS();
        nm->inserta("m", Pos(1, 1));
        NodoTrieS *nl = new NodoTrieS("l", NULL, nm);
        na = new NodoTrieS("a", nl);
        llena_ss(sis2, na);
        delete na;
        mezclaRec(sis1, sis2, sos, true, true, NULL, &renum);
        nr = new NodoTrieS("r");
        no = new NodoTrieS("o", nr);
        nm = new NodoTrieS("m", no);
        nm->inserta("m", Pos(1, 1));
        nl = new NodoTrieS("l", NULL, nm);
        na = new NodoTrieS("a", nl);
        llena_ss(sist, na);
        delete na;
        //cout<<"2 sos='"<<sos.str()<<"'"<<endl;
        //cout<<"2 sist='"<<sist.str()<<"'"<<endl;
        CPPUNIT_ASSERT(sos.str() == sist.str());

        //depuraos = &sos;
        na = new NodoTrieS();
        na->inserta("CINTURA", Pos(1, 1));
        na->inserta("CONTABAN", Pos(1, 15));
        na->inserta("COMFESABAN", Pos(1, 11));
        na->inserta("COMIA", Pos(1, 8));
        sos.clear();
        sos.str(MARCAIND + "\n");
        sos.seekp(MARCAIND.length() +1);
        sos.seekg(MARCAIND.length() +1);
        sos.clear();
        escribePlanoStream(na, sos);
        sos.seekg(0);
        // cout << "COMIA sos=" << sos.str() << ", l=" << sos.str().length() << endl;
        sos.seekg(0);
        CPPUNIT_ASSERT(sos.str().length() == 134);
        delete na;

        //depuraos=&sos;
        sos.clear();
        sos.str("");
        renum.clear();
        renum.push_back(1);
        sos << MARCAIND << "\n";
        na = new NodoTrieS();
        na->inserta("EM", Pos(1, 1));
        llena_ss(sis1, na);
        delete na;
        na = new NodoTrieS();
        na->inserta("CINTURA", Pos(1, 1));
        na->inserta("CONTABAN", Pos(1, 15));
        na->inserta("COMFESABAN", Pos(1, 11));
        na->inserta("COMIA", Pos(1, 8));
        sis2.clear();
        sis2.str("");
        /*	sis2.str(MARCAIND + "\n");
        	sis2.seekp(MARCAIND.length() +1);
        	sis2.seekg(MARCAIND.length() +1);
        	sis2.clear(); */
        escribePlanoStream(na, sis2);
        //cout << "sis2.str()=" << sis2.str() << endl;
        sis2.seekg(0);
        delete na;
        //sis2.seekg(MARCAIND.length() +1);
        mezclaRec(sis1, sis2, sos, true, true, NULL, &renum);
        na = new NodoTrieS();
        na->inserta("CINTURA", Pos(2, 1));
        na->inserta("CONTABAN", Pos(2, 15));
        na->inserta("COMFESABAN", Pos(2, 11));
        na->inserta("COMIA", Pos(2, 8));
        na->inserta("EM", Pos(1, 1));
        //na->aDotty(cout, "");
        sist.clear();
        sist.str("");
        sist.seekp(-1);
        sist.seekg(0);
        sist.clear();
        escribePlanoStream(na, sist, MARCAIND.length() + 1);
        sist.seekg(0);
        delete na;
        //cout<<"sis1='"<<sis1.str()<<"'"<<endl;
        //cout<<"sis2='"<<sis2.str()<<"'"<<endl;
        //cout<<"sist='"<<MARCAIND << "\n" << sist.str()<<"'"<<endl;
        //cout<<"sos='"<<sos.str()<<"'"<<endl;

        /*sist.str(MARCAIND + "\n" + sist.str());
        sist.seekg(MARCAIND.length() + 1);
        NodoTrieS *l1 = leePlanoStream(sist);
        l1->aDotty(cout, "");
        sos.seekg(MARCAIND.length() + 1);
        NodoTrieS *l2 = leePlanoStream(sos);
        l2->aDotty(cout, ""); */

        CPPUNIT_ASSERT(sos.str() == (MARCAIND + "\n" + sist.str()));
        CPPUNIT_ASSERT((long)sos.tellp() == (long)sos.str().size() );

        sos.clear();
        sos.str("");
        renum.clear();
        renum.push_back(1);
        renum.push_back(1);
        renum.push_back(2);
        sos << MARCAIND << "\n";
        na = new NodoTrieS();
        na->inserta("JESUS", Pos(2, 2));
        llena_ss(sis1, na, MARCAIND + "\n");
        delete na;
        //	cout << "JESUS sis1=" << sis1.str() << endl;
        na = new NodoTrieS();
        na->inserta("JORAM", Pos(3, 2));
        na->inserta("JOTAM", Pos(2, 2));
        llena_ss(sis2, na, MARCAIND + "\n");
        delete na;
        //cout << "JORAM JOTAM sis2=" << sis2.str() << endl;
        try {
                sis1.seekg(MARCAIND.length() + 1);
                sis2.seekg(MARCAIND.length() + 1);
                mezclaRec(sis1, sis2, sos, true, true, NULL, &renum);
                //cout << "J sos = " << sos.str();
                CPPUNIT_ASSERT(sos.str() == ps15);
                CPPUNIT_ASSERT((long)sos.tellp() == (long)sos.str().size() );

                //cout << "------------------------------x"<<endl;

                sos.clear();
                sos.str("");
                renum.clear();
                renum.push_back(0);

                vector<int64_t> renum1;
                renum1.clear();
                renum1.push_back(2);
                sos << MARCAIND << "\n";
                na = new NodoTrieS();
                na->inserta("JESUS", Pos(1, 1));
                llena_ss(sis1, na, MARCAIND + "\n");
                delete na;

                //cout << "1" << endl;

                na = new NodoTrieS();
                na->inserta("AMADO", Pos(1, 1));
                na->inserta("MIO", Pos(2, 2));
                llena_ss(sis2, na, MARCAIND + "\n");
                delete na;
                renum.clear();
                renum.push_back(0);
                renum.push_back(1);

                //cout << "2" << endl;
                sis1.seekg(MARCAIND.length() + 1);
                sis2.seekg(MARCAIND.length() + 1);
                mezclaRec(sis1, sis2, sos, true, true, &renum1, &renum);

                //cout << "3" << endl;
                sos.seekg(MARCAIND.length() + 1);
                NodoTrieS *l1 = leePlanoStream(sos);
                //l1->aDotty(cout, "");
                set<Pos> cr = l1->busca("JESUS");
                CPPUNIT_ASSERT(cr.size() == 1);
                set<Pos>::iterator i = cr.begin();
                CPPUNIT_ASSERT((*i).numd == 3);
                cr = l1->busca("AMADO");
                CPPUNIT_ASSERT(cr.size() == 1);
                i = cr.begin();
                CPPUNIT_ASSERT((*i).numd == 1);

                //cout << "4" << endl;
                delete l1;
        } catch (std::string s) {
                cerr << s << endl;
                exit(1);
        }

}

void TrieSDiscoCasoPrueba::prueba_leeRelacion()
{
        vector<Doc> docs;

        {
                fstream os("/tmp/z", ios_base::out);
                os << "";
                os.close();
        }
        docs.clear();
        clog << "0" << endl;
        CPPUNIT_ASSERT_THROW(leeRelacion("/tmp/z", docs), std::string);

        clog << "1" << endl;
        {
                fstream os("/tmp/z", ios_base::out);
                os << "0fffffff";
                os.close();
        }
        docs.clear();
        CPPUNIT_ASSERT_THROW(leeRelacion("/tmp/z", docs), std::string);

        clog << "2" << endl;
        {
                fstream os("/tmp/z", ios_base::out);
                os << MARCAREL << "\n" ;
                os << "a1 12345678901234567890123456789012 2009-01-01" << endl;
                os << "b2 abcdefabcdefabcdefabcdefabcdefab 2009-01-01" << endl;
                os << "c3 00000000000000000000000000000000 2009-01-01" << endl;
                os.close();
        }
        docs.clear();
        leeRelacion("/tmp/z", docs);
        clog << "3" << endl;
        CPPUNIT_ASSERT(docs.size() == 3);
        CPPUNIT_ASSERT(docs[0].URL == "a1");
        CPPUNIT_ASSERT(docs[1].URL == "b2");
        CPPUNIT_ASSERT(docs[2].URL == "c3");
        CPPUNIT_ASSERT(docs[0].cond == "12345678901234567890123456789012");
        CPPUNIT_ASSERT(docs[1].cond == "abcdefabcdefabcdefabcdefabcdefab");
        CPPUNIT_ASSERT(docs[2].cond == "00000000000000000000000000000000");

        {
                fstream os("/tmp/z", ios_base::out);
                os.seekg(0);
                os << MARCAREL << "\n" ;
                for (uint32_t i = 0; i < MAXLURL+2; i++) {
                        os << "x";
                }
                os.close();
        }
        docs.clear();
        CPPUNIT_ASSERT_THROW(leeRelacion("/tmp/z", docs), std::string);
        clog << "4" << endl;
}


void TrieSDiscoCasoPrueba::prueba_escribeRelacion()
{
        vector<Doc> docs;
        docs.clear();
        escribeRelacion("/tmp/z", docs);
        docs.push_back(Doc("a","b","2009-10-14"));
        escribeRelacion("/tmp/z", docs);
        docs.push_back(Doc("c","d","2009-10-13"));
        escribeRelacion("/tmp/z", docs);
        vector<int64_t> reord;
        reord.push_back(1);
        reord.push_back(0);
        escribeRelacion("/tmp/z1", docs, &reord);

}


void TrieSDiscoCasoPrueba::prueba_buscaPlano()
{
        fstream os1("ej.indice", ios_base::out);
        os1 << "";
        os1.close();

        vector<Doc> docs;
        docs.push_back(Doc("ej.txt","m","2010-01-05"));
        escribeRelacion("ej.relacion", docs, NULL);

        CPPUNIT_ASSERT_THROW(buscaPlano("ej.indice", "ej.relacion", "JESUS",
                                        docs), std::string);

        fstream os("ej.indice", ios_base::out);
        os << ps15;
        os.close();

        set<Pos> *b = buscaPlano("ej.indice", "ej.relacion",
                                      "JESUS", docs);
        CPPUNIT_ASSERT(b->size() == 1);
}


void TrieSDiscoCasoPrueba::prueba_buscaPlanoStream()
{
        stringstream sis;
        sis.clear();
        sis.str(ps15);
        sis.seekg(MARCAIND.length() + 1);
        set<Pos> *b = buscaPlanoStream(sis, "P");
        CPPUNIT_ASSERT(b == NULL);
        delete b;

        //cout << "-----------" << endl;
        
        NodoTrieS *na = new NodoTrieS();
        na->inserta("CINTURA", Pos(2, 1));
        na->inserta("CONTABAN", Pos(2, 15));
        na->inserta("COMFESABAN", Pos(2, 11));
        na->inserta("COMIA", Pos(2, 8));
        na->inserta("EM", Pos(1, 1));
        llena_ss(sis, na);
        set<Pos> *b2 = buscaPlanoStream(sis, "CINTURA");
        CPPUNIT_ASSERT(b2 != NULL);
        //cout << *b2 << endl;
        CPPUNIT_ASSERT(b2->size() == 1);
        delete na;

        /*	vector<string> idocs(0);
        	idocs.empty();
        	set<Pos> *cpos = buscaPlano("prueba.idxtxt", "BUENO", idocs);
         
        	if (cpos==NULL) {
        		cerr << "cpos no debería ser NULL" <<endl;
        	}
        	else {
        		cout << cpos->size() << endl;
        		cout << (*cpos) << endl;
        	} */
}

void TrieSDiscoCasoPrueba::setUp()
{
        return;
}
