/* 
 * File:   espaco.c
 * Author: droopy
 *
 * Created on 22 de Abril de 2020, 17:43
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"
#include "espaco.h"
#include "local.h"



void mostraEspaco(plocal espaco,int total){                                     //mostra espaço
    if(total == 0){                                                             //caso o espaço não tenha locais
        puts("\nEspaco sem locais!");
        return;} 
    puts("\n-----------------------------");
    for(int i=0; i<total; i++)                                                  //caso tenha, percorre todos e mostra um a um.
        mostraLocal(espaco[i]);
} 
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int validaCapacidade(plocal espaco,int total){                                  //valida capacidade de todos os locais e retorna a capacidade total do espaço
    int cheios=0,capacidade = 0;
    for(int i=0; i<total; i++)                                                  //percorre todos os locais de um determinado espaço
        if (espaco[i].capacidade == 0)                                          //caso local não tem mais capacidade incrementa a variavel cheoios
            cheios++;
        else
            capacidade += espaco[i].capacidade;                                 //caso contrario atualiza capacidade.
    if (total == cheios){                                                       //caso todos os locais estão cheios significa que o espaço não tem mais capacidade e devolve 0
        puts("\nEspaco atual atingiu limite maximo de capacidade!");
        return 0;
    }
    return capacidade;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int validaID(plocal espaco,int total, local validacao){                         //verifica se o ID do local passado já se encontra no espaço
    for(int i=0; i<total; i++){                                                 //percorre todos os locais de um determinado espaço
        if (espaco[i].id == validacao.id){                                      // verifica se há algum local com o ID do local validacao
                    printf("\nLocal com ID repetido encontrado ( ID: %d )\n",validacao.id);
                    puts("\n---------------------------");
                    mostraLocal(espaco[i]);
                    return 0;                                                   //devolve 0 caso enontre ID igual no espaço
                }              
    }
    return 1;                                                                   //devolve 1 se não encontrou o ID
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int validaEspaco(plocal espaco,int total){                                      //valida espaço
    int ligacao = 0,ligacoes=0,verificadas=0;
    
    if (espaco == NULL){                                                        //caso nao tem locais validação falha
        puts("\nEspaco sem locais.\n");
        return 0;}

    if (total == 1){                                                            // VALIDAÇÃO MANUAL PARA O PRIMEIRO LOCAL ( para todos os outros a validação exige mais parâmetros)
        if (espaco[0].id < 1){                                                  //como é o primeiro local do espaço não pode ser validado completamente visto que não pode ter ligaçoes
            puts("\nERRO: ID negativo ou ID = 0");                              //verifica id
            printf("O id %d nao e permitido", espaco[0].id);
            return 0;}
        if (espaco[0].capacidade < 0){                                          //verifica se capacidade é positiva 
            puts("\nERRO: Capacidade negativa");
            printf("A capacidade de %d nao e valida", espaco[0].capacidade);
            return 0;}
        for(int i=0; i<MAX_LIGACOES; i++)                                       //verifica se todas as ligações estão a -1
            if(espaco[0].liga[i] != -1){
                puts("\nERRO: O primeiro local não pode ter ligacoes!");
                return 0;}
        return 1;                                                               //caso o primeiro local passe pelas validaçoes devolve 1
    }
    
    for(int i=0; i<total; i++){                                                 //caso geral: percorre todos os locais de um determinado espaço
        if (validaLocal(espaco[i]) == 0){                                       // valida cada local que percorre
            puts("Erro ao validar o local : \n");
            mostraLocal(espaco[i]);
            return 0;}
        for(int j=i+1; j<total; j++)                                            //percorre todos os locais a partir do local percorrido pelo primeiro ciclo
            if (espaco[i].id == espaco[j].id){                                  // verifica se há algum local com ID repetido
                puts("Local com ID repetido encontrado\n");
                mostraLocal(espaco[i]);
                return 0;}
        ligacoes=0;
        verificadas=0;
        for(int j=0; j<MAX_LIGACOES; j++)                                       // percorre todas as ligacoes
            if (espaco[i].liga[j] != -1){                                       //caso exista ligação
                ligacoes++;
                for(int k=0; k<total; k++)                                      //percorre todos os locais a procura da ID da ligação
                    if(espaco[i].liga[j] == espaco[k].id)                       //encontrou ID do local
                        for(int m=0; m<MAX_LIGACOES; m++)                       //percorre todas as ligações do local encontrado
                            if(espaco[k].liga[m] != -1 &&
                               espaco[k].liga[m] == espaco[i].id)               //caso seja diferente de -1 e tem o ID do local i
                                verificadas++;}
        
        if (ligacoes != verificadas){                                           // caso falhe na verificação de alguma ligação mostra onde falhou e devolve 0
            puts("\nEste local nao verifica todas as ligacoes");
            mostraLocal(espaco[i]);
            return 0;}
    }
    return 1;
} 
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int ligacaoPossivel(plocal espaco,int total, local localtemp){                  //verifica se um local preenchido consegue fazer as ligações pretendidas
    int ligacoes = 0, quantas=0, j=0, encontrou = 0;

    for(int i=0; i<MAX_LIGACOES; i++){                                          //percorre toodas as ligações do local recebido
        ligacoes=0;
        if (localtemp.liga[i] != -1)                                            //caso a ligação está preenchida
            quantas++;                                                          //quantas ligaçoes estao preenchidas no local recebido
            for(j=0; j<total; j++)                                              //percorre todos os locais a procura do ID
                if (localtemp.liga[i] == espaco[j].id){                         //caso encontre o ID
                    encontrou++;                                                //quantas das ligaçoes pertencem a locais validos do espaço
                    for(int k=0; k<MAX_LIGACOES; k++)                           //percorre todas as ligaçoes do ID em causa
                        if(espaco[j].liga[k] != -1)                             //verifica se estão vazias e caso contrario incrementa var ligações
                            ligacoes++;
                    if (ligacoes == 3){                                         //caso o ID ao qual pretendemos ligar o nosso local recebido esteja cheio devolve 0
                        printf("O local ID: %d ja nao pode ter mais ligacoes\n",espaco[j].id);
                        return 0;}
                }
        if (quantas != encontrou){                                              //caso percorra todos os locais e não encontrou o ID de alguma ligação devolve 0
            printf("ID %d nao encontrado no espaco atual!",localtemp.liga[i]);
            return 0;}
    }
    return 1;                                                                   //caso as ligaçoes do localtemp sejam possiveis de efetuar devolve 1
} 
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
plocal adicionaLocal(plocal espaco, int* total){                                //esta funcionalidade não era pedida no enunciado mas já a tinha implementado antes de me aperceber disso
    local localtemp;                                                            //algumas partes de repetição de código poderiam ser melhoradas e dividida por varias funçoes que seriam utilizadas
    plocal novo_espaco;                                                         //também pela eliminaLocal
    int alterou=0, j;
    
    if(*total>0){                                                               //caso geral para todos os locais excepto o primeiro                                                       
        if(preencheLocal(&localtemp) == 0)                                      //preenche local, devolve espaco inalterado caso falhe
            return espaco;

        if (validaID(espaco,*total,localtemp) != 1)                             //verifica se o ID do local preenchido já se encontra no espaço, devolve espaço inalterado caso isso suceda
            return espaco;

        if(ligacaoPossivel(espaco,*total,localtemp) != 1)                       //verifica se é possivel fazer as ligaçoes indicadas no preenchimento do local
            return espaco;                                                      //devolve espao inalterado caso nao seja possivel

        for(int i=0; i<MAX_LIGACOES; i++)                                       //percorre todas as ligaçoes do local preenchido
            if (localtemp.liga[i] != -1){                                       //caso a ligação tenha algum ID especificado
                alterou = 0;                                                    //variavel auxiliar que indica caso tenham sido feitas as ligações
                for(j=0; j<*total; j++)                                         //percorre todos os locais do espaço atual
                    if (localtemp.liga[i] == espaco[j].id)                      //quando a ligação encontra o ID do local no espaço atual
                        for(int k=0; k<MAX_LIGACOES; k++)                       //percorre todas as ligaçoes do local que tem o ID da ligação do local preenchido
                            if (espaco[j].liga[k] == -1){                       //procura por ligação não preenchida
                                espaco[j].liga[k] = localtemp.id;               //coloca o ID do local preenchido na ligação do local encontrado com o ID da ligação do local preenchido
                                alterou = 1;                                    //flag indica que foi feita a ligação
                                break;                                          //salta fora
                            }
                if (alterou == 0 && i == 0){                                    
                    printf("\nERRO: Impossivel criar ligacao %d, verifique se o ID %d existe\n",i+1,localtemp.liga[i]);
                    localtemp.liga[i] = -1;
                    return espaco;
                }
                else if (alterou == 0){
                    printf("\nAVISO: Impossivel criar ligacao %d, verifique se o ID %d existe\n",i+1,localtemp.liga[i]);
                    localtemp.liga[i] = -1;
                }
            }   
    }
 
    else if(*total == 0){                                                       //Preenchimento manual para o caso de ser o primeiro local a ser adicionado. 
            printf("\nID: ");                                                   //(escusa de passar pelas verificações e dar erros visto que o primeiro local não verifica ligaçoes apenas id e capacidade
            if (!scanf(" %d", &localtemp.id)){
                puts("\nERRO: erro de leitura ao introduzir o ID");
            return 0;
            }
            
            printf("Capacidade: ");
            if (!scanf(" %d", &localtemp.capacidade)){
                 puts("\nERRO: erro de leitura ao introduzir a Capacidade");
            return 0;
            }
            localtemp.pessoas = 0; // ao preencher um local e colocar no espaço ele entra sem distribuição
            for(int i=0; i<MAX_LIGACOES; i++)
                localtemp.liga[i]=-1;
            
            localtemp.prox = NULL;
    }
    
    novo_espaco = realloc(espaco,(*total+1) * sizeof(local));                   //casso o local foi preenchido coretamente e as ligações foram verificadas e feitas com sucesso realoca o array espaco
    if (novo_espaco == NULL){                                                   //guarda o ponteiro da realocação no novo_espaco para o caso de correr mal manter o pontreio espaco a apontar para o array
        puts("Erro a reservar memroia");
        return espaco;
    }
    novo_espaco[*total] = localtemp;                                            //coloca o local preenchido em ultima posição e incrementa o total.
    (*total)++;
    
    if (validaEspaco(novo_espaco,*total) == 0){                                 //caso a validação do espaço corra mal informa o utilizador mas devolve o ponteiro novo_espaco na mesma visto que
        printf("Possivel corropcao do espaco\n");                               //se chegou até aqui já existiram algomas modificações nas ligações dos outros locais
        return novo_espaco;
    }
    
    puts("\nLocal adicionado com sucesso!"); 
    return novo_espaco;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
plocal eliminaLocal(plocal espaco, int* total){                                 // da erro ao eliminar espaco base ( espaco id 3 na pp 1)
    int id,i,conta;
    local temp,temp2;
    plocal aux;
    
    if(*total == 0){                                                            //se o total está a 0 não há nenhum local no espaço logo devolve NULL
        printf("\nNao existem locais para eliminar no espaco atual");
        return NULL;}
    
    printf("\nID do local a eliminar: ");                                       //pede Id do local a eliminar
    scanf(" %d", &id);
    
    for(i=0; i<*total && espaco[i].id != id; i++);                              //incrementa o i(posição) até encontrar o id indicado pelo utilizador
    
    if(i == *total){                                                            //se chegou a ultima posição do array e não encontrou Id significa que não existe
        puts("Local inexistente!");
        return espaco;}
    else if(*total == 1){                                                       //caso encontrou o ID e o total de locais no espaço é 1 signifca que vamos eliminar o unico local
        free(espaco);
        *total=0;
        return NULL;}
    else{                                                                       //para o caso geral
        temp = espaco[i];                                                       //variavel temp guarda o local a eliminar
        espaco[i] = espaco[*total-1];                                           //posição no array do local a ser eliminado fica preenchida pela ultima posição do array
        aux = realloc(espaco, sizeof(local) * (*total-1));                      //aux guarda a realocação do espaço para o caso de correr mal-
        if (aux!=NULL){                                                         //se correu bem entao atribuimos o ponteiro ao espaco, decrementamos o total, significa que o local foi eliminado
            espaco = aux;
            (*total)--;}
        else{
            puts("Erro de realocacao!");
            espaco[i] = temp;
            return espaco;}
    }

    for(int j=0; j<MAX_LIGACOES; j++){                                          //percorre todas as ligações do local que elimnamos e o guardamos na variavel temp
        conta = 0;
        if (temp.liga[j] != -1)                                                 //caso exista ligação
            for(int m=0; m<*total; m++)                                         //percorre todos os restantes locais 
                if (temp.liga[j] == espaco[m].id)                               //caso a ligaçao existente é igual do id do local atual
                    for(int n=0; n<MAX_LIGACOES; n++)                           //percorre todas as ligações do espaço que t inha ligaçoes com o eliminado
                        if (temp.id == espaco[m].liga[n]){                      // caso encontre o id do local a eliminar nas ligacoes do local atual
                            espaco[m].liga[n] = -1;                             // elimina a ligação
                            for(int o=0; o<MAX_LIGACOES; o++)                   // percorre todas as ligações do local atual para verificar se o local ficou sem ligações
                                if (espaco[m].liga[o] == -1)                    // por cada NÃO LIGAÇÃO aumenta o contador
                                    conta++;                                    //("O espaço em que decorre a simulação é constituído por um conjunto de locais interligadosentre si")
                            if (conta == 3){                                    // caso o local não tenha nenhuma ligação apaga o local 
                                if (*total == 1){                               // caso durante o ciclo foram se eliminando locais ate sobrar 1 o espaco fica sem ligaçoes entre locais logo deixa de existir espaço.
                                    puts("O ficou sem locais!\n"
                                         "Adicione um local e siga os passos indicados para criar um novo espaco!");
                                    free(espaco);                               
                                    *total=0;
                                    return NULL;}
                                temp2 = espaco[m];                              //caso o local ficou sem ligaçoes e existem mais do que 1 local no espaço entao ocorre outra eliminação.
                                espaco[m] = espaco[*total-1];
                                aux = realloc(espaco, sizeof(local) * (*total-1));
                                if (aux!=NULL){                                 //esta funcionalidade não era pedida no enunciado mas já a tinha implementado antes de me aperceber disso
                                    espaco = aux;                               //algumas partes de repetição de código poderiam ser melhoradas.
                                    (*total)--;}
                                else{
                                    puts("Erro de realocacao!");
                                    espaco[m] = temp;
                                    return espaco;}                      
                            }
                        }
    }
    
    organizaLigacoes(espaco,*total);                                            //reorganiza a ordem das ligações de todos os locais restantes.
    if(validaEspaco(espaco,*total)==1)                                          //indica ao utilizador se o espaço ficou valido depois de eliminar o local.
        printf("Local eliminado com os devidos ajustes.");
    else
        printf("ERRO: algo inesperado aconteceu!");
    return espaco;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
