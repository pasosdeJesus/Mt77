#!/bin/sh

# Script genérico para preparar herramientas de LaTeX y configurar.
# Esta fuente se cede al dominio público 2003. No se ofrecen garantías.
# Puede enviar reportes de fallas a structio-info@lists.sourceforge.net

# Créditos
# Manejo de variables de configuración: Miembros de Structio.  
#	http://structio.sourceforge.net/
# Línea de comandos: WWWeb Tide Team 
#	http://www.ebbtide.com/Util/ksh_parse.html 
# que también es de dominio público de acuerdo a http://www.ebbtide.com/Util/
# "The following utilities have been written by the members of WWWeb Tide 
# Team. We considered them to be infinitely useful in every day systems and 
# web site administration. So much so, in fact, we have decided to put the 
# sources in the public domain." 


# Leyendo variables de configuración
if (test ! -f confv.sh) then {
        cp confv.empty confv.sh
} fi;

. ./confv.sh


# Leyendo funciones para ayudar en configuración
. herram/confaux.sh
. herram/misc.sh

# Reconociendo línea de comandos

BASENAME=$(basename $0)
USAGE="$BASENAME [-v] [-h] [-M] [-p prefijo]"
# Remember: if you add a switch above, don't forget to add it to: 
#	1) "Parse command line options" section 
#	2) "MANPAGE_HEREDOC" section
#	3) "check for help" section
ARG_COUNT=0 	# This nubmer must reflect true argument count
OPT_FLAG=0 	# Command line mistake flag
OPT_COUNT=0	# Number of options on the command line
MAN_FLAG=0 	# Default: no man pages requited
HELP_FLAG=0	# Default: no help required
VERBOSE_FLAG=0 	# Default: no verbose
WARNING=0 	# General purpose no fail warning flag

# initialize local variables
vbs=""
prefix="/usr/local"

# Parse command line options
while getopts :p:Mhv arguments 
do
   # remember treat r: switches with: R_VALE = $OPTARG ;;
   case $arguments in
      p)    prefix=$OPTARG;;
      M)    MAN_FLAG=1 ;;		# Display man page
      v)    # increment the verboseness count and...
	    VERBOSE_FLAG=$(($VERBOSE_FLAG+1))
	    # gather up all "-v" switches
	    vbs="$vbs -v"
	    ;;
      h)    HELP_FLAG=1;;		# display on-line help
      \?)   echo "Opción no reconocida: $OPTARG" >&2	# flag illegal switch 
	    OPT_FLAG=1;;
   esac
done
OPT_COUNT=$(($OPTIND-1))
shift $OPT_COUNT

options_help="
   -p prefijo	Prefijo de la ruta de instalación (por defecto /usr/local)
   -h           Presenta ayuda corta
   -M           Presenta ayuda más completa
   -v           Presenta información de depuración durante ejecución"
 
# check for man page request
if (test "$MAN_FLAG" = "1" ) then {
	if (test "$PAGER" = "" ) then {
		if ( test "$VERBOSE_FLAG" -gt "0" ) then {
			echo "$BASENAME: Resetting PAGER variable to more" >&2

	       	} fi;
	       	export PAGER=more;
	} fi;
	$PAGER << MANPAGE_HEREDOC

NOMBRE

	$BASENAME - Configura fuentes de $PROYECTO

	$USAGE


DESCRIPCIÓN

	Establece el valor de las variables de configuración y genera
	archivos en diversos formatos empleados por las fuentes DocBook
	con ayudas de 'repasa' del proyecto $PROYECTO.
	* $PRY_DESC
	* $URLSITE

	Las variables de configuración y sus valores por defecto están
	en confv.empty (debajo de cada variable hay un comentario con la 
	descripción).
	Este script modifica el archivo confv.sh (o de no existir lo crea
        a partir de confv.empty) y genera los archivos Make.inc y confv.ent 
	con las variables de configuración instanciadas.  
	Para la instanciación este tiene en cuenta:
	* Detecta procesadores para hojas de estilo  DocBook, hojas de estilo 
	  y de requerirse verifica sus versiones (Jade, OpenJade, xsltproc)
	* Adapta métodos de generación (por defecto prefiere emplear xsltproc
	  para generar HTML, OpenJade para generar PostScript y ps2pdf para
	  generar PDF).
	* Detecta herramientas auxiliares empleadas para la generación y
	  operación (e.g collateindex, dvips, convert, ps2pdf, awk, sed)
	* Detecta herraminetas opcionales que pueden servir para la
	  actualización del proyecto en Internet  (ncftpput o scp)
	* Actualiza fecha del proyecto de algún programa).
	
	Si este script no logra completar alguna detección, indicará el 
	problema, junto con la posible ayuda que se haya configurado en
	confv.empty y permitirá ingresar directamente la información o
	cancelar para reanudar posteriormente.

	De requerirlo usted puede cambiar directamente los valores detectados
	modificando el archivo confv.sh y ejecutando nuevamente ./conf.sh.

