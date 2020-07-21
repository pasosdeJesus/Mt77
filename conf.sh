#!/bin/sh

# This source is released to the public domain since 2001. No warranty.
# Citation of the source is appreciated.
# http://www.pasosdejesus.org/devel/conf

# The command line management of this script is based on public
# domain code of WWWeb Tide Team 
#    http://www.ebbtide.com/Util/ksh_parse.html 

# Reading configuration variables
if (test ! -f confv.sh) then {
        cp confv.empty confv.sh
} fi;
. ./confv.sh


# Reading functions to help in configuration
. herram/confaux.sh

#Parsing options

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
confdoc="" 
confphp="portal/conf.php"

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
	archivos en diversos formatos empleados por las fuentes de 
	$PROYECTO 
	* $PRY_DESC
	* $URLSITE

	Las variables de configuración y sus valores por defecto están
	en confv.empty (debajo de cada variable hay un comentario con la 
	descripción).
	Este script genera los archivos
		confv.sh, Make.inc, confv.ml y $confdoc
	con las variables de configuración instanciadas.  
	Para la instanciación este script puede:

	* Verificar el valor de algunas variables (por ejemplo que
	  la versión de un programa sea la requerida).
	* Buscar valor para algunas variables (por ejemplo ubicación
	  de algún programa).
	* Completar el valor de algunas variables (por ejemplo fecha actual)
	* Dejar el valor por defecto configurado en confv.empty (por ejemplo 
	  nombre del proyecto y versión).
	* Pedir información al usuario en los casos que no logra instanciar


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
	Fuentes disponibles en: http://www.pasosdejesus.org/devel/confsh
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
if (test "$prefix" != "") then {
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


check "AWK" "" "test -x \$AWK" `which awk 2> /dev/null`
check "CP" "" "test -x \$CP" `which cp 2> /dev/null`
check "CVS" "optional" "test -x \$CVS" `which cvs 2> /dev/null`
check "ED" "" "test -x \$ED" `which ed 2> /dev/null`
check "FIND" "" "test -x \$FIND" `which find 2> /dev/null`
check "GZIP" "" "test -x \$GZIP" `which gzip 2> /dev/null`
check "MAKE" "" "test -x \$MAKE" `which make 2> /dev/null`
check "MV" "" "test -x \$MV" `which mv 2> /dev/null`
check "MKDIR" "" "test -x \$MKDIR" `which mkdir 2> /dev/null`
check "PERL" "optional" "test -x \$PERL" `which perl 2> /dev/null`
check "RM" "" "test -x \$RM" `which rm 2> /dev/null`
check "SCP" "" "test -x \$SCP" `which scp 2> /dev/null`
check "SED" "" "test -x \$SED" `which sed 2> /dev/null`
check "TAR" "" "test -x \$TAR" `which tar 2> /dev/null`
check "TOUCH" "" "test -x \$TOUCH" `which touch 2> /dev/null`
check "W3M" "optional" "test -x \$W3M" `which w3m 2> /dev/null` `which lynx 2> /dev/null`
l=`echo $W3M | sed -e "s|.*lynx.*|si|g"`
W3M_OPT="";
if (test "$l" = "si") then {
	W3M_OPT="-nolist";
} fi;
changeVar W3M_OPT 1;
check "ZIP" "optional" "test -x \$ZIP" `which zip 2> /dev/null`


if (test "$ACT_PROC" = "act-ncftpput") then {
	check "NCFTPPUT" "" "test -x \$NCFTPPUT" `which ncftpput 2> /dev/null`
}
elif (test "$ACT_PROC" = "act-scp") then {
	check "SCP" "" "test -x \$SCP" `which scp 2> /dev/null`
} fi;

FECHA_ACT=`date "+%d/%m/%Y"`;
changeVar FECHA_ACT 1;
m=`date "+%m" | sed -e "s/01/Enero/;s/02/Febrero/;s/03/Marzo/;s/04/Abril/;s/05/Mayo/;s/06/Junio/;s/07/Julio/;s/08/Agosto/;s/09/Septiembre/;s/10/Octubre/;s/11/Noviembre/;s/12/Diciembre/"`
a=`date "+%Y"`
MES_ACT="$m de $a";
changeVar MES_ACT 1;


if (test "$VERBOSE_FLAG" -gt "0") then {
	echo "Guardando variables de configuración"
} fi;
changeConfv;

if (test "$VERBOSE_FLAG" -gt "0") then {
	echo "Generando Make.inc";
} fi;

echo "# Some variables for Makefile" > Make.inc;
echo "# This file is generated automatically by conf.sh.  Don't edit" >> Make.inc;
echo "" >> Make.inc

if (test "$VERBOSE_FLAG" -gt "1") then {
	echo "Adding configuration variables to Make.inc";
} fi;
addMakeConfv Make.inc;


touch $confphp 2> /dev/null
if (test "$?" = "0") then {

	echo "<?php # Parámetros de configuración " > $confphp
	echo "# Generado automáticamente por conf.sh.  No editar" >> $confphp
	if (test "$VERBOSE_FLAG" -gt "1") then {
		echo "Agregando variables de configuración a $confphp";
	} fi;
	addPHPConfv $confphp
	echo "?>" >> $confphp
} fi;

if (test "$confdoc" != "" -a ! -f "$confdoc") then {
	cp $confdoc.plantilla $confdoc
} fi;

echo "Configuración completada";

