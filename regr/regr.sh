#!/bin/sh
# Pruebas de regresión a Mt77
# Dominio público. 2009. vtamara@pasosdeJesus.org

# para poder definir facilmente si se sale del programa en caso de
# encontrar un error
respuesta_error() {
    echo "** Error: $1"
    exit # salir en caso de encontrar errores
}

# para ponerlo antes de un comando en caso de querer correrlo con egdb
correr_con_egdb() {
    egdb --args $@
}

# se borra el indice y la tendencia del parametro 1
limpiar () {
    indice="$1.indice"
    tendencia="$1.indice.tendencia"

    if [ -f "$indice" ]
    then
        rm -f "$indice"
    fi
    # echo "rm -r $1.indice.tendencia"
    if [ -f "$tendencia" ]
    then
        rm -f "$tendencia"
    fi
}

# 1 y 2 son los archivos a comparar, 3 es el mensaje de error
comparar () {

    if [ ! -f "$1" ]
    then
        respuesta_error "no existe archivo $1"
    fi

    if [ ! -f "$2" ]
    then
        respuesta_error "no existe archivo $2"
    fi

    cmp "$1" "$2"
    if test "$?" != "0"
    then
        diff "$1" "$2"
        respuesta_error "$3"
    fi
}

buscar_palabras () {
    indice="$1"
    for palabra in $2
    do
        ../buscador "$indice" "$palabra"
    done
}

requiere () {
    for archivo in "$@"
    do
        # echo "$archivo"
        if [ ! -f "$archivo" ]
        then
            respuesta_error "archivo $archivo no encontrado"
        fi
    done
}

prueba_indice_vacio () {
    echo "Índice vacio:"
    limpiar r0
    ../indexador r0.indice t.indice "http://r" "" > r0.out 2>&1
    ../buscador r0.indice LA >> r0.out 2>&1
    ../operaindice lista r0.indice >> r0.out 2>&1

    limpiar rc0
    ../indexador rc0.indice t.indice "http://r" "" >> r0.out 2>&1

    echo "ram" >> r0.out 2>&1
    limpiar rm0
    ../operaindice mezclaram rm0.indice r0.indice rc0.indice >> r0.out 2>&1
    ../operaindice lista rm0.indice >> r0.out 2>&1
    ../operaindice mezcladisco rd0.indice r0.indice rc0.indice >> r0.out 2>&1
    ../operaindice lista rd0.indice >> r0.out 2>&1

    comparar r0.out esp/r0.out "r0 falló"
}

