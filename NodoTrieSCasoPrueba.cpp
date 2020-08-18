// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file NodoTrieSCasoPrueba.cpp
 *
 * Pruebas de regresión a NodoTrieS.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: NodoTrieSCasoPrueba.cpp,v 1.11 2010/01/18 16:12:50 vtamara Exp $
 */

#include <sstream>
#include <iostream>
#include <fstream>

#include "NodoTrieSCasoPrueba.hpp"
#include "NodoTrieS.hpp"

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(NodoTrieSCasoPrueba);


void NodoTrieSCasoPrueba::prueba_constructora()
{
        NodoTrieS *ct = new NodoTrieS();

        CPPUNIT_ASSERT(ct->preorden() == "");

        CPPUNIT_ASSERT(ct->busca("hola").size() == 0);
}

void NodoTrieSCasoPrueba::prueba_inserta()
{
        NodoTrieS *ct = new NodoTrieS();

        ct->inserta("BUEN", Pos(1,1));
        CPPUNIT_ASSERT(ct->preorden() == "BUEN");
        ct->inserta("PODEROSO", Pos(1,2));
        CPPUNIT_ASSERT(ct->preorden() == "BUENPODEROSO");
        ct->inserta("BUENO", Pos(1,3));
        CPPUNIT_ASSERT(ct->preorden() == "BUENOPODEROSO");
        ct->inserta("BUENOS", Pos(1,4));
        CPPUNIT_ASSERT(ct->preorden() == "BUENOSPODEROSO");
        ct->inserta("A", Pos(1,5));
        CPPUNIT_ASSERT(ct->preorden() == "ABUENOSPODEROSO");
        ct->inserta("C", Pos(1,6));
        CPPUNIT_ASSERT(ct->preorden() == "ABUENOSCPODEROSO");
        ct->inserta("BUEN", Pos(1,7));
        CPPUNIT_ASSERT(ct->preorden() == "ABUENOSCPODEROSO");
        ct->inserta("BUENA", Pos(1,8));
        CPPUNIT_ASSERT(ct->preorden() == "ABUENAOSCPODEROSO");
        ct->inserta("BUENO", Pos(1,9));
        CPPUNIT_ASSERT(ct->preorden() == "ABUENAOSCPODEROSO");
        ct->inserta("BUENZ", Pos(1,10));
        CPPUNIT_ASSERT(ct->preorden() == "ABUENAOSZCPODEROSO");
        ct->inserta("BU", Pos(1,11));
        CPPUNIT_ASSERT(ct->preorden() == "ABUENAOSZCPODEROSO");
        ct->inserta("BUS", Pos(1,12));
        CPPUNIT_ASSERT(ct->preorden() == "ABUENAOSZSCPODEROSO");
        ct->inserta("AMIGO", Pos(1,13));
        CPPUNIT_ASSERT(ct->preorden() == "AMIGOBUENAOSZSCPODEROSO");
        ct->inserta("AMIGZ", Pos(1,14));
        //	clog << ct->preorden() << endl;
        CPPUNIT_ASSERT(ct->preorden() == "AMIGOZBUENAOSZSCPODEROSO");
}

void NodoTrieSCasoPrueba::prueba_insertaConEtiqueta()
{
        NodoTrieS *ct = new NodoTrieS();

        ct->insertaConEtiqueta("nombre", "titulo", 1, 1);
        CPPUNIT_ASSERT( ct->preorden() == "TITULO:NOMBRE");
        ct->insertaConEtiqueta("nombre con espacio", "titulo", 1, 1);
        stringstream ss;
        ct->aDotty(ss, "");
        //cout << ss.str() << endl;
        CPPUNIT_ASSERT( ss.str().size() > 0);

        cout << ct->preorden() << endl;
        CPPUNIT_ASSERT( ct->preorden() == "TITULO:CONESPACIONOMBRE");
        ct->insertaConEtiqueta("nombre.con.punto", "titulo", 1, 1);
        CPPUNIT_ASSERT( ct->preorden() == "TITULO:CONESPACIONOMBRE.CON.PUNTO");

        ct->insertaConEtiqueta("nombre.con.punto.perodeverdadmuypermuyperomuyperosuficientelargocomoparaquetengaquecortarse", "otro", 1, 1);
        CPPUNIT_ASSERT( ct->preorden() == "OTRO:NOMBRE.CON.PUNTO.PERODEVERDTITULO:CONESPACIONOMBRE.CON.PUNTO");
        delete ct;

        ct = new NodoTrieS();
        ct->insertaConEtiqueta("nombre.con.punto.perodeverd.aaa", "otro", 1, 1);
        CPPUNIT_ASSERT( ct->preorden() == "OTRO:NOMBRE.CON.PUNTO.PERODEVERD");
        delete ct;

        ct = new NodoTrieS();
        ct->insertaConEtiqueta("nombre.cdefghijklmnopqrstuv w", "otro", 1, 1);
        /*	ct->aDotty(ss, "");
        	cout << ss.str() << endl; 
        	cout << ct->preorden() << endl; */
        CPPUNIT_ASSERT( ct->preorden() == "OTRO:NOMBRE.CDEFGHIJKLMNOPQRSTUVW");
        delete ct;



}


