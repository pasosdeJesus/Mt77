#!/usr/bin/awk
# Extrae respuestas de un documento LaTeX que siga convenciones de repasa.

/\\label{guia.*-ejercicios/ {
	if (enumabierto==1) {
		print "\\end{enumerate}";
	}
	match($0, /guia[0-9]*-/);
	n=substr($0,RSTART+4, RLENGTH-5);
	p=substr($0,RSTART+RLENGTH,length($0)-(RSTART+RLENGTH));
	sub(/}.*/,"",p);
	t=p;
	sub(/-/," de ",t);
	print "\\section*{Respuestas a " t " de \\ref{guia" n "-" p "}}";
	print "\\begin{enumerate}";
	enumabierto=1;
}

/^[ ]*\\end{respuesta}/ {
	imp=0;
}

/.*/ {
	if (imp==1) {
		print $0;
	}
}


/^[ ]*\\begin{respuesta}/ {
	imp=1;
	print "\\item %" c;	
}


/^[ ]*\\ejercicio/ {
	c=c+1;	
}


/^[ ]*\\end{enumerate}/ {
	c=0;
}

BEGIN {
	c=0;
}

END {
	if (enumabierto==1) {
		print "\\end{enumerate}";
	}
}
