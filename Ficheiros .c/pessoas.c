/* Paul Ionut Bob - 2016013407
 * File:   pessoas.c
 * Author: droopy
 *
 * Created on 8 de Maio de 2020, 9:07
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "estruturas.h"
#include "pessoas.h"
#include "simulacao.h"

int preenchePessoa(pPessoa cobaia){                                             //preenche um no pessoa
    printf("Nome: ");
    scanf(" %s", cobaia->nome);
    printf("Idade: ");
    scanf(" %d", &cobaia->idade);
    printf("Legenda: S -> Saudavel\n         D -> Doente\n         I -> Imune\n");
    printf("Estado: ");
    scanf(" %c", &cobaia->estado);
    cobaia->estado = toupper(cobaia->estado);                                   // converte estado para maiusculas caso o utilizador ,por descuido ou a tentar crashar-me o programa, insereiu minuscula
    if (cobaia->estado == 'D' || cobaia->estado == 'd'){                        // caso a pessoa foi preenchida como sendo doente é pedido o numero de dias ao utilizador
        printf("Doente ha quantos dias: ");
        scanf(" %d", &cobaia->contaDias);
    }
    else
        cobaia->contaDias = -1;
    if(cobaia->contaDias > duracaoMax(cobaia->idade))
    cobaia->prox = NULL;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int verificaPessoa(pPessoa populacao, pessoa cobaia){                           //verifica uma pessoa
    while (populacao != NULL){                                                  //enquanto existir pessoas na população/"fila"
        if (strcmp(cobaia.nome,populacao->nome) == 0){                          //caso já exista o nome da pessoa a verificar na população devolve 0
            printf("\n%s ja existe na populacao.\n", cobaia.nome);
            return 0;}    
    populacao = populacao->prox;                                                //atualiza ponteiro para a próxima pessoa
    }
    
    if (cobaia.idade < 1 || cobaia.idade>117){                                  //de acordo com a wikipidia o homem mais velho do mundo viveu 122 anos e o homem mais velho da atualidade tem 117 anos
        printf("%s apresenta idade invalida.\n", cobaia.nome);
        return 0;}
    
    if (cobaia.estado != 'S' && cobaia.estado != 'D' && cobaia.estado != 'I'){  //de acordo com o enunciado estes são os únicos casos possiveis para o estado de uma pessoa.
        printf("%c tem um estado nao reconecido pelo programa.", cobaia.estado);
        return 0;}
    
    if ((cobaia.estado == 'S' && cobaia.contaDias != -1)  ||                    //caso esteja saudavel ou imune mas o contaDias não está a -1 significa que algo correu mal
        (cobaia.estado == 'I' && cobaia.contaDias != -1)){
        printf("%s tem incopatibilidade entre estado(%c) - contaDias(%d)\n", cobaia.nome,cobaia.estado,cobaia.contaDias);
        return 0;}
    
    if (cobaia.estado == 'D' && cobaia.contaDias > duracaoMax(cobaia.idade)){  //caso esteja doente mas o nr de dias excedeu o considerado maximo para esta aplicação
        printf("\n%s , doente ha %d dias excede o numero maximo (%d dias) que uma pessoa com %d anos pode ficar doente nesta simualacao.\n"
                ,cobaia.nome,cobaia.contaDias,duracaoMax(cobaia.idade),cobaia.idade);
        return 0;}                                               
    return 1;                                                                   //devolve 1 se a pessoa passou por todas as verificações.
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mostraPessoa(pessoa cobaia){                                               //mostra uma pessoa
    char* ficarBonito = "Doente ha ";
    printf("Nome:   %-25s |  Idade: %-7d|  Estado: %c ", cobaia.nome, cobaia.idade, cobaia.estado);
    switch(cobaia.estado){
        case 'S': printf("(Saudavel)             |"); break;
        case 'D': printf("(%s%-2d dias.)   |",ficarBonito,cobaia.contaDias); break;
        case 'I': printf("(Imune)                |"); break;
    }
    puts("\n----------------------------------------------------------------------------------------");
}
