# Reglas
# Dominio público. 2008. vtamara@pasosdeJesus.org

include Make.inc

FUENTES=Doc.cpp DocCasoPrueba.cpp Elias.cpp EliasCasoPrueba.cpp NodoTrieS.cpp NodoTrieSCasoPrueba.cpp Operaciones.cpp Pos.cpp PosCasoPrueba.cpp RamDisco.cpp RamDiscoCasoPrueba.cpp TrieSDisco.cpp TrieSDiscoCasoPrueba.cpp buscador.cpp comun.cpp comunCasoPrueba.cpp depuraindice.cpp funzipuno.cpp indexador.cpp leeHTML.cpp leeODT.cpp leePDF.cpp leeXML.cpp operaindice.cpp pruebaCppUnit.cpp sha256.cpp tomsha256.cpp txtdeodt.cpp unzipuno.cpp

ENCABEZADOS=CasosPrueba.hpp Doc.hpp DocCasoPrueba.hpp Elias.hpp EliasCasoPrueba.hpp NodoTrieS.hpp NodoTrieSCasoPrueba.hpp Operaciones.hpp Pos.hpp PosCasoPrueba.hpp RamDisco.hpp RamDiscoCasoPrueba.hpp TrieSDisco.hpp TrieSDiscoCasoPrueba.hpp comun.hpp comunCasoPrueba.hpp funzipuno.hpp leeHTML.hpp leeODT.hpp leePDF.hpp leeXML.hpp sha256.hpp

CFLAGS ?= -O0
CFLAGS += -Wall -I/usr/X11R6/include -I/usr/local/include #-I/usr/X11R6/include -fprofile-arcs -ftest-coverage -p -pg
#CFLAGS=-I/usr/local/include -I/usr/X11R6/include
LFLAGS += -L/usr/local/lib -L/usr/X11R6/lib # -p -pg -fprofile-arcs -ftest-coverage 

DEPLFLAGS=-g -p -pg 
DEPCFLAGS=-O0 -g -p -pg

produccion: 
	LFLAGS="-g" CFLAGS="-g -O0 -DNDEBUG" make all -DNDEBUG

prueba: 
	LFLAGS="-g" CFLAGS="-g -O0" make all 

BINARIOS=indexador buscador operaindice unzipuno txtdeodt tomsha256 depuraindice

all: $(BINARIOS)

# Requieren cppunit instalado en /usr/local/include --por defecto en
# paquete cppunit en OpenBSD (pkg_add cppunit)
PUNIDAD=pruebaComun pruebaElias pruebaPos pruebaDoc pruebaNodoTrieS pruebaTrieSDisco pruebaRamDisco 
unidad: 
	CFLAGS="$(DEPCFLAGS)" make $(PUNIDAD)
	for i in $(PUNIDAD) ; do echo $$i; /usr/bin/time ./$$i; done

depura: 
	LFLAGS="$(DEPLFLAGS)" CFLAGS="$(DEPCFLAGS)" make all

cobertura: 
	for i in $(PUNIDAD); do echo $$i; nf=`echo $$i | sed -e "s/prueba//g;s/Comun/comun/g"`; echo $$nf; ./cobertura.sh "$$nf.cpp" "$$i"; done


regr: depura
	(cd regr; ./regr.sh)

FTOMSHA256 = sha256.o tomsha256.o
tomsha256: $(FTOMSHA256)
	c++ $(LFLAGS) -o $(.TARGET) -lc $(FTOMSHA256)

FTXTDEODT = comun.o Elias.o Pos.o Doc.o sha256.o NodoTrieS.o funzipuno.o leeODT.o txtdeodt.o 
txtdeodt : $(FTXTDEODT)
	c++ $(LFLAGS) -o $(.TARGET) -lc -lxml2 -lxslt -lz $(FTXTDEODT)

FUNZIPUNO = funzipuno.o unzipuno.o
unzipuno: $(FUNZIPUNO)
	c++ $(LFLAGS) -o $(.TARGET) -lc -lz $(FUNZIPUNO)

