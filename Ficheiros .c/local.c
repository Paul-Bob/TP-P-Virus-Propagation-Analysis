/* Paul Ionut Bob - 2016013407
 * File:   local.c
 * Author: droopy
 *
 * Created on 22 de Abril de 2020, 17:58
 */

#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"
#include "local.h"



void mostraLocal(local sala){                                                   //mostra local
    printf("ID: %-24d|\n",sala.id);
    printf("Capacidade Total: %-10d|\n", sala.capacidade + sala.pessoas);
    if (sala.pessoas != 0)
    printf("Capacidade Atual: %-10d|\n", sala.capacidade);
    for(int i=0; i<MAX_LIGACOES; i++)                                           //percorre todas as ligaçoes e caso exista ligações ( != -1) mostra as ligações
        if (sala.liga[i]!=-1)
            printf("Ligacao %d: %-17d|\n",i+1,sala.liga[i]);
    if(sala.prox == NULL)                                                       //verifica se tem pessoas associadas, se sim, quantas.
        printf("Sem pessoas distribuidas.   |\n");
    else
        printf("Pessoas Distribuidas: %-6d|\n",sala.pessoas);
    puts("-----------------------------");
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int validaLocal(local sala){                                                    //faz validação do local
    int conta=0;
    if (sala.id < 1){                                                           //verifica se o id é positivo e maior do que 0
            puts("\nERRO: ID negativo ou ID = 0");
            printf("O id %d nao e permitido", sala.id);
            return 0;}
    if (sala.capacidade < 0){                                                   //verifica se a capacidade é positiva 
            puts("\nERRO: Capacidade negativa");
            printf("A capacidade de %d nao e valida", sala.capacidade);
            return 0;}
    for(int i=0; i<MAX_LIGACOES; i++){                                          //percorre todas as ligações e verifica se existe pelo menos uma ligação
        if (sala.liga[i] == -1){                                                
            conta++;
            if (conta == 3){                                                    //caso não exista nenhuma ligação devolve 0
                puts("\nERRO: Local isolado (SEM LIGACOES)");   
                return 0;}} 
        else if (sala.liga[i] < 0){                                             //caso exista ligação verifica se é 0 ou negativa 
            puts("Erro na validacao da ligacao ( ID negativo ou ID = 0)\n");
            return 0;}
        else if (sala.liga[i] == sala.id){                                      //verifica se o local tem o seu ID nas ligaçoes, se sim devolve logo 0
            puts("\nERRO: O local nao pode ter o proprio ID nas ligacoes");
            return 0;}
        else
        for(int j=i+1; j<MAX_LIGACOES; j++)                                     //percorre o resto das ligações, e compara com a atual para certificar que não existem ligações repetidas.
            if (sala.liga[i]!= -1 && sala.liga[i] == sala.liga[j]){
                puts("Erro na validacao da ligacao ( Ligacao repetida)\n");
                return 0;}
    }
    return 1;                                                                   //caso passou todas as verificações devolve 1
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int preencheLocal(plocal local){                                                //preenche um local passado com ponteiro 
    int ligacoes=0;
    
    printf("\nID: ");                                                           //preenche ID
    if (!scanf(" %d",&local->id)){
        puts("\nERRO: erro de leitura ao introduzir o ID");
        return 0;}
    
    printf("Capacidade: ");                                                     //preenche capacidade
    if (!scanf(" %d",&local->capacidade)){
        puts("\nERRO: erro de leitura ao introduzir a Capacidade");
        return 0;}

    printf("Quantas ligacoes tera o local? (Min:1 | Max:%d)\nLigacoes: ",MAX_LIGACOES);//quantas ligações?
    if (!scanf(" %d",&ligacoes)){
        puts("\nERRO: erro de leitura ao introduzir o numero de ligacoes");
        return 0;}
    
    if (ligacoes > MAX_LIGACOES || ligacoes < 1){                               //caso introduzir um numero inválido da erro e devolve 0
        puts("\nERRO: Numero de ligacoes invalido!");
        return 0;
    }
        
    for(int i=0; i<ligacoes; i++){                                              //pede o ID da ligação o número de vezes que foi introduzido no scanf acima
        printf("ID da ligacao %d: ",i+1);
        if (scanf(" %d",&local->liga[i]) != 1 || local->liga[i]<1){             //caso o scanf falhe ou a ligação é menor do que um devolve 0
            printf("\nERRO: Ligacao %d invalida ou erro de leitura ", i+1);
            return 0;}
    }
    for(int j=ligacoes; j<MAX_LIGACOES; j++)                                    //coloca o resto das ligações que não foram preenchidas a -1
        local->liga[j] = -1;
    local->pessoas = 0;                                                         //assegurar que o espaço não tem pessoas associadas
    local->prox = NULL;

    if (validaLocal(*local) == 1)                                               //caso o local passe pela validação devolve 1
        return 1;
    return 0;                                                                   //caso contrario devolve 0
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void organizaLigacoes(plocal espaco, int total){                                //organiza as ligaçoes para que as ligações vazias ( == -1) apareçam depos das ligaçoes preenchidas
    for(int i=0; i<total; i++)                                                  // é utilizada para evitar bugs quando existe eliminação de locais 
        for(int j=0; j<MAX_LIGACOES; j++){                                      //ciclo que força a função percorrer o array o numero necessario até que um valor da ultima posição 
            for(int k=1; k<MAX_LIGACOES; k++)                                   //chegue a primeira posição ex: ( -1 -1 -1 -1 -1 3) neste enucniado o maior exemplo possivel é (-1 -1 3)
                if(espaco[i].liga[k] != -1 && espaco[i].liga[k-1] == -1){
                    espaco[i].liga[k-1] = espaco[i].liga[k];
                    espaco[i].liga[k] = -1;
                }     
        }
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/