void NodoTrieSCasoPrueba::prueba_mezcla()
{
        NodoTrieS *ct = new NodoTrieS();

        NodoTrieS *n7=new NodoTrieS("PODEROSS", NULL, NULL, Pos(18,1));
        NodoTrieS *n5=new NodoTrieS("CASA", NULL, n7, Pos(16,1));
        NodoTrieS *n2=new NodoTrieS("CAD1", NULL, NULL, Pos(15,1));
        NodoTrieS *n3=new NodoTrieS("B", n2, n5); //resp de liberar n2 pasa a n3
        NodoTrieS *n6=new NodoTrieS("MIGA", NULL, NULL, Pos(17,1));
        NodoTrieS *n4=new NodoTrieS("A", n6, n3, Pos(14,1)); // resp de liberar n3 pasa a n4

        NodoTrieS *rm = mezcla(ct, n4);
        ct = NULL;
        n4=NULL;
        //cout << rm->preorden() << endl;
        CPPUNIT_ASSERT(rm->preorden() == "AMIGABCAD1CASAPODEROSS");
        delete rm;

        ct = new NodoTrieS("AMIGA", NULL, NULL, Pos(1,1));
        n4 = new NodoTrieS("AMIGA", NULL, NULL, Pos(2,1));
        rm = mezcla(ct, n4);
        CPPUNIT_ASSERT(rm->preorden() == "AMIGA");
        delete rm;

        ct = new NodoTrieS("AMIGA", NULL, NULL, Pos(1,1));
        n4 = new NodoTrieS("AMIGO", NULL, NULL, Pos(2,1));
        rm = mezcla(ct, n4);
        CPPUNIT_ASSERT(rm->preorden() == "AMIGAO");
        delete rm;

        ct = new NodoTrieS("AMIGO", NULL, NULL, Pos(1,1));
        n4 = new NodoTrieS("AMIGA", NULL, NULL, Pos(2,1));
        rm = mezcla(ct, n4);
        CPPUNIT_ASSERT(rm->preorden() == "AMIGAO");
        delete rm;


        ct = new NodoTrieS("AMIG", NULL, NULL, Pos(1,1));
        n4 = new NodoTrieS("AMIGO", NULL, NULL, Pos(2,1));
        rm = mezcla(ct, n4);
        CPPUNIT_ASSERT(rm->preorden() == "AMIGO");
        delete rm;

        ct = new NodoTrieS("AMIGO", NULL, NULL, Pos(1,1));
        n4 = new NodoTrieS("AMIG", NULL, NULL, Pos(2,1));
        rm = mezcla(ct, n4);
        CPPUNIT_ASSERT(rm->preorden() == "AMIGO");
        delete rm;




}

void NodoTrieSCasoPrueba::prueba_renumeraDocs()
{
        NodoTrieS *n=new NodoTrieS("PODEROSO", NULL, NULL, Pos(1,18));
        vector<int64_t> renum;
        renum.push_back(1); // 0->1   indicando que documento 1 pasa a ser 2
        // pues al renumerar se usa indexado de vector en lugar del indexado Pos
        n->renumeraDocs(renum);
        set<Pos> sp = n->busca("PODEROSO");
        CPPUNIT_ASSERT(sp.size() == 1);
        set<Pos>::iterator cpi;
        cpi = sp.begin();
        CPPUNIT_ASSERT((*cpi).numd == 2 && (*cpi).numb == 18);
        delete n;

        NodoTrieS *her=new NodoTrieS("B", NULL, NULL, Pos(1,1));
        NodoTrieS *hij=new NodoTrieS("B", NULL, NULL, Pos(1,2));
        n = new NodoTrieS("A", her, hij, Pos(1,3));
        n->renumeraDocs(renum);
        stringstream ss;
        ss << n->depuracpos();
        //clog << "ss=" << ss.str() << endl;
        CPPUNIT_ASSERT(ss.str() == "{(2,3)}");
        delete n;
}

void NodoTrieSCasoPrueba::prueba_leeTexto()
{
        NodoTrieS t;
        leeTexto("regr/verdad-latin1.txt", 1, t, false, true);
        CPPUNIT_ASSERT(t.preorden() == "LAIBERAR\xC1,NOSSIVERDAD");

        NodoTrieS u;
        leeTexto("regr/verdadlarga-latin1.txt", 1, u, false, true);
        //clog << "u=" << u.preorden() << endl;
        CPPUNIT_ASSERT(u.preorden() == "LAVERDADSINOSLIBERAR\xC1SIABCDEFGHI");

        NodoTrieS v;
        leeTexto("regr/verdad-latin1.txt", 1, v, true, true);
        //clog << "v=" << v.preorden() << endl;
        CPPUNIT_ASSERT(v.preorden() == "LIBERARANOSSIVERDAD");

        NodoTrieS w;
        leeTexto("regr/verdad-utf8.txt", 1, w, false, false);
        CPPUNIT_ASSERT(w.preorden() == "LAIBERARÁ,NOSSIVERDAD");
        
        NodoTrieS x;
        leeTexto("regr/verdad-utf8.txt", 1, x, true, false);
        clog << endl << "x=" << x.preorden() << endl;
        CPPUNIT_ASSERT(x.preorden() == "LIBERARANOSSIVERDAD");

}

void NodoTrieSCasoPrueba::setUp()
{
        return;
}