FPRUEBACOMUN = comun.o comunCasoPrueba.o pruebaCppUnit.o
pruebaComun:  $(FPRUEBACOMUN)
	c++ $(LFLAGS) -L/usr/local/lib -lcppunit -o $(.TARGET) $(FPRUEBACOMUN)

FPRUEBAELIAS = comun.o Elias.o EliasCasoPrueba.o pruebaCppUnit.o
pruebaElias:  $(FPRUEBAELIAS)
	c++ $(LFLAGS) -L/usr/local/lib -lcppunit -o $(.TARGET) $(FPRUEBAELIAS)

FPRUEBAPOS = comun.o Elias.o Pos.o PosCasoPrueba.o pruebaCppUnit.o
pruebaPos:  $(FPRUEBAPOS)
	c++ $(LFLAGS) -L/usr/local/lib -lcppunit -o $(.TARGET) $(FPRUEBAPOS)

FPRUEBADOC = comun.o Doc.o DocCasoPrueba.o pruebaCppUnit.o
pruebaDoc:  $(FPRUEBADOC)
	c++ $(LFLAGS) -L/usr/local/lib -lcppunit -o $(.TARGET) $(FPRUEBADOC)

FPRUEBANODOTRIES=comun.o Elias.o Pos.o Doc.o sha256.o NodoTrieS.o NodoTrieSCasoPrueba.o pruebaCppUnit.o
pruebaNodoTrieS: $(FPRUEBANODOTRIES)
	c++ $(LFLAGS) -L/usr/local/lib -lcppunit -o pruebaNodoTrieS $(FPRUEBANODOTRIES)

FPRUEBATRIESDISCO=comun.o Elias.o Pos.o Doc.o sha256.o NodoTrieS.o TrieSDisco.o RamDisco.o TrieSDiscoCasoPrueba.o pruebaCppUnit.o
pruebaTrieSDisco: $(FPRUEBATRIESDISCO)
	c++ $(LFLAGS) -L/usr/local/lib -lcppunit -o $(.TARGET) $(FPRUEBATRIESDISCO)

FPRUEBARAMDISCO=comun.o Elias.o Pos.o Doc.o sha256.o NodoTrieS.o TrieSDisco.o RamDisco.o RamDiscoCasoPrueba.o pruebaCppUnit.o
pruebaRamDisco: $(FPRUEBARAMDISCO)
	c++ $(LFLAGS) -L/usr/local/lib -lcppunit -o $(.TARGET) $(FPRUEBARAMDISCO)

funzipuno.o: funzipuno.hpp funzipuno.cpp

unzipuno.o: funzipuno.hpp funzipuno.o unzipuno.cpp

tomsha256.o: sha256.hpp sha256.o tomsha256.cpp

txtdeodt.o: funzipuno.hpp comun.hpp leeODT.hpp funzipuno.o leeODT.o txtdeodt.cpp

pruebaCppUnit.o: pruebaCppUnit.cpp

pruebaNodoTrieS.o: comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp NodoTrieSCasoPrueba.hpp NodoTrieSCasoPrueba.cpp

TrieSDiscoCasoPrueba.o: sha256.hpp NodoTrieS.hpp TrieSDisco.hpp TrieSDiscoCasoPrueba.hpp TrieSDiscoCasoPrueba.cpp

RamDiscoCasoPrueba.o: sha256.hpp NodoTrieS.hpp TrieSDisco.hpp RamDisco.hpp RamDiscoCasoPrueba.hpp RamDiscoCasoPrueba.cpp

TrieSDisco.o: comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp TrieSDisco.cpp

Pos.o: comun.hpp Pos.hpp Pos.cpp

Doc.o: comun.hpp Doc.hpp Doc.cpp

comun.o: comun.hpp comun.cpp

RamDisco.o: comun.hpp Doc.hpp Pos.hpp Elias.hpp RamDisco.hpp