prueba2 () {

    requiere r1-latin1.txt

    echo "Creación de índice r1 a partir de texto LATIN1";

    indice="r1.indice"

    limpiar r1
    ../indexador -l "$indice" t.indice "http://r/" r1-latin1.txt > r1.out
    #rm -f verdad.indice; ../indexador -l verdad.indice t.indice "http://pasosdeJesus/" verdad-latin1.txt

    echo "Búsqueda sobre índice r1"
    conocereis_l1=$(cat conocereis-latin1.txt)
    hara_l1=$(cat hara-latin1.txt)
    (
        ../buscador -l "$indice" "$conocereis_l1" ;
        ../buscador "$indice" CONOCEREIS ;
        ../buscador "$indice" CONOCEREÍS ;
        ../buscador "$indice" LA ;
        ../buscador "$indice" VERDAD;
        ../buscador "$indice" Y  ;
        ../buscador "$indice" VERDAD;
        ../buscador "$indice" OS;
        ../buscador -l "$indice" "$hara_l1";
        ../buscador "$indice" HARA;
        ../buscador "$indice" HARÁ;
        ../buscador "$indice" LIBRES;
        ../buscador "$indice" INEXISTENTE ;
        ../operaindice lista "$indice"
    ) | grep -v "\"fecha\":" >> r1.out 2>&1

    comparar r1.out esp/r1.out "r1 falló"

    echo "Búsqueda de metainformación sobre r1"

    buscar_palabras r1.indice "sitio:r sitio:s tipo:texto tipo:xrlt titulo:r1 titulo:s titulo:txt" | grep -v "fecha\":" > r1m.out 2>&1

    comparar r1m.out esp/r1m.out "r1m falló"

    echo "Creación de índices r1-u8 a partir de texto UTF-8";
    limpiar r1-u8
    ../indexador r1-u8.indice t.indice "http://r/" r1-utf8.txt

    echo "Búsqueda sobre índice r1-u8"
    (
        ../buscador -l r1-u8.indice "$conocereis_l1" ;
        ../buscador r1-u8.indice CONOCEREIS ;
        ../buscador r1-u8.indice CONOCEREÍS ;
        ../buscador r1-u8.indice LA;
        ../buscador r1-u8.indice VERDAD;
        ../buscador r1-u8.indice Y  ;
        ../buscador r1-u8.indice VERDAD;
        ../buscador r1-u8.indice OS;
        ../buscador -l r1-u8.indice "$hara_l1";
        ../buscador r1-u8.indice HARA;
        ../buscador r1-u8.indice HARÁ;
        ../buscador r1-u8.indice LIBRES;
        ../buscador r1-u8.indice INEXISTENTE ;
        ../operaindice lista r1-u8.indice
    ) | grep -v "\"fecha\":" > r1-u8.out 2>&1

    comparar r1-u8.out esp/r1-u8.out "r1-u8 falló"

    echo "Búsqueda de metainformación sobre r1-u8"

    buscar_palabras r1-u8.indice "sitio:r sitio:s tipo:texto tipo:xrlt titulo:r1 titulo:s titulo:txt" | grep -v "fecha\":" > r1m-u8.out 2>&1

    comparar r1m-u8.out esp/r1m-u8.out "r1m-u8 falló"
}

prueba4 () {
    requiere nombrearc-latin1.txt "nombre con espacio.txt"  "nombre con eñe.txt"

    echo "Caso vacío";
    nombrearc_latin1=$(cat nombrearc-latin1.txt)
    limpiar nse
    (
        ../indexador -l nse.indice t.indice "http://r/" "nombre con espacio.txt" "$nombrearc_latin1";
        ../indexador nse.indice t.indice "http://r/" "nombre con eñe.txt";
        ../buscador nse.indice HOLA ;
        ../buscador nse.indice "" ;
        ../operaindice lista nse.indice
    ) | grep -v "fecha:\"" > nse.out

    comparar nse.out esp/nse.out "nse falló"

    echo "Otro índice";
    limpiar r2
    ../indexador -l r2.indice t.indice "http://r/" r2-latin1.txt > r2.out

    echo "Búsqueda sobre índice"

    unigenito_l1=$(cat unigenito-latin1.txt)
    (
        ../buscador -l r2.indice "$conocereis_l1";
        ../buscador r2.indice CREA;
        ../buscador -l r2.indice "$unigenito_l1";
        ../buscador r2.indice VIDA;
        ../buscador r2.indice ETERNA;
        ../buscador r2.indice QUE;
        ../buscador r2.indice HIJO;
        ../operaindice lista r2.indice
    ) | grep -v "fecha\":" >> r2.out 2>&1

    comparar r2.out esp/r2.out "r2 fallo"
}

prueba5 () {
    requiere r4-latin1.txt

    echo "Creación de otro índice";
    limpiar r4
    ../indexador -l r4.indice t.indice "http://r/" r4-latin1.txt

    comparar r1.out esp/r1.out "r1 falló"
}

