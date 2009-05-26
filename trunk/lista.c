#include <stdlib.h>
#include "lista.h"
#include "nodo.h"

Lista* Lista_Crear(void){
     Lista *lista = (Lista*)malloc(sizeof(Lista));
     lista->primero = NULL;
     lista->ultimo = NULL;
     lista->tamanio = 0;
     return lista;
}

void* Lista_Insertar(Lista* lista, void* elemento){
     if(lista != NULL){
	  Nodo* nodo = Nodo_Crear(elemento);
	  lista->tamanio++;
	  /* es el primer elemento */
	  if(lista->primero == NULL){
	       /* inserto el primer elemento de la lista */
	       lista->primero = nodo;
	       lista->ultimo = nodo;
	  }
	  else{
	       /* no es el primer nodo */
	       Nodo_SetSiguiente(lista->ultimo, nodo);
	       lista->ultimo = nodo;
	  }
     }
}

void* Lista_PrimerElemento(Lista* lista){
     return Nodo_GetElemento(lista->primero);
}

int Lista_EstaVacia(Lista* lista){
     return (lista->tamanio==0);
}

void* Lista_RemoverPrimero(Lista* lista){
     Nodo *nodoAuxiliar=NULL;
     void* elemento=NULL;
     if(lista->primero != NULL){
	  nodoAuxiliar = lista->primero;
	  lista->tamanio--;
	  if(lista->primero != lista->ultimo){
	       lista->primero = Nodo_GetSiguiente(lista->primero);
	  }
	  else{
	       lista->primero = lista->ultimo = NULL;
	  }
	  elemento = Nodo_GetElemento(nodoAuxiliar);
	  Nodo_Liberar(nodoAuxiliar);
     }
     
     return elemento;
}

void Lista_Liberar(Lista* lista){
     if(lista != NULL){
	  while(lista->tamanio > 0){
	       Lista_RemoverPrimero(lista);
	  }
	  free(lista);
     }
}

void Lista_LiberarTodo(Lista* lista){
     if(lista != NULL){
	  while(lista->tamanio > 0){
	       free(Lista_RemoverPrimero(lista));
	  }
	  free(lista);
     }
}
