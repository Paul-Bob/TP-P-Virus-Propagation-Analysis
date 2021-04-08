/* Paul Ionut Bob - 2016013407
 * File:   espaco.h
 * Author: droopy
 *
 * Created on 22 de Abril de 2020, 17:55
 */

#ifndef ESPACO_H
#define ESPACO_H
void mostraEspaco(plocal espaco,int total);                                     // mostra todas as estruturas disponiveis no array dinamico
int validaID(plocal espaco,int total, local validacao);                         //valida se um determinado ID já se encontra no espaco 
int validaEspaco(plocal espaco,int total);                                      // verifica se ha IDs repetidos e se as ligacoes coincidem
int ligacaoPossivel(plocal espaco,int total, local localtemp);                  // verifica se o array das ligacoes esta cheio
int validaCapacidade(plocal espaco,int total);                                  //verifica se o espaco ainda tem capacidade para mais pessoas
plocal adicionaLocal(plocal espaco, int* total);                                // Adiciona um local ao array dinamico espaco tomando as devidas precauções
plocal eliminaLocal(plocal espaco, int* total);                                 // elimina um local do array dinamico espaco tomando as devidas precauções
#endif /* ESPACO_H */

