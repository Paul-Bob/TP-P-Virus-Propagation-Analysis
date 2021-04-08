/* Paul Ionut Bob - 2016013407 
 * File:   populacao.h
 * Author: droopy
 *
 * Created on 8 de Maio de 2020, 12:57
 */

#ifndef POPULACAO_H
#define POPULACAO_H
void mostraPopulacao(pPessoa populacao);                                        //mostra toda a lista ligada população/"fila"
void contaPessoas(pPessoa populacao,int *total);                                //conta quantas pessoas existem na lista ligada
pPessoa inserePessoaInicio(pPessoa primeiro);                                   //preenche e insere uma pessoa no inicio da lista ligada
pPessoa inserePessoaFinal(pPessoa primeira,pessoa lida,int modo);               //preenche e insere uma pessoa ou insere uma pessoa já preenchida no final da lista ligada
pPessoa inserirPessoa(pPessoa populacao);                                       //permite escolher entre adicionar no inicio ou no final da fila ( não muito relevante para este enunciado)
pPessoa eliminaPessoa(pPessoa populacao,char *nome);                            //elimina no da lista
pPessoa libertaLista(pPessoa lista);                                            //liberta lista ligada
#endif /* POPULACAO_H */

