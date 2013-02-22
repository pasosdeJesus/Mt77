/** @file sha256.hpp
 * Calcula hash sha256
 *
 * Agradablemente copiado de sha256.c y encabezados de LibTomCrypt 1.16
 *
 * @package Mt77
 * @author Tom St Denis, tomstdenis@gmail.com, http://libtomcrypt.com
 *
 * LibTomCrypt, modular cryptographic library -- Tom St Denis
 *
 * LibTomCrypt is a library that provides various cryptographic
 * algorithms in a highly modular and flexible manner.
 *
 * The library is free for all purposes without any express
 * guarantee it works.
 *
 * @version   $Id: sha256.hpp,v 1.7 2010/03/08 12:02:40 vtamara Exp $
 */

#if !defined(sha256_hpp)
#define sha256_hpp

#include <stdint.h>

#define ENDIAN_LITTLE

// De tomcrypt.h

#define CRYPT   0x0116
#define SCRYPT  "1.16"

/* Máximo tamaño de un bloque del hash */
#define MAXBLOCKSIZE  128

/* tamaño de la tabla de descriptores */
#define TAB_SIZE      32

/* códigos de error */
enum {
        CRYPT_OK=0,             /* El resultado está bien*/
        CRYPT_ERROR,            /* Error genérico */
        CRYPT_NOP,              /* No es falla pero no se efectuó operación */

        CRYPT_INVALID_KEYSIZE,  /* Tamaño de llave invalido*/
        CRYPT_INVALID_ROUNDS,   /* Número de rondas invalido */
        CRYPT_FAIL_TESTVECTOR,  /* El algoritmo fallo con vectores de prueba */

        CRYPT_BUFFER_OVERFLOW,  /* No hay suficiente espacio para el resultado */
        CRYPT_INVALID_PACKET,   /* Paquete de entrada invalido */

        CRYPT_INVALID_PRNGSIZE, /* Número de bits invalidos pra PRNG */
        CRYPT_ERROR_READPRNG,   /* No pudo leer suficiente de un PRNG */

        CRYPT_INVALID_CIPHER,   /* Cifrador invalido */
        CRYPT_INVALID_HASH,     /* Hash invalido */
        CRYPT_INVALID_PRNG,     /* PRNG invalido */

        CRYPT_MEM,              /* Sin memoria */

        CRYPT_PK_TYPE_MISMATCH, /* Tipos no equivalente de llaves PK */
        CRYPT_PK_NOT_PRIVATE,   /* Requiere un llave PK privada */

        CRYPT_INVALID_ARG,      /* Argumento general invalido */
        CRYPT_FILE_NOTFOUND,    /* ARchivo no encontrado */

        CRYPT_PK_INVALID_TYPE,  /* Tipo de llave PK invalido */
        CRYPT_PK_INVALID_SYSTEM,/* Sistema PK invalido */
        CRYPT_PK_DUP,           /* LLave duplicada en anillo de llaves */
        CRYPT_PK_NOT_FOUND,     /* Llave no encontrada en anillo de llaves */
        CRYPT_PK_INVALID_SIZE,  /* Tamaño de entrada invalido en parametros de PK */

        CRYPT_INVALID_PRIME_SIZE,/* Tamaño de primo invalido */
        CRYPT_PK_INVALID_PADDING /* Relleno en entrada invalido */
};

// De tomcrypt_macros.h

#define CONST64(n) n ## ULL
typedef uint64_t ulong64;

typedef uint32_t ulong32;

#ifdef ENDIAN_NEUTRAL

#define STORE32L(x, y)                                                                     \
     { (y)[3] = (unsigned char)(((x)>>24)&255); (y)[2] = (unsigned char)(((x)>>16)&255);   \
       (y)[1] = (unsigned char)(((x)>>8)&255); (y)[0] = (unsigned char)((x)&255); }

