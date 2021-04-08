/* Paul Ionut Bob - 2016013407
 * File:   simulacao.c
 * Author: droopy
 *
 * Created on 25 de Maio de 2020, 18:55
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturas.h"
#include "simulacao.h"
#include "auxiliar.h"
#include "relatorio.h"
#include "populacao.h"
#include "espaco.h"
#include "pessoas.h"
#include "preparacao.h"

#define TAXA_IMUNIDADE 0.2                                                      // A taxa de imunidade é de 20% caso a pessoa fique curada.
#define TAXA_DISSEMINACAO 0.05                                                  // Cada doente infeta 5% das pessoas num determinado local.
#define DIVIDENDO_RECUPERACAO 1                                                 // Probabilidade de recuperação = DIVIDENDO_RECUPERACAO/idade
#define TERMO_DURACAO 5                                                         // duração máxima = 5 + 1 dia por cada dezena de anos de vida(arredondando para baixo)
#define DURACAO_CONDICAO 10                                                     // + 1 dia por cada dezena de anos de vida 
//decidi colocar os ultimos tres defines caso for preciso trocar os valores bases (apesar das probabilidades estarem dependentes de mais termos)
//podem não ser utilizados mas estou a previnir caso peçam alteraçoes a este nivel na defesa do trabalho.

int imunidade(){                                                                // esta e algumas das proximas funçoes não são propriamente necessarias mas facilitam na perceção do código
    return probEvento(TAXA_IMUNIDADE);                                          // devolve 1 caso a pessoa fique imune
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int duracaoMax(int idade){                                                     // devolve duração maxima que uma pessoa pode ficar doente (em dias)
    return TERMO_DURACAO + ((idade - (idade%DURACAO_CONDICAO)) / DURACAO_CONDICAO); 
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int recupera(int idade){                                                        //devolve 1 caso o paciente ficou curado
    return probEvento(DIVIDENDO_RECUPERACAO/(float)idade);
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int disseminacaoLocal(int pessoas){                                            //devolve quantas pessoas serão infetadas por cada doente de um determinado local
    return (float)pessoas*TAXA_DISSEMINACAO;  
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
pInforma aplicaModelo(plocal espaco, int total_locais, int* dia, plocal voltaUmDia, plocal voltaDoisDias, plocal voltaTresDias, int* doentesT, pInforma informacao){ //aplica recuperação e dissiminação no espaço e avança um dia 
    pPessoa atual = NULL, infetado = NULL;
    informa proximaInformacao;
    pInforma atualInforma = NULL, anteriorInforma = NULL;
    int infeta=0;                                                               //variavel que guarda o valor de quantas pessoas serão infetadas por cada doente de um determinado local
    int quem=0;                                                                 // variavel que guarda a posição em lista ligada da pessoa a ser infetada
    int pos=0;                                                                  // variavel que guarda a posição em lista ligada da pessoa que esta a transmitir
    //int recuperou=0;                                                          //variavel que indica se um doente recuperou ou não
    int curados=0,infetados=0,imunes=0,pessoas = 0;
    *doentesT=0;
    
    for(int i=0; i<total_locais; i++){
        atual = espaco[i].prox;
        pos=1;
        infeta = disseminacaoLocal(espaco[i].pessoas);
        while(atual != NULL){
            //recuperou=0;
            if (atual->estado == 'D'){                                          //escolhe as pessoas doentes da fila
                (*doentesT)++;
                (atual->contaDias)++;
                if (atual->contaDias > duracaoMax(atual->idade) || recupera(atual->idade)){ //caso o doente recupera
                    if (imunidade()){                                           // caso fique imune
                        atual->estado = 'I';
                        atual->contaDias = -1;
                        imunes++;}
                    else{                                                       //se não imune, fica saudavel
                        atual->estado = 'S';
                        atual->contaDias = -2;}                                 //-2 para quando for aplicar a disseminação não ter possibilidade de adoecer no mesmo dia que ficou curado.
                    curados++;
                }
                else{                                                           //caso não recupera, infeta outras pessoas.
                    for(int j=0; j<infeta; j++){
                        infetado = espaco[i].prox;                              //coloca a primeira pessoa da lista ligada na variavel infetado
                        do{
                            quem = intUniformRnd(1,espaco[j].pessoas);          // aplica um numero aleatorio a variavel quem enquanto ela não for diferente da posição atual ( pessoa nao pode auto-infetar )
                        }while(quem == pos); 
                        for(int l=1; l<quem; l++)                               //ciclo para chegar a posição indicada pela variavel quem
                            infetado = infetado->prox;
                        if (infetado->estado == 'S' && infetado->contaDias != -2){//apenas pessoas saudaveis e que não recuperaram nesta iteração podem ser contaminadas;
                            infetado->estado = 'D';
                            infetado->contaDias = 0;                              
                            infetados++;}
                    }
                }
            }
            atual = atual->prox;
        }
        
        atual = espaco[i].prox;                                                 //atual volta a apontar para o inicio da lista depois de aplicar o modelo a todas as pessoas do local.
        while(atual != NULL){                                                   //percorre outra vez a lista e coloca as pessoas curadas com o conta dias a -1
            if(atual->contaDias == -2)
                atual->contaDias = -1;
            atual = atual->prox;
            pessoas++;
        }
    }
    (*dia)++;
    if(dia>0){
        atualInforma = informacao;
        while(atualInforma != NULL){                                            //guarda a ultima informaca
            anteriorInforma = atualInforma;
            atualInforma = atualInforma->prox;
        }
        proximaInformacao.dia = *dia;
        proximaInformacao.doentes = anteriorInforma->doentes + infetados - curados;
        proximaInformacao.imunes = anteriorInforma->imunes + imunes;
        proximaInformacao.saudaveis = anteriorInforma->saudaveis - infetados + curados - imunes;
        proximaInformacao.pessoas = pessoas;
        proximaInformacao.valido = 1;
        informacao = insereInformacao(informacao,proximaInformacao);
    }
    
    puts("\n-------------------------------------------------------");
    printf("    Ao avancar para o dia %3d                         |\n",*dia);
    printf("%3d pessoas recuperaram das quais %3d ficaram imunes. |\n",curados,imunes);
    printf("%3d pessoas apanharam o virus.                        |\n",infetados);
    puts("-------------------------------------------------------");
    
    return informacao;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

informa informacaoSimulacao(plocal espaco, int total_locais, int pessoas_distribuidas, int dia, int *doentesT, plocal diaAnterior, int podeRecuar, informa primeira){ //escreve algumas informações sobre o estado atual da simulação
    int menorIdade=0, maiorIdade=0, doentes=0,mediaIdades=0, mediaIdadesDoentes=0,total=0,saudavel=0,imune=0,soma=0,idade_total = 0, doentesOntem = 0, saudaveisOntem = 0, imunesOntem = 0;
    float percentagemDoentes=0, percentagemSaudaveis=0, percentagemImunes=0, percentagemSubidaDescidaDoentes=-100, percentagemSubidaDescidaSaudaveis=-100 ,percentagemSubidaDescidaImunes=-100;
    pPessoa pessoas = NULL, ontem = NULL;  

    for(int i=0; i<total_locais; i++){                                          // percorre todos os locais
        pessoas = espaco[i].prox;
        if (podeRecuar > 0)
            ontem   = diaAnterior[i].prox;
        while(pessoas != NULL){                                                 // enquanto houver pessoas no local
            if (pessoas->estado == 'D'){
                if(menorIdade == 0)
                    menorIdade = pessoas->idade;                                //inicializa menor com a idade do primeiro doente.
                doentes++;
                if (pessoas->idade > maiorIdade)                                //caso o doente lido seja mais velho do que a maiorIdade registada até ao momento
                    maiorIdade = pessoas->idade;
                else if (pessoas->idade < menorIdade)                           //caso o doente lido seja mais novo do que a menorIdade registada até ao momento
                    menorIdade = pessoas->idade;
                soma += pessoas->idade;
                }
            else if(pessoas->estado == 'S')                                     // caso saudavel incrementa o contador saudavel
                saudavel++;
            else                                                                // caso não saudavel nem doente incrementa o contador imune
                imune++;
            if (ontem != NULL && ontem->estado == 'D')
                doentesOntem ++;
            else if (ontem != NULL && ontem->estado == 'S')
                saudaveisOntem ++;
            else if (ontem != NULL && ontem->estado == 'I')
                imunesOntem ++;
        idade_total += pessoas->idade;    
        pessoas = pessoas->prox;
        if (ontem != NULL)
            ontem   = ontem->prox;
        }
    }
    total=doentes+saudavel+imune;
    percentagemDoentes   = (float)doentes  /total*100;                          // calcula media de doentes 
    if(podeRecuar > 0){
        percentagemSubidaDescidaDoentes   += (float)doentes / (float)doentesOntem * 100;
        percentagemSubidaDescidaSaudaveis += (float)saudavel / (float)saudaveisOntem * 100;
        percentagemSubidaDescidaImunes    += (float)imune / (float)imunesOntem * 100;}
    percentagemSaudaveis = (float)saudavel/total*100;                           //                   saudaveis 
    percentagemImunes    = (float)imune   /total*100;                           //                   imunes 
    mediaIdadesDoentes   = soma/(float)doentes;                                 //                   idades dos doentes
    mediaIdades          = idade_total/(float)total;                            //                   idades das pessoas distribuidas

    puts("\n\n-------------------");
    printf("Dia atual: %3d    |\n",dia);
    puts("--------------------------------------------------------------------");
    printf("Existem %5d pessoas distribuidas pelos %3d locais.               |\n",pessoas_distribuidas,total_locais);
    printf("A media de idades das pessoas distribuidas 'e de %3d anos.         |\n",mediaIdades);
    puts("--------------------------------------------------------------------");
    printf("Atualmente existem %5d doentes                                   |\n",doentes);
    if (doentes > 1){
        printf("O doente mais novo tem %3d anos e o mais velho tem %3d anos.       |\n",menorIdade,maiorIdade);
        printf("A media de idades dos doentes 'e de %d anos.                       |\n",mediaIdadesDoentes);}
    else if (doentes == 1){
        printf("O doente tem %3d anos                                          |\n",menorIdade);}
    puts("--------------------------------------------------------------------");
    printf("%6.2f%% das pessoas distribuidas estao Doentes    |  %3d Doentes   |\n",percentagemDoentes,doentes); //pergentagemns com erros de arredondamentos minimos as vezes.
    printf("%6.2f%% das pessoas distribuidas estao Saudaveis  |  %3d Saudaveis |\n",percentagemSaudaveis,saudavel);
    printf("%6.2f%% das pessoas distribuidas sao Imunes       |  %3d Imunes    |\n",percentagemImunes,imune);
    
    if (podeRecuar > 0 && (percentagemSubidaDescidaDoentes != 0 || percentagemSubidaDescidaSaudaveis != 0 || percentagemSubidaDescidaImunes != 0)){
        puts("----------------------------------------------------------------------------------");
        if (percentagemSubidaDescidaDoentes != 0)
            printf("%+7.2f%% doentes do que ontem                     |  %+3d Doentes do que ontem!   |\n",percentagemSubidaDescidaDoentes,doentes-doentesOntem);
        if (percentagemSubidaDescidaSaudaveis != 0)
            printf("%+7.2f%% saudaveis do que ontem                   |  %+3d Saudaveis do que ontem! |\n",percentagemSubidaDescidaSaudaveis,saudavel-saudaveisOntem);
        if (percentagemSubidaDescidaImunes != 0)
        printf("%+7.2f%% imunes do que ontem                      |  %+3d Imunes do que ontem!    |\n",percentagemSubidaDescidaImunes,imune-imunesOntem);
        puts("----------------------------------------------------------------------------------");}
    else
        puts("-------------------------------------------------------------------");
    *doentesT = doentes;
    if (dia == 0){
        primeira.dia = dia;
        primeira.doentes = doentes;
        primeira.imunes = imune;
        primeira.pessoas = total;
        primeira.prox = NULL;
        primeira.saudaveis = saudavel;
        primeira.valido = 1;
    }
    
    return primeira;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void guardaDistribuicao(plocal espaco,int total,char* fich){                   // Versão com inteiro no inicio
    pPessoa aux=NULL;
    FILE *f;
    f = fopen(fich,"wb");
    if (f == NULL){
        printf("\nErro no acesso ao ficheiro %s\n\n",fich);
        return;
    }
    
    fwrite(&total, sizeof(int), 1, f);                                          // inteiro a indicar quantos locais estao no array dinamico espaco
    fwrite(espaco, sizeof(local), total, f);                                    // escreve todos os locais do array dinamico no ficheiro
    
    for(int i=0; i<total; i++){                                                 // percorre todos os locais e escreve cada estrutura pessoa
        aux = espaco[i].prox;
        while(aux != NULL){
            fwrite(aux, sizeof(pessoa), 1, f);
            aux = aux->prox;
        }
    }
    
    //IMPLEMENTAR GUARDAR PESSOAS DA FILA NO FINAL DE TUDO
    
    fclose(f);
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
plocal carregaDistribuicao(plocal espaco,char* nome){               //le ficheiro que foi guardado pela função guardaDistribuicao
    plocal novo_espaco=NULL;                                                    //função carrega_distribuição e guarda_distribuição também não eram pedidas... andei a inventar funcionalidades o semestre todo
    pessoa lida;                                                                //devido a esse facto elas não verificam devidamente os freads e os fwrites
    int novo_total;
    
/*
    if( strcmp(nome,"engano.bin")!=0 ){
    printf("Introduza o nome do ficheiro que tem a distribuicao pretendida\nFicheiro: ");
    scanf(" %s",nome);}
*/
    
    FILE *f;
    f = fopen(nome, "rb");
    if (f == NULL){
        printf("\nErro no acesso ao ficheiro %s\n\n",nome);
        return espaco;
    }

    fread(&novo_total, sizeof(int), 1, f);                                      // Le o inteiro do inicio do ficheiro e guarda na variavel novo_total
    
    novo_espaco = malloc(novo_total * sizeof(local));                           //Aloca espaço em memoria para o total de estruturas encontradas
    if (novo_espaco == NULL){                                                   //Caso falhe
        puts("Erro a reservar memoria para o espaco da experiencia");
        fclose(f);
        return espaco;
    }
    fread(novo_espaco, sizeof(local), novo_total, f);                           // todas as estruturas local guardadas no novo_espaço
    
    for(int j=0; j<novo_total; j++){                                            //percorre os locais todos
            novo_espaco[j].prox = NULL;                                         //inicializa o ponteiro para lista ligada a NULL em todos
            for(int k=0; k<novo_espaco[j].pessoas; k++){                        //percorre o ciclo o nr de pessoas que tinha antes de ser gravado
                fread(&lida, sizeof(pessoa), 1, f);                             //caso tenha informação de algumas pessoas vai lendo do ficheiro 
                novo_espaco[j].prox = inserePessoaFinal(novo_espaco[j].prox,lida,1); //e insere no final da lista
            }
        }
    
    fclose(f);
    return novo_espaco;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void adicionaDoente(plocal espaco,int total_locais,int *distribuidas){
    pPessoa populacao;                                                          //ponteiro que vai ser utilizado para verificar se o nome já existe em algum local do espaço
    pessoa doente;
    int id=0,i=0;
        
     if (validaCapacidade(espaco,total_locais) == 0){
         puts("Impossivel adicionar doente a esta simulacao por falta de capacidade!");
         return;
     }
    
    printf("ID do local onde pretende inserir doente: ");                       //pede id ao utilizador
    scanf(" %d", &id);
            
    for(i=0; i<total_locais && espaco[i].id != id; i++);                        //incrementa o i até a posição do ID pretendido no espaço
    if (i==total_locais){                                                       //caso nao encontrou ID
        puts("\nID inexistente na distribuicao atual.\nImpossivel adicionar Doente.\n");
        return;}
    if (espaco[i].capacidade == 0){                                             //caso o local não tenha capacidade...
        puts("\nLocal indicado ja atingiu o limite de capacidade.\nImpossivel adicionar Doente.\n");
        return;}
    
    printf("Nome do doente: ");
    scanf(" %s", doente.nome);
    for(int j=0; j<total_locais; j++){                                          //verifica se o nome já existe
        populacao = espaco[j].prox;
        while (populacao != NULL){                                              //enquanto existir pessoas na população/"fila"
            if (strcmp(doente.nome,populacao->nome) == 0){                      //caso já exista o nome da pessoa a verificar na população devolve 0
                printf("\n%s ja existe no local com ID:%d.\n", doente.nome,espaco[j].id);
                return;}    
        populacao = populacao->prox;                                            //atualiza ponteiro para a próxima pessoa
        }
    }
    printf("Idade do doente:");
    scanf(" %d", &doente.idade);
    printf("Doente ha quantos dias: ");
    scanf(" %d", &doente.contaDias);
    doente.estado = 'D';
    doente.prox = NULL;

    if (verificaPessoa(populacao,doente) == 0)                                  //verifica o resto dos parâmetros
        return;

    (espaco+i)->prox = inserePessoaFinal((espaco+i)->prox,doente,1);
    (espaco[i].capacidade)--;
    (espaco[i].pessoas)++;
    (*distribuidas)++;
    puts("Doente adicionado com sucesso!");
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
plocal recuaSimulacao(plocal espaco, plocal recuaUm, plocal recuaDois, plocal recuaTres, plocal inicio, int total_locais, int *distribuidas,int* dia, int* podeRecuar, int* recuou){
    int recuar = 0,opcao = 0;
    (*recuou) = 0;
    char* engano = "engano.bin";
    
    if (*podeRecuar == 0){
    puts("\nImpossivel recuar!");
    return espaco;}
    
    printf("Deseja recuar para o inicio da simulacao?!\n\n1 - Sim\n2 - Nao\n\n");
    do{                                                                         //ciclo enquanto não escolher 1 ou 2 
        printf("Escolha uma opcao: ");
        scanf(" %d", &opcao);
    } while( opcao < 1 || opcao > 2);
    
    if (*podeRecuar == 0 && opcao == 2){
        puts("\nImpossivel recuar!");
        return espaco;}
    
    if(opcao == 2){                                                                            //printf("\nno recuaSimulacao o recua um: %p",recuaUm);
    printf("\nQuantos dias (%d no maximo) deseja recuar na simulacao?\n",*podeRecuar);
    do{                                                                         //ciclo enquanto não escolher 1 ou 2 
        printf("Dias a recuar: ");
        scanf(" %d", &recuar);
    } while( recuar < 0 || recuar > (*podeRecuar));
    
    if (recuar > *dia){
        printf("\nImpossivel recuar %d dias quando a simulacao esta no dia %d\n",recuar,*dia);
        return espaco;}}
    


    
    guardaDistribuicao(espaco,total_locais,engano);                            //guarda o espaço atual no ficheiro engano.bin para o caso de ter entrado sem querer neste menu e depois deseje voltar ao estado inicial
                                                                                //poderia ter feito uma opçao para no mini menu para cancelar o recuar mas tinha esta função implementada sem lhe dar uso...

    if(recuar == 1){
       // printf("\nantes da copia %p\n",espaco);
       espaco = copiaEspaco(recuaUm,total_locais,espaco);
       // printf("\ndepois da copia %p\n",espaco);
    }
    else if(recuar == 2)
            espaco = copiaEspaco(recuaDois,total_locais,espaco);
    else if(recuar == 3)
            espaco = copiaEspaco(recuaTres,total_locais,espaco);
    else if(opcao == 1)
            espaco = copiaEspaco(inicio,total_locais,espaco);


    //como pode ter havido insersão de novos doentes em algum dos dias vou verificar e atualizar as pessoas distribuidas
    *distribuidas = 0;

    for(int i=0; i<total_locais; i++)
        (*distribuidas) += espaco[i].pessoas;

    
    (*dia) -= recuar;
    if (recuar == 0 && opcao == 2)
        puts("Nao recuou na simulacao!");
    else if (recuar > 1 && opcao == 2)
        printf("\nRecuou %d dias na simulacao!\n",recuar);
    else if(opcao == 2)
        puts("Recuou 1 dia na simulacao!");
    
    (*podeRecuar) -= recuar;
    (*recuou) = recuar;
    
    if(opcao == 1){
        (*podeRecuar) = 0;
        (*recuou) = *dia;
        (*dia) = 0;
        puts("Recuou para o inicio da simulacao!");
    }
    return espaco;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
void transferePessoas(plocal espaco,int total_locais){
    int quantas = 0, origem = 0, destino = 0, idO = 0, idD = 0, pos = 0;
    pPessoa transferida;
    
    if (validaCapacidade(espaco,total_locais) == 0){
        puts("Impossivel transferir pessoas, nenhum local com capacidade!");
        return;
    }
    
    printf("\nID local origem: ");
    scanf(" %d", &origem);
    
    for(idO=0; idO<total_locais && origem != espaco[idO].id; idO++);
    if (idO == total_locais){
        puts("\nImpossivel transferir pessoas,ID do local indicado como origem nao existe!");
        return;}
    if (espaco[idO].pessoas == 0){
        puts("\nImpossivel transferir pessoas dum local vazio!");
        return;
    }
    
    printf("O local %d tem %d pessoas.\n\nQuantas pessoas deseja transferir?\nPessoas: ", espaco[idO].id,espaco[idO].pessoas);
    scanf(" %d", &quantas);
    
    if (espaco[idO].pessoas < quantas){
        printf("\nImpossivel transferir %d pessoas dum local com %d pessoas!",quantas,espaco[idO].pessoas);
        return;}
    
    printf("\nID local destino: ");
    scanf(" %d", &destino);
    
    for(idD=0; idD<total_locais && destino != espaco[idD].id; idD++);
    if (idD == total_locais){
        puts("\nImpossivel transferir pessoas,ID do local indicado como origem nao existe!");
        return;}
    if (idO == idD){
        puts("\nImpossovel transferir pessoas para o proprio local!");
        return;
    }
    if (espaco[idD].capacidade < quantas){
        printf("\nO local %d com capacidade %d nao pode receber %d pessoas!\n",espaco[idD].id,espaco[idD].capacidade,quantas);
        return;
    }
    
    for(quantas; quantas > 0; quantas--){
        pos = intUniformRnd(1, espaco[idO].pessoas);                            //posicao entre 1 e o nr de pessoas que ha no local
        transferida = espaco[idO].prox;
        while(transferida != NULL && pos>1){                                    //pega na pessoa escolhida aleatoriamente.
            transferida = transferida->prox; 
            pos--;}
        espaco[idD].prox = inserePessoaFinal(espaco[idD].prox,*transferida,1);        //transfere e faz as devidas eliminaçoes/alteraçoes no local.
        espaco[idO].prox = eliminaPessoa(espaco[idO].prox,transferida->nome);
        (espaco[idD].pessoas) ++;
        (espaco[idD].capacidade) --;
        (espaco[idO].pessoas) --;
        (espaco[idO].capacidade) ++;
    }
    
    puts("\nPessoas transferidas com sucesso!\n");
}





/*
void recuperaModelo(plocal espaco, int total_locais, int* curados, int* imunes, int* *doentesT){
    pPessoa atual = NULL;

        for(int i=0; i<total_locais; i++){                                      //aplica recuperação em todas as pessoas menos as que foram infetadas "hoje"( percorre todos os locais)
            atual = espaco[i].prox;
            while (atual != NULL){                                                  //enqanto houver pessoas no local aplica o modelo de recuperação'
                if ((atual->estado == 'D' && atual->contaDias>0) &&                 //caso a pessoa esteja doente há pelo menos 1 dia e  recupere ( tanto por prob de recuperar ou por exceder o max de dias)
                    (recupera(atual->idade) || atual->contaDias == plocal copiaEspaco(plocal espaco, int total_locais,plocal destino);             // faz copia do espaco atual para outro vetor independente e elibera memoria caso o vetor ja tinha ifnromação(atual->idade))){    
                        puts("welelecas");
                    (*curados)++;
                        puts("welelecas");
                    if(imunidade()){                                                // caso fique imune
                        atual->estado = 'I';
                        atual->contaDias = -1;
                        (*imunes)++;}
                    else{                                                           //se não imune, fica saudavel
                        atual->estado = 'S';
                        atual->contaDias = -1;}
                }
                else if(atual->estado == 'D' && atual->contaDias != -5){            //caso não recuperou entao aumenta o nr de dias que esta doente
                    (atual->contaDias)++;
                    (*doentesT)++;}
                else if(atual->estado == 'D' && atual->contaDias == -5)             // se estava a -5 significa que ficou doente nesta iteração entao depois de passar pelo incrementa caso nao é doente desta iteração
                    atual->contaDias = 0;                                           //coloco o conta dias a 0, foi a primeira opção que me ocorreu para este "problema"
                            mostraPessoa(*atual);
                atual = atual->prox;
            } 
    }    
}
*/
/*
void disseminaModelo(plocal espaco, int total_locais){
    pPessoa atual = NULL
        for(int j=0; j<total_locais; j++){                                          // Aplica modelo de disseminação ((REVEEEEEEEEER E TESTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR)))))))))))))
        atual = espaco[j].prox;
        pos=1;                                                                  //inicializa primeira posição sempre que muda de local
        infeta = disseminacaoLocal(espaco[j].pessoas);                         // em cada local cada pessoa infeta um nr diferente de pessoas
        while (atual != NULL){                                                  //percorre a lista ligada associada ao local posição j do espaço
            if (atual->estado == 'D' && atual->contaDias != 0)                  // caso a pessoa esteja doente mas não ter sido infetada nesta iteração
                for(int k=0; k<infeta; k++){                                    // aplica infeção a 'infeta' pessoas 
                    infetado = espaco[j].prox;
                    do{
                        quem = intUniformRnd(1,espaco[j].pessoas);              // aplica um numero aleatorio a variavel quem enquanto ela não for diferente da posição atual ( pessoa nao pode auto-infetar )
                    } while(quem == pos); 
                    for(int l=1; l<quem; l++)                                   //ciclo para chegar a posição indicada pela variavel quem
                        infetado = infetado->prox;
                    if (infetado->estado == 'S'){                               //apenas pessoas saudaveis podem ser contaminadas;
                        infetado->estado = 'D';
                        infetado->contaDias = -5;                               //-5 para não ser incrementado pelo modelo seguinte nesta iteração.
                        infetados++;}
                }
        pos++;
        atual = atual->prox;
        }
    }
}
*/