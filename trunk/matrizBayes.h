#ifndef MATRIZBAYES_H_
#define MATRIZBAYES_H_

#include "archivo.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#define MATRIZ1 "_matriz1"
#define MATRIZ2 "_matriz2"
#define MATRIZ3 "_matriz3"
#define LEXICO  "_lexico"
#define PLEXICO "_punterosLexico"
#define HIPERPARAM "_hiperParam"

#define QUERY_ELEM_UNIT 5

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
     int* agregar;
     int cantidadAgregar;
     int* sacar;
     int cantidadSacar;
}Query;

/* Arma una matriz segun el archivo auxiliar */
int armarMatriz(Tarch* archAuxiliar, char* prefijo);

/* Calcula hiperparametros dada la matriz X y un parametro de
 * concentracion de Dirichlet (generalmente dParam=2) */
HiperParametros* BSParam(Matriz *X, double dParam);

/* Carga la matriz principal */
Matriz* cargarMatriz(char* prefijo);

/* carga dos vectores de hiperparametros */
HiperParametros* cargarHParam(Matriz* X, char* prefijo);

/* Carga un archivo de lexico */
Tarch* cargarLexico(char* prefijo);

/* Carga un archivo de punteros al lexico */
Tarch* cargarPLexico(char* prefijo);

/* Calcula el puntaje de cada articulo segun el algoritmo de sets
 * bayesianos  */
double* BSets(Matriz *X, Query* q, HiperParametros *param);

/* Arma un Query agregando el termino especificado. Si query es NULL,
 * devuelve un nuevo Query, si no, agrega un nuevo termino. */
Query* ArmarQuery(Query* query, const char* termino, Tarch* lexico, Tarch* punterosLexico);

/* Libera la memoria utilizada por el query */
void DestruirQuery(Query* q);

#endif /*MATRIZBAYES_H_*/
