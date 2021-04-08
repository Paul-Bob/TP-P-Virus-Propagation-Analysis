/* Paul Ionut Bob - 2016013407
 * File:   relatorio.h
 * Author: droopy
 *
 * Created on 2 de Junho de 2020, 11:25
 */

#ifndef RELATORIO_H
#define RELATORIO_H
#include <time.h>
void mostraInformacao(pInforma lista);
void atualizaEstado(pInforma lista,int dia);
void guardaInformacao(plocal espaco, int tam, pInforma lista, int dia);
void escreveInformacao(FILE *fich, informa inicial, informa atual, informa anterior, char* string1, char *string2, int* iteracoes);
void guardaPessoas(plocal espaco, int total_locais);
float variacaoRelativa(int atual,int anterior);
pInforma insereInformacao(pInforma lista, informa no);
pInforma eliminaInvalido(pInforma lista);
#endif /* RELATORIO_H */

