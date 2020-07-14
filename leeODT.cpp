// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file leeODT.cpp
 * Lee un archivo en formato odt de Open Document Format e incluye su 
 * información a un índice.
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: leeODT.cpp,v 1.11 2010/01/06 10:09:30 vtamara Exp $
 */


#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string>
#include <iostream>
#include <fstream>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
//#include <libxml/DOCBparser.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>


#include "funzipuno.hpp"
#include "NodoTrieS.hpp"
#include "comun.hpp"
#include "leeODT.hpp"

using namespace std;

/**
 * Aplica XSLT que  extrae textos al archivo de nombre arch.
 *
 * En vez de usar librería libxslt podría recorrerse el XML con expat
 * haciendo lo que hace la hoja de estilo.
 * @param dt Directorio temporal
 * @param arch nombre de archivo al cual se aplicará xslt
 * @param nomsal archivo donde se dejará resultado
 **/
void
aplicaXSLT(string dt, string arch, string nomsal)
{
        //cerr << "OJO aplicaXSLT(" << dt << ", " << arch << ", " << nomsal << ")" << endl;
        FILE *f = NULL;
        string nx = dt + "/convierte.xslt";
        //cerr << "OJO nx=" << nx << endl;
        f = fopen(nx.c_str(), "w+");
        fprintf(f, "<!DOCTYPE xsl:stylesheet [\n"
                "]>\n"
                "<xsl:stylesheet version=\"1.0\"\n"
                "	xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\"\n"
                "	xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\"\n"
                "	xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\"\n"
                "	xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\"\n"
                "	xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\"\n"
                "	xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\"\n"
                "	xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0\"\n"
                "	xmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
                "	xmlns:dc=\"http://purl.org/dc/elements/1.1/\"\n"
                "	xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\"\n"
                "	xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\"\n"
                "	xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\"\n"
                "	xmlns:chart=\"urn:oasis:names:tc:opendocument:xmlns:chart:1.0\"\n"
                "	xmlns:dr3d=\"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0\"\n"
                "	xmlns:math=\"http://www.w3.org/1998/Math/MathML\"\n"
                "	xmlns:form=\"urn:oasis:names:tc:opendocument:xmlns:form:1.0\"\n"
                "	xmlns:script=\"urn:oasis:names:tc:opendocument:xmlns:script:1.0\"\n"
                "	xmlns:ooo=\"http://openoffice.org/2004/office\"\n"
                "	xmlns:ooow=\"http://openoffice.org/2004/writer\"\n"
                "	xmlns:oooc=\"http://openoffice.org/2004/calc\"\n"
                "	xmlns:dom=\"http://www.w3.org/2001/xml-events\"\n"
                "	xmlns:xforms=\"http://www.w3.org/2002/xforms\"\n"
                "	xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\"\n"
                "	xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
                "	exclude-result-prefixes=\"office style text table draw fo xlink dc meta number svg chart dr3d math form script ooo ooow oooc dom xforms xsd xsi\">\n"
                "\n"
                "<xsl:output method=\"text\" encoding=\"ISO-8859-1\" />\n"
                "\n"
                "<xsl:template match=\"office:document-contet\">\n"
                "	<xsl:apply-templates select=\".//office:body\"/>\n"
                "</xsl:template>\n"
                "\n"
                "<xsl:template match=\"office:body\"> \n"
                "	<xsl:apply-templates/>\n"
                "</xsl:template>\n"
                "\n"
                "<xsl:template match=\"text:p\">\n"
                "	<xsl:apply-templates/>\n"
                "</xsl:template>\n"
                "\n"
                "<xsl:template match=\"text()|@*\">\n"
                "    <xsl:value-of select=\".\"/><xsl:text>\n"
                "</xsl:text>\n"
                "</xsl:template>\n"
                "\n"
                "<xsl:template match=\"text:h|text:index-title-template|text:table-of-content|text:user-index|office:automatic-styles|office:font-face-decls\">\n"
                "</xsl:template>\n"
                "\n"
                "</xsl:stylesheet>\n"
               );
        fclose(f);
        /* Porcion tomada de http://xmlsoft.org/XSLT/tutorial/libxslttutorial.html */
        int i;
        const char *params[16 + 1];
        int nbparams = 0;
        xsltStylesheetPtr cur = NULL;
        xmlDocPtr doc, res;

        xmlSubstituteEntitiesDefault(1);
        xmlLoadExtDtdDefaultValue = 1;
        cur = xsltParseStylesheetFile((const xmlChar *)nx.c_str());
        doc = xmlParseFile(arch.c_str());
        res = xsltApplyStylesheet(cur, doc, NULL);
        if (nomsal == "") {
                xsltSaveResultToFile(stdout, res, cur);
        } else {
                FILE *s=fopen(nomsal.c_str(), "w");
                if (s == NULL) {
                        stringstream ss;
                        ss << "No puede abrir " << nomsal ;
                        throw ss.str();
                }
                xsltSaveResultToFile(s, res, cur);
                fclose(s);
        }
        xsltFreeStylesheet(cur);
        xmlFreeDoc(res);
        xmlFreeDoc(doc);
        xsltCleanupGlobals();
        xmlCleanupParser();
}