#define LOAD32L(x, y)                            \
     { x = ((uint64_t)((y)[3] & 255)<<24) | \
           ((uint64_t)((y)[2] & 255)<<16) | \
           ((uint64_t)((y)[1] & 255)<<8)  | \
           ((uint64_t)((y)[0] & 255)); }

#define STORE64L(x, y)                                                                     \
     { (y)[7] = (unsigned char)(((x)>>56)&255); (y)[6] = (unsigned char)(((x)>>48)&255);   \
       (y)[5] = (unsigned char)(((x)>>40)&255); (y)[4] = (unsigned char)(((x)>>32)&255);   \
       (y)[3] = (unsigned char)(((x)>>24)&255); (y)[2] = (unsigned char)(((x)>>16)&255);   \
       (y)[1] = (unsigned char)(((x)>>8)&255); (y)[0] = (unsigned char)((x)&255); }

#define LOAD64L(x, y)                                                       \
     { x = (((ulong64)((y)[7] & 255))<<56)|(((ulong64)((y)[6] & 255))<<48)| \
           (((ulong64)((y)[5] & 255))<<40)|(((ulong64)((y)[4] & 255))<<32)| \
           (((ulong64)((y)[3] & 255))<<24)|(((ulong64)((y)[2] & 255))<<16)| \
           (((ulong64)((y)[1] & 255))<<8)|(((ulong64)((y)[0] & 255))); }

#define STORE32H(x, y)                                                                     \
     { (y)[0] = (unsigned char)(((x)>>24)&255); (y)[1] = (unsigned char)(((x)>>16)&255);   \
       (y)[2] = (unsigned char)(((x)>>8)&255); (y)[3] = (unsigned char)((x)&255); }

#define LOAD32H(x, y)                            \
     { x = ((uint64_t)((y)[0] & 255)<<24) | \
           ((uint64_t)((y)[1] & 255)<<16) | \
           ((uint64_t)((y)[2] & 255)<<8)  | \
           ((uint64_t)((y)[3] & 255)); }

#define STORE64H(x, y)                                                                     \
   { (y)[0] = (unsigned char)(((x)>>56)&255); (y)[1] = (unsigned char)(((x)>>48)&255);     \
     (y)[2] = (unsigned char)(((x)>>40)&255); (y)[3] = (unsigned char)(((x)>>32)&255);     \
     (y)[4] = (unsigned char)(((x)>>24)&255); (y)[5] = (unsigned char)(((x)>>16)&255);     \
     (y)[6] = (unsigned char)(((x)>>8)&255); (y)[7] = (unsigned char)((x)&255); }

#define LOAD64H(x, y)                                                      \
   { x = (((ulong64)((y)[0] & 255))<<56)|(((ulong64)((y)[1] & 255))<<48) | \
         (((ulong64)((y)[2] & 255))<<40)|(((ulong64)((y)[3] & 255))<<32) | \
         (((ulong64)((y)[4] & 255))<<24)|(((ulong64)((y)[5] & 255))<<16) | \
         (((ulong64)((y)[6] & 255))<<8)|(((ulong64)((y)[7] & 255))); }

#endif /* ENDIAN_NEUTRAL */

#ifdef ENDIAN_LITTLE

#if !defined(LTC_NO_BSWAP) && (defined(INTEL_CC) || (defined(__GNUC__) && (defined(__DJGPP__) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__i386__) || defined(__x86_64__))))

#define STORE32H(x, y)           \
asm __volatile__ (               \
   "bswapl %0     \n\t"          \
   "movl   %0,(%1)\n\t"          \
   "bswapl %0     \n\t"          \
      ::"r"(x), "r"(y));

#define LOAD32H(x, y)          \
asm __volatile__ (             \
   "movl (%1),%0\n\t"          \
   "bswapl %0\n\t"             \
   :"=r"(x): "r"(y));

#else

