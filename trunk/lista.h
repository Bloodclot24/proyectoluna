#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include "nodo.h"

/* Estructura de una lista simple */
typedef struct Lista{
     Nodo* primero;
     Nodo* ultimo;
     int tamanio;
}Lista;

/* Crea una nueva lista vacia */
Lista* Lista_Crear(void);

/* Inserta un dato al final de la lista */
void* Lista_Insertar(Lista* lista, void* elemento);

/* Devuelve el primer elemento de la lista */
void* Lista_PrimerElemento(Lista* lista);

/* Indica si la lista esta o no vacia */
int Lista_EstaVacia(Lista* lista);

/* Devuelve el primer elemento de la lista y lo extrae de la misma*/
void* Lista_RemoverPrimero(Lista* lista);

/* Elimina la lista y libera toda la memoria alocada. *NO* LIBERA LOS
 * ELEMENTOS QUE PUDIERON HABER QUEDADO EN LA LISTA. */
void Lista_Liberar(Lista* lista);

/* Elimina la lista y libera toda la memoria alocada. Libera los
 * elementos que pudieron haber quedado en la lista. */
void Lista_Liberar(Lista* lista);

#endif /* LISTA_H_INCLUDED */
