all: SOC-manual.dvi SOC-manual.ps SOC-manual.pdf

SOC-manual.dvi: *tex
	platex SOC-manual.tex
	platex SOC-manual.tex

SOC-manual.ps: SOC-manual.dvi
	pdvips -f SOC-manual.dvi > SOC-manual.ps

SOC-manual.pdf: SOC-manual.ps
	ps2pdf13 SOC-manual.ps