FBASE=comun.o Elias.o Pos.o Doc.o sha256.o NodoTrieS.o TrieSDisco.o RamDisco.o 
FINDEXADOR=$(FBASE) leeXML.o funzipuno.o leeODT.o leeHTML.o leePDF.o Operaciones.o indexador.o 
indexador: $(FINDEXADOR)
	c++ $(LFLAGS) -lxml2 -lxslt -lz -lexpat -lc -o indexador $(FINDEXADOR)

leeODT.o: comun.hpp funzipuno.hpp sha256.hpp NodoTrieS.hpp funzipuno.o leeODT.hpp leeODT.cpp
	c++ -I /usr/local/include -I /usr/local/include/libxml2/ -c leeODT.cpp

sha256.o: sha256.hpp sha256.cpp

NodoTrieS.o: comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.cpp NodoTrieS.hpp

Operaciones.o: comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp TrieSDisco.hpp RamDisco.hpp comun.hpp leeODT.hpp leePDF.hpp sha256.hpp leeHTML.hpp  Operaciones.hpp Operaciones.cpp

leeXML.o: comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp leeXML.cpp

leeHTML.o: comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp leeHTML.hpp leeHTML.cpp

leePDF.o: comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp leePDF.hpp leePDF.cpp

indexador.o: indexador.cpp comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp sha256.hpp Operaciones.hpp

FBUSCADOR=$(FBASE) 
buscador:  $(FBUSCADOR)  buscador.o
	c++ $(LFLAGS) -static -o buscador $(FBUSCADOR) buscador.o

buscador.o: buscador.cpp $(FBUSCADOR:S/.o/.hpp/g)

FDEPURAINDICE=$(FBASE) 
depuraindice:  $(FDEPURAINDICE)  depuraindice.o
	c++ $(LFLAGS) -static -o depuraindice $(FDEPURAINDICE) depuraindice.o


FOPERAINDICE=$(FBASE) Operaciones.o operaindice.o 
operaindice: $(FOPERAINDICE)
	c++ $(LFLAGS)  -static -o operaindice /usr/local/lib/libxml2.a $(FOPERAINDICE)

.SUFFIXES: .o .cpp

.cpp.o:
	c++ $(CFLAGS) -c -o $@ $<


limpia:
	rm -f pruebaComun pruebaPos pruebaTrieSDisco pruebaNodoTrieS pruebaRamDisco buscador indexador operaindice txtdeodt unzipuno tomsha256 *.o *~ *.core gmon.out prueba.indice regr/*~ herram/*~ regr/{*~,*.core,*.out,*.relacion,*.indice,*.lista}

desempeno: 
	make limpia
	CFLAGS="-I/usr/local/include -I/usr/X11R6/include -fprofile-arcs -ftest-coverage -p -pg" LFLAGS="-fprofile-arcs -ftest-coverage -p -pg" make indexador operaindice
	echo `date` >> desempeno.txt
	echo -n "indexador " >> desempeno.txt
	/usr/bin/time ./indexador marcos.indice "./" ../../../corpus/marcos.txt  >> desempeno.txt 2>&1
	gprof indexador > indexador.prof
	echo -n "agregadoc " >> desempeno.txt
	/usr/bin/time ./operaindice agregadoc ml.indice marcos.indice "./" ../../../corpus/lucas.txt >> desempeno.txt 2>&1
	gprof operaindice > operaindice.prof


indexador.gcov: 
	rm -f *.gcov 
	/usr/bin/time ./indexador marcos.indice "./" ../../../corpus/marcos.txt   

operaindice.gcov:
	/usr/bin/time ./operaindice agregadoc ml.indice marcos.indice "./" ../../../corpus/lucas.txt 
	for i in $(FOPERAINDICE:S/.o/.cpp/g) ; do gcov -o . $$i; done


CCDOC=../herram/ccdocv08r41/bin_opt_gcc_/ccdoc.exe

indenta:
	for i in *hpp *cpp; do echo $$i; cp $$i $$i.sini; astyle --style=linux -c -C -s8 < $$i.sini | awk ' /set$$/ { if (g != "") { gsub(/^  */, "", $$0); g = g $$0;} else {g = $$0;} enset = 1 } /.*/ { if (enset == 0 && g != "") { gsub(/^  */, "", $$0); print g $$0; g = ""; } else if (enset == 1) { enset = 0; } else { print $$0; }} BEGIN {g=""; enset = 0; }' > $$i; done

