#include "nodo.h"
#include <stdio.h>
#include <stdlib.h>

Nodo* Nodo_Crear(void *elemento){
     Nodo *nodo=NULL;

     nodo=malloc(sizeof(Nodo));
     if(nodo != NULL){
	  nodo->proximoNodo = NULL;
	  nodo->elemento = elemento;
     }
     return nodo;
}

int Nodo_SetSiguiente(Nodo* nodo, Nodo* siguiente){
     if(nodo == NULL){
	  return 1;
     }
     nodo->proximoNodo = siguiente;
     return 0;
}



Nodo* Nodo_GetSiguiente(Nodo* nodo){
     if(nodo== NULL){
	  return NULL;
     }
     else{
	  return nodo->proximoNodo;
     }
}

Nodo* Nodo_GetAnterior(Nodo* nodo){
     if(nodo== NULL){
	  return NULL;
     }
     else{
	  return nodo->proximoNodo;
     }
}


void* Nodo_GetElemento(Nodo *nodo){
     if(nodo == NULL){
	  return NULL;
     }
     else{
	  return nodo->elemento;
     }
}

void Nodo_Liberar(Nodo *nodo){
     if(nodo != NULL){
	  free(nodo);
     }
}
