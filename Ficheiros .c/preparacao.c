/* Paul Ionut Bob - 2016013407
 * File:   preparacao.c
 * Author: droopy
 *
 * Created on 15 de Maio de 2020, 21:40
 */

#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"
#include "preparacao.h"
#include "auxiliar.h"
#include "espaco.h"
#include "local.h"
#include "populacao.h"
#include "pessoas.h"
#include "simulacao.h"


plocal preparaEspaco(plocal espaco,int* total,char* nome){
    plocal novo_espaco = NULL;
    pessoa lida;
    local quantos;
    int novo_total=0;
    char casoFalhe[NOME_FICHEIRO];
    
    strcpySegura(nome,casoFalhe,NOME_FICHEIRO,NOME_FICHEIRO);                  // guarda o nome do espaco anterior para o caso de o utilizador indicar um ficheiro invalido
    printf("Introduza o nome do ficheiro que tem o espaco pretendido\nex:\nPara o ficheiro: E1.bin\nIntroduzir:      E1 ou e1\n...\nFicheiro: ");
    scanf(" %49[^\n]", nome);
    sprintf(nome,"%s.bin",nome);
  
    FILE *f;
    f = fopen(nome, "rb");
    if (f == NULL){
        printf("\nErro no acesso ao ficheiro %s\n\n",nome);
        strcpySegura(casoFalhe,nome,NOME_FICHEIRO,NOME_FICHEIRO);              // caso ficheiro invalido volta a colocar o nome do espaco original.
        return espaco;
    }

    fseek(f, 0, SEEK_END);                                                      //desloca ponteiro para o final do ficheiro
    novo_total = ftell(f) / sizeof(local_leitura);                              //divide o nr total de bytes pelo tamanho de uma estrutura local_leitura
    
    novo_espaco = malloc(novo_total * sizeof(local));                           //Aloca espaço em memoria para o total de estruturas encontradas (aloca para o tamanho da estrutura modificada)
    if (novo_espaco == NULL){                                                   //caso alocação falha
        fprintf(stderr,"\nErro na alocacao de memoria!\n");
        fclose(f);                                                              //devolve espaco anterior
        strcpySegura(casoFalhe,nome,NOME_FICHEIRO,NOME_FICHEIRO);
        return espaco;
    }
    
    fseek(f, 0, SEEK_SET);                                                      //Desloca o ponteiro para ficheiro para o inicio do ficheiro
    for(int i=0; i<novo_total; i++){                                            //Transcreve as estruturas do ficheiro para o vetor de estruturas dinamicas novo_espaco
        if (fread(novo_espaco+i, sizeof(local_leitura), 1, f) != 1){ //Caso falhe na leitura
            fprintf(stderr,"Erro no formato do ficheiro [%s]\n",nome);
            puts("Espaco anterior mantido.");
            free(novo_espaco);
            fclose(f);
            strcpySegura(casoFalhe,nome,NOME_FICHEIRO,NOME_FICHEIRO);          //coloca o nomne do espaco original
            return espaco;                                                      //retorna o espaco original ( NULL caso seja a primeira abertura )
        }
        (novo_espaco+i)->prox = NULL;                                           //coloca o campo que guarda as pessoas distribuidas a NULL
        (novo_espaco+i)->pessoas = 0;                                           //coloca a variavel das pessoas penduradas a 0
    }

    fclose(f);
    
    if (validaEspaco(novo_espaco,novo_total) == 1){                             //caso passe pela validação
        puts("\nEspaco criado com sucesso!\n");
        free(espaco);                                                           //liberta espaço original
        *total=novo_total;                                                      //atualiza variavel total
        return novo_espaco;                                                     //devolve novo_espaco
    }
    
    free(novo_espaco);
    strcpySegura(casoFalhe,nome,NOME_FICHEIRO,NOME_FICHEIRO);                  //caso contrario coloca o nomne do espaco original e devolve o mesmo.
    return espaco;                                                              
} 
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pPessoa preparaPopulacao(pPessoa populacao, char *nome){
    pPessoa novaPopulacao = NULL;
    pessoa nova;
    char casoFalhe[NOME_FICHEIRO];
    int conta=0;
    
    strcpySegura(nome,casoFalhe,NOME_FICHEIRO,NOME_FICHEIRO);                  // guarda o nome da populacao anterior para o caso de o utilizador indicar um ficheiro invalido
    printf("Introduza o nome do ficheiro que tem a populacao pretendida\nex:\nPara o ficheiro: pessoasA.txt\nIntroduizr:      pessoasA ou pessoasa\n...\nFicheiro: ");
    scanf(" %49[^\n]", nome);

    sprintf(nome,"%s.txt",nome);

    FILE *f=fopen(nome,"rt");
    if (!f) {
        fprintf(stderr,"\nErro a abrir o ficheiro %s\n\n",nome);
        strcpySegura(casoFalhe,nome,NOME_FICHEIRO,NOME_FICHEIRO);              // caso ficheiro invalido volta a colocar o nome da populacao original.
        return populacao;
    }
    
    while(fscanf(f," %s %d %c ", nova.nome, &nova.idade, &nova.estado) == 3){   //enquanto consegue ler nome idade estado
        conta++;                                                                // incrementa variavel que conta o numero da linha
        nova.contaDias = -1;                                                    // inicializa todas as pessoas como não doentes e com o contaDias a -1
        if (nova.estado == 'D')                                                 // caso esteja doente entao 
                if (fscanf(f," %d", &nova.contaDias) == 0){                     // atualiza o contaDias , mas caso falhe
                    fprintf(stderr,"Erro no formato do ficheiro [%s] na linha [%d]\n",nome,conta);
                    puts("Populacao anterior mantida.");
                    fclose(f);
                    novaPopulacao = libertaLista(novaPopulacao);
                    strcpySegura(casoFalhe,nome,NOME_FICHEIRO,NOME_FICHEIRO);  //coloca o nomne da populacao original
                    return populacao;                                           //retorna a populacao original ( NULL caso seja a primeira abertura )
                }
                else{
                    if (nova.estado == 'D' && nova.contaDias > duracaoMax(nova.idade)){ //caso esteja doente mas o nr de dias excedeu o considerado maximo para esta aplicação decidi simplesmente "curar" a pessoa
                        printf("\n%s , doente ha %d dias excede o numero maximo (%d dias) que uma pessoa com %d anos pode ficar doente nesta simualacao.\n"
                                "%s esta automaticamente Saudavel!\n",nova.nome,nova.contaDias,duracaoMax(nova.idade),nova.idade,nova.nome);
                        nova.estado = 'S';
                        nova.contaDias = -1;}                                   //neste caso não considero adequado aplicar a imunidade.
                }
        if (verificaPessoa(novaPopulacao,nova) == 1)                            // se a pessoa passa na verificação
            novaPopulacao = inserePessoaFinal(novaPopulacao,nova,1);                 //entao é inseirda no final da população
    }
    
    if(!feof(f)){                                                               // caso saia do ciclo while antes do final do ficheiro algo correu mal 
        fprintf(stderr,"Erro no formato do ficheiro [%s]\n",nome);
        puts("Populacao anterior mantida.");
        fclose(f);
        novaPopulacao = libertaLista(novaPopulacao);
        strcpySegura(casoFalhe,nome,NOME_FICHEIRO,NOME_FICHEIRO);
        return populacao;
    }
    
    populacao = libertaLista(populacao);                                                            //se chega aqui correu tudo bem entao liberta a população original e retorna a novaPopulação
    fclose(f);
    puts("\nPopulacao criada com sucesso!\n");
    return novaPopulacao;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pPessoa preparaDistribuicao(plocal espaco, int total, pPessoa populacao, int* distribuidas){ 
    int i=-1 ,pessoas_fila = 0, respostaDist = 0;
    
    if (espaco == NULL){                                                        //verifica se existem locais no espaço
        puts("\nImpossivel distribuir (espaco sem locais)");
        return populacao;}
    if (populacao == NULL){                                                     //verifica se existem pessoas na população/"fila"
        puts("\nImpossivel distribuir (populacao sem pessoas)");
        return populacao;}
    
    if (*distribuidas != 0){                                                    //caso já exista uma distribuição peço espaço    
        puts("\nJa existe uma distribuicao!\n"
             "Ao avancar ira misturar as pessoas da fila com as pessoas ja distribuidas!\n"
             "Deseja continuar?!"
             "\n 1 - Sim!\n 2 - Nao!\n 3 - Talvez!");
        do{                                                                     //ciclo enquanto não escolher 1 2 ou 3
            scanf(" %d", &respostaDist);
        } while( respostaDist < 1 || respostaDist > 3);
        
        if(respostaDist == 3)                                                   //brincadeira ='))
        puts("Hmmm... Decida-se e volte mais tarde! ='))");
    }
    
    if(respostaDist != 2 && respostaDist != 3){                                 //caso a resposta anterior não seja nem 2 nem 3 entao aplica a distribuição
        while(populacao != NULL){                                               // enquanto existe pessoas na população / 'fila'
            if (validaCapacidade(espaco,total) == 0){                           // verifica capacidade do espaco antes de adicionar cada pessoa
                if (i != -1)                                                    // caso já tenha sido distribuida alguma pessoa
                    puts("\nDistribuicao feita com sucesso!");
                else
                    puts("Nao foi feita nenhuma distribuicao!");
                return populacao;                                               //retorna população. (MENSAGENS DE ERRO NA FUNÇÃO validaCapacidade)
            }

            i = intUniformRnd(0,total-1);                                       //numero aleatorio entre 0 e total-1 (para depois aceder ao array do espaço
            if (((espaco+i)->capacidade) > 0){                                  //caso o local devolvido aleatoriamente tenha capacidade
                (espaco+i)->prox = inserePessoaFinal((espaco+i)->prox,*populacao,1); //inserimos no final da lista ligada associada ao campo prox desse local
                ((espaco+i)->capacidade) --;                                    //atualizo capacidade
                ((espaco+i)->pessoas) ++;                                       //incremento a variavel que guarda o numero de pessoas desse local.
                populacao = eliminaPessoa(populacao,populacao->nome);           // elimina pessoa da fila
                (*distribuidas)++;                                              //incrementa variavel que guarda quantas pessoas estão distribuidas
            }
        }
    puts("\nDistribuicao feita com sucesso!");
    }
    return populacao;                                                           
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void mostraDistribuicao(plocal espaco, int total){
    pPessoa aux = NULL;
    int flag = 0,pos = 0;

    for(int i=0; i<total; i++){                                                 //percorre todos os locais
        pos=1;
        if (espaco[i].prox != NULL){                                            //caso exista pessoas associadas ao local
            aux = espaco[i].prox;                                               //aux toma ponteiro para primeira pessoa do local
            puts("----------");
            puts("No local |");
            puts("-----------------------------");
            mostraLocal(espaco[i]);
            puts("|             |             |");
            puts("-----------------------------");
            puts("Estao as seguintes pessoas  |");
            //puts("--------------- ");
            puts("----------------------------------------------------------------------------------------");
            while(aux != NULL){                                                 //enquanto ha pessoas no local mostra pessoas e atualiza ponteiro
                  
            //    printf("Posicao: %3d  |",pos);
            //    puts("\n----------------------------------------------------------------------------------------");
                mostraPessoa(*aux);
                if (aux->estado == 'D'){
                    printf("                                  |                |  Duracao maxima da doenca: %-3ddias|\n",duracaoMax(aux->idade));
                    puts("----------------------------------------------------------------------------------------");}

                aux = aux->prox;
                pos++;
            }     
            flag = 1;                                                           //caso algum local tenha pessoas associadas altera flag
            puts("\n");
        } 
    }
    
    if (flag == 0)                                                              //caso flag não foi alterada entao não foi feita nenhuma distribuição ainda.
        puts("\nAinda nao foi feita a distribuicao");
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void apagaDistribuicao(plocal espaco, int total, int* distribuidas){            //faz o que diz, elimina as pessoas da distribuição
    for(int i=0; i<total; i++){                                                 //percorre todos os locais e liberta as alocações de memoria
        espaco[i].prox=libertaLista(espaco[i].prox);
        espaco[i].capacidade += espaco[i].pessoas;                              //faz os devides ajustes na capacidade e no nr de pessoas associadas.
        espaco[i].pessoas = 0;}
    
    if ((*distribuidas) != 0)                                                   //ao ser chamada pela anulaDistribuição ou pelo aplicaModelo não quero que mostre esta mensagem.
        puts("\nDistribuicao eliminada com Sucesso!");
    *distribuidas=0;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pPessoa anulaDistribuicao(pPessoa populacao, plocal espaco, int total, int* distribuidas){
    pPessoa aux = NULL;
    
    if (*distribuidas == 0){
        puts("\nNao existe nenhuma pessoa distribuida pelo espaco!");
        return populacao;}
    
    for(int i=0; i<total; i++){                                                 //percorre todos os locais e adiciona as pessoas ao final da fila!
        aux = espaco[i].prox;   
        while( aux != NULL ){
            populacao = inserePessoaFinal(populacao,*aux,1);                         // aloca memoria e insere no final da fila
            aux = aux->prox;
            (*distribuidas)--;
        }
    }

    if (*distribuidas == 0)
        puts("\nPessoas voltaram para a \"fila\" com sucesoo!");
    else
        puts("\nERRO inesperado sucedeu!");
    
    apagaDistribuicao(espaco,total,distribuidas);                              // apaga a memoria alocada antes de vir para a função (podia simpelsmente só transferir mas achei mais simples e rapido utilizar as funçoes já implementadas)
    return populacao;
    
    
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void informacaoEspacoPopulacao(plocal espaco, pPessoa populacao, int total_locais, int pessoas_distribuidas, char* nomeE, char* nomeP ){
    int pessoas_fila=0,total_pessoas=0;                                         //escreve informações sobre o espaço e sobre a opulação
    
    if(espaco != NULL)
        printf("\nO espaco atual foi obtido do ficheiro \"%s\"\n"
                "Existem %d locais no espaco %s\n"
                "Atualmente o espaco tem capacidade para %d pessoas\n\n"
                ,nomeE,total_locais,nomeE,validaCapacidade(espaco,total_locais));
    else
        puts("Nenhum espaco carregado com sucesso para a simulacao!\n");
  
    if(populacao != NULL || (populacao == NULL && pessoas_distribuidas>0)){
        contaPessoas(populacao,&pessoas_fila);
        printf("A populacao atual foi obtida do ficheiro \"%s\"\n"
                "Existem %d pessoas na populacao %s\n\n"
                "%d Pessoas estao distribuidas\n"
                "%d Pessoas estao na \"fila\"\n"
                ,nomeP,total_pessoas=pessoas_fila+pessoas_distribuidas,nomeP,pessoas_distribuidas,pessoas_fila);}
    else
        puts("Nenhuma populacao carregada com sucesso para a simulacao!\n");
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
plocal copiaEspaco(plocal espaco, int total_locais,plocal destino){                            //faz copia do espaco atual para outro vetor independente devolve o outro vetor
    plocal novo_espaco = destino;
    pPessoa aux = NULL;
    int auxiliar = 0;                                                           //variavel para enviar ao apagaDistribuição
    
    if (espaco == NULL)                                                         //caso tente copiar NULL devolve logo NULL
        return NULL;
                                                            //printf("na copia entra: %p\n",destino);
    if (novo_espaco != NULL){
        apagaDistribuicao(novo_espaco,total_locais,&auxiliar);                  //elibera a memoria caso tenha algo preenchido
        free(novo_espaco);
        novo_espaco = NULL;}
        
    
    novo_espaco = malloc(total_locais * sizeof(local));                         //aloca espaço
    if (novo_espaco == NULL){
        puts("Erro a reservar memoria para o espaco(copia) da experiencia");
        return NULL;
    }
                                                                                //printf("na copia: %p\n",novo_espaco);
    for(int i=0; i<total_locais; i++){                                          //copia cada local um a um do espaco para o novo espaco até chegar ao fim, então devolve o novo_espaco
        novo_espaco[i]=espaco[i];
        novo_espaco[i].prox = NULL;                                             //coloca campo prox a null para fazer alocaçõs de memoria separadas para a distribuição
        aux = espaco[i].prox;
        while(aux != NULL){
            novo_espaco[i].prox = inserePessoaFinal(novo_espaco[i].prox,*aux,1);//vai preenchendo a distribuiçao com alocações de memória separadas.
            aux = aux->prox;
        }
    }
                            //printf("na copia sai: %p\n",novo_espaco);
    return novo_espaco;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/