#include "particiones.h"

Tparticiones* crearParticiones(const char* nombre){
     Tparticiones *TP = (Tparticiones*)malloc(sizeof(Tparticiones));
     TP = (Tparticiones*)malloc(sizeof(Tparticiones));
     TP->nombre = (char*)malloc(strlen(nombre));
     strcpy(TP->nombre, nombre);
     TP->cantidad = 0;
     ls_Crear(&TP->particiones, sizeof(Tarch));
     return TP;
}

void agregarParticion(Tparticiones* TP, Tarch* TA){
     if(TP != NULL && TA != NULL){
	  TMovimiento_Ls modo = LS_SIGUIENTE;
	  if(ls_Vacia(TP->particiones))
	       modo = LS_PRIMERO;
	  ls_Insertar(&TP->particiones, modo, (void*)TA);
	  TP->cantidad++;
     }
}

/*
 * Obtiene la primera particion y la elimina de la lista.
 */
Tarch* obtenerParticion(Tparticiones* TP){
     Tarch* TA=NULL;
     if(TP != NULL){
	  if(!ls_Vacia(TP->particiones)){
	       ls_MoverCorriente(&TP->particiones, LS_PRIMERO);
	       ls_ElemCorriente(TP->particiones, &TA);
	       ls_BorrarCorriente(&TP->particiones);
	       TP->cantidad--;
	  }
     }
     return TA;
}

void eliminarParticiones(Tparticiones* TP){
     if(TP != NULL){
	  if(TP->cantidad > 0){
	       /* Si quedan cosas colgadas, las borro */
	       printf("WARNING: Se elimino el grupo de particiones %s, y quedaban %d particiones sin procesar.\n", TP->nombre, TP->cantidad);
	       Tarch* TA;
	       while((TA=obtenerParticion(TP))!=NULL){
		    Fclose(TA);
	       }
	  }
	  free(TP->nombre);
	  free(TP);
     }
}
