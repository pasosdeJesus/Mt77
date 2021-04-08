// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file RamDiscoCasoPrueba.cpp
 * Pruebas de regresión a operaciones en disco y RAM
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: RamDiscoCasoPrueba.cpp,v 1.14 2010/01/18 16:12:50 vtamara Exp $
 */

#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <fstream>
#include <istream>
#include <sstream>


#include "RamDiscoCasoPrueba.hpp"
#include "RamDisco.hpp"
#include "CasosPrueba.hpp"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(RamDiscoCasoPrueba);


void RamDiscoCasoPrueba::prueba_precalcula_escribe_con_hermanos()
{
        CPPUNIT_ASSERT(precalcula_escribe_con_hermanos(NULL) == 0);

        NodoTrieS *n = new NodoTrieS("abc");
        uint32_t p = precalcula_escribe_con_hermanos(n) ;
        //cout << p << endl;
        CPPUNIT_ASSERT(p == 16);
        delete n;

        n = new NodoTrieS("bc");
        NodoTrieS *n2 = new NodoTrieS("a", n);
        p = precalcula_escribe_con_hermanos(n2) ;
        //cout << p << endl;
        CPPUNIT_ASSERT(p == 14);
        delete n2;

        n = new NodoTrieS("bc");
        n2 = new NodoTrieS("a", NULL, n);
        p = precalcula_escribe_con_hermanos(n2) ;
        //cout << p << endl;
        CPPUNIT_ASSERT(p == 28);
        delete n;

}

void RamDiscoCasoPrueba::prueba_precalcula_escribe()
{
        CPPUNIT_ASSERT(precalcula_escribe(NULL) == 0);

        NodoTrieS *n = new NodoTrieS("abc");
        uint32_t p = precalcula_escribe(n) ;
        //cout << p << endl;
        CPPUNIT_ASSERT(p == 16);
        delete n;

        n = new NodoTrieS("bc");
        NodoTrieS *n2 = new NodoTrieS("a", n);
        p = precalcula_escribe(n2) ;
        //cout << p << endl;
        CPPUNIT_ASSERT(p == 29);
        delete n2;

        n = new NodoTrieS("bc");
        n2 = new NodoTrieS("a", NULL, n);
        p = precalcula_escribe(n2) ;
        //cout << p << endl;
        CPPUNIT_ASSERT(p == 28);
        delete n;

}


extern stringstream *depuraos;


void RamDiscoCasoPrueba::prueba_leePlano()
{
        fstream os("ej.indice", ios_base::out);
        os << ps15;
        os.close();

        vector<Doc> docs;
        docs.push_back(Doc("ej.txt","m","2010-01-05"));
        escribeRelacion("ej.relacion", docs, NULL);

        docs.clear();

        Arbol_huffman arbolHuffman;
        NodoTrieS *n = leePlano((char *)"ej.indice", (char *)"ej.relacion", 
                                docs, arbolHuffman);
        cout << n->preorden() << endl;
        CPPUNIT_ASSERT(docs.size() == 1);
        CPPUNIT_ASSERT(n->preorden() == "JESUSORAMTAM");
}


void RamDiscoCasoPrueba::prueba_escribePlano()
{

        NodoTrieS t;
        Arbol_huffman ah;
        leeTexto("verdad.txt", 1, t, false);
        fstream os("verdad.docindice", ios_base::out);
        escribePlanoStream(&t, os, ah, 0);
        os.close();

        NodoTrieS *ct = new NodoTrieS();
        string rs;

        ct->inserta("BUEN", Pos(1,1));
        ct->inserta("PODEROSO", Pos(1,2));
        ct->inserta("BUENO", Pos(1,3));
        ct->inserta("BUENOS", Pos(1,4));
        ct->inserta("A", Pos(1,5));
        ct->inserta("C", Pos(1,6));
        ct->inserta("BUEN", Pos(1,7));
        ct->inserta("BUENA", Pos(1,8));
        ct->inserta("BUENO", Pos(1,9));
        ct->inserta("BU", Pos(1,11));
        ct->inserta("BUS", Pos(1,12));
        ct->inserta("AMIGO", Pos(1,13));

        NodoTrieS *n7=new NodoTrieS("PODEROSS", NULL, NULL, Pos(18,1));
        NodoTrieS *n5=new NodoTrieS("CASA", NULL, n7, Pos(16,1));
        NodoTrieS *n2=new NodoTrieS("CAD1", NULL, NULL, Pos(15,1));
        NodoTrieS *n3=new NodoTrieS("B", n2, n5); //resp de liberar n2 pasa a n3
        NodoTrieS *n6=new NodoTrieS("MIGA", NULL, NULL, Pos(17,1));
        NodoTrieS *n4=new NodoTrieS("A", n6, n3, Pos(14,1)); // resp de liberar n3 pasa a n4

        NodoTrieS *rm = mezcla(ct, n4);
        ct = NULL;
        n4=NULL;

        vector<Doc> docs;
        docs.push_back(Doc("ejemplo.txt", "12345678901234567890123456789012",
                           "2009-09-30"));
        escribePlano(*rm, docs, "prueba.indice", "prueba.relacion", ah);

        cerr << "Examinar resultado con: less prueba.indice" << endl;
        delete rm;

}