#define STORE32H(x, y)                                                                     \
     { (y)[0] = (unsigned char)(((x)>>24)&255); (y)[1] = (unsigned char)(((x)>>16)&255);   \
       (y)[2] = (unsigned char)(((x)>>8)&255); (y)[3] = (unsigned char)((x)&255); }

#define LOAD32H(x, y)                            \
     { x = ((uint64_t)((y)[0] & 255)<<24) | \
           ((uint64_t)((y)[1] & 255)<<16) | \
           ((uint64_t)((y)[2] & 255)<<8)  | \
           ((uint64_t)((y)[3] & 255)); }

#endif


/* Procesador x86_64 */
#if !defined(LTC_NO_BSWAP) && (defined(__GNUC__) && defined(__x86_64__))

#define STORE64H(x, y)           \
asm __volatile__ (               \
   "bswapq %0     \n\t"          \
   "movq   %0,(%1)\n\t"          \
   "bswapq %0     \n\t"          \
      ::"r"(x), "r"(y));

#define LOAD64H(x, y)          \
asm __volatile__ (             \
   "movq (%1),%0\n\t"          \
   "bswapq %0\n\t"             \
   :"=r"(x): "r"(y));

#else

#define STORE64H(x, y)                                                                     \
   { (y)[0] = (unsigned char)(((x)>>56)&255); (y)[1] = (unsigned char)(((x)>>48)&255);     \
     (y)[2] = (unsigned char)(((x)>>40)&255); (y)[3] = (unsigned char)(((x)>>32)&255);     \
     (y)[4] = (unsigned char)(((x)>>24)&255); (y)[5] = (unsigned char)(((x)>>16)&255);     \
     (y)[6] = (unsigned char)(((x)>>8)&255); (y)[7] = (unsigned char)((x)&255); }

#define LOAD64H(x, y)                                                      \
   { x = (((ulong64)((y)[0] & 255))<<56)|(((ulong64)((y)[1] & 255))<<48) | \
         (((ulong64)((y)[2] & 255))<<40)|(((ulong64)((y)[3] & 255))<<32) | \
         (((ulong64)((y)[4] & 255))<<24)|(((ulong64)((y)[5] & 255))<<16) | \
         (((ulong64)((y)[6] & 255))<<8)|(((ulong64)((y)[7] & 255))); }

#endif

#ifdef ENDIAN_32BITWORD

#define STORE32L(x, y)        \
     { ulong32  __t = (x); XMEMCPY(y, &__t, 4); }

#define LOAD32L(x, y)         \
     XMEMCPY(&(x), y, 4);

#define STORE64L(x, y)                                                                     \
     { (y)[7] = (unsigned char)(((x)>>56)&255); (y)[6] = (unsigned char)(((x)>>48)&255);   \
       (y)[5] = (unsigned char)(((x)>>40)&255); (y)[4] = (unsigned char)(((x)>>32)&255);   \
       (y)[3] = (unsigned char)(((x)>>24)&255); (y)[2] = (unsigned char)(((x)>>16)&255);   \
       (y)[1] = (unsigned char)(((x)>>8)&255); (y)[0] = (unsigned char)((x)&255); }

#define LOAD64L(x, y)                                                       \
     { x = (((ulong64)((y)[7] & 255))<<56)|(((ulong64)((y)[6] & 255))<<48)| \
           (((ulong64)((y)[5] & 255))<<40)|(((ulong64)((y)[4] & 255))<<32)| \
           (((ulong64)((y)[3] & 255))<<24)|(((ulong64)((y)[2] & 255))<<16)| \
           (((ulong64)((y)[1] & 255))<<8)|(((ulong64)((y)[0] & 255))); }

#else /* 64-bit words then  */

#define STORE32L(x, y)        \
     { ulong32 __t = (x); XMEMCPY(y, &__t, 4); }

#define LOAD32L(x, y)         \
     { XMEMCPY(&(x), y, 4); x &= 0xFFFFFFFF; }