OPCIONES

$options_help


EJEMPLOS

	./conf.sh
	Configura fuentes y deja como prefijo para la ruta de instalación 
	"/usr/local"

	./conf.sh -p /usr/
	Configura fuentes y deja como prefijo para la ruta de instalación
	"/usr"


ESTÁNDARES
	Este script pretende ser portable. Debe cumplir POSIX.


FALLAS
	

VER TAMBIÉN
	Para mejorar este script o hacer uno similar ver fuentes de 
	herram/confaux.sh


CRÉDITOS Y DERECHOS DE REPRODUCCIÓN 

 	Script de dominio público.  Sin garantías.
	Fuentes disponibles en: http://structio.sourceforge.net/repasa
	Puede enviar reportes de problemas a 
		structio-info@lists.sourceforge.net

	Incluye porciones de código dominio público escritas por:
	  Miembros de Structio http://structio.sourceforge.net
	  WWWeb Tide Team http://www.ebbtide.com/Util/
	Puede ver más detalles sobre los derechos y créditos de este script en
	las fuentes.
MANPAGE_HEREDOC
   exit 0;
} fi;

# check for help
if (test "$HELP_FLAG" = "1" ) then {
   echo " Utilización: $USAGE"
   cat << HLP_OP
$options_help
HLP_OP
   exit 0
} fi;

# check for illegal switches
if (test "$OPT_FLAG" = "1") then {
   echo "$BASENAME: Se encontró alguna opción invalida" >&2
   echo "Utilización: $USAGE" >&2
   exit 1
}
elif (test "$#" != "$ARG_COUNT" ) then {
   echo "$BASENAME: se encontraron $# argumentos, pero se esperaban $ARG_COUNT." >&2
   echo "Utilización: $USAGE" >&2
   exit 1;
} fi;

echo "Configurando $PROYECTO $PRY_VERSION";

if (test "$DPLANO" != "si" -a "$prefix" != "") then {
        INSBIN="$prefix/bin";
        changeVar INSBIN 1;
        INSDOC="$prefix/share/doc/$PROYECTO";
        changeVar INSDOC 1;
	INSDATA="$prefix/share/$PROYECTO";
	changeVar INSDATA 1;
} fi;

if (test "$VERBOSE_FLAG" -gt "0") then {
	echo "Chequeando y detectando valor de variables de configuración";
} fi;
check "LATEX" "" "test -x \$LATEX" `which latex 2> /dev/null` 
check "PDFLATEX" "" "test -x \$PDFLATEX" `which pdflatex 2> /dev/null` 
check "BIBTEX" "" "test -x \$BIBTEX" `which bibtex 2> /dev/null`
check "MAKEINDEX" "" "test -x \$MAKEINDEX" `which makeindex 2> /dev/null`
check "HEVEA" "" "test -x \$HEVEA" `which hevea 2> /dev/null`
# If teTeX is installed could try
# find in `kpsewhich -expand-var='$TEXMFMAIN'`
check "HEVEA_STY" "" "test -f \$HEVEA_STY/hevea.sty" "/usr/local/lib/hevea" "/usr/lib/hevea" "/usr/share/hevea" 
check "DVIPS" "" "test -x \$DVIPS" `which dvips 2> /dev/null`

# We would like to call PS2PDF the next one but LaTeX doesn't accept numbers 
# in the name of a macro.
check "PSPDF" "" "test -x \$PSPDF" `which ps2pdf 2> /dev/null`

if (test "$ACT_PROC" = "act-ncftpput") then {
	check "NCFTPPUT" "optional" "test -x \$NCFTPPUT" `which ncftpput 2> /dev/null`
} 
elif (test "$ACT_PROC" = "act-scp") then { 
	check "SCP" "optional" "test -x \$SCP" `which scp 2> /dev/null`
} fi;