void RamDiscoCasoPrueba::prueba_leeTexto()
{

        Arbol_huffman ah;
        fstream os("verdad.txt", ios_base::out);
        os << "LA VERDAD SI NOS LIBERAR\xC1 SI" << endl;
        os.close();
        NodoTrieS t;
        leeTexto("verdad.txt", 1, t, false);

        //cout << "OJO " << t.preorden() << endl;
        CPPUNIT_ASSERT(t.preorden() == "LAIBERAR\xC1NOSSIVERDAD");

        //t.aDotty(cout, "");
        std::stringstream sos;
        llena_ss(sos, &t, ah);
        //cout << "sos=" << sos.str() << endl;

}


void RamDiscoCasoPrueba::prueba_mezclaDiscoRam()
{

        Arbol_huffman ahlectura, ahescritura;
        std::stringstream sos;
        std::stringstream sis1;
        NodoTrieS *a2 = NULL;

        try {
                sos.clear();
                sos.str("");
                sos << " ";
                sis1.clear();
                sis1.str("");
                cout << "1" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, false, false, NULL, NULL, ahlectura, ahescritura);
                CPPUNIT_ASSERT(sos.str() == " \n" );

                sos.clear();
                sos.str("");
                sos << " ";
                sis1.clear();
                sis1.str("\n");
                a2 = new NodoTrieS();
                cout << "2" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, false, false, NULL, NULL, ahlectura, ahescritura);
                CPPUNIT_ASSERT(sos.str() == " \n" );

                sos.clear();
                sos.str("");
                sos << " ";
                sis1.clear();
                sis1.str("\n");
                cout << "3" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                CPPUNIT_ASSERT(sos.str() == " \n" );

                sos.clear();
                sos.str("");
                sos << " ";
                sis1.clear();
                sis1.str("x");
                CPPUNIT_ASSERT_THROW(mezclaDiscoRam(sis1, a2, 0, sos, false, false, NULL, NULL, ahlectura, ahescritura),
                                     std::string);

                stringstream d;
                depuraos = &d;
                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps1 + "\n");
                cout << "4" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                //cout << "'" << sos.str() << "'" << endl;
                //cout << "' " + ps1 + "\n'" << endl;
                CPPUNIT_ASSERT(sos.str() == (ps1 + "\n") );
                delete a2;

                //cout << "Comenzamos" << endl;
                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps4);
                a2 = NULL;
                cout << "5" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                //cout << "1 sos='" << sos.str() << "'" << endl;
                CPPUNIT_ASSERT(sos.str() == ps4 );

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str("\n");
                a2 = new NodoTrieS("a");
                //sis2.clear(); sis2.str(ps1 + "\n");
                cout << "6" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                CPPUNIT_ASSERT(sos.str() == (ps1 + "\n") );
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str("\n");
                //sis2.clear(); sis2.str(ps4);
                a2 = new NodoTrieS("m");
                a2 = new NodoTrieS("a", a2, NULL);
                cout << "7" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                //cout << "2 sos='" << sos.str() << "'" << endl;
                //cout << "2 ps4='" << ps4 << "'" << endl;
                CPPUNIT_ASSERT(sos.str() == ps4);
                delete a2;


                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps2 + "\n");
                a2 = NULL;
                //sis2.clear(); sis2.str("\n");
                cout << "8" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                CPPUNIT_ASSERT(sos.str() == (ps2 + "\n") );
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps1 + "\n");
                a2 = new NodoTrieS("a", NULL, NULL);
                //sis2.clear(); sis2.str(ps1 + "\n");
                cout << "9" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                CPPUNIT_ASSERT(sos.str() == (ps1 + "\n") );
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps4);
                a2 = new NodoTrieS("m");
                a2 = new NodoTrieS("a", a2, NULL);
                //sis2.clear(); sis2.str(ps4);
                cout << "10" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                CPPUNIT_ASSERT(sos.str() == ps4 );
                delete a2;


                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps2 + "\n");
                a2 = new NodoTrieS("a", NULL, NULL, Pos(2,2));
                //sis2.clear(); sis2.str(ps2 + "\n");
                cout << "11" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                CPPUNIT_ASSERT(sos.str() == (ps2 + "\n") );
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps2 + "\n");
                set<Pos> cpos;
                cpos.insert(Pos(2,2));
                cpos.insert(Pos(2,4));
                a2 = new NodoTrieS("a", NULL, NULL, cpos);
                stringstream s2;
                s2.clear();
                llena_ss(s2, a2, ahescritura);
                //cout << "s2.str='" << s2.str() << "'" << endl;
                //sis2.clear(); sis2.str(ps3 + "\n");
                cout << "12" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                //cout << "sos.str='" << sos.str() << "'" << endl;
                //cout << "ps3='" << ps3 + "\n" << "'" << endl;
                CPPUNIT_ASSERT(sos.str() == (ps3 + "\n") );
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps5);
                //sis2.clear(); sis2.str(ps5);
                a2 = new NodoTrieS("b");
                a2 = new NodoTrieS("a", NULL, a2);
                llena_ss(s2, a2, ahescritura);
                //cout << "sis1=" << sis1.str() << endl;
                //cout << "s2=" << s2.str() << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, false, false, NULL, NULL, ahlectura, ahescritura);
                //cout << "sos='" << sos.str() << "'" << endl;
                //cout << "ps1='" << ps1 + "\n" << "'" << endl;
                CPPUNIT_ASSERT(sos.str() == (ps1 + "\n") );
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps8);
                a2 = new NodoTrieS("b", NULL, NULL, Pos(2,2));
                a2 = new NodoTrieS("a", NULL, a2);
                //sis2.clear(); sis2.str(ps9);
                //cout << "ps9" << endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, false, NULL, NULL, ahlectura, ahescritura);
                CPPUNIT_ASSERT(sos.str() == ps8 );
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps8);
                //cout << "ps9-2" << endl;
                //sis2.clear(); sis2.str(ps9);
                a2 = new NodoTrieS("b", NULL, NULL, Pos(2,2));
                a2 = new NodoTrieS("a", NULL, a2);
                mezclaDiscoRam(sis1, a2, 0, sos, false, true, NULL, NULL, ahlectura, ahescritura);
                CPPUNIT_ASSERT(sos.str() == ps9);
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps8);
                //cout << "con pos" << endl;
                //sis2.clear(); sis2.str(ps9);
                a2 = new NodoTrieS("b", NULL, NULL, Pos(2,2));
                a2 = new NodoTrieS("a", NULL, a2);
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                //cout << "sos=" << sos.str() << endl;
                CPPUNIT_ASSERT(sos.str() == ps10);
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps9);
                //sis2.clear(); sis2.str(ps8);
                //cout << "ps9 - ps8" << endl;
                a2 = new NodoTrieS("b", NULL, NULL, Pos(2,4));
                a2 = new NodoTrieS("a", NULL, a2);
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                CPPUNIT_ASSERT(sos.str() == ps10);
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps1 + "\n");
                //sis2.clear(); sis2.str(ps11);
                //cout << "ps1 - ps11" << endl;
                a2 = new NodoTrieS("am", NULL, NULL);
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                //cout<<"ps4 sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == ps4 );
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps11);
                //sis2.clear(); sis2.str(ps1 + "\n");
                //cout << "ps11 - ps1" << endl;
                a2 = new NodoTrieS("a", NULL, NULL);
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                //cout<<"sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == ps4 );
                delete a2;


                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps12);
                //cout << "ps12 - ps11" << endl;
                //sis2.clear(); sis2.str(ps11);
                a2 = new NodoTrieS("am", NULL, NULL);
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                //cout<<"ps13 sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == ps13 );
                delete a2;

                sos.clear();
                sos.str("");
                sis1.clear();
                sis1.str(ps11);
                //sis2.clear(); sis2.str(ps12);
                //cout << "ps11 - ps12" << endl;
                a2 = new NodoTrieS("l", NULL, NULL);
                a2 = new NodoTrieS("a", a2, NULL);
                llena_ss(s2, a2, ahescritura);
                //cout<<"s2='"<<s2.str()<<"'"<<endl;
                //cout<<"ps12='"<<ps12<<"'"<<endl;
                //cout<<"ps11='"<<ps11<<"'"<<endl;
                mezclaDiscoRam(sis1, a2, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                //cout<<"sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == ps13 );
                delete a2;


                NodoTrieS *nam = new NodoTrieS("am", NULL, NULL, set<Pos>());
                llena_ss(sis1, nam, ahescritura);
                NodoTrieS *na = new NodoTrieS();
                na->inserta("a", Pos(1,1));
                //llena_ss(sis2, na, ahescritura);
                sos.clear();
                sos.str("");
                mezclaDiscoRam(sis1, na, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                delete na;

                NodoTrieS *nm = new NodoTrieS("m", NULL, NULL, set<Pos>());
                na = new NodoTrieS("a", nm, NULL, Pos(1,1));
                std::stringstream sist;
                llena_ss(sist, na, ahescritura);

                //cout<<"sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == sist.str());
                delete na;  // Borra a nm también
                delete nam;


                sos.clear();
                sos.str("");
                nam = new NodoTrieS("am", NULL, NULL, set<Pos>());
                llena_ss(sis1, nam, ahescritura);
                NodoTrieS *no = new NodoTrieS("o", NULL, NULL, set<Pos>());
                na = new NodoTrieS("a", no, NULL, set<Pos>());
                //llena_ss(sis2, na, ahescritura);

                mezclaDiscoRam(sis1, na, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                delete na; // Borra también no
                delete nam;

                no = new NodoTrieS("o", NULL, NULL, set<Pos>());
                nm = new NodoTrieS("m", NULL, no , set<Pos>());
                na = new NodoTrieS("a", nm, NULL, set<Pos>());
                llena_ss(sist, na, ahescritura);
                //cout<<"amo sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == sist.str() );
                delete na;


                NodoTrieS *naa = new NodoTrieS("aa", NULL, NULL, set<Pos>());
                llena_ss(sis1, naa, ahescritura);
                NodoTrieS *nab = new NodoTrieS();
                nab->inserta("ab", Pos(1,1));
                //cout << "aa y ab" << endl;
                llena_ss(s2, nab, ahescritura);
                //cout << "sis1='" << sis1.str() << "'" << endl;
                //cout << "s2='" << s2.str() << "'" << endl;
                //nab->aDotty(cout);
                sos.clear();
                sos.str("");
                mezclaDiscoRam(sis1, nab, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                delete naa;
                delete nab;
                NodoTrieS *nb = new NodoTrieS();
                nb->inserta("b", Pos(1,1));
                NodoTrieS *na2 = new NodoTrieS("a", NULL, nb, set<Pos>());
                na = new NodoTrieS("a", na2, NULL, set<Pos>());
                llena_ss(sist,na, ahescritura);
                //cout<<"aaab sos='"<<sos.str()<<"'"<<endl;
                //cout<<"sist='"<<sist.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == sist.str() );
                delete na;


                sos.clear();
                sos.str("");
                nm = new NodoTrieS("m", NULL, NULL, set<Pos>());
                naa = new NodoTrieS("aa", nm, NULL, set<Pos>());
                llena_ss(sis1, naa, ahescritura);
                delete naa;
                nm = new NodoTrieS("m", NULL, NULL, set<Pos>());
                nab = new NodoTrieS("ab", nm, NULL, set<Pos>());
                //llena_ss(sis2, nab, ahescritura);
                //delete nab;
                mezclaDiscoRam(sis1, nab, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                NodoTrieS *nm2 = new NodoTrieS("m", NULL, NULL, set<Pos>());
                nm = new NodoTrieS("m", NULL, NULL, set<Pos>());
                nab = new NodoTrieS("b", nm, NULL, set<Pos>());
                naa = new NodoTrieS("a", nm2, nab, set<Pos>());
                na = new NodoTrieS("a", naa, NULL, set<Pos>());
                llena_ss(sist, na, ahescritura);
                delete na;
                //cout<<"mbaa sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == sist.str() );


                sos.clear();
                sos.str("");
                nab = new NodoTrieS("ab", NULL, NULL, set<Pos>());
                llena_ss(sis1, nab, ahescritura);
                delete nab;
                naa = new NodoTrieS();
                naa->inserta("aa", Pos(1,1));
                //llena_ss(sis2, naa, ahescritura);
                mezclaDiscoRam(sis1, naa, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                delete naa;
                nb = new NodoTrieS("b");
                naa = new NodoTrieS("a",NULL, nb, Pos(1,1));
                na = new NodoTrieS("a", naa, NULL);
                llena_ss(sist, na, ahescritura);
                delete na;
                //cout<<"baa sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == sist.str() );

                sos.clear();
                sos.str("");
                nm = new NodoTrieS("m");
                nab = new NodoTrieS("ab", nm, NULL);
                llena_ss(sis1, nab, ahescritura);
                delete nab;
                nm = new NodoTrieS("m");
                naa = new NodoTrieS("aa", nm, NULL);
                //llena_ss(sis2, naa, ahescritura);
                mezclaDiscoRam(sis1, naa, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                delete naa;

                nm = new NodoTrieS("m");
                nab = new NodoTrieS("b", nm);
                nm2 = new NodoTrieS("m");
                naa = new NodoTrieS("a", nm2, nab);
                na = new NodoTrieS("a", naa);
                llena_ss(sist, na, ahescritura);
                delete na;
                //cout<<"mbmaa sos='"<<sos.str()<<"'"<<endl;

                CPPUNIT_ASSERT(sos.str() == sist.str());

                sos.clear();
                sos.str("");
                na = new NodoTrieS("a");
                llena_ss(sis1, na, ahescritura);
                delete na;
                nb = new NodoTrieS("b", NULL, NULL, Pos(1,1));
                //llena_ss(sis2, nb, ahescritura);
                mezclaDiscoRam(sis1, nb, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                delete nb;
                nb = new NodoTrieS("b", NULL, NULL, Pos(1,1));
                na = new NodoTrieS("a", NULL, nb);
                llena_ss(sist, na, ahescritura);
                delete na;
                //cout<<"abba sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == sist.str() );

                sos.clear();
                sos.str("");
                NodoTrieS *nr = new NodoTrieS("r");
                no = new NodoTrieS("o", nr);
                nm = new NodoTrieS("m", no);
                na = new NodoTrieS("a", nm);
                llena_ss(sis1, na, ahescritura);
                delete na;
                na = new NodoTrieS();
                na->inserta("a", Pos(1,1));
                //llena_ss(sis2, na, ahescritura);
                mezclaDiscoRam(sis1, na, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                delete na;
                nr = new NodoTrieS("r");
                no = new NodoTrieS("o", nr);
                nm = new NodoTrieS("m", no);
                na = new NodoTrieS("a", nm);
                na->inserta("a", Pos(1,1));
                llena_ss(sist, na, ahescritura);
                delete na;
                //cout<<"amor sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == sist.str());

                sos.clear();
                sos.str("");
                na = new NodoTrieS();
                na->inserta("a", Pos(1,1));
                llena_ss(sis1, na, ahescritura);
                delete na;
                nr = new NodoTrieS("r");
                no = new NodoTrieS("o", nr);
                nm = new NodoTrieS("m", no);
                na = new NodoTrieS("a", nm);
                //llena_ss(sis2, na, ahescritura);
                mezclaDiscoRam(sis1, na, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                delete na;
                nr = new NodoTrieS("r");
                no = new NodoTrieS("o", nr);
                nm = new NodoTrieS("m", no);
                na = new NodoTrieS("a", nm);
                na->inserta("a", Pos(1,1));
                llena_ss(sist, na, ahescritura);
                delete na;
                //cout<<"amora sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == sist.str());

                //cout << "Sin escribir hermanos" << endl;
                sos.clear();
                sos.str("");
                nb = new NodoTrieS("b");
                na = new NodoTrieS("a", NULL, nb);
                llena_ss(sis1, na, ahescritura);
                delete na;
                na = new NodoTrieS("a");
                mezclaDiscoRam(sis1, na, 0, sos, false, true, NULL, NULL, ahlectura, ahescritura);
                //stringstream s2;
                llena_ss(s2, na, ahescritura);
                delete na;
                CPPUNIT_ASSERT(sos.str() == s2.str() );

                sos.clear();
                sos.str("");
                //cout << "a, ab" << endl;
                na = new NodoTrieS("a");
                llena_ss(sis1, na, ahescritura);
                delete na;
                nb = new NodoTrieS("b");
                na = new NodoTrieS("a", NULL, nb);
                //llena_ss(sis2, na, ahescritura);
                mezclaDiscoRam(sis1, na, 0, sos, true, false, NULL, NULL, ahlectura, ahescritura);
                delete na;
                //cout<<"sinh aba sos1='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == sis1.str() );
                /*
                 *        1         2         3         4         5         6         7
                01234567890123456789012345678901234567890123456789012345678901234567890123456789
                a{}0000000c\a{}00000023b{}0000002F\m{}00000000\m{}00000000
                */

                sos.clear();
                sos.str("");
                //cout << "a, amor" << endl;
                na = new NodoTrieS("a");
                llena_ss(sis1, na, ahescritura);
                delete na;
                nr = new NodoTrieS("r");
                no = new NodoTrieS("o", nr);
                nm = new NodoTrieS("m", no);
                na = new NodoTrieS("a", nm);
                na->inserta("a", Pos(1,1));
                //llena_ss(sis2, na, ahescritura);
                mezclaDiscoRam(sis1, na, 0, sos, true, false, NULL, NULL, ahlectura, ahescritura);
                //cout<<"sinh amor sos='"<<sos.str()<<"'"<<endl;
                llena_ss(s2, na, ahescritura);
                delete na;
                CPPUNIT_ASSERT(sos.str()== s2.str());

                sos.clear();
                sos.str("");
                //cout << "amor, a" << endl;
                //depuraos=&sos;
                nr = new NodoTrieS("r");
                no = new NodoTrieS("o", nr);
                nm = new NodoTrieS("m", no);
                na = new NodoTrieS("a", nm);
                na->inserta("a", Pos(1,1));
                llena_ss(sis1, na, ahescritura);
                delete na;
                na = new NodoTrieS("a");
                //llena_ss(sis2, na, ahescritura);
                mezclaDiscoRam(sis1, na, 0, sos, false, false, NULL, NULL, ahlectura, ahescritura);
                delete na;
                //	cout<<"sinh amor2 sos='"<<sos.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == sis1.str());


                // En un sos no vacío

                sos.clear();
                sos.str("");
                sos << "a{0000=00000}";
                NodoTrieS *nc = new NodoTrieS("c");
                nb = new NodoTrieS("b", NULL, nc);
                llena_ss(sis1, nb, ahescritura);
                delete nb;
                nb = new NodoTrieS("b");
                //llena_ss(sis2, nb, ahescritura);
                mezclaDiscoRam(sis1, nb, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                delete nb;
                nc = new NodoTrieS("c");
                nb = new NodoTrieS("b", NULL, nc);
                na = new NodoTrieS("a", NULL, nb);
                llena_ss(sist, na, ahescritura);
                delete na;
                CPPUNIT_ASSERT(sos.str() == sist.str());

                sos.clear();
                sos.str("");
                sos << "a{0000=00000}";
                nc = new NodoTrieS("c");
                nb = new NodoTrieS("b", nc);
                llena_ss(sis1, nb, ahescritura);
                delete nb;
                nb = new NodoTrieS("b");
                //llena_ss(sis2, nb, ahescritura);
                mezclaDiscoRam(sis1, nb, 0, sos, true, true, NULL, NULL, ahlectura, ahescritura);
                delete nb;
                nc = new NodoTrieS("c");
                nb = new NodoTrieS("b", nc);
                na = new NodoTrieS("a", NULL, nb);
                llena_ss(sist, na, ahescritura);
                delete na;
                CPPUNIT_ASSERT(sos.str() == sist.str() );

                sos.clear();
                sos.str("");
                na = new NodoTrieS();
                na->inserta("a", Pos(1,1));
                llena_ss(sis1, na, ahescritura);
                delete na;
                nr = new NodoTrieS("r");
                no = new NodoTrieS("o", nr);
                nm = new NodoTrieS("m", no);
                na = new NodoTrieS("a", nm);
                //llena_ss(sis2, na, ahescritura);
                vector<int64_t> renum;
                renum.push_back(0);
                mezclaDiscoRam(sis1, na, 0, sos, true, true, NULL, &renum, ahlectura, ahescritura);
                delete na;
                //cout<<"renum sos='"<<sos.str()<<"'"<<endl;
                nr = new NodoTrieS("r");
                no = new NodoTrieS("o", nr);
                nm = new NodoTrieS("m", no);
                na = new NodoTrieS("a", nm);
                na->inserta("a", Pos(1,1));
                llena_ss(sist, na, ahescritura);
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
                llena_ss(sis1, na, ahescritura);
                delete na;
                na = new NodoTrieS();
                na->inserta("a", Pos(1,1));
                //llena_ss(sis2, na, ahescritura);
                //cout<<"renum amor por Mezclar"<<endl;
                mezclaDiscoRam(sis1, na, 0, sos, true, true, NULL, &renum, ahlectura, ahescritura);
                delete na;
                nr = new NodoTrieS("r");
                no = new NodoTrieS("o", nr);
                nm = new NodoTrieS("m", no);
                na = new NodoTrieS("a", nm);
                na->inserta("a", Pos(2,1));
                llena_ss(sist, na, ahescritura);
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
                llena_ss(sis1, na, ahescritura);
                delete na;
                nm = new NodoTrieS();
                nm->inserta("m", Pos(1, 1));
                NodoTrieS *nl = new NodoTrieS("l", NULL, nm);
                na = new NodoTrieS("a", nl);
                //llena_ss(sis2, na, ahescritura);
                mezclaDiscoRam(sis1, na, 0, sos, true, true, NULL, &renum, ahlectura, ahescritura);
                delete na;
                nr = new NodoTrieS("r");
                no = new NodoTrieS("o", nr);
                nm = new NodoTrieS("m", no);
                nm->inserta("m", Pos(1, 1));
                nl = new NodoTrieS("l", NULL, nm);
                na = new NodoTrieS("a", nl);
                llena_ss(sist, na, ahescritura);
                delete na;
                //cout<<"2 sos='"<<sos.str()<<"'"<<endl;
                //cout<<"2 sist='"<<sist.str()<<"'"<<endl;
                CPPUNIT_ASSERT(sos.str() == sist.str());

                depuraos = NULL;
                //depuraos=&sos;
                sos.clear();
                sos.str("");
                //cout << "Cs" << endl;
                renum.clear();
                renum.push_back(1);
                sos << MARCAIND << "\n";
                na = new NodoTrieS();
                na->inserta("EM", Pos(1, 1));
                llena_ss(sis1, na, ahescritura);
                delete na;
                na = new NodoTrieS();
                na->inserta("CINTURA", Pos(1, 1));
                na->inserta("CONTABAN", Pos(1, 15));
                na->inserta("COMFESABAN", Pos(1, 11));
                na->inserta("COMIA", Pos(1, 8));
                //sis2.clear();  sis2.str("");
                /*	sis2.str(MARCAIND + "\n");
                	sis2.seekp(MARCAIND.length() +1);
                	sis2.seekg(MARCAIND.length() +1);
                	sis2.clear(); */
                //escribePlanoStream(na, sis2);
                //cout << "sis2.str()=" << sis2.str() << endl;
                //sis2.seekg(0);
                //sis2.seekg(MARCAIND.length() +1);
                mezclaDiscoRam(sis1, na, 0, sos, true, true, NULL, &renum, ahlectura, ahescritura);
                delete na;
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
                escribePlanoStream(na, sist, ahescritura, MARCAIND.length() + 1);
                sist.seekg(0);
                delete na;
                //cout<<"sis1='"<<sis1.str()<<"'"<<endl;
                //cout<<"sos='"<<sos.str()<<"'"<<endl;
                //cout<<"sist='"<<MARCAIND << "\n" << sist.str()<<"'"<<endl;

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
                llena_ss(sis1, na, ahescritura, MARCAIND + "\n");
                delete na;
                //cout << "JESUS sis1=" << sis1.str() << endl;
                na = new NodoTrieS();
                na->inserta("JORAM", Pos(3, 2));
                na->inserta("JOTAM", Pos(2, 2));
                //llena_ss(sis2, na, ahescritura, MARCAIND + "\n");
                //cout << "JORAM JOTAM sis2=" << sis2.str() << endl;
                sis1.seekg(MARCAIND.length() + 1);
                //sis2.seekg(MARCAIND.length() + 1);
                mezclaDiscoRam(sis1, na, 0, sos, true, true, NULL, &renum, ahlectura, ahescritura);
                delete na;
                //cout << endl << "J sos = " << sos.str() << endl;
                //cout << "ps15 = " << ps15 << endl;
                CPPUNIT_ASSERT(sos.str() == ps15);
                CPPUNIT_ASSERT((long)sos.tellp() == (long)sos.str().size() );

                sos.clear();
                sos.str("");
                renum.clear();
                renum.push_back(1);
                sos << MARCAIND << "\n";
                na = new NodoTrieS();
                na->inserta("LIBRE", Pos(1, 2));
                llena_ss(sis1, na, ahescritura, MARCAIND + "\n");
                delete na;
                //cout << "JESUS sis1=" << sis1.str() << endl;
                na = new NodoTrieS();
                na->inserta("LIBERTAD", Pos(1, 2));
                //llena_ss(sis2, na, ahescritura, MARCAIND + "\n");
                //cout << "JORAM JOTAM sis2=" << sis2.str() << endl;
                sis1.seekg(MARCAIND.length() + 1);
                //sis2.seekg(MARCAIND.length() + 1);
                mezclaDiscoRam(sis1, na, 0, sos, true, true, NULL, &renum, ahlectura, ahescritura);
                //cout << "sos=" << sos.str() << endl;
                delete na;

                na = new NodoTrieS();
                na->inserta("LIBRE", Pos(1, 2));
                na->inserta("LIBERTAD", Pos(2, 2));
                stringstream sos2;
                llena_ss(sos2, na, ahescritura, MARCAIND + "\n");
                //cout << "sos2=" << sos2.str() << endl;

                CPPUNIT_ASSERT(sos.str() == sos2.str());

                //cout << "------------------------------x"<<endl;
                sos.clear();
                sos.str("");
                renum.clear();
                renum.push_back(1);
                sos << MARCAIND << "\n";
                na = new NodoTrieS();
                na->inserta("AHOGARON", Pos(1, 2));
                na->inserta("AHI", Pos(1, 4));
                llena_ss(sis1, na, ahescritura, MARCAIND + "\n");
                delete na;
                //cout << "AHORA sis1=" << sis1.str() << endl;
                na = new NodoTrieS();
                na->inserta("AHORA", Pos(1, 2));

                sis1.seekg(MARCAIND.length() + 1);
                mezclaDiscoRam(sis1, na, 0, sos, true, true, NULL, &renum, ahlectura, ahescritura);
                //cout << "sos=" << sos.str() << endl;
                delete na;

                na = new NodoTrieS();
                na->inserta("AHOGARON", Pos(1, 2));
                na->inserta("AHI", Pos(1, 4));
                na->inserta("AHORA", Pos(2, 2));
                sos2.clear();
                sos2.str("");
                llena_ss(sos2, na, ahescritura, MARCAIND + "\n");
                //cout << "sos2=" << sos2.str() << endl;

                CPPUNIT_ASSERT(sos.str() == sos2.str());

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
                llena_ss(sis1, na, ahescritura, MARCAIND + "\n");
                delete na;

                //cout << "1" << endl;

                na = new NodoTrieS();
                na->inserta("AMADO", Pos(1, 1));
                na->inserta("MIO", Pos(2, 2));
                //llena_ss(sis2, na, ahescritura, MARCAIND + "\n");
                renum.clear();
                renum.push_back(0);
                renum.push_back(1);

                //cout << "2" << endl;
                sis1.seekg(MARCAIND.length() + 1);
                //sis2.seekg(MARCAIND.length() + 1);
                mezclaDiscoRam(sis1, na, 0, sos, true, true, &renum1, &renum, ahlectura, ahescritura);
                delete na;

                //cout << "3" << endl;
                sos.seekg(MARCAIND.length() + 1);
                Arbol_huffman arbolHuffman;
                NodoTrieS *l1 = leePlanoStream(sos, arbolHuffman);
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
#if 0
#endif

}


void RamDiscoCasoPrueba::prueba_subindiceDiscoaRAM()
{

        stringstream sis1;
        sis1  << ps15;
        NodoTrieS *t= new NodoTrieS();
        sis1.seekg(MARCAIND.length() + 1);
        subindiceDiscoaRAM(sis1, t, 10);
        cout << "preorden: " << t->preorden() << endl;
        CPPUNIT_ASSERT(t->preorden() == "");
        sis1.seekg(MARCAIND.length() + 1);
        subindiceDiscoaRAM(sis1, t, 2);
        cout << "preorden: " << t->preorden() << endl;
        CPPUNIT_ASSERT(t->preorden() == "JESUSOTAM");

}

void RamDiscoCasoPrueba::setUp()
{
        return;
}