#define STORE64L(x, y)        \
     { ulong64 __t = (x); XMEMCPY(y, &__t, 8); }

#define LOAD64L(x, y)         \
    { XMEMCPY(&(x), y, 8); }

#endif /* ENDIAN_64BITWORD */

#endif /* ENDIAN_LITTLE */

#ifdef ENDIAN_BIG
#define STORE32L(x, y)                                                                     \
     { (y)[3] = (unsigned char)(((x)>>24)&255); (y)[2] = (unsigned char)(((x)>>16)&255);   \
       (y)[1] = (unsigned char)(((x)>>8)&255); (y)[0] = (unsigned char)((x)&255); }

#define LOAD32L(x, y)                            \
     { x = ((uint64_t)((y)[3] & 255)<<24) | \
           ((uint64_t)((y)[2] & 255)<<16) | \
           ((uint64_t)((y)[1] & 255)<<8)  | \
           ((uint64_t)((y)[0] & 255)); }

#define STORE64L(x, y)                                                                     \
   { (y)[7] = (unsigned char)(((x)>>56)&255); (y)[6] = (unsigned char)(((x)>>48)&255);     \
     (y)[5] = (unsigned char)(((x)>>40)&255); (y)[4] = (unsigned char)(((x)>>32)&255);     \
     (y)[3] = (unsigned char)(((x)>>24)&255); (y)[2] = (unsigned char)(((x)>>16)&255);     \
     (y)[1] = (unsigned char)(((x)>>8)&255); (y)[0] = (unsigned char)((x)&255); }

#define LOAD64L(x, y)                                                      \
   { x = (((ulong64)((y)[7] & 255))<<56)|(((ulong64)((y)[6] & 255))<<48) | \
         (((ulong64)((y)[5] & 255))<<40)|(((ulong64)((y)[4] & 255))<<32) | \
         (((ulong64)((y)[3] & 255))<<24)|(((ulong64)((y)[2] & 255))<<16) | \
         (((ulong64)((y)[1] & 255))<<8)|(((ulong64)((y)[0] & 255))); }

#ifdef ENDIAN_32BITWORD

#define STORE32H(x, y)        \
     { ulong32 __t = (x); XMEMCPY(y, &__t, 4); }

#define LOAD32H(x, y)         \
     XMEMCPY(&(x), y, 4);

#define STORE64H(x, y)                                                                     \
     { (y)[0] = (unsigned char)(((x)>>56)&255); (y)[1] = (unsigned char)(((x)>>48)&255);   \
       (y)[2] = (unsigned char)(((x)>>40)&255); (y)[3] = (unsigned char)(((x)>>32)&255);   \
       (y)[4] = (unsigned char)(((x)>>24)&255); (y)[5] = (unsigned char)(((x)>>16)&255);   \
       (y)[6] = (unsigned char)(((x)>>8)&255);  (y)[7] = (unsigned char)((x)&255); }

#define LOAD64H(x, y)                                                       \
     { x = (((ulong64)((y)[0] & 255))<<56)|(((ulong64)((y)[1] & 255))<<48)| \
           (((ulong64)((y)[2] & 255))<<40)|(((ulong64)((y)[3] & 255))<<32)| \
           (((ulong64)((y)[4] & 255))<<24)|(((ulong64)((y)[5] & 255))<<16)| \
           (((ulong64)((y)[6] & 255))<<8)| (((ulong64)((y)[7] & 255))); }

#else /* 64-bit words then  */

#define STORE32H(x, y)        \
     { ulong32 __t = (x); XMEMCPY(y, &__t, 4); }

#define LOAD32H(x, y)         \
     { XMEMCPY(&(x), y, 4); x &= 0xFFFFFFFF; }

#define STORE64H(x, y)        \
     { ulong64 __t = (x); XMEMCPY(y, &__t, 8); }