check "CONVERT" "" "test -x \$CONVERT" `which convert 2> /dev/null`

check "AWK" "" "test -x \$AWK" `which awk 2> /dev/null`
check "CP" "" "test -x \$CP" `which cp 2> /dev/null`
check "CVS" "optional" "test -x \$CVS" `which cvs 2> /dev/null`
check "ED" "" "test -x \$ED" `which ed 2> /dev/null`
check "FIND" "" "test -x \$FIND" `which find 2> /dev/null`
check "GZIP" "" "test -x \$GZIP" `which gzip 2> /dev/null`
# Corrección ortografica
check "ISPELL" "optional" "test -x \$ISPELL" `which ispell 2> /dev/null`
check "MAKE" "" "test -x \$MAKE" `which make 2> /dev/null`
check "MV" "" "test -x \$MV" `which mv 2> /dev/null`
check "MKDIR" "" "test -x \$MKDIR" `which mkdir 2> /dev/null`
check "PERL" "optional" "test -x \$PERL" `which perl 2> /dev/null`
check "RM" "" "test -x \$RM" `which rm 2> /dev/null`
check "SED" "" "test -x \$SED" `which sed 2> /dev/null`
check "TAR" "" "test -x \$TAR" `which tar 2> /dev/null`
check "TIDY" "optional" "test -x \$TIDY" `which tidy 2> /dev/null`
check "TOUCH" "" "test -x \$TOUCH" `which touch 2> /dev/null`

check "WTM" "optional" "test -x \$WTM" `which w3m 2> /dev/null` `which lynx 2> /dev/null`
l=`echo $WTM | sed -e "s|.*lynx.*|si|g"`
WTM_OPT=""; 
if (test "$l" = "si") then {
       WTM_OPT="-nolist";
} fi;
changeVar WTM_OPT 1;

check "ZIP" "optional" "test -x \$ZIP" `which zip 2> /dev/null`

BLB_TARGET="";
if (test "$DPLANO" = "si") then {
	BLB_TARGET="";
	# Objetivo para generar bibliografía, dejar en blanco si no tiene
#	IDX_TARGET="";
	# Objetivo para generar indice, dejar en blanco si no tiene
} else {
	BLB_TARGET="$PROYECTO.blb";
#	IDX_TARGET="$PROYECTO.idx";
} fi;

changeVar BLB_TARGET 1;
changeVar IDX_TARGET 1;

FECHA_ACT=`date "+%d/%m/%Y"`;
changeVar FECHA_ACT 1;
m=`date "+%m" | sed -e "s/01/Enero/;s/02/Febrero/;s/03/Marzo/;s/04/Abril/;s/05/Mayo/;s/06/Junio/;s/07/Julio/;s/08/Agosto/;s/09/Septiembre/;s/10/Octubre/;s/11/Noviembre/;s/12/Diciembre/"`
a=`date "+%Y"`
MES_ACT="$m de $a";
changeVar MES_ACT 1;

if (test "$VERBOSE_FLAG" -gt "0") then {
	echo "Guardando variables de configuración";
} fi;
changeConfv;

if (test "$VERBOSE_FLAG" -gt "0") then {
	echo "Generando Make.inc";
} fi;

echo "# Algunas variables para el Makefile" > Make.inc;
echo "# Este archivo es generado automáticamente por conf.sh. No editar" >> Make.inc;
echo "" >> Make.inc

# Adding configuration variables to Make.inc
addMakeConfv Make.inc;
echo "PREFIX=$prefix" >> Make.inc

if (test "$VERBOSE_FLAG" -gt "0") then {
	echo "Generando confv.tex"
} fi;

echo "% Variables de configuración" > confv.tex
echo "% Este archivo es generado automáticamente por conf.sh. No editar " >> confv.tex

addLATeXConfv confv.tex;

if (test "$DPLANO" != "si") then {
	if (test "$VERBOSE_FLAG" -gt "0") then {
		echo "Creando directorios auxiliares"
	} fi;
	mkdir -p html
} fi;

if (test "$VERBOSE_FLAG" -gt "0") then {
	echo "Cambiando ruta de awk en script"
} fi;
echo ",s|/usr/bin/awk|$AWK|g
w
q
" | ed herram/latex2rep 2> /dev/null

if (test ! -f repasa.hva) then {
	ln -s repasa.sty repasa.hva
} fi;
echo "Configuración completada";

