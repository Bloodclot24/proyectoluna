#ifndef AGUA_H
#define AGUA_H

#include "matrizBayes.h"

typedef struct Agua{
     Matriz *X;
     HiperParametros *H;
     Tarch *lexico;
     Tarch *punterosLexico;
}Agua;

int comparar(const void* numero1, const void* numero2);

Agua* comenzar_agua(char* prefijo);

#endif 
