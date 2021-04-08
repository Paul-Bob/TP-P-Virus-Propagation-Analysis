/* Paul Ionut Bob - 2016013407
 * File:   populacao.c
 * Author: droopy
 *
 * Created on 8 de Maio de 2020, 12:57
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"
#include "populacao.h"
#include "pessoas.h"


void mostraPopulacao(pPessoa populacao){                                        //mostra toda a lista ligada população/"fila"
    if(populacao == NULL){
        puts("\nPopulacao sem pessoas!");
        return;
    }
    puts("\n----------------------------------------------------------------------------------------");
    while(populacao != NULL){
        mostraPessoa(*populacao);
        populacao = populacao->prox;
    }
} 
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pPessoa inserePessoaInicio(pPessoa populacao){                                       //preenche e insere uma pessoa no inicio da lista ligada
    pPessoa nova;
    
    nova = malloc(sizeof(pessoa));                                               // Alocar
    if(nova == NULL){
        fprintf(stderr,"Erro na alocacao! \n");
        return populacao;
    }
    
    preenchePessoa(nova);                                                        // Preencher
     if (verificaPessoa(populacao,*nova) == 0)
        return populacao;                                                                                
    
    nova->prox = populacao;                                                     //Inserir no inicio
    populacao = nova;
    
    return populacao;                                                           //devolve lista atualizada.
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pPessoa inserePessoaFinal(pPessoa populacao,pessoa lida,int modo){                   //preenche e insere uma pessoa ou insere uma pessoa já preenchida no final da lista ligada
    pPessoa nova = NULL,aux = NULL;

    nova = malloc(sizeof(pessoa));                                              // Alocar
    if(nova == NULL){
        fprintf(stderr,"Erro na alocacao! \n");
        return populacao;
    }

    if (modo == 0)                                                              //modo 0 = preencher e adicionar pessoa
        preenchePessoa(nova);                                                   // Preencher
    else if (modo == 1){                                                        // modo 1 = adicionar pessoa ja recebida e preenchida
        *nova=lida;
        nova->prox = NULL;
    }
    if (verificaPessoa(populacao,*nova) == 0)
        return populacao;
    //Inserir no final
    if(populacao == NULL)                                                       //se a população fica vazia atualiza o ponteiro para a unica pessoa da lista
        populacao = nova;
    else{                                                                       //caso contrario
        aux = populacao;
        while(aux->prox != NULL)                                                //percorre até ao final e adiciona no final
            aux = aux->prox;
        aux->prox = nova;
    }

    return populacao;                                                            //devolve lista atualizada.
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pPessoa inserirPessoa(pPessoa populacao){                                      //permite escolher entre adicionar no inicio ou no final da fila ( não muito relevante para este enunciado)
    pessoa aux;                                                                 // variavel para enviar no inserePessoaFinal, podia enviar *populacao, mas quando populacao está a NULL não vai funcionar.
    int i=0;
    printf("Metodos de insercao:\n1 - Inserir no inicio da lista\n2 - Inserir no final da lista\nMetodo:");
    scanf(" %d", &i);
    
    if(i==1)
        return inserePessoaInicio(populacao);
    else if(i==2)
        return inserePessoaFinal(populacao,aux,0);
    
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pPessoa eliminaPessoa(pPessoa populacao,char *nome){                            //elimina no da lista
    pPessoa atual=populacao , ant=NULL;

    while(atual != NULL && strcmp(atual->nome,nome) != 0){                      //mete ponteiro a apontar para o no a eliminar tendo o nome como referencia
        ant = atual;
        atual = atual->prox;
    }
    
    if(atual == NULL)                                                            //se pessoa nao existe devolve população
        return populacao;
    
    if(atual == populacao)                                                      // se o ponteiro aponta para o inicio da lista entao elimia a primeira pessoa
        populacao = populacao->prox;
    else                                                                        // caso contrario eliminar qualquer outra pessoa (atualiza os ponteiros) 
        ant->prox = atual->prox;

    free(atual);                                                                //liberta alocação de memória feita pelo no que pretendemos eliminar

    return populacao;                                                           //devolve lista atualizada.
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pPessoa libertaLista(pPessoa lista){                                              //liberta lista ligada
    pPessoa aux;
    while(lista != NULL){                                                       //enquanto existem pessoas na população vamos avançando e eliminando
        aux = lista;
        lista = lista->prox;
        free(aux);
    }
    return NULL;                                                                
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void contaPessoas(pPessoa populacao,int *total){                               //conta quantas pessoas existem na lista ligada
    *total=0;
    while(populacao != NULL){
        (*total)++;
        populacao = populacao->prox;
    }
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/