doctec: Doxyfile $(ENCABEZADOS) $(FUENTES)
	doxygen Doxyfile

# Create the ccdoc documentation if the header file changed.
htdocs/ccdoc.db : htdocs 
	rm -f htdocs/*
	$(CCDOC) -db htdocs/ccdoc.db \
		*.hpp \
		-index \
		-root SINCODH \
		-rootfile htdocs/index.html \
		-html htdocs/
        
# Create the htdocs directory, if necessary.
htdocs : ; @mkdir $@

GENDIST=
# Dependencias para generar distribución

ACTHOST=sn2.pasosdejesus.org
ACTDIR=/var/www/pasosdeJesus/Mt77
#USER=vtamara

GENACT=doc/tecnica-$(PRY_VERSION).pdf distcvs 
# Reglas por emplear antes de actualizar sitio en Internet

FILESACT=$(PROYECTO)-$(PRY_VERSION).tar.gz portal/index.php portal/estilo.css portal/conf.php doc/tecnica-$(PRY_VERSION).pdf
# Archivos que se debe actualizar en sitio de Internet

# To crear distribución de fuentes y actualizar en Internet
include herram/comdist.mak


# Elimina hasta configuración
limpiadist: limpiamas
	rm -f confv.sh confv.ent Make.inc
	rm confaux.sed confaux.tmp confv.bak depuraindice

# Elimina archivos generables
limpiamas: limpia
	rm -rf $(HTML_DIR)
	rm -rf $(PRINT_DIR)
	rm -f img/*.eps img/*.ps
	rm -f $(PROYECTO)-$(PRY_VERSION).tar.gz

#	$(MKDIR) -p $(DESTDIR)$(INSDATA)/
#	$(CP) herram/*.mak herram/*.sh $(DESTDIR)$(INSDATA)/
#	$(SED) -e 's|rutaconfsh=".*"|rutaconfsh="$(DESTDIR)$(INSDATA)"|g' herram/confaux.sh > $(DESTDIR)$(INSDATA)/confaux.sh
instala:
	for i in $(BINARIOS); do echo $$i; \
		cp $$i $(DESTDIR)$(INSBIN)/; \
	done;

#	$(RM) -rf $(INSDATA)
desinstala:
	for i in $(BINARIOS); do echo $$i; \
		rm $(DESTDIR)$(INSBIN)/$$i; \
	done;



#.PRECIOUS: .pdoc

doctec-act: doctec-$(ACT_PROC)
	if (test "$(OTHER_ACT)" != "") then { make doctec-$(OTHER_ACT); } fi;

doctec-act-scp:
	        $(SCP) -r doctec/* $(USERACT)@$(ACTHOST):$(ACTDIR)/doctec

doctec-act-ncftpput:
	        $(NCFTPPUT) -u $(USERACT) $(ACTHOST) $(ACTDIR)/doctec doctec/

doc/tecnica-$(PRY_VERSION).pdf:
	(cd doc; ./conf.sh; make)
	cp doc/tecnica.pdf doc/tecnica-$(PRY_VERSION).pdf

# DO NOT DELETE

Doc.o: comun.hpp
DocCasoPrueba.o: Doc.hpp comun.hpp
NodoTrieS.o: sha256.hpp Pos.hpp comun.hpp
Operaciones.o: RamDisco.hpp comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp
Operaciones.o: TrieSDisco.hpp
Pos.o: comun.hpp
RamDisco.o: comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp TrieSDisco.hpp
TrieSDisco.o: sha256.hpp NodoTrieS.hpp Pos.hpp comun.hpp Doc.hpp
leeHTML.o: sha256.hpp NodoTrieS.hpp Pos.hpp comun.hpp
leeODT.o: sha256.hpp NodoTrieS.hpp Pos.hpp comun.hpp
leePDF.o: comun.hpp sha256.hpp NodoTrieS.hpp Pos.hpp
leeXML.o: sha256.hpp NodoTrieS.hpp Pos.hpp comun.hpp
Doc.o: comun.hpp Doc.hpp
DocCasoPrueba.o: DocCasoPrueba.hpp Doc.hpp comun.hpp
Elias.o: comun.hpp Elias.hpp
EliasCasoPrueba.o: Elias.hpp EliasCasoPrueba.hpp
NodoTrieS.o: Pos.hpp comun.hpp sha256.hpp NodoTrieS.hpp
NodoTrieSCasoPrueba.o: NodoTrieSCasoPrueba.hpp sha256.hpp NodoTrieS.hpp Pos.hpp
NodoTrieSCasoPrueba.o: comun.hpp
Operaciones.o: RamDisco.hpp comun.hpp Pos.hpp Doc.hpp
Operaciones.o: sha256.hpp NodoTrieS.hpp TrieSDisco.hpp Operaciones.hpp sha256.hpp
Operaciones.o: leeODT.hpp leeXML.hpp leeHTML.hpp
Operaciones.o: leePDF.hpp
Pos.o: comun.hpp Pos.hpp Elias.hpp
PosCasoPrueba.o: PosCasoPrueba.hpp Elias.hpp Pos.hpp comun.hpp
RamDisco.o: RamDisco.hpp comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp
RamDisco.o: TrieSDisco.hpp
RamDiscoCasoPrueba.o: RamDiscoCasoPrueba.hpp RamDisco.hpp comun.hpp Pos.hpp
RamDiscoCasoPrueba.o: Doc.hpp sha256.hpp NodoTrieS.hpp TrieSDisco.hpp CasosPrueba.hpp
TrieSDisco.o: TrieSDisco.hpp sha256.hpp NodoTrieS.hpp Pos.hpp comun.hpp Doc.hpp
TrieSDiscoCasoPrueba.o: TrieSDiscoCasoPrueba.hpp TrieSDisco.hpp sha256.hpp NodoTrieS.hpp
TrieSDiscoCasoPrueba.o: Pos.hpp comun.hpp Doc.hpp RamDisco.hpp
TrieSDiscoCasoPrueba.o: CasosPrueba.hpp
buscador.o: Pos.hpp comun.hpp TrieSDisco.hpp
buscador.o: sha256.hpp NodoTrieS.hpp Doc.hpp
comun.o: comun.hpp
comunCasoPrueba.o: comunCasoPrueba.hpp comun.hpp
depuraindice.o: Pos.hpp comun.hpp TrieSDisco.hpp
depuraindice.o: sha256.hpp NodoTrieS.hpp Doc.hpp
funzipuno.o: funzipuno.hpp 
indexador.o: Operaciones.hpp
indexador.o: RamDisco.hpp comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp
indexador.o: TrieSDisco.hpp
leeHTML.o: sha256.hpp NodoTrieS.hpp Pos.hpp comun.hpp
leeODT.o: sha256.hpp NodoTrieS.hpp
leeODT.o: Pos.hpp comun.hpp leeODT.hpp
leePDF.o: sha256.hpp NodoTrieS.hpp
leePDF.o: Pos.hpp comun.hpp leePDF.hpp
leeXML.o: sha256.hpp NodoTrieS.hpp Pos.hpp comun.hpp
operaindice.o: RamDisco.hpp comun.hpp Pos.hpp Doc.hpp sha256.hpp NodoTrieS.hpp
operaindice.o: TrieSDisco.hpp Operaciones.hpp
sha256.o: sha256.hpp 
tomsha256.o: sha256.hpp 
txtdeodt.o: leeODT.hpp sha256.hpp NodoTrieS.hpp
txtdeodt.o: Pos.hpp comun.hpp
unzipuno.o: funzipuno.hpp 
