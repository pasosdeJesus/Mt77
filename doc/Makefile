# Reglas para generar HTML, PostScript y PDF de tecnica a partir
# de fuentes LaTeX
# Basadas en infraestructura de dominio público de repasa 
#   (http://structio.sourceforge.net/repasa)

include Make.inc

SOURCES=$(PROYECTO).tex verdad.hexdump verdadindice.hexdump velocidadindexado.tex verdad.relacion
# Listado de fuentes LaTeX. Preferiblmente en el orden de inclusión.
# Si desea agregar un solucionario generado automáticamente quite el 
# comentario que precede a soluciones.tex 

IMAGES=img/verdad.png img/arbol.png img/arbol2.png
# Listado de imagenes, preferiblemente en formato PNG

HTML_TARGET=$(PROYECTO).html
# Nombre del HTML principal 



# Variables requeridas por comdist.mk

GENDIST=$(SOURCES) $(IMAGES)
# Dependencias por cumplir antes de generar distribución

ACTHOST=
# Sitio en Internet donde actualizar. Método indicado por ACT_PROC de confv.sh

ACTDIR=
# Directorio en ACTHOST por actualizar

#USER=
# Usuario en ACTHOST.  Si es el mismo que en la máquina local comentar.


GENACT=dist all $(PROYECTO)-$(PRY_VERSION).ps.gz $(PROYECTO)-$(PRY_VERSION).pdf $(PROYECTO).html
# Dependencias por cumplir antes de actualizar sitio en Internet al publicar

FILESACT=$(PROYECTO)-$(PRY_VERSION).tar.gz $(PROYECTO)-$(PRY_VERSION).ps.gz $(PROYECTO)-$(PRY_VERSION).pdf $(PROYECTO).html
# Archivos que se debe actualizar en sitio de Internet cuando se publica

all:  $(PROYECTO).ps $(PROYECTO).pdf $(HTML_TARGET)

soluciones.tex: $(GUIAS) herram/sacares.awk
	awk -f herram/sacares.awk $(GUIAS) > soluciones.tex

repasa:
	DEF=$(PROYECTO).def CLA=$(PROYECTO).cla SEC=$(PROYECTO).sec DESC="Información extraida de $(PROYECTO) $(PRY_VERSION)" FECHA="$(FECHA_ACT)" BIBLIO="$(URLSITE)" TIPO_DERECHOS="Dominio público" TIEMPO_DERECHOS="$(MES_ACT)" DERECHOS="Se cede al dominio público y se dedica a Dios" AUTORES="Vladimir Támara Patiño" IDSIGNIFICADO="sig" $(AWK) -f herram/latex2rep $(SOURCES)

# Para generar documentación con LaTeX y Hevea
include herram/comlatex.mak

# Para crear distribución de fuentes y publicar en Internet
include herram/comdist.mak

# Elimina hasta configuración
limpiadist: limpiamas
	rm -f confv.sh confv.tex Make.inc

# Elimina archivos generables
limpiamas: limpia
	rm -rf $(HTML_DIR)
	rm -f img/*.eps img/*.ps
	rm -f $(PROYECTO)-$(PRY_VERSION).tar.gz


# Elimina backups y archivos temporales
limpia:
	rm -f *.core *.bak *~ *.tmp confaux.tmp confaux.sed $(PROYECTO)-$(PRY_VERSION)_html.tar.gz
	rm -f $(PROYECTO)-4.1.*
	rm -f $(PROYECTO).{dvi,ps,pdf,aux,log,idx,hind,haux,htoc,blg,bbl,ilg,ind,html,image.tex.new}
	rm -f $(PROYECTO).image.tex $(PROYECTO).txt

img/arbol.png: img/arbol.dot
	dot -Tpng img/arbol.dot > img/arbol.png
	
img/arbol2.png: img/arbol2.dot
	dot -Tpng img/arbol2.dot > img/arbol2.png

verdad.hexdump: ../verdad.docindice
	hexdump -C ../verdad.docindice > verdad.hexdump

verdadindice.hexdump: ../regr/verdad.indice
	hexdump -C ../regr/verdad.indice > verdadindice.hexdump

verdad.relacion: ../regr/verdad.relacion
	cp ../regr/verdad.relacion .

verdadrelacion.hexdump: ../regr/verdad.relacion
	hexdump -C ../regr/verdad.relacion > verdadrelacion.hexdump

# http://www.fnal.gov/docs/products/gnuplot/tutorial/
velocidadindexado.tex: velocidadindexado.gnuplot velindexado3.dat velindexado4.dat velindexado5.dat
	gnuplot < velocidadindexado.gnuplot