pruebaMezclaRam() {
    requiere simple.txt r0.indice r1.indice

    echo "prueba simple"
    ../indexador simple.indice t.indice "http://s/"  simple.txt > simple.out

    ../operaindice mezclaram simpler0.indice r0.indice simple.indice

    echo "Mezcla"

    limpiar rm01
    ../operaindice mezclaram rm01.indice r0.indice r1.indice

    comparar rm01.indice r1.indice "rm01 fallo"

    limpiar rm10
    ../operaindice mezclaram rm10.indice r1.indice r0.indice

    comparar rm10.indice r1.indice "rm10 fallo"

    limpiar rm
    ../operaindice mezclaram rm.indice r1.indice r2.indice
    buscar_palabras rm.indice "VERDAD HIJO DIOS Y" | grep -v "fecha\":" > rm.out 2>&1

    comparar rm.out esp/rm.out "rm fallo"

    echo "Mezcla indices"
    ../operaindice mezclaram rm2.indice rm.indice r1.indice r2.indice

    comparar rm.indice rm2.indice "rm2 y rm -f deberían ser identicos"
}

prueba8 () {
    requiere r1-latin1.txt r2-latin1.txt
    echo "Otra mezcla"

    limpiar ro
    ../indexador -l ro.indice t.indice "http://r/" r1-latin1.txt r2-latin1.txt

    comparar ro.indice rm.indice "ro y rm deberían ser identicos"
}

prueba9 () {
    requiere mateo-utf8.txt marcos-utf8.txt lucas-utf8.txt juan-utf8.txt

    echo "Comparando mezcla en memoria con mezcla en disco"
    limpiar mateo
    limpiar marcos
    limpiar lucas
    limpiar juan

    ../indexador mateo.indice t.indice ./ mateo-utf8.txt
    ../indexador marcos.indice t.indice ./ marcos-utf8.txt
    ../indexador lucas.indice t.indice ./ lucas-utf8.txt
    ../indexador juan.indice t.indice ./ juan-utf8.txt

    # hasta aqui bien

    limpiar mm
    # este genera el primer error: marcos.indice:8979589794: Se esperaba digito 128b
    ../operaindice mezclaram mm.indice marcos.indice mateo.indice
    # exit

    # este genera el error: No se indexó completa palabra larga...

    ../operaindice mezcladisco md.indice marcos.indice mateo.indice

    comparar mm.indice md.indice "mm y md deberían ser identicos"

    # exit

    ../operaindice mezclaram mm2.indice mm.indice lucas.indice
    ../operaindice mezcladisco md2.indice md.indice lucas.indice

    comparar mm2.indice md2.indice "mm2 y md2 deberían ser identicos"


    exit

    ../operaindice mezclaram mm3.indice mm2.indice juan.indice
    ../operaindice mezcladisco md3.indice md2.indice juan.indice

    comparar mm3.indice md3.indice "mm3 y md3 deberían ser identicos"

    ../operaindice mezclaram mmt.indice marcos.indice mateo.indice lucas.indice juan.indice

    comparar mmt.indice md3.indice "mmt y md3 deberían ser identicos"

    buscar_palabras md3.indice "VERDAD HIJO DIOS Y" | grep -v "fecha\":" > rm3.out 2>&1

    comparar rm3.out esp/rm3.out "rm3 fallo"
}

requiere ../indexador ../buscador ../operaindice

rm *.indice *.indice.tendencia

# TODO: poner mejores nombres
prueba_indice_vacio
prueba2
prueba4
prueba5
pruebaMezclaRam
prueba8
prueba9

# TODO: pasar el resto a funciones

echo "Búsqueda de varias palabras"
../buscador r1.indice "LA VERDAD" 1 0 | grep -v "fecha\":" > r5.out 2>&1;
../buscador r1.indice "VERDAD VERDAD" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador r1.indice "OS VERDAD" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador r1.indice "LIBRES OS VERDAD" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador -l r1.indice "$conocereis_l1 LA VERDAD Y LA VERDAD OS $hara_l1 LIBRES" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador r1.indice "OS VERDAD LIBRES" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador r1.indice "OS Y Y Y Y Y VERDAD LIBRES" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador r1.indice "FALSO VERDAD" 1 0  | grep -v "fecha\":" >> r5.out 2>&1

