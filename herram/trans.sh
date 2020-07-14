#!/bin/sh
# Aplica a todas las fuentes las transformaciones de herram/trans.sed
# y herram/trans.awk si existe
# Dominio público. 2009. vtamara@pasosdeJesus.org

mkdir -p /tmp/pruebas/aut
mkdir -p /tmp/aut
mkdir -p /tmp/herram

sn="";
for i in `find . -name "*cpp" -o -name "*hpp"`; do
	echo "Transformando $i";
	if (test -f herram/trans.awk) then {
		awk -f herram/trans.awk $i > /tmp/$i-1
	} else {
		cp $i /tmp/$i-1
	} fi;
	if (test -f herram/trans.sed) then {
		sed -f herram/trans.sed /tmp/$i-1 > /tmp/$i;
	} else {
		cp $i-1 /tmp/$i
	} fi;

	diff $i /tmp/$i;
	if (test "$?" = "1") then {
		echo -n "Remplazar (s/n)? ";
		if (test "$sn" != "S") then {
			read sn
		} fi;
		if (test "$sn" = "s" -o "$sn" = "S") then {
			cp /tmp/$i $i;
		} fi;
	} fi;
done
