#ifndef PILA_H_INCLUDED
#define PILA_H_INCLUDED

#include "nodo.h"

/*
 * Implementacion de una pila
 */

typedef struct Pila{
     Nodo *primerElemento;
     Nodo *ultimoElemento;
}Pila;

/*
 * Reserva memoria para la pila.
 */
Pila* Pila_Crear(void);

/*
 * Inserta al inicio de la pila un puntero a 'elemento'.
 * Devuelve 0 si tuvo exito.
 */
int Pila_Apilar(Pila *pila, void *elemento);

/*
 *  Devuelve 1 si hay mas elementos.
 *  Devuelve 0 si no hay elementos.
 *  Devuelve 2 si 'pila' es NULL.
 */
int Pila_HayMasElementos(Pila *pila);

/*
 *  Devuelve un puntero al primer elemento en la pila o 
 *  NULL si 'pila' es NULL
 */
void* Pila_PrimerElemento(Pila *pila);

/*
 * Elimina el primer elemento de la pila.
 * Devuelve el puntero al elemento si tuvo exito, NULL si hubo algun error
 * (por ejemplo pila == NULL)
 */
void* Pila_Desapilar(Pila *pila);

/*
 * Destruye la pila
 */
void Pila_Liberar(Pila *pila);

/*
 * Destruye la pila pero libera todos los elementos almacenados.
 */
void Pila_LiberarTodo(Pila *pila);

#endif /* PILA_H_INCLUDED */