comparar r5.out esp/r5.out "r5 fallo";

echo "En XML"
rm -f rx.indice*
../indexador rx.indice t.indice http://h/ p2.xrlat
(../buscador rx.indice "organizacionresponsable:pasos" ;
../buscador rx.indice "organizacion:religiosa" ;
../buscador rx.indice "organizacion:noesta" ;
../buscador rx.indice "nombre:vi" ;
../buscador rx.indice "departamento:CAUCA" ;
../buscador rx.indice "iglesia:Cruzada" ;
../buscador rx.indice "sexo:Femenino" ;
../buscador rx.indice "noesta:noesta" ;
../buscador rx.indice "sexo:masculino" 1 0) | grep -v "fecha\":" > rx.out 2>&1

comparar rx.out esp/rx.out "rx fallo"

#} ; # function t

echo "En XML BDC2009"
rm -f bdc-ene2009.indice*
../indexador bdc-ene2009.indice t.indice http://127.0.0.1:17443/ bdc-ene2009.xrlat > bdc.out
../operaindice lista bdc-ene2009.indice >> bdc.out

comparar bdc.out esp/bdc.out "bdc fallo"

echo "En ODT"
rm -f poema_ser_como_ninos.indice*
../indexador poema_ser_como_ninos.indice t.indice http://ejemplo.com/ poema_ser_como_ninos.odt
../operaindice lista poema_ser_como_ninos.indice > poema_ser_como_ninos.lista

comparar poema_ser_como_ninos.lista esp/poema_ser_como_ninos.lista "poema_ser_como_ninos.lista fallo";

echo "En HTML"
rm -f html.indice*
../indexador html.indice t.indice http://ejemplo.com/ html.html
../operaindice lista html.indice > html.lista

comparar html.lista esp/html.lista "html.lista fallo"

echo "Elimina un documento"

rm -f ro.indice*
../indexador -l ro.indice t.indice "http://r/" r1-latin1.txt r2-latin1.txt 
../buscador -l ro.indice "$conocereis_l1" 1 0 | grep -v "fecha\":" > ro2.out 2>&1;

# nuevo indice, antiguo indice, posicion de documento a eliminar
../operaindice eliminadoc ro2.indice ro.indice 1

../buscador -l ro2.indice "$conocereis_l1" 1 0 | grep -v "fecha\":" >> ro2.out 
../operaindice lista ro2.indice >> ro2.out

comparar ro2.out esp/ro2.out "ro2 fallo"


echo "Mezcla indices no necesariamente al final"
rm -f ra1.indice*
../indexador -l ra1.indice t.indice "http://r/" r1-latin1.txt
rm -f ra2.indice*
../indexador -l ra2.indice t.indice "http://r/" r2-latin1.txt
../operaindice mezcladisco ra3.indice ra1.indice ra2.indice 1
../operaindice mezcladisco ra4.indice ra1.indice ra2.indice 2
../operaindice grafo ra3.indice > ra.out #| awk '/.*/ { n = n + 1; if (n>2) { print $0; }}' > /tmp/z1
../operaindice grafo ra4.indice >> ra.out

rm -f ra5.indice*
../indexador -l ra5.indice t.indice "http://pasosdeJesus/" verdad-latin1.txt 
rm -f ra6.indice*
../operaindice mezcladisco ra6.indice ra4.indice ra5.indice 1
../operaindice grafo ra6.indice >> ra.out
../operaindice mezcladisco ra7.indice ra4.indice ra5.indice 2
../operaindice grafo ra7.indice >> ra.out
../operaindice mezcladisco ra8.indice ra4.indice ra5.indice 3
../operaindice grafo ra8.indice >> ra.out


comparar ra.out esp/ra.out "ra fallo"

