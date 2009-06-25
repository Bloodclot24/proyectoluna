#ifndef AGUA_H
#define AGUA_H

#include "archivo.h"
#include "matrizBayes.h"

typedef struct Agua{
     Matriz *X;
     HiperParametros *H;
     Tarch *lexico;
     Tarch *punterosLexico;
}Agua;

#include "feedBack.h"
#include "lista.h"
#include "Controlador/controlador.h"
#include "matrizBayes.h"

int comparar(const void* numero1, const void* numero2);

Agua* comenzar_agua(char* prefijo);

int agua(Agua* agua, Datos* datos);

#endif 
