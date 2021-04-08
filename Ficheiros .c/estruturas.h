/* Paul Ionut Bob - 2016013407
 * File:   estruturas.h
 * Author: droopy
 *
 * Created on 2 de Junho de 2020, 22:50
 */

#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#define NOME_FICHEIRO 50
#define NOME_PESSOA 20 // ATENÇÃO AO ALTERAR ESTE VALOR DEVE ajustar O VALOR DO PRINTF MOSTRA PESSOA DO FICHEIRO PESSOAS.C POR UMA QUESTAO DE ESTETICA E EVITAR ERROS AO MOSTRAR ALGUMAS PESSOAS!
                       //alterar na parte Nome:   %-25s  do printf 
#define MAX_LIGACOES 3
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct sala_leitura local_leitura, *plocal_leitura;                     //estrutura declarada meramente para leitura e obtenção dos locais dum ficheiro
struct sala_leitura{                                                            //criada uma vez que alterei a estrutura sala que vinha no enunciado e
 int id; // id numérico do local                                                // sizeof(local)-sizeof(pPessoa)-sizeof(int) não é muito viável
 int capacidade; // capacidade máxima
 int liga[3]; // id das ligações (-1 nos casos não usados)
};
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct cobaia pessoa, *pPessoa;                                         //estrutura pessoa
struct cobaia{
    char nome[NOME_PESSOA];                                                     // nome da pessoa                                            
    int idade;                                                                  // idade da pessoa
    char estado;                                                                // estado da pessoa S-saudavel D-doente I-imune
    int contaDias;                                                              // caso a pessoa esteja doente, há quantos dias
 
    pPessoa prox;                                                               //ponteiro para criação de listas ligadas(tem como principal objetivo apontar para outra estrutura pessoa)
};
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct sala local, *plocal;
struct sala{
    int id;                                                                     // id numérico do local
    int capacidade;                                                             // capacidade máxima
    int liga[MAX_LIGACOES];                                                                // id das ligações (-1 nos casos não usados)
    int pessoas;                                                                // quantas pessoas tem associadas
    
    pPessoa prox;                                                               //ponteiro que guarda primeira estrutura de uma lista ligada de pessoas
};
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
typedef struct informacaoDias informa, *pInforma;                    
struct informacaoDias{                                                            
 int dia;                                                                       // dia atual da simulacao
 int pessoas;                                                                   // total pessoas , pode alterar caso adicionem doentes!
 int doentes;                                                                   // doentes
 int saudaveis;                                                                 // quantos saudaveis
 int imunes;                                                                    //  quantos imunes
 int valido;                                                                    // manipula durante as partes em que recua para depois reatualizar informaçao
 pInforma prox;                                                                 // para criação de listas ligadas
};
#endif /* ESTRUTURAS_H */

