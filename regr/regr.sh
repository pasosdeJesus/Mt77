#!/bin/sh
# Pruebas de regresión a Mt77
# Dominio público. 2009. vtamara@pasosdeJesus.org

#function x {

echo "Índice vacio:"
rm -rf r0.indice; ../indexador r0.indice t.indice "http://r" "" > r0.out 2>&1
../buscador r0.indice LA >> r0.out 2>&1
../operaindice lista r0.indice >> r0.out 2>&1
rm -rf rc0.indice; ../indexador rc0.indice t.indice "http://r" "" >> r0.out 2>&1
echo "ram" >> r0.out 2>&1
rm rm0.indice; ../operaindice mezclaram rm0.indice r0.indice rc0.indice >> r0.out 2>&1
../operaindice lista rm0.indice >> r0.out 2>&1
../operaindice mezcladisco rd0.indice r0.indice rc0.indice >> r0.out 2>&1
../operaindice lista rd0.indice >> r0.out 2>&1 #1

cmp r0.out esp/r0.out
if (test "$?" != "0") then {
	echo "** ERROR: r0 falló";
} fi;

echo "Creación de índice r1 a partir de texto LATIN1";
rm -f r1.indice; ../indexador -l r1.indice t.indice "http://r/" r1-latin1.txt > r1.out #1
#rm -f verdad.indice; ../indexador -l verdad.indice t.indice "http://pasosdeJesus/" verdad-latin1.txt 

#function t {

echo "Búsqueda sobre índice r1"
conocereis_l1=`cat conocereis-latin1.txt`
hara_l1=`cat hara-latin1.txt`
(
../buscador -l r1.indice $conocereis_l1 ; #1
../buscador r1.indice CONOCEREIS ; 
../buscador r1.indice CONOCEREÍS ;
../buscador r1.indice LA ;
../buscador r1.indice VERDAD;
../buscador r1.indice Y  ;
../buscador r1.indice VERDAD;
../buscador r1.indice OS;
../buscador -l r1.indice $hara_l1;
../buscador r1.indice HARA;
../buscador r1.indice HARÁ;
../buscador r1.indice LIBRES;
../buscador r1.indice INEXISTENTE ;
../operaindice lista r1.indice ) | grep -v "\"fecha\":" >> r1.out 2>&1

cmp r1.out esp/r1.out
if (test "$?" != "0") then {
	echo "** ERROR: r1 falló";
} fi;

echo "Búsqueda de metainformación sobre r1"
(../buscador r1.indice sitio:r ; #1
../buscador r1.indice sitio:s;
../buscador r1.indice tipo:texto ;
../buscador r1.indice tipo:xrlt;
../buscador r1.indice titulo:r1;
../buscador r1.indice titulo:s;
../buscador r1.indice titulo:txt) | grep -v "fecha\":" > r1m.out 2>&1

cmp r1m.out esp/r1m.out
if (test "$?" != "0") then {
	echo "** ERROR: r1m falló";
} fi;

echo "Creación de índices r1-u8 a partir de texto UTF-8";
rm -f r1-u8.indice; ../indexador r1-u8.indice t.indice "http://r/" r1-utf8.txt   #1

echo "Búsqueda sobre índice r1-u8"
(../buscador -l r1-u8.indice $conocereis_l1 ; #1
../buscador r1-u8.indice CONOCEREIS ;
../buscador r1-u8.indice CONOCEREÍS ;
../buscador r1-u8.indice LA;
../buscador r1-u8.indice VERDAD;
../buscador r1-u8.indice Y  ;
../buscador r1-u8.indice VERDAD;
../buscador r1-u8.indice OS;
../buscador -l r1-u8.indice $hara_l1;
../buscador r1-u8.indice HARA;
../buscador r1-u8.indice HARÁ;
../buscador r1-u8.indice LIBRES;
../buscador r1-u8.indice INEXISTENTE ;
../operaindice lista r1-u8.indice ) | grep -v "\"fecha\":" > r1-u8.out 2>&1

