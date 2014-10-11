default: pdf

pdf:
	pdflatex -shell-escape slides.tex
	bibtex slides
	pdflatex -shell-escape slides.tex
	pdflatex -shell-escape slides.tex

ps: dvi
	dvips slides.dvi

dvi:
	latex slides.tex
	bibtex slides
	latex slides.tex
	latex slides.tex

clean:
	rm -vf slides.{aux,bbl,blg,brf,fdb_latexmk,lof,log,lot,out,toc}

mrproper: clean
	rm -vf slides.{dvi,pdf,ps}

