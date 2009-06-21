#ifndef MATRIZBAYES_H_
#define MATRIZBAYES_H_

#include "archivo.h"

typedef struct {
     Tarch *elementos, *columnas, *inicioFila;
     uint32_t numFilas;
     uint32_t numColumnas;
}Matriz;

typedef struct{
     double* alpha;
     double* beta;
}HiperParametros;

typedef struct{
     int* query;
     int elementos;
}Query;

/* Arma una matriz segun el archivo auxiliar */
Matriz* armarMatriz(Tarch* archAuxiliar);

/* Calcula hiperparametros dada la matriz X y un parametro de
 * concentracion de Dirichlet (generalmente dParam=2) */
HiperParametros* BSParam(Matriz *X, double dParam);

/* Calcula el puntaje de cada articulo segun el algoritmo de sets
 * bayesianos  */
double* BSets(Matriz *X, Query* q, HiperParametros *param);

#endif /*MATRIZBAYES_H_*/