cmp r1-u8.out esp/r1-u8.out
if (test "$?" != "0") then {
	echo "** ERROR: r1-u8 falló";
} fi;

echo "Búsqueda de metainformación sobre r1-u8" #1
(../buscador r1-u8.indice sitio:r ;
../buscador r1-u8.indice sitio:s ;
../buscador r1-u8.indice tipo:texto ;
../buscador r1-u8.indice tipo:xrlt ;
../buscador r1-u8.indice titulo:r1 ;
../buscador r1-u8.indice titulo:s ;
../buscador r1-u8.indice titulo:txt 1 0 ) | grep -v "fecha\":" > r1m-u8.out 2>&1

cmp r1m-u8.out esp/r1m-u8.out
if (test "$?" != "0") then {
	echo "** ERROR: r1m-u8 falló";
} fi;



echo "Caso vacío";
nombrearc_latin1="`cat nombrearc-latin1.txt`"
(rm -f nse.indice; ../indexador -l nse.indice t.indice "http://r/" "nombre con espacio.txt" "$nombrearc_latin1" #1
../indexador nse.indice t.indice "http://r/" "nombre con eñe.txt"; #1
../buscador nse.indice HOLA ; #1
../buscador nse.indice "" ; #1
../operaindice lista nse.indice ) | grep -v "fecha:\"" > nse.out #1

cmp nse.out esp/nse.out
if (test "$?" != "0") then {
	echo "** ERROR: nse falló";
} fi;


echo "Creación de otro índice";
rm -f r4.indice; ../indexador -l r4.indice t.indice "http://r/" r4-latin1.txt 


cmp r1.out esp/r1.out
if (test "$?" != "0") then {
	echo "** ERROR: r1 falló";
} fi;


echo "Otro índice";
rm -f r2.indice; ../indexador -l r2.indice t.indice "http://r/" r2-latin1.txt > r2.out

echo "Búsqueda sobre índice"

unigenito_l1=`cat unigenito-latin1.txt`
(../buscador -l r2.indice $conocereis_l1;
../buscador r2.indice CREA;
../buscador -l r2.indice $unigenito_l1;
../buscador r2.indice VIDA;
../buscador r2.indice ETERNA;
../buscador r2.indice QUE;
../buscador r2.indice HIJO;
../operaindice lista r2.indice) | grep -v "fecha\":" >> r2.out 2>&1
cmp r2.out esp/r2.out
if (test "$?" != "0") then {
	echo "** ERROR: r2 fallo";
} fi;


echo "Mezcla"

rm -f rm01.indice; ../operaindice mezclaram rm01.indice r0.indice r1.indice 
cmp rm01.indice r1.indice
if (test "$?" != "0") then {
  echo "** ERROR: rm01 fallo";
} fi;
rm -f rm10.indice; ../operaindice mezclaram rm10.indice r1.indice r0.indice
cmp rm10.indice r1.indice
if (test "$?" != "0") then {
  echo "** ERROR: rm10 fallo";
} fi;

../operaindice mezclaram rm.indice r1.indice r2.indice
(../buscador rm.indice VERDAD;
../buscador rm.indice HIJO;
../buscador rm.indice DIOS;
../buscador rm.indice Y 1 0 ) | grep -v "fecha\":" > rm.out 2>&1
cmp rm.out esp/rm.out
if (test "$?" != "0") then {
	echo "** ERROR: rm -f fallo";
} fi;


../operaindice mezclaram rm2.indice rm.indice r1.indice r2.indice
cmp rm.indice rm2.indice
if (test "$?" != "0") then {
	echo "** ERROR: rm2 y rm -f deberían ser identicos";
} fi;

echo "Otra mezcla"

rm -f ro.indice; ../indexador -l ro.indice t.indice "http://r/" r1-latin1.txt r2-latin1.txt  #1

