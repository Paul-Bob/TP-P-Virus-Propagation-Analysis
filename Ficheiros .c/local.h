/* Paul Ionut Bob - 2016013407
 * File:   local.h
 * Author: droopy
 *
 * Created on 22 de Abril de 2020, 17:59
 */

#ifndef LOCAL_H
#define LOCAL_H
void mostraLocal(local sala);                                                   //mostra local
void organizaLigacoes(plocal espaco, int total);                                //organiza as ligaçoes para que as ligações vazias ( == -1) apareçam depos das ligaçoes preenchidas
int validaLocal(local sala);                                                    //faz validação do local devolve 1 se correu tudo bem, se não, devolve 0
int preencheLocal(plocal local);                                                //preenche um local passado com ponteiro devolve 1 se correu tudo bem, se não, devolve 0
#endif /* LOCAL_H */