#define LOAD64H(x, y)         \
    { XMEMCPY(&(x), y, 8); }

#endif /* ENDIAN_64BITWORD */
#endif /* ENDIAN_BIG */

#define BSWAP(x)  ( ((x>>24)&0x000000FFUL) | ((x<<24)&0xFF000000UL)  | \
                    ((x>>8)&0x0000FF00UL)  | ((x<<8)&0x00FF0000UL) )


#define ROLc ROL
#define RORc ROR


/* rotates the hard way */
#define ROL(x, y) ( (((uint64_t)(x)<<(uint64_t)((y)&31)) | (((uint64_t)(x)&0xFFFFFFFFUL)>>(uint64_t)(32-((y)&31)))) & 0xFFFFFFFFUL)
#define ROR(x, y) ( ((((uint64_t)(x)&0xFFFFFFFFUL)>>(uint64_t)((y)&31)) | ((uint64_t)(x)<<(uint64_t)(32-((y)&31)))) & 0xFFFFFFFFUL)


#define ROL64c ROL64
#define ROR64c ROR64

#define ROL64(x, y) \
    ( (((x)<<((ulong64)(y)&63)) | \
      (((x)&CONST64(0xFFFFFFFFFFFFFFFF))>>((ulong64)64-((y)&63)))) & CONST64(0xFFFFFFFFFFFFFFFF))

#define ROR64(x, y) \
    ( ((((x)&CONST64(0xFFFFFFFFFFFFFFFF))>>((ulong64)(y)&CONST64(63))) | \
      ((x)<<((ulong64)(64-((y)&CONST64(63)))))) & CONST64(0xFFFFFFFFFFFFFFFF))

#ifndef MAX
   #define MAX(x, y) ( ((x)>(y))?(x):(y) )
#endif

#ifndef MIN
   #define MIN(x, y) ( ((x)<(y))?(x):(y) )
#endif

/* extract a byte portably */
#ifdef _MSC_VER
   #define byte(x, n) ((unsigned char)((x) >> (8 * (n))))
#else
   #define byte(x, n) (((x) >> (8 * (n))) & 255)
#endif


// De tomctyp_argchk.h


#include <signal.h>
#include <stdint.h>
#include <stdint.h>

/* this is the default LibTomCrypt macro  */
void crypt_argchk(char *v, char *s, int d);
#define LTC_ARGCHK(x) if (!(x)) { crypt_argchk(#x, __FILE__, __LINE__); }
#define LTC_ARGCHKVD(x) LTC_ARGCHK(x)

struct sha256_state
{
        ulong64 length;
        ulong32 state[8], curlen;
        unsigned char buf[64];
};

typedef union Hash_state {
        char dummy[1];
        struct sha256_state sha256;
        void *data;
} hash_state;


/** Descriptor de hash */
extern  struct ltc_hash_descriptor
        {
                /** nombre del hash */
                char *name;
                /** ID interna */
                unsigned char ID;
                /** Tamaño del condensado en octetos */
                uint64_t hashsize;
                /** Tamaño del bloque de entrada en octetos */
                uint64_t blocksize;
                /** ASN.1 OID */
                uint64_t OID[16];
                /** Longitud del codificado DER */
                uint64_t OIDlen;

                /** Inicializa el estado de un hash
                  @param hash   Hash por inicializar
                  @return CRYPT_OK si tiene éxito
                */
                int (*init)(hash_state *hash);

                /** Procesa un bloque de datos
                  @param hash   Estado del hash
                  @param in     Datos para el hash
                  @param inlen  Longitud de los datos (octetos)
                  @return CRYPT_OK si tiene éxito
                */
                int (*process)(hash_state *hash, const unsigned char *in, uint64_t inlen);

                /** Produce el condensado y lo almacena
                  @param hash   Estado del hash
                  @param out    [out] Destino del condensado
                  @return CRYPT_OK si tiene éxito
                */
                int (*done)(hash_state *hash, unsigned char *out);

                /** auto-prueba
                  @return CRYPT_OK si tiene éxito, CRYPT_NOP si se han deshabilitado auto-pruebas
                */
                int (*test)(void);

                /* Función respuesta a hmac acelerado: si necesita multiples
                 * paquetes usar el hmac_memory generico y proveer una función
                 * de respuesta */
                int  (*hmac_block)(const unsigned char *key, uint64_t  keylen,
                                   const unsigned char *in,  uint64_t  inlen,
                                   unsigned char *out, uint64_t *outlen);

        }