cmp ro.indice rm.indice
if (test "$?" != "0") then {
	echo "** ERROR: ro y rm deberían ser identicos";
} fi;


echo "Comparando mezcla en memoria con mezcla en disco"
rm -f mateo.indice; ../indexador mateo.indice t.indice ./ mateo-utf8.txt #1
rm -f marcos.indice; ../indexador marcos.indice t.indice ./ marcos-utf8.txt #1
rm -f lucas.indice; ../indexador lucas.indice t.indice ./ lucas-utf8.txt #1
rm -f juan.indice; ../indexador juan.indice t.indice ./ juan-utf8.txt #1

rm mm.indice
../operaindice mezclaram mm.indice marcos.indice mateo.indice
../operaindice mezcladisco md.indice marcos.indice mateo.indice
cmp mm.indice md.indice
if (test "$?" != "0") then {
	echo "** ERROR: mm y md deberían ser identicos";
} fi;

../operaindice mezclaram mm2.indice mm.indice lucas.indice
../operaindice mezcladisco md2.indice md.indice lucas.indice
cmp mm2.indice md2.indice
if (test "$?" != "0") then {
	echo "** ERROR: mm2 y md2 deberían ser identicos";
} fi;

../operaindice mezclaram mm3.indice mm2.indice juan.indice
../operaindice mezcladisco md3.indice md2.indice juan.indice
cmp mm3.indice md3.indice
if (test "$?" != "0") then {
	echo "** ERROR: mm3 y md3 deberían ser identicos";
} fi;

../operaindice mezclaram mmt.indice marcos.indice mateo.indice lucas.indice juan.indice
cmp mmt.indice md3.indice
if (test "$?" != "0") then {
	echo "** ERROR: mmt y md3 deberían ser identicos";
} fi;

(../buscador md3.indice VERDAD;
../buscador md3.indice HIJO;
../buscador md3.indice DIOS;
../buscador md3.indice Y 1 0 ) | grep -v "fecha\":" > rm3.out 2>&1
cmp rm3.out esp/rm3.out
if (test "$?" != "0") then {
	echo "** ERROR: rm3 fallo";
} fi;

echo "Búsqueda de varias palabras"
../buscador r1.indice "LA VERDAD" 1 0 | grep -v "fecha\":" > r5.out 2>&1;
../buscador r1.indice "VERDAD VERDAD" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador r1.indice "OS VERDAD" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador r1.indice "LIBRES OS VERDAD" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador -l r1.indice "$conocereis_l1 LA VERDAD Y LA VERDAD OS $hara_l1 LIBRES" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador r1.indice "OS VERDAD LIBRES" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador r1.indice "OS Y Y Y Y Y VERDAD LIBRES" 1 0 | grep -v "fecha\":" >> r5.out 2>&1;
../buscador r1.indice "FALSO VERDAD" 1 0  | grep -v "fecha\":" >> r5.out 2>&1
cmp r5.out esp/r5.out
if (test "$?" != "0") then {
	echo "** ERROR: r5 fallo";
} fi;

echo "En XML"
rm -f rx.indice; ../indexador rx.indice t.indice http://h/ p2.xrlat
(../buscador rx.indice "organizacionresponsable:pasos" ;
../buscador rx.indice "organizacion:religiosa" ;
../buscador rx.indice "organizacion:noesta" ;
../buscador rx.indice "nombre:vi" ;
../buscador rx.indice "departamento:CAUCA" ;
../buscador rx.indice "iglesia:Cruzada" ;
../buscador rx.indice "sexo:Femenino" ;
../buscador rx.indice "noesta:noesta" ;
../buscador rx.indice "sexo:masculino" 1 0) | grep -v "fecha\":" > rx.out 2>&1
cmp rx.out esp/rx.out
if (test "$?" != "0") then {
	echo "** ERROR: rx fallo";
} fi;

