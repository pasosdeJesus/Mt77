// vim: set expandtab tabstop=8 shiftwidth=8 foldmethod=marker:
/** @file funzipuno.cpp
 * Descomprime un archivo de un contenedro ZIP
 *
 * Basado  en
 * - Archivo funzip incluido en unzip 5.12
 *   http://read.pudn.com/downloads/sourcecode/zip/1208/funzip.c__.htm
 *   que dice: "funzip.c -- put in the public domain by Mark Adler"
 * - Tabla crc_32_tab de 
 *   http://www.haxxed.com/belkin/download/tools/trx.c 
 * - Ideas de:
 * 	- Archivo funzip de versiones recientes de unzip (que no son de 
 * 		dominio público)
 * 	- Ejemplo miniunz incluido en zlib-1.2.3 
 * 	- examples/zpipe.c de zlib-1.2.3
 *
 * @package Mt77
 * @author Vladimir Támara Patiño. vtamara@pasosdeJesus.org
 * Dominio público. 2009.  Sin garantías
 * http://creativecommons.org/licenses/publicdomain/
 * @version   $Id: funzipuno.cpp,v 1.6 2010/01/18 16:12:50 vtamara Exp $
 */

#include <cstdlib>
#include <cstring>
#include "funzipuno.hpp"
#include <stdint.h>

using namespace std;

/* Globals */
ulg outsiz;                     /* total bytes written to out */
int encrypted;                  /* flag to turn on decryption */
int qflag = 1;                  /* turn off messages in inflate.c */

// Colchon de entrada
#define TAMCE 16384
unsigned char colen[TAMCE];

// Colchon de salida
#define TAMCS 32768
unsigned char colsal[TAMCS];


/**
 * Run a set of bytes through the crc shift register.  If s is a NULL
 * pointer, then initialize the crc shift register contents instead.
 * Return the current crc in either case.
 **/
ulg updcrc(uch *s, ulg n)
{
        register ulg c;       /* temporary variable */

        static ulg crc = 0xffffffffL; /* shift register contents */

        if (s == (uch *)NULL) {
                c = 0xffffffffL;
        } else {
                c = crc;
                while (n--) {
                        c = crc_32_tab[((int)c ^ (*s++)) & 0xff] ^ (c >> 8);
                }
        }
        crc = c;
        return c ^ 0xffffffffL;       /* (instead of ~c for 64-bit machines) */
}

/**
 * Exit on error with a message and a code 
 **/
void err(int n, char *m)
{
        fprintf(stderr, "funzip error: %s\n", m);
        exit(n);
}


/**
 * Given a zip file on stdin, decompress the first entry to stdout.
 **/
