#include "pila.h"
#include <stdio.h>
#include <stdlib.h>

Pila* Pila_Crear(void){

     Pila *pila=NULL;
     pila = (Pila*)malloc(sizeof(Pila));
     if(pila != NULL){
	  pila->primerElemento = NULL;
	  pila->ultimoElemento = NULL;
     }
     return pila;
}

int Pila_Apilar(Pila *pila, void *elemento){
     int returnValue;
     if(pila == NULL){
	  /* nodo invalido, sale */
	  returnValue=1;
     }
     else{
	  /* nodo valido, procesamos */
	  Nodo *nodo=NULL;
	  /* creamos el nodo con el nuevo elemento */
	  if( (nodo = Nodo_Crear(elemento)) != NULL){
	       /* nodo OK, lo insertamos donde corresponda */ 
	       if(pila->primerElemento == NULL){
		    /* es el primer (y unico) elemento de la pila */
		    pila->primerElemento = nodo;
		    pila->ultimoElemento = nodo;
		    /* todo OK,la pila tiene un elemento */
		    returnValue = 0;
	       }
	       else{
		    /* la pila tiene uno o mas elementos */
		    if( Nodo_SetSiguiente(nodo, pila->primerElemento) == 0){
			 pila->primerElemento = nodo;
			 /* El nodo se inserta exitosamente */
			 returnValue = 0;
		    }
		    else{
			 
			 /* No se pudo insertar el nodo */
			 Nodo_Liberar(nodo);
			 returnValue = 1;
		    }
	       }
	       
	       
	  }
	  else{
	       /* No se pudo crear el nodo */
	       returnValue = 1;
	  }
     }
     return returnValue;     
}


int Pila_HayMasElementos(Pila *pila){
     if(pila == NULL){
	  return 2;
     }
     else{

	  if(pila->primerElemento == NULL)
	       return 0;
	  else 
	       return 1;
     }

}

void* Pila_PrimerElemento(Pila *pila){
     if(Pila_HayMasElementos(pila)){
	  
	  return pila->primerElemento->elemento;
     }
     else{
	  return NULL;
     }


}

void* Pila_Desapilar(Pila *pila){

     Nodo *nodo;
     void *elemento;
     void* returnValue;
     if(pila == NULL){
	  return NULL;
     }
     else{

	  nodo = pila->primerElemento;
	  if(nodo != NULL){
	       /* la pila tiene al menos un elemento */
	       elemento = Nodo_GetElemento(nodo);
	  	  
	       if(pila->primerElemento == pila->ultimoElemento){
		    /* la pila tiene un solo elemento */
		    
		    pila->primerElemento = pila->ultimoElemento = NULL;
		    /* pila vacia */
	       }
	       else{
		    /* queda mas de unelemento */
		    
		    pila->primerElemento = Nodo_GetSiguiente(nodo);
		    
	       }
	       
	       Nodo_Liberar(nodo);
	       /* libero el nodo */
	       returnValue = elemento;
	       
	  }
	  else{
	       /* la pila no tiene elementos*/
	       returnValue = NULL; 
	  }
     }
     return returnValue;
}

void Pila_Liberar(Pila *pila){
     if(pila != NULL){

	  while( Pila_Desapilar(pila) != 0){
	       /* libera todos los nodos */
	  }

	  free(pila);
     }
}

void Pila_LiberarTodo(Pila *pila){
     if(pila != NULL){
	  void* elemento;
	  while( (elemento=Pila_Desapilar(pila)) != 0){
	       /* libera todos los nodos */
	       free(elemento); /* libero los elementos que quedan */ 
	  }

	  free(pila);
     }
}

