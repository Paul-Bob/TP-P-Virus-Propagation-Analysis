/* Paul Ionut Bob - 2016013407
 * File:   auxiliar.h
 * Author: droopy
 *
 * Created on 30 de Abril de 2020, 17:47
 */

#ifndef AUXILIAR_H
#define AUXILIAR_H

void initRandom();                                                              // Inicializa o gerador de numeros aleatorios.Esta funcao deve ser chamada apenas uma vez no inicio da execucao do programa
char* strcpySegura(char* origem,char* destino,int tamO,int tamD);              //copia strings de forma segura.
int intUniformRnd(int a, int b);                                                //Devolve um valor inteiro aleatorio distribuido uniformemente entre [a, b]
int probEvento(float prob);                                                     // Devolve o valor 1 com probabilidade prob. Caso contrario, devolve 0

#endif /* AUXILIAR_H */