void unzipUno(FILE *in, char *nombre, FILE *out)
{
        //fprintf(stderr, "OJO unzipUno(%p, %s, %p)\n", in, nombre, out);
        uch h[LOCHDR];
        /* read local header, check validity, and skip name and extra fields */
        int n = getc(in);
        n |= getc(in) << 8;
        h[0] = getc(in);
        h[1] = getc(in);
        while (n == ZIPMAG && SH(h) == LOCREM) {
                char noma[PATH_MAX];
                int cn;
                //		printf("\n ------ \n OJO ZIPMAG n=%i, ftell=%li\n", n, ftell(in));
                if (fread((char *)(h+2), 1, LOCHDR - 2, in) != LOCHDR - 2) {
                        err(3, "invalid zip file");
                }
                //		printf("OJO h=%s\n", h);
                if (SH(h + LOCHOW) != Z_STORED && SH(h + LOCHOW) != Z_DEFLATED) {
                        err(3, "first entry not deflated or stored--can't funzip");
                }
                //		printf("OJO lochow=%i\n", SH(h+LOCHOW));
                //		printf("OJO locfil=%i\n", SH(h+LOCFIL));
                cn = 0;
                for (n = SH(h + LOCFIL); n--; ) {
                        int g = getc(in);
                        //			printf("%c", (char)g);
                        if (cn < PATH_MAX - 1) {
                                noma[cn] = g;
                                cn++;
                        }
                }
                noma[cn] = '\0';
                //		printf("\nOJO locext=%i\n", SH(h+LOCEXT));
                for (n = SH(h + LOCEXT); n--; ) {
                        getc(in);
                        //			printf("%c", (char)g);
                }
                encrypted = h[LOCFLG] & CRPFLG;
                //		printf("OJO encrypted=%i\n", encrypted);

                /* if entry encrypted, decrypt and validate encryption header */
                if (encrypted) {
                        err(3, "cannot decrypt entry (need to recompile with full crypt.c)");
                }


                /* prepare output buffer and crc */
                outsiz = 0L;
                updcrc(NULL, 0L);
                z_stream stream;
                int tl ;


                /* decompress */
                if (h[LOCHOW]) {                             /* deflated entry */
                        //			printf("OJO deflated\n");
                        stream.zalloc = (alloc_func)0;
                        stream.zfree = (free_func)0;
                        stream.opaque = (voidpf)0;
                        int ri = inflateInit2(&stream, -MAX_WBITS);
                        if (ri != Z_OK) {
                                fprintf(stderr, "No pudo inicializar stream, err=%i\n",
                                        ri);
                                exit(1);
                        }

                        ri = Z_STREAM_END + 1;
                        //int w = 0;
                        int flush = 0;
                        do {
                                if ((tl = fread((char *)colen, 1, TAMCE, in)) < 0)
                                        err(4, "vacío");
                                //				printf("OJO tl=%i\n", tl);
                                stream.next_in = colen;
                                stream.avail_in = tl;
                                do {
                                        stream.next_out = (Bytef*)colsal;
                                        stream.avail_out = TAMCS;
                                        flush = feof(in) ? Z_FINISH : Z_NO_FLUSH;
                                        ri = inflate(&stream, flush);
                                        if (ri == Z_STREAM_ERROR) {
                                                err(4, "problema descomprimiendo");
                                        }
                                        uint64_t w =
                                                TAMCS - stream.avail_out;
                                        updcrc(colsal, w);
                                        if (strcmp(noma, nombre) == 0) {
                                                if (fwrite((char *)colsal,
                                                                1, w, out) != w ||
                                                                ferror(out))   {
                                                        err(9, "problema al escribir");
                                                }
                                        }
                                        outsiz += w;
                                } while (stream.avail_out == 0);
                        } while (flush != Z_FINISH && ri != Z_STREAM_END) ;
                        inflateEnd(&stream);
                } else {                             /* stored entry */

                        //			printf("OJO stored\n");
                        tl = 0;
                        stream.avail_in = 0;
                        register ulg n;
                        n = LG(h + LOCLEN);
                        if (n != LG(h + LOCSIZ)) {
                                fprintf(stderr, "len %ld, siz %ld\n", n, LG(h + LOCSIZ));
                                err(4, "invalid compressed data--length mismatch");
                        }
                        uint64_t nsal = 0;
                        while (n--) {
                                ush c = getc(in);
                                colsal[nsal++] = (uch)c;
                                if (nsal == TAMCS)    /* do FlushOutput() */
                                {
                                        updcrc(colsal, nsal);
                                        if (strcmp(noma, nombre) == 0)
                                        {
                                                if (fwrite((char *)colsal, 1,
                                                                nsal, out) != nsal) {
                                                        err(9, "out of space on stdout");
                                                }
                                        }
                                        outsiz += nsal;
                                        nsal = 0;
                                }
                        }
                        if (nsal)   /* flush one last time; no need to reset outptr/outcnt */
                        {
                                updcrc(colsal, nsal);
                                if (strcmp(noma, nombre) == 0)
                                {
                                        if (fwrite((char *)colsal, 1, nsal, out) != nsal) {
                                                err(9, "out of space on stdout");
                                        }
                                }
                                outsiz += nsal;
                        }
                }
                if (strcmp(noma, nombre) == 0) {
                        fflush(out);
                }

                if ((h[LOCFLG] & EXTFLG)) {
                        //			printf("OJO EXTFLG stream.avail_in=%i\n", stream.avail_in);
                        uint32_t i;
                        for (i = 0; i < EXTHDR && i < stream.avail_in; i++) {
                                h[LOCCRC - 4 + i] =
                                        colen[tl - stream.avail_in + i];
                        }
                        stream.avail_in -= i;
                        if (i < EXTHDR) {
                                //				fprintf(stderr," OJO toco leer mas exthdr\n");
                                if (fread((char *)h + LOCCRC - 4 + i, 1,
                                                EXTHDR - i, in) != EXTHDR - i) {
                                        err(3, "zip file ended prematurely");
                                }
                                stream.avail_in = 0;
                                // no devolverse
                        }

                        /*			if (fread((char *)h + LOCCRC - 4, 1, EXTHDR, in) != EXTHDR) {
                        				err(3, "zip file ended prematurely");
                        			} */
                }

                /* validate decompression */
                uint64_t crcu = updcrc(colsal, 0L);
                if (LG(h + LOCCRC) != crcu) {
                        fprintf(stderr, "invalid compressed data--crc error en %s, dio %li, se esperaba %li\n", noma, crcu, LG(h + LOCCRC));
                }
                if (LG(h + LOCLEN) != outsiz) {
                        fprintf(stderr, "invalid compressed data--length error en %s, dio %li, se esperaba %li\n", noma, outsiz, LG(h + LOCLEN));
                }

                /* check if there are more entries */
                if (stream.avail_in > 0) {
                        //			fprintf(stderr, "OJO stream.avail_in = %i \n", stream.avail_in);
                        long pa=ftell(in);
                        //			fprintf(stderr, "OJO ftell = %li \n", pa);
                        fseek(in, pa - stream.avail_in, SEEK_SET);
                        //			fprintf(stderr, "OJO ftell = %li \n", ftell(in));
                }
                if (!feof(in)) {
                        n = getc(in);
                        n |= getc(in) << 8;
                        h[0] = getc(in);
                        h[1] = getc(in);
                } else {
                        n = 0;
                }
        }
        if (n != 0 && SH(h) != 513) {
                //		printf("OJO n=%i\n", n);
                err(3, "input not a zip or gzip file");
        }
}

