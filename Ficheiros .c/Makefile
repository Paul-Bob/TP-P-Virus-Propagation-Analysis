all: main.o espaco.o local.o auxiliar.o pessoas.o populacao.o preparacao.o relatorio.o simulacao.o
	cc *.o -o tpProgramacao

main.o: main.c estruturas.h auxiliar.h espaco.h populacao.h preparacao.h relatorio.h simulacao.h
	cc -c main.c

espaco.o: espaco.c estruturas.h espaco.h local.h
	cc -c espaco.c

local.o: local.c estruturas.h local.h
	cc -c local.c

auxiliar.o: auxiliar.c auxiliar.h
	cc -c auxiliar.c

pessoas.o: pessoas.c estruturas.h pessoas.h simulacao.h
	cc -c pessoas.c

populacao.o: populacao.c estruturas.h populacao.h pessoas.h
	cc -c populacao.c

relatorio.o: relatorio.c estruturas.h relatorio.h espaco.h
	cc -c relatorio.c

simulacao.o: simulacao.c estruturas.h simulacao.h auxiliar.h relatorio.h populacao.h espaco.h pessoas.h preparacao.h
	cc -c simulacao.c

clean:
	rm *.o tpProgramacao