#} ; # function t

echo "En XML BDC2009"
rm -f bdc-ene2009.indice; ../indexador bdc-ene2009.indice t.indice http://127.0.0.1:17443/ bdc-ene2009.xrlat > bdc.out
../operaindice lista bdc-ene2009.indice >> bdc.out
cmp bdc.out esp/bdc.out
if (test "$?" != "0") then {
	echo "** ERROR: bdc fallo";
} fi;

echo "En ODT"
rm -f poema_ser_como_ninos.indice; ../indexador poema_ser_como_ninos.indice t.indice http://ejemplo.com/ poema_ser_como_ninos.odt
../operaindice lista poema_ser_como_ninos.indice > poema_ser_como_ninos.lista
cmp poema_ser_como_ninos.lista esp/poema_ser_como_ninos.lista
if (test "$?" != "0") then {
	echo "** ERROR: poema_ser_como_ninos.lista fallo";
} fi;

echo "En HTML"
rm -f html.indice; ../indexador html.indice t.indice http://ejemplo.com/ html.html
../operaindice lista html.indice > html.lista
cmp html.lista esp/html.lista
if (test "$?" != "0") then {
	echo "** ERROR: html.lista fallo";
} fi;

echo "Elimina un documento"

rm -f ro.indice; ../indexador -l ro.indice t.indice "http://r/" r1-latin1.txt r2-latin1.txt 
../buscador -l ro.indice "$conocereis_l1" 1 0 | grep -v "fecha\":" > ro2.out 2>&1;
../operaindice eliminadoc ro2.indice ro.indice 1 
../buscador -l ro2.indice "$conocereis_l1" 1 0 | grep -v "fecha\":" >> ro2.out 
../operaindice lista ro2.indice >> ro2.out 
cmp ro2.out esp/ro2.out
if (test "$?" != "0") then {
	echo "** ERROR: ro2 fallo";
} fi;


echo "Mezcla indices no necesariamente al final"
rm -f ra1.indice; ../indexador -l ra1.indice t.indice "http://r/" r1-latin1.txt
rm -f ra2.indice; ../indexador -l ra2.indice t.indice "http://r/" r2-latin1.txt
../operaindice mezcladisco ra3.indice ra1.indice ra2.indice 1
../operaindice mezcladisco ra4.indice ra1.indice ra2.indice 2
../operaindice grafo ra3.indice > ra.out #| awk '/.*/ { n = n + 1; if (n>2) { print $0; }}' > /tmp/z1
../operaindice grafo ra4.indice >> ra.out

rm -f ra5.indice; ../indexador -l ra5.indice t.indice "http://pasosdeJesus/" verdad-latin1.txt 
rm -f ra6.indice; ../operaindice mezcladisco ra6.indice ra4.indice ra5.indice 1
../operaindice grafo ra6.indice >> ra.out
../operaindice mezcladisco ra7.indice ra4.indice ra5.indice 2
../operaindice grafo ra7.indice >> ra.out
../operaindice mezcladisco ra8.indice ra4.indice ra5.indice 3
../operaindice grafo ra8.indice >> ra.out

cmp ra.out esp/ra.out
if (test "$?" != "0") then {
	echo "** ERROR: ra fallo";
} fi;

echo "Agrega documentos a un índice"
cp r1.indice rd1.indice; cp r1.relacion rd1.relacion
../indexador -l rd1.indice t.indice "http://r/" r2-latin1.txt
../operaindice grafo rd1.indice > rd.out #| awk '/.*/ { n = n + 1; if (n>2) { print $0; }}' > /tmp/z1

../indexador -l rd1.indice t.indice "http://r/" verdad-latin1.txt poema_ser_como_ninos.odt
../operaindice grafo rd1.indice >> rd.out #| awk '/.*/ { n = n + 1; if (n>2) { print $0; }}' > /tmp/z1

