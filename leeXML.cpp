// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file leeXML.cpp
 * Basado en ejemplo de expat
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: leeXML.cpp,v 1.11 2010/01/18 16:12:50 vtamara Exp $
 */

#include <stdio.h>
#include <iostream>
#include "expat.h"
#include "NodoTrieS.hpp"

using namespace std;
#if defined(__amigaos__) && defined(__USE_INLINE__)
#include <proto/expat.h>
#endif

#ifdef XML_LARGE_SIZE
#if defined(XML_USE_MSC_EXTENSIONS) && _MSC_VER < 1400
#define XML_FMT_INT_MOD "I64"
#else
#define XML_FMT_INT_MOD "ll"
#endif
#else
#define XML_FMT_INT_MOD "l"
#endif

const int MAXPROF = 100;

const char *elem[MAXPROF];
const char **vatts[MAXPROF];

int profundidad = 0;
int numdoc = -1;
NodoTrieS *nodotries;
XML_Parser parser = NULL;


static void XMLCALL
startElement(void *userData, const char *name, const char **atts)
{
        /*int i;
         for (i = 0; i < profundidad; i++)
            putchar('\t');
          puts(name); 
          for (i = 0; atts[i] != NULL; i+=2) {
        	  printf("%s -> %s\n", atts[i], atts[i+1]);
          } */
        if (profundidad < MAXPROF ) {
                elem[profundidad] = name;
                vatts[profundidad] = atts;
        }
        profundidad ++;
}

/** Elimina espacios redundantes en s */
string
solopal(string s) 
{
        string r="";
        bool eraesp = true;
        for (uint32_t i = 0; i < s.length() ; i++) {
                if (!isspace(s[i]) || !eraesp) {
                        r += s[i];
                } 
                eraesp = isspace(s[i]);
        }

        return r;
}


long inipal = -1000; // Posición en la que comienza la palabra ultpal

string ultpal = "";

static void XMLCALL
endElement(void *userData, const char *name)
{
        //long pa = XML_GetCurrentByteIndex(parser);

        if (ultpal != "") {
                string nec = solopal(ultpal);
                vector<string> pals=estalla(" ", ultpal);
                long la = 0;
                for(uint32_t i=0; i < pals.size(); i++) {
                        if (pals[i] != "") {
                                nodotries->insertaConEtiqueta(pals[i],
                                                elem[profundidad - 1],
                                                numdoc, inipal + la);
                                nodotries->insertaNormalizando(pals[i],
                                                numdoc, inipal + la, true);

                                //clog << "OJO insertado " << pals[i] << " en " << elem[profundidad - 1] << ", posición " << inipal + la << endl;
                                la += pals[i].length() + 1;
                        }
                }
                ultpal = "";
        }
        profundidad--;
}


static void XMLCALL
charHandler(void *userData, const char *s, int len)
{
        if (len >= 0 && profundidad > 0 && profundidad < MAXPROF) {
                //clog << "OJO charHandler(userData, " << s << ", " << len << ")" << endl;
                if (len > 0) {
                        string pal = utf8_a_latin1(s, len);
                        if (ultpal == "") {
                                long pa = XML_GetCurrentByteIndex(parser);
                                inipal = pa;
                        }
                        ultpal += pal;
                }
        }
}


/** Construye un trieS a partir de un XML*/
void leeXML(const char *na, long ndoc, NodoTrieS &t)
{
        ASSERT(na!=NULL && na[0] != '\0' && strlen(na)<FILENAME_MAX);
        ASSERT(ndoc >= 0);

        numdoc = ndoc;
        char buf[BUFSIZ];
        parser = XML_ParserCreate("ISO-8859-1");
        if (XML_SetEncoding(parser, "ISO-8859-1") != XML_STATUS_OK) {
                throw "No puede ponerse codificación ISO-8859-1" ;
        }
        int done;
        int depth = 0;
        XML_SetUserData(parser, &depth);
        XML_SetElementHandler(parser, startElement, endElement);
        XML_SetCharacterDataHandler(parser, charHandler);
        nodotries = &t;
        FILE *fh = fopen(na, "r");
        if (fh == NULL) {
                return;
        }
        do {
                uint32_t len = (int)fread(buf, 1, sizeof(buf), fh);
                done = len < sizeof(buf);
                if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
                        fprintf(stderr,
                                "%s at line %" XML_FMT_INT_MOD "u\n",
                                XML_ErrorString(XML_GetErrorCode(parser)),
                                XML_GetCurrentLineNumber(parser));
                        return;
                }
        } while (!done);
        XML_ParserFree(parser);
        fclose(fh);
}
