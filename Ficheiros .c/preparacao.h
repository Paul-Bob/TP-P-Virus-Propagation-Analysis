/* Paul Ionut Bob - 2016013407
 * File:   preparacao.h
 * Author: droopy
 *
 * Created on 15 de Maio de 2020, 21:40
 */

#ifndef PREPARACAO_H
#define PREPARACAO_H
void mostraDistribuicao(plocal espaco, int total);                              //mostra a distribuição
void informacaoEspacoPopulacao(plocal espaco, pPessoa populacao, int total_locais, int pessoas_distribuidas, char* nomeE, char* nomeP ); //escreve informações sobre o espaço e a população atuais
void apagaDistribuicao(plocal espaco, int total, int* distribuidas);            //elimina distribuição
plocal preparaEspaco(plocal espaco,int* total,char* nome);                      // abre o ficheiro indicado pelo utilizador e copia as estruturas guardadas para o array dinamico espaco
plocal copiaEspaco(plocal espaco, int total_locais,plocal destino);             // faz copia do espaco atual para outro vetor independente e elibera memoria caso o vetor ja tinha ifnromação
pPessoa preparaPopulacao(pPessoa populacao, char *nome);                        //abre o ficheiro indicado pelo utilizador e copia as pessoas guardadas para a lista ligada populacao
pPessoa preparaDistribuicao(plocal espaco, int total, pPessoa populacao, int* distribuidas); // distribui aleatoriamente as pessoas da lista ligada principal por listas ligadas associadas a cada local
pPessoa anulaDistribuicao(pPessoa populacao, plocal espaco, int total, int* distribuidas); //"devolve" pessoas a "fila"
#endif /* PREPARACAO_H */

