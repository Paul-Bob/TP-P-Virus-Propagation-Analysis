/* Paul Ionut Bob - 2016013407
 * File:   relatorio.c
 * Author: droopy
 *
 * Created on 2 de Junho de 2020, 11:25
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"
#include "relatorio.h"
#include "espaco.h"



pInforma insereInformacao(pInforma lista, informa no){
    pInforma nova = NULL, aux = NULL;

    nova = malloc(sizeof(informa));                                              // Alocar
    if(nova == NULL){
        fprintf(stderr,"Erro na alocacao! \n");
        return lista;
    }

        *nova=no;
        nova->prox = NULL;
        
    //Inserir no final
    if(lista == NULL)                                                           //se a lsita fica vazia atualiza o ponteiro para o unico no da lista
        lista = nova;
    else{                                                                       //caso contrario
        aux = lista;
        while(aux->prox != NULL)                                                //percorre até ao final e adiciona no final
            aux = aux->prox;
        aux->prox = nova;
    }

    return lista;                                                               //devolve lista atualizada.
}

void mostraInformacao(pInforma lista){
    pInforma aux = lista;
    while(aux != NULL){
        printf("\nDIA: %d\n", aux->dia);
        printf("Doentes: %d\n" ,aux->doentes);
        printf("Saudaveis: %d\n" ,aux->saudaveis);
        printf("Imunes: %d\n" ,aux->imunes);
        printf("Pessoas: %d\n", aux->pessoas);
        printf("ESTADO : %d\n" ,aux->valido);
        aux = aux->prox;
    }
}

void atualizaEstado(pInforma lista,int dia){                                    // em casos de recuar no programa invalida os dias em questao e depois se anular o recuar reatualiza e valida
    pInforma aux = lista;
    while(aux != NULL){
        if(aux->dia > dia)
            aux->valido = 0;
        else
            aux->valido = 1;
        aux = aux->prox;
    }
}

pInforma eliminaInvalido(pInforma lista){
    pInforma atual = lista, ant = NULL, aux = NULL;
    while(atual != NULL){
        if(atual->valido == 0){
            while(atual != NULL){
                puts("welelecas");
                aux = atual;
                atual = atual->prox;
                free(aux);
            }
        ant->prox = NULL;
        return lista;}
        ant = atual;
        atual = atual->prox;  
    }
    return lista;
}

float variacaoRelativa(int atual,int anterior){
    return ((float)atual/(float)anterior*100)-100;
}

void guardaInformacao(plocal espaco, int tam, pInforma lista, int dia){
    pInforma atual = NULL;
    informa anterior;
    char nomeFicheiro[NOME_FICHEIRO],intervalo[10];
    char *stringDia="o dia"; char* noDia="No dia"; char* stringSemana="a semana"; char* naSemana="Na semana"; char* stringMes="o mes"; char *noMes="No mes";
    int diaDATA,mes,ano,hora,minuto,modo=1,corrupcao=0,capacidade=0,gravaMax=1,iteracoes = 0;
    float mediaDiaria = 0, mediaSemanal = 0, mediaMensal = 0;
    
    if(dia/7 > 2)
        gravaMax=2;
    if(dia/30 > 2)
        gravaMax=3;

    

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    diaDATA = local->tm_mday;
    mes = local->tm_mon + 1;
    ano = local->tm_year + 1900;
    hora = local->tm_hour;
    minuto = local->tm_min;
    
    sprintf(nomeFicheiro,"%d_%02d_%02d.Simulacao.%02dh%02dm.txt",ano,mes,diaDATA,hora,minuto);

    puts("\nAntes de concluir a simulacao e fechar o porgrama ira ser gerado um relatorio com informacao sobre a simulacao feita!");
    printf("Existem 3 maneiras de guardar a informacao, escolha uma para ser gravado num ficheiro de texto %s\n",nomeFicheiro);
    if  (gravaMax == 3)
        puts("\n1 - Diario\n2 - Semanal\n3 - Mensal\n");
    if (gravaMax == 2){
        puts("Infelizmente nao aconteceram iteracoes suficientes para disponibilizar todos os modos, apenas dois estao desponiveis!");
        puts("\n1 - Diario\n2 - Semanal\n");
    }
    if (gravaMax != 1)
        do{
            printf("Escolha um modo: ");
            scanf(" %d", &modo);
        } while( 1 > modo > gravaMax);
    else
        puts("Infelizmente nao aconteceram iteracoes suficientes para disponibilizar todos os modos,\nA informacao foi guardada no modo Diario!!");
    FILE *f;
    f = fopen(nomeFicheiro, "wt");
    if (f == NULL){
        printf("\nErro no acesso ao ficheiro %s e falha na construção do relatorio!\n\n",nomeFicheiro);
        return;
    }
    
    for(int i=0; i<tam; i++)
        capacidade += espaco[i].capacidade + espaco[i].pessoas;
    
    
    fprintf(f, "%s \nInformacao completa sobre a simulação realizada dia %02d/%02d/%04d as %02dh%02dm \n", nomeFicheiro,diaDATA,mes,ano,hora,minuto);
    fprintf(f, "O espaco no qual decorreu a simulação tem %d locais e tem uma capacidade total de %d pessoas!\n", tam,capacidade);
    fprintf(f, "Inicialmente o espaco tinha %d pessoas distribuidas!\n\n",lista->pessoas);
    if(capacidade - lista->pessoas != validaCapacidade(espaco,tam))
        fprintf(f, "Durante a simulacao foram inseridos %d doentes na distribuicao!\n",capacidade - validaCapacidade(espaco,tam) - lista->pessoas);
    
    atual = lista;
    anterior.valido = 0;
    while(atual != NULL){
        if (atual->valido == 0)                                              //neste momento apenas informaçao valida deveria ter chegado a esta função mas usei o if pelo sim pelo nao
                corrupcao = 1;                                                  //flag que deixara uma mensagem em caso de corrupção de informação
        if (modo == 1){
            escreveInformacao(f,*lista,*atual,anterior,noDia,stringDia,&iteracoes);   
            anterior = *atual;}
        if (modo == 2 && (atual->dia)%7 == 0){
            escreveInformacao(f,*lista,*atual,anterior,naSemana,stringSemana,&iteracoes);   
            anterior = *atual;}
        if(modo == 3 && (atual->dia)%30 == 0){
            escreveInformacao(f,*lista,*atual,anterior,noMes,stringMes,&iteracoes);  
            anterior = *atual;
        }
        atual = atual->prox;
    }
    
    
    fclose(f);
}

void escreveInformacao(FILE *fich, informa inicial, informa atual, informa anterior, char* string1, char *string2, int* iteracoes){
    float doentesVd=0,saudaveisVd=0,imunesVd=0,saudaveisVp=0,doentesVp=0,imunesVp=0;
    fprintf(fich, "\n\n.---------------------------------.\n| %-9s %3d, das %4d pessoas |\n",string1,(*iteracoes)++,atual.pessoas);
    fprintf(fich, "|---------------------------------------------.\n| %4d (%5.2f%% das pessoas) estavam doentes   |\n",atual.doentes,atual.doentes/(float)atual.pessoas*100);
    fprintf(fich, "| %4d (%5.2f%% das pessoas) estavam saudaveis |\n| %4d (%5.2f%% das pessoas) estavam imunes    |\n",atual.saudaveis,atual.saudaveis/(float)atual.pessoas*100,atual.imunes,atual.imunes/(float)atual.pessoas*100);
            if (anterior.valido == 1){                                            //faz com que o programa não escreva lixo no dia 0
                fprintf(fich, "|---------------------------------------------------------------------------------------------------------------------------------------------------------------.\n");
                doentesVd = variacaoRelativa(atual.doentes,anterior.doentes);   //variação dos doentes saudaveis e imunes num dia
                doentesVp = variacaoRelativa(atual.doentes,inicial.doentes);
                saudaveisVd = variacaoRelativa(atual.saudaveis,anterior.saudaveis);
                saudaveisVp = variacaoRelativa(atual.saudaveis,inicial.saudaveis);
                imunesVd = variacaoRelativa(atual.imunes,anterior.imunes);
                imunesVp = variacaoRelativa(atual.imunes,inicial.imunes);
                fprintf(fich, "|    Informacao da variacao percentual relativamente a populacao inicial    |  Informacao da variacao percentual relativamente a populacao d%-8s anterior   |\n",string2);
                fprintf(fich, "|---------------------------------------------------------------------------|-----------------------------------------------------------------------------------|\n");
        
                if (doentesVp != 0)
                    if (doentesVp > 0)
                        fprintf(fich, "|     (AUMENTO)    %+7.2f%% em relação ao número de pessoas doentes!        |", doentesVp);
                    else
                        fprintf(fich, "|     (DECRESCIMO) %+7.2f%% em relação ao número de pessoas doentes!        |", doentesVp);
                else
                    fprintf(fich, "|                                                                           |");
                if (doentesVd != 0)
                    if (doentesVd > 0)
                        fprintf(fich, "      (AUMENTO)    %+7.2f%% em relação ao número de pessoas doentes!               |\n", doentesVd);
                    else
                        fprintf(fich, "      (DECRESCIMO) %+7.2f%% em relação ao número de pessoas doentes!               |\n", doentesVd);
                else
                    fprintf(fich, "                                                                                   |\n");
                
                
                if (saudaveisVp != 0)
                    if(saudaveisVp > 0)
                        fprintf(fich, "|     (AUMENTO)    %+7.2f%% em relação ao número de pessoas saudaveis!      |", saudaveisVp);
                    else
                        fprintf(fich, "|     (DECRESCIMO) %+7.2f%% em relação ao número de pessoas saudaveis!      |", saudaveisVp);
                else
                    fprintf(fich, "|                                                                           |");
                                if (saudaveisVd != 0)
                                    
                if(saudaveisVd > 0)
                        fprintf(fich, "      (AUMENTO)    %+7.2f%% em relação ao número de pessoas saudaveis!             |\n", saudaveisVd);
                    else
                        fprintf(fich, "      (DECRESCIMO) %+7.2f%% em relação ao número de pessoas saudaveis!             |\n", saudaveisVd);
                else
                    fprintf(fich, "                                                                                   |\n");
                
                
                if (imunesVp != 0)
                    if(imunesVp > 0)
                        fprintf(fich, "|     (AUMENTO)    %+7.2f%% em relação ao número de pessoas imunes!         |", imunesVp);
                    else
                        fprintf(fich, "|     (DECRESCIMO) %+7.2f%% em relação ao número de pessoas imunes!         |", imunesVp); // espero que nunca execute =')))))))))
                else
                    fprintf(fich, "|                                                                           |");
                
                if (imunesVd != 0)
                    if(imunesVd > 0)
                        fprintf(fich, "      (AUMENTO)    %+7.2f%% em relação ao número de pessoas imunes!                |\n", imunesVd);
                    else
                        fprintf(fich, "      (DECRESCIMO) %+7.2f%% em relação ao número de pessoas imunes!                |\n", imunesVd); // espero que nunca execute =')))))))))
                else
                    fprintf(fich, "                                                                                   |\n");
                fprintf(fich, ".---------------------------------------------------------------------------------------------------------------------------------------------------------------.\n");
            }
            else
                fprintf(fich, ".---------------------------------------------.");
}

void guardaPessoas(plocal espaco, int total_locais){
    pPessoa lista;
    char nomeFicheiro[NOME_FICHEIRO];
    puts("\nA populacao do ultimo dia sera guardada num ficheiro de texto");
    printf("Introduza o nome que deseja para o ficheiro:");
    scanf(" %49[^\n]", nomeFicheiro);
    sprintf(nomeFicheiro,"%s.txt",nomeFicheiro);
    FILE *fich;
    fich = fopen(nomeFicheiro, "wt");
    if (fich == NULL){
        printf("\nErro no acesso ao ficheiro %s e falha na construção do relatorio!\n\n",nomeFicheiro);
        return;
    }
    
    for(int i = 0; i<total_locais; i++){
        lista = espaco[i].prox;
        while(lista != NULL){
            fprintf(fich, "%-50s %3d %c", lista->nome,lista->idade,lista->estado);
            if (lista->estado == 'D')
                fprintf(fich, "   %2d\n", lista->contaDias);
            else
                fprintf(fich, "\n");
            lista = lista->prox;
        } 
    }
    
    fclose(fich);
}