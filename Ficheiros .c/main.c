/* Paul Ionut Bob - 2016013407
 * File:   main.c
 * Author: droopy
 *
 * Created on 22 de Abril de 2020, 9:48
 */

#include <stdio.h>
#include <stdlib.h>
#include "estruturas.h"
#include "auxiliar.h"
#include "espaco.h"
#include "populacao.h"
#include "preparacao.h"
#include "relatorio.h"
#include "simulacao.h"

int menu(){ // Menu Principal!
	int opcao;
	puts("\n");
        puts("**************************************************");
        puts("*          M E N U     P R I N C I P A L         *");
        puts("**************************************************");        
        puts("|  1 - Informacoes atuais sobre a preparacao     |");
	puts("|  2 - Menu Espaco                               |");
        puts("|  3 - Menu Populacao                            |");
        puts("|  4 - Distribuir populacao pelo espaco          |");
        puts("|  5 - Mostra Distribuicao                       |");
        puts("|  6 - Desfazer Distribuicao                     |");
        puts("|  7 - Eliminar Distribuicao                     |");
        puts("|  8 - Iniciar Simulacao                         |");
	puts("|  9 - Terminar                                  |");
        puts("**************************************************");
	
	do{
            printf("Escolha uma opcao: ");
            scanf("%d", &opcao);
	}while(opcao<1 || opcao>9);
	return opcao;	
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int menuE(){ // Menu Espaço!
	int opcaoE;
	puts("\n");
        puts("**************************************************");
        puts("*             M E N U     E S P A C O            *");
        puts("**************************************************"); 
        puts("|  1 - Trocar de espaco                          |");
	puts("|  2 - Adiciona local ao espaco atual            |");
        puts("|  3 - Elimina local do espaco atual             |");
        puts("|  4 - Mostra todos os locais do espaco atual    |");
        puts("|  5 - Validar espaco                            |");
	puts("|  6 - Voltar ao Menu Principal                  |");
        puts("**************************************************");
	
	do{
            printf("Escolha uma opcao: ");
            scanf("%d", &opcaoE);
	}while(opcaoE<1 || opcaoE>6);
	return opcaoE;	
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int menuP(){ // Menu População!
	int opcaoP;
	puts("\n");
        puts("**************************************************");
        puts("*          M E N U     P O P U L A C A O         *");
        puts("**************************************************"); 
        puts("|  1 - Trocar de populacao                       |");
	puts("|  2 - Adiciona pessoa a populacao (a \"fila\")    |");
        puts("|  3 - Elimina pessoa da populacao (da \"fila\"    |");
        puts("|  4 - Mostra pessoas na populacao (na \"fila\")   |");
        puts("|  5 - Validar Populacao (por implementar)       |");
        puts("|  6 - Voltar ao Menu Principal                  |");
        puts("**************************************************");
	
	do{
            printf("Escolha uma opcao: ");
            scanf("%d", &opcaoP);
	}while(opcaoP<1 || opcaoP>6);
	return opcaoP;	
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int menuS(){ // Menu Simulação!
        int opcaoS;
        puts("\n");
        puts("**************************************************");
        puts("*          M E N U     S I M U L A C A O         *");
        puts("**************************************************"); 
        puts("|  1 - Avancar 1 dia                             |");
        puts("|  2 - Informacoes atuais sobre a simulacao      |");
        puts("|  3 - Mostrar distribuicao                      |");
        puts("|  4 - Transferir Pessoas                        |");
        puts("|  5 - Recuar (max 3 dias ou INICIO DA SIMULACAO)|");
        puts("|  6 - Anular opcao Recuar                       |");
        puts("|  7 - Adicionar Doente                          |");
        puts("| 10 - Terminar                                  |");
        puts("**************************************************");
        
        do{
            printf("Escolha uma opcao: ");
            scanf("%d", &opcaoS);
        }while(opcaoS<1 || opcaoS>10);
        return opcaoS;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int menuFS(){ // Menu Simulação!
        int opcaoFS;
        puts("\n");
        puts("**************************************************");
        puts("*          M E N U     S I M U L A C A O         *");
        puts("**************************************************"); 
        puts("|  1 - Informacoes atuais sobre a simulacao      |");
        puts("|  2 - Mostrar distribuicao                      |");
        puts("|  3 - Recuar para o inicio da simulacao         |");
        puts("|  4 - Terminar e gerar relatorio!               |");
        puts("**************************************************");
        
        do{
            printf("Escolha uma opcao: ");
            scanf("%d", &opcaoFS);
        }while(opcaoFS<1 || opcaoFS>4);
        return opcaoFS;
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char** argv) {
    initRandom();                                                               // esta função só deve ser chamada uma vez
    plocal espaco = NULL, primDist = NULL, voltaUmDia = NULL, voltaDoisDias = NULL, voltaTresDias = NULL, anulaRecua = NULL;// criação dos vetores dinamicos
    pPessoa populacao = NULL; // criação da lista ligada para pessoas
    pInforma informacao = NULL; // criação lista ligada que guarda informação de cada dia da simulação
    informa primeiraInformacao; // primeiro no da lista informação fica com os dados do dia 0
    int total_locais = 0,pessoas_distribuidas = 0,pessoas_fila=0,total_pessoas=0, dia=0, opcao = 0,opcaoFS = 0, opcaoS=0, opcaoE = 0, opcaoP=0, miniOpcao = 0, doentes = 0, podeRecuar = 0, recuou = 0;
    char nomeE[NOME_FICHEIRO];
    char nomeP[NOME_FICHEIRO];
    char nomeElemina[NOME_PESSOA];
    char* engano = "engano.bin";
   
    espaco = preparaEspaco(espaco,&total_locais,nomeE);
    populacao = preparaPopulacao(populacao,nomeP);

    do {
        opcao = menu();
	switch(opcao){
            case 1: validaEspaco(espaco,total_locais); 
                    informacaoEspacoPopulacao(espaco,populacao,total_locais,pessoas_distribuidas,nomeE,nomeP); break;
                    
            case 2: if (pessoas_distribuidas != 0){
                        puts("\nAtencao! Ja existe uma distribuicao!\nNao podera trocar de espaco nem eliminar locais!");}
                    do {
                        opcaoE = menuE();
                        switch(opcaoE){
                            case 1: if (pessoas_distribuidas != 0){
                                        puts("\nNao e possivel trocar o espaco caso ja exista uma distribuicao!"); break;}
                                    espaco = preparaEspaco(espaco,&total_locais,nomeE); break;
                                    
                            case 2: if (espaco == NULL){
                                        printf("\nEsta prestes a iniciar a criacao de um novo espaco.\nPasso 1: Indique o nome que deseja dar ao espaco (no maximo %d caracteres)\n"
                                                "Nome: ", NOME_FICHEIRO);
                                        scanf(" %s", nomeE);
                                        printf("\nPasso 2: Introduza o ID e a Capacidade do primeiro local");
                                        espaco = adicionaLocal(espaco,&total_locais);
                                        printf("\nPasso 3: Preencha o segundo local, certifique-se que faz apenas uma licacao, com o ID: %d", espaco[0].id);
                                        espaco = adicionaLocal(espaco,&total_locais);
                                        if (total_locais == 2)
                                            printf("Espaco %s criado com sucesso.", nomeE);
                                        break;
                                    }
                                    espaco = adicionaLocal(espaco,&total_locais); break;
                                    
                            case 3: if (pessoas_distribuidas != 0){
                                        puts("\nNao e possivel eliminar locais caso ja exista uma distribuicao!"); break;}
                                    espaco = eliminaLocal(espaco,&total_locais); break;
                                    
                            case 4: mostraEspaco(espaco,total_locais); break;
                            
                            case 5:  if(validaEspaco(espaco,total_locais)) puts("\nEspaco validado com sucesso!"); break;
                        }
                    }while(opcaoE != 6); break;
                    
            case 3: if (pessoas_distribuidas != 0){
                        puts("\nAtencao! Ja existe uma distribuicao!\nNao podera trocar de populacao e todas as alteracoes feitas na populacao serao guardadas na fila!");}
                    do {
                        opcaoP = menuP();
                        switch(opcaoP){
                            case 1: if (pessoas_distribuidas != 0){
                                        puts("\nNao e possivel fazer alterar a populacao caso ja exista uma distribuicao!"); break;}
                                    populacao = preparaPopulacao(populacao,nomeP); break;
                                    
                            case 2: populacao = inserirPessoa(populacao); break;
                                    
                            case 3: printf("Nome:");scanf(" %s", nomeElemina);populacao=eliminaPessoa(populacao,nomeElemina); break;
                                    
                            case 4: mostraPopulacao(populacao); break;
                            
                            case 5:  puts("\nOpcao por implementar"); break;
                        }
                    }while(opcaoP != 6); break;
                    

            case 4: if (validaEspaco(espaco,total_locais) == 1){
                        populacao = preparaDistribuicao(espaco,total_locais,populacao,&pessoas_distribuidas);} break;
                        
            case 5: mostraDistribuicao(espaco,total_locais); break;
            
            case 6: populacao = anulaDistribuicao(populacao,espaco,total_locais,&pessoas_distribuidas); break;
            
            case 7: if (pessoas_distribuidas == 0){
                        puts("\nNao existe nenhuma pessoa distribuida pelo espaco!"); break;}
                    puts("\nAtencao! Esta prestes a eliminar a distribuicao!");
                    puts("Todas as pessoas distribuidas irao ser eliminadas definitivamente da distribuicao e do programa!");
                    puts("Caso pretenda transferir as pessoas para a fila utilize a opcao 6 do menu principal!");
                    puts("Deseja eliminar todas as pessoas distribuidas?!\n\n1 - Sim!\n2 - Nao!\n");
                    do{                                                         //ciclo enquanto não escolher 1 ou 2 
                        printf("Escolha uma opcao: ");
                        scanf(" %d", &miniOpcao);
                      } while( miniOpcao < 1 || miniOpcao > 2);
                    if (miniOpcao == 2)
                        break;
                    apagaDistribuicao(espaco,total_locais,&pessoas_distribuidas); break;  
                    
            case 8: puts("\nAtencao! Esta prestes a iniciar a simulacao!");
                    puts("Nao podera voltar a nenhum dos menus anteriores!");
                    puts("Certifique-se que tem a populacao desejada distribuida pelo espaco escolhido!");
                    puts("Caso continue as pessoas da \"fila\" irao para casa e deixara de poder distribuirlas!");
                    puts("Deseja continuar?!\n\n1 - Sim!\n2 - Nao!\n");
                    do{                                                         //ciclo enquanto não escolher 1 ou 2 
                        printf("Escolha uma opcao: ");
                        scanf(" %d", &miniOpcao);
                      } while( miniOpcao < 1 || miniOpcao > 2);
                    if (miniOpcao == 2 || !validaEspaco(espaco,total_locais))    //caso escolha nao avançar ou a validação do espaço falha não entra na simulação
                        break;
                    if (pessoas_distribuidas < MIN_DISTRIBUIDAS){
                        printf("\nDeve ter mais do que %d pessoas distribuidas pelo espaco para iniciar a simulacao\n",MIN_DISTRIBUIDAS);
                        break;}
                    populacao = libertaLista(populacao);
                    primDist = copiaEspaco(espaco,total_locais,primDist);
                    primeiraInformacao = informacaoSimulacao(espaco,total_locais,pessoas_distribuidas,dia,&doentes,voltaUmDia,podeRecuar,primeiraInformacao);
                    informacao = insereInformacao(informacao,primeiraInformacao);
                    do {
                        opcaoS = menuS();
                        switch(opcaoS){
                            case 1: if (doentes == 0){
                                        puts("\nNao existem doentes distribuidos na simulacao!"); break;}
                                    voltaTresDias = copiaEspaco(voltaDoisDias,total_locais,voltaTresDias);
                                    voltaDoisDias = copiaEspaco(voltaUmDia,total_locais,voltaDoisDias);
                                    voltaUmDia = copiaEspaco(espaco,total_locais,voltaUmDia);
                                    informacao = eliminaInvalido(informacao);
                                    informacao = aplicaModelo(espaco,total_locais,&dia,voltaUmDia,voltaDoisDias,voltaTresDias,&doentes,informacao);
                                    if (podeRecuar <3)
                                    podeRecuar++;
                                    recuou = 0; 
                                    break;
                                    
                            case 2: informacaoSimulacao(espaco,total_locais,pessoas_distribuidas,dia,&doentes,voltaUmDia,podeRecuar,primeiraInformacao); break;
                            
                            case 3: mostraDistribuicao(espaco,total_locais); break;
                            
                            case 4: transferePessoas(espaco,total_locais); break;
                                    
                            case 5: espaco = recuaSimulacao(espaco,voltaUmDia,voltaDoisDias,voltaTresDias,primDist,total_locais,&pessoas_distribuidas,&dia,&podeRecuar,&recuou);  //reveeeer para o caso de voltar 2
                                    if(recuou == 0)
                                        break;
                                    else if(recuou == 1){
                                        voltaUmDia = copiaEspaco(voltaDoisDias,total_locais,voltaUmDia); 
                                        voltaDoisDias = copiaEspaco(voltaTresDias,total_locais,voltaDoisDias);}
                                    else if(recuou == 2)
                                        voltaUmDia = copiaEspaco(voltaTresDias,total_locais,voltaUmDia);
                                    informacaoSimulacao(espaco,total_locais,pessoas_distribuidas,dia,&doentes,voltaUmDia,podeRecuar,primeiraInformacao);
                                    atualizaEstado(informacao,dia); break;
                                    
                            case 6: if (recuou == 0){
                                        puts("\nOpcao disponivel apenas no instante a seguir ao recuar na simualacao!"); break;}
                                    puts("\nCaso recuou sem querer na simulacao pode anular esse descuido e voltar ao dia em que estava!");
                                    puts("Ira manter a distribuicao e o estado que tinha antes de recuar na simulacao!");
                                    puts("ATENCAO!!!\nApenas conseguira recuar novamente depois de avancar na simulacao a partir do dia em questao!");
                                    puts("Deseja continuar?!\n\n1 - Sim!\n2 - Nao!\n");
                                    do{                                                         //ciclo enquanto não escolher 1 ou 2 
                                        printf("Escolha uma opcao: ");
                                        scanf(" %d", &miniOpcao);
                                      } while( miniOpcao < 1 || miniOpcao > 2);
                                    if(miniOpcao == 2)
                                        break;
                                    anulaRecua = carregaDistribuicao(espaco,engano);
                                    espaco = copiaEspaco(anulaRecua,total_locais,espaco);
                                    podeRecuar = 0;
                                    apagaDistribuicao(anulaRecua,total_locais,&podeRecuar);
                                    free(anulaRecua);
                                    anulaRecua = NULL; 
                                    dia += recuou;
                                    atualizaEstado(informacao,dia);
                                    break;
                                    
                            case 7: adicionaDoente(espaco,total_locais,&pessoas_distribuidas); break;
                            
                            case 10:guardaInformacao(espaco,total_locais,informacao,dia);
                                    guardaPessoas(espaco,total_locais);
                                    populacao=libertaLista(populacao); apagaDistribuicao(espaco,total_locais,&pessoas_distribuidas);free(espaco);
                                    apagaDistribuicao(primDist,total_locais,&pessoas_distribuidas); free(primDist);
                                    if(dia>0)
                                    apagaDistribuicao(voltaUmDia,total_locais,&pessoas_distribuidas);free(voltaUmDia);
                                    if(dia>1)
                                    apagaDistribuicao(voltaDoisDias,total_locais,&pessoas_distribuidas); free(voltaDoisDias); 
                                    if(dia>2)
                                    apagaDistribuicao(voltaTresDias,total_locais,&pessoas_distribuidas); free(voltaTresDias);
                                    puts("\nPior do que o COVID-19 e o COVID-izer =)\nAlocacoes de memoria eliminadas com sucesso!");   
                                    return (EXIT_SUCCESS);
                        }
                    }while(opcaoS != 10); break;
                    
            case 9: populacao=libertaLista(populacao); apagaDistribuicao(espaco,total_locais,&pessoas_distribuidas);free(espaco);
                                apagaDistribuicao(primDist,total_locais,&pessoas_distribuidas); free(primDist); 
                                apagaDistribuicao(voltaUmDia,total_locais,&pessoas_distribuidas);free(voltaUmDia);
                                apagaDistribuicao(voltaDoisDias,total_locais,&pessoas_distribuidas); free(voltaDoisDias); 
                                apagaDistribuicao(voltaTresDias,total_locais,&pessoas_distribuidas); free(voltaTresDias);
                                    puts("\nPior do que o COVID-19 e o COVID-izer =)\nAlocacoes de memoria eliminadas com sucesso!");  
	}
    } while(opcao != 9);
    
    return (EXIT_SUCCESS);
}

