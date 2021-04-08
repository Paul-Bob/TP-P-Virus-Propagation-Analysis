/* Paul Ionut Bob - 2016013407
 * File:   auxiliar.c
 * Author: droopy
 *
 * Created on 30 de Abril de 2020, 17:46
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "auxiliar.h"


void initRandom(){                                                              // Inicializa o gerador de numeros aleatorios.Esta funcao deve ser chamada apenas uma vez no inicio da execucao do programa
    srand(time(NULL));
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
char* strcpySegura(char* origem,char* destino,int tamO,int tamD){              //copia strings de forma segura.
    if(tamD<tamO){
        puts("String origem maior do que string destino.");
        return origem;
    }
    for(int i=0; i<tamD; i++)
        destino[i] = origem[i];
    
return destino;    
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int intUniformRnd(int a, int b){                                                //Devolve um valor inteiro aleatorio distribuido uniformemente entre [a, b]
    return a + rand()%(b-a+1);
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
int probEvento(float prob){                                                     // Devolve o valor 1 com probabilidade prob. Caso contrario, devolve 0
    return prob > ((float)rand()/RAND_MAX);
}
/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