echo "Agrega documentos a un índice"
cp r1.indice rd1.indice; cp r1.relacion rd1.relacion
../indexador -l rd1.indice t.indice "http://r/" r2-latin1.txt
../operaindice grafo rd1.indice > rd.out #| awk '/.*/ { n = n + 1; if (n>2) { print $0; }}' > /tmp/z1

../indexador -l rd1.indice t.indice "http://r/" verdad-latin1.txt poema_ser_como_ninos.odt
../operaindice grafo rd1.indice >> rd.out #| awk '/.*/ { n = n + 1; if (n>2) { print $0; }}' > /tmp/z1


comparar rd.out esp/rd.out "rd fallo"


echo "Comparando mezcla en disco con agregar documento en disco"

rm -f marcos.indice*
../indexador marcos.indice t.indice ./ marcos-utf8.txt
time (rm -f juan.indice; ../indexador juan.indice t.indice ./ juan-utf8.txt; ../operaindice mezcladisco mmj.indice marcos.indice juan.indice)
time (cp marcos.indice mmja.indice; cp marcos.relacion mmja.relacion; ../indexador mmja.indice t.indice "./" juan-utf8.txt)

comparar mmj.indice mmja.indice "mmj y mmja deberían ser identicos"

time (rm -f mateo.indice; ../indexador mateo.indice t.indice ./ mateo-utf8.txt; ../operaindice mezcladisco mmj.indice juan.indice mateo.indice; rm -f marcos.indice; ../indexador marcos.indice t.indice ./ marcos-utf8.txt; ../operaindice mezcladisco mmj2.indice mmj.indice marcos.indice; rm -f lucas.indice; ../indexador lucas.indice t.indice ./ lucas-utf8.txt; ../operaindice mezcladisco mmj.indice mmj2.indice lucas.indice;)
time (cp juan.indice mmja.indice; cp juan.relacion mmja.relacion; ../indexador mmja.indice t.indice "./" mateo-utf8.txt marcos-utf8.txt lucas-utf8.txt)

comparar mmj.indice mmja.indice "mmj y mmja deberían ser identicos"


echo "Comparando indexado en grupos"
rm -f -f t.indice*
rm -f rg1.indice*; MT77MAXG=120 ../indexador -l rg1.indice t.indice "http://r/" r1-latin1.txt r2-latin1.txt  #1
if (test ! -f t.indice) then {
	echo "No se uso temporal t.indice";
} fi;
rm -f rgt1.indice; ../indexador -l rgt1.indice t.indice "http://r/" r1-latin1.txt r2-latin1.txt
comparar rg1.indice rgt1.indice "rg1 y rgt1 deberían ser identicos"

echo "Búsqueda de cadenas"
(../buscador -l r1.indice "$conocereis_l1"  ;
../buscador r1.indice "CONOCEREIS LA VERDAD" ;
../buscador -l r1.indice "\"Y LA VERDAD OS $hara_l1 LIBRES\""
../buscador md3.indice "HIJO DE DIOS"
../buscador md3.indice '"HIJO DE DIOS"' 
) | grep -v "fecha\":" > rc1.out 2>&1

comparar rc1.out esp/rc1.out "rc1 falló"

echo "En PDF"
rm -f pdf.indice; ../indexador pdf.indice t.indice http://ejemplo.com/ OpenHymnalChristmas2009.pdf
../operaindice lista pdf.indice > pdf.lista
comparar pdf.lista esp/pdf.lista "pdf.lista fallo"

#}

echo "Extrae subíndice"
../operaindice subindice rsub.indice r1.indice 1
../operaindice lista rsub.indice > rsub.lista
comparar rsub.lista esp/rsub.lista "rsub.lista fallo"

../operaindice subindice rsubm.indice mmj.indice 1 # Saca Mateo
comparar rsubm.indice juan.indice "rsubm.indice y mateo.indice debería ser iguales"
