
# Common rules to generate HTML, PostScript and PDF versions of a LATeX document
# Released to the public domain 2003. No waranties. 
# structio-info@lists.sourceforge.net

# Variables required

# Tools
# GZIP, LATEX, MAKEINDEX, HEVEA, HEVEA_STY, DVIPS, PSPDF paths to programs

# Of the particular project
# PROYECTO With project name and name of main file (extension .tex)
# SOURCES Sources including $PROYECTO.tex
# IMAGES 
# HTML_TARGET Target for HTML

$(PROYECTO).txt: $(PROYECTO).tex
	TEXINPUTS=$$TEXINPUTS:$(HEVEA_STY) $(HEVEA) -text $(PROYECTO).tex
	cp $(PROYECTO).txt /tmp/$(PROYECTO).txt
	sed -e "s/<hr>//g;s/[0-9][.][0-9]cm/¬¬/g;s/[0-9]cm/¬/g" /tmp/$(PROYECTO).txt | tr "¬" "\\n" > $(PROYECTO).txt
#	if (test "$(WTM)" = "") then { echo "Se requiere w3m o lynx"; exit 1; } fi;
#	$(WTM) -dump $(WTM_OPT) $(PROYECTO).html > $(PROYECTO).txt

$(HTML_TARGET):  $(SOURCES) $(IMAGES) $(BLB_TARGET)
	-TEXINPUTS=$$TEXINPUTS:$(HEVEA_STY) $(HEVEA) $(PROYECTO).tex
	-TEXINPUTS=$$TEXINPUTS:$(HEVEA_STY) imagen $(PROYECTO)
	TEXINPUTS=$$TEXINPUTS:$(HEVEA_STY) $(HEVEA) -fix $(PROYECTO).tex
	cp $(HTML_TARGET) /tmp/$(HTML_TARGET)
	sed -e "s/!!/~/g;s/[0-9][.][0-9]cm/<p>/g;s/[0-9]cm/<p>/g" /tmp/$(HTML_TARGET) > $(HTML_TARGET)

$(PROYECTO).blb: $(PROYECTO).bib $(SOURCES) $(IMAGES:.png=.eps)
	TEXINPUTS=$$TEXINPUTS:$(HEVEA_STY) $(LATEX) $(PROYECTO).tex
	$(BIBTEX) $(PROYECTO)
	rm -f $(PROYECTO).dvi

$(PROYECTO).dvi: $(SOURCES) $(IMAGES:.png=.eps) $(BLB_TARGET)
	TEXINPUTS=$$TEXINPUTS:$(HEVEA_STY) $(LATEX) $(PROYECTO).tex
	if (test "$(IDX_TARGET)" != "") then { \
		$(MAKEINDEX) -s $(PROYECTO).ist $(IDX_TARGET) ; \
	} fi;
	TEXINPUTS=$$TEXINPUTS:$(HEVEA_STY) $(LATEX) $(PROYECTO).tex
	TEXINPUTS=$$TEXINPUTS:$(HEVEA_STY) $(LATEX) $(PROYECTO).tex

$(PROYECTO)-$(PRY_VERSION).html.gz: $(PROYECTO).html
	cp $(PROYECTO).html $(PROYECTO)-$(PRY_VERSION).html
	rm -f $(PROYECTO)-$(PRY_VERSION).html.gz
	$(GZIP) $(PROYECTO)-$(PRY_VERSION).html

$(PROYECTO)-$(PRY_VERSION).ps.gz: $(PROYECTO).ps
	cp $(PROYECTO).ps $(PROYECTO)-$(PRY_VERSION).ps
	rm -f $(PROYECTO)-$(PRY_VERSION).ps.gz
	$(GZIP) $(PROYECTO)-$(PRY_VERSION).ps

$(PROYECTO)-$(PRY_VERSION).pdf: $(PROYECTO).pdf
	cp $(PROYECTO).pdf $(PROYECTO)-$(PRY_VERSION).pdf

$(PROYECTO).pdf: $(PROYECTO).ps
	$(PSPDF) -sPAPERSIZE=$(PAPEL) $(PROYECTO).ps $(PROYECTO).pdf

$(PROYECTO).ps: $(PROYECTO).dvi
	$(DVIPS) -t $(PAPEL) -o $(PROYECTO).ps $(PROYECTO).dvi

.SUFFIXES: .eps .png .dot .fig

.png.eps:
	$(CONVERT) $< EPS:$@

.dot.png:
	dot -Tgif $< >/tmp/dotpng.gif
	convert /tmp/dotpng.gif /tmp/dotpng.jpg  # convert no pasaba bien de gif a png
	convert /tmp/dotpng.jpg $@

.fig.png:
	fig2dev -L png $< $@


# Revisa ortografía con ispell
ispell: $(PROYECTO).txt
	$(TOUCH) $(PROYECTO).ispell
	$(ISPELL) -dspanish -p $(PROYECTO).ispell $(PROYECTO).txt

