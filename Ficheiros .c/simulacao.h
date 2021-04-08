/* Paul Ionut Bob - 2016013407
 * File:   simulacao.h
 * Author: droopy
 *
 * Created on 25 de Maio de 2020, 18:55
 */

#ifndef SIMULACAO_H
#define SIMULACAO_H
#define MIN_DISTRIBUIDAS 5                                                      //minimo de pessoas distribuidas pelos locaos para iniciar simulação (5 é demasiado pequeno, escolhi meramente para poder
#include "estruturas.h"                                                         //fazer alguns testes com ficheiros de pequenas populações). Pode ser alterado a qualquer altura.
void guardaDistribuicao(plocal espaco,int total,char* nome);                   //guarda distribuição atual num ficheiro binário
void adicionaDoente(plocal espaco,int total_locais,int *distribuidas);          //adiciona doente num id especifico.
int imunidade();                                                                // devolve 1 caso a pessoa fique imune
int duracaoMax(int idade);                                                     // devolve duração maxima que uma pessoa pode ficar doente (em dias)
int recupera(int idade);                                                        //devolve 1 caso o paciente ficou curado
int disseminacaoLocal(int pessoas);                                            //devolve quantas pessoas serão infetadas por cada doente de um determinado local
void transferePessoas(plocal espaco,int total_locais);
informa informacaoSimulacao(plocal espaco, int total_locais, int pessoas_distribuidas, int dia, int *doentesT, plocal diaAnterior, int podeRecuar, informa primeira);//escreve algumas informações sobre o estado atual da simulação
pInforma aplicaModelo(plocal espaco, int total_locais, int* dia, plocal voltaUmDia, plocal voltaDoisDias, plocal voltaTresDias, int* doentesT, pInforma informacao);//aplica recuperação e dissiminação no espaço e avança um dia e atualiza os recuas
plocal carregaDistribuicao(plocal espaco,char* nome);               //carrega distribuição atual dum ficheiro binário
plocal recuaSimulacao(plocal espaco, plocal recuaUm, plocal recuaDois, plocal recuaTres, plocal inicio, int total_locais, int *distribuidas,int* dia, int* podeRecuar, int* recuou); //permite esolher quantos dias recua e atualiza o estado da simulacao

#endif /* SIMULACAO_H */