hash_descriptor[];

/**
 * Inicializa el estado del condensado
 * @param md   Estado del condensado que usted desea inicializar
 * @return CRYPT_OK si tiene éxito
 */
int sha256_init(hash_state * md);


/**
 * Procesa un bloque de memoria con el calculador de condensado
 * @param md     Estado del condensado
 * @param in     Datos por pasar por el hash
 * @param inlen  Longitud de los datos en in (octetos)
 * @return CRYPT_OK si tiene éxito
 */
int sha256_process(hash_state * md, const unsigned char *in,
                   uint64_t inlen);


/**
 * Termina el cálculo para retornar el condensado final.
 * @param md  Estado del condensado
 * @param hash [out] Destino del condensado (32 bytes)
 * @return CRYPT_OK si tiene éxito
 */
int sha256_done(hash_state *md, unsigned char *hash);


extern const struct ltc_hash_descriptor sha256_desc;

/* Macro para hacer funciones de procesamiento del hash */
#define HASH_PROCESS(func_name, compress_name, state_var, block_size)                       \
int func_name (hash_state * md, const unsigned char *in, uint64_t inlen)               \
{                                                                                           \
    uint64_t n;                                                                        \
    int           err;                                                                      \
    LTC_ARGCHK(md != NULL);                                                                 \
    LTC_ARGCHK(in != NULL);                                                                 \
    if (md-> state_var .curlen > sizeof(md-> state_var .buf)) {                             \
       return CRYPT_INVALID_ARG;                                                            \
    }                                                                                       \
    while (inlen > 0) {                                                                     \
        if (md-> state_var .curlen == 0 && inlen >= block_size) {                           \
           if ((err = compress_name (md, (unsigned char *)in)) != CRYPT_OK) {               \
              return err;                                                                   \
           }                                                                                \
           md-> state_var .length += block_size * 8;                                        \
           in             += block_size;                                                    \
           inlen          -= block_size;                                                    \
        } else {                                                                            \
           n = MIN(inlen, (block_size - md-> state_var .curlen));                           \
           memcpy(md-> state_var .buf + md-> state_var.curlen, in, (size_t)n);              \
           md-> state_var .curlen += n;                                                     \
           in             += n;                                                             \
           inlen          -= n;                                                             \
           if (md-> state_var .curlen == block_size) {                                      \
              if ((err = compress_name (md, md-> state_var .buf)) != CRYPT_OK) {            \
                 return err;                                                                \
              }                                                                             \
              md-> state_var .length += 8*block_size;                                       \
              md-> state_var .curlen = 0;                                                   \
           }                                                                                \
       }                                                                                    \
    }                                                                                       \
    return CRYPT_OK;                                                                        \
}

#include <string>


/**
 * Retorna cadena en hexadecimal a partir de condensado retornado por
 * sha256_done
 *
 * @param hash Condensado calculado
 * @param l    Longitud del condensado
 *
 * @return Representación hexadecimal como para imprimir
 */
std::string hexaDeHash(unsigned const char *hash, int l);


/**
 * Calcula sha256 al archivo dado
 * @param narch nombre de archivo (debe existir)
 * @return Cadena de 64 caracteres con los 32 números del hash sha256
 *  	   del archivo cada uno en hexadecimal
 */
std::string sha256archivo(std::string narch);

#endif