/**
 * Extrae mimetype y content.xml del contenedor zip que recibe en el 
 * directorio dt
 * @param odt
 * @param odt Ruta al archivo ODT del cual extraer
 * @param dt Ruta al directorio tempral donde se dejarán archivos
 *
 * Ver funzipuno.c
 */
void
extraezip(string odt, string dt)
{
        //cerr << "OJO extraezip(" << odt << ", " << dt << ")" << endl;
        char nomout[PATH_MAX];
        FILE *in, *out;

        if ((in = fopen(odt.c_str(), "rb")) == (FILE *)NULL) {
                stringstream ss;
                ss << "No puede abrir " << odt ;
                throw ss.str();
        }
        snprintf(nomout, PATH_MAX, "%s/mimetype", dt.c_str());
        if ((out = fopen(nomout, "wb")) == (FILE *)NULL) {
                stringstream ss;
                ss << "No puede escribir " << nomout;
                fclose(in);
                throw ss.str();
        }
        unzipUno(in, "mimetype", out);
        fclose(out);

        fseek(in, 0, SEEK_SET);
        snprintf(nomout, PATH_MAX, "%s/content.xml", dt.c_str());
        if ((out = fopen(nomout, "wb")) == (FILE *)NULL) {
                stringstream ss;
                ss << "No puede escribir " << nomout ;
                fclose(in);
                throw ss.str();
        }
        unzipUno(in, "content.xml", out);
        fclose(out);

        fclose(in);
}


/**
 * Prepara directorio con lo extraido de un odt 
 * @param odt Ruta a archivo odt
 **/
string prepara(string odt)
{
        //cerr << "OJO prepara(" << odt << ")" << endl;
        string dt = directorio_temp();
        //cerr << "OJO prepara. dt=" << dt << endl;
        extraezip(odt, dt);
        string mime = dt + string("/mimetype");
        fstream is(mime.c_str(), ios_base::in);
        char tipo[100];
        is.getline(tipo, 100);
        if (strcmp(tipo, "application/vnd.oasis.opendocument.text") != 0) {
                throw "Sólo puede convertir textos OpenDocument" ;
        }
        return dt;
        //xsltproc  $md/odtatexto.xsl /var/tmp/odtatexto/content.xml > $s
}

/** Construye un trieS a partir de un ODT */
void leeODT(const char *na, long ndoc, NodoTrieS &t, bool normalizaPal)
{
        ASSERT(na!=NULL && na[0] != '\0' && strlen(na)<FILENAME_MAX);
        ASSERT(ndoc >= 0);

        //cerr << "OJO leeODT(" << na << ", " << ndoc << ", t)" << endl;

        string dt = prepara(string(na));
        // cerr << "OJO dt = " << dt << endl;
        string ns = dt + string("/salida.txt");
        // cerr << "OJO ns = " << ns << endl;
        aplicaXSLT(dt, dt + "/content.xml", ns);
        leeTexto(ns.c_str(), ndoc, t, normalizaPal);
        unlink(string(dt + "/mimetype").c_str());
        unlink(string(dt + "/content.xml").c_str());
        unlink(string(dt + "/salida.txt").c_str());
        unlink(string(dt + "/convierte.xslt").c_str());
        rmdir(dt.c_str());
}