cmp rd.out esp/rd.out
if (test "$?" != "0") then {
	echo "** ERROR: rd fallo";
} fi;


echo "Comparando mezcla en disco con agregar documento en disco"

rm -f marcos.indice; ../indexador marcos.indice t.indice ./ marcos-utf8.txt
time (rm -f juan.indice; ../indexador juan.indice t.indice ./ juan-utf8.txt; ../operaindice mezcladisco mmj.indice marcos.indice juan.indice) #1
time (cp marcos.indice mmja.indice; cp marcos.relacion mmja.relacion; ../indexador mmja.indice t.indice "./" juan-utf8.txt) #1
cmp mmj.indice mmja.indice
if (test "$?" != "0") then {
	echo "** ERROR: mmj y mmja deberían ser identicos";
} fi;

time (rm -f mateo.indice; ../indexador mateo.indice t.indice ./ mateo-utf8.txt; ../operaindice mezcladisco mmj.indice juan.indice mateo.indice; rm -f marcos.indice; ../indexador marcos.indice t.indice ./ marcos-utf8.txt; ../operaindice mezcladisco mmj2.indice mmj.indice marcos.indice; rm -f lucas.indice; ../indexador lucas.indice t.indice ./ lucas-utf8.txt; ../operaindice mezcladisco mmj.indice mmj2.indice lucas.indice;) #1
time (cp juan.indice mmja.indice; cp juan.relacion mmja.relacion; ../indexador mmja.indice t.indice "./" mateo-utf8.txt marcos-utf8.txt lucas-utf8.txt) #1
cmp mmj.indice mmja.indice
if (test "$?" != "0") then {
	echo "** ERROR: mmj y mmja deberían ser identicos";
} fi;


echo "Comparando indexado en grupos"
rm -f -f t.indice 
rm -f rg1.indice; MT77MAXG=120 ../indexador -l rg1.indice t.indice "http://r/" r1-latin1.txt r2-latin1.txt  #1
if (test ! -f t.indice) then {
	echo "No se uso temporal t.indice";
} fi;
rm -f rgt1.indice; ../indexador -l rgt1.indice t.indice "http://r/" r1-latin1.txt r2-latin1.txt  #1
cmp rg1.indice rgt1.indice
if (test "$?" != "0") then {
	echo "** ERROR: rg1 y rgt1 deberían ser identicos";
} fi;


echo "Búsqueda de cadenas"
(../buscador -l r1.indice "$conocereis_l1"  ;
../buscador r1.indice "CONOCEREIS LA VERDAD" ;
../buscador -l r1.indice "\"Y LA VERDAD OS $hara_l1 LIBRES\"" 
../buscador md3.indice "HIJO DE DIOS" 
../buscador md3.indice '"HIJO DE DIOS"' 
) | grep -v "fecha\":" > rc1.out 2>&1

cmp rc1.out esp/rc1.out
if (test "$?" != "0") then {
	echo "** ERROR: rc1 falló";
} fi;

echo "En PDF"
rm -f pdf.indice; ../indexador pdf.indice t.indice http://ejemplo.com/ OpenHymnalChristmas2009.pdf
../operaindice lista pdf.indice > pdf.lista
cmp pdf.lista esp/pdf.lista
if (test "$?" != "0") then {
	echo "** ERROR: pdf.lista fallo";
} fi;

#}

echo "Extrae subíndice"
../operaindice subindice rsub.indice r1.indice 1
../operaindice lista rsub.indice > rsub.lista
cmp rsub.lista esp/rsub.lista
if (test "$?" != "0") then {
	echo "** ERROR: rsub.lista fallo";
} fi;

../operaindice subindice rsubm.indice mmj.indice 1 # Saca Mateo
cmp rsubm.indice juan.indice
if (test "$?" != "0") then {
	echo "** ERROR: rsubm.indice y mateo.indice debería ser iguales";
} fi;



