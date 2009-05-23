#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

/* Implementacion de un nodo simplemente enlazado. */
typedef struct Nodo{
     void *elemento;
     struct Nodo *proximoNodo;
}Nodo;

/* Crea un nuevo nodo conteniendo el elemento especificado */
Nodo* Nodo_Crear(void *elemento);

/* Enlaza el nodo actual con otro nodo, asignandolo a este ultimo como
 * el siguiente. */
int Nodo_SetSiguiente(Nodo* nodo, Nodo* siguiente);

/* Obtiene el siguiente nodo, o NULL si no hay mas nodos. */
Nodo* Nodo_GetSiguiente(Nodo* nodo);

/* Obtiene elelemento asociado al nodo. */
void* Nodo_GetElemento(Nodo *nodo);

/* Libera la memoria ocupada por el nodo. */
void Nodo_Liberar(Nodo *nodo);


#endif /* NODO_H_INCLUDED */
