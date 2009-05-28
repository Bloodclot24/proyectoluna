#include "particiones.h"

Tparticiones* crearParticiones(const char* nombre){
     Tparticiones *TP = NULL;
     TP = (Tparticiones*)malloc(sizeof(Tparticiones));
     TP->nombre = (char*)malloc(strlen(nombre)+1);
     strcpy(TP->nombre, nombre);
     TP->cantidad = 0;
     TP->listaParticiones = Lista_Crear();
     return TP;
}

void agregarParticion(Tparticiones* TP, Tarch* TA){
     if(TP != NULL && TA != NULL){
	  Lista_Insertar(TP->listaParticiones, (void*)TA);
	  TP->cantidad++;
     }
}

Tarch* obtenerParticion(Tparticiones* TP){
     Tarch* TA=NULL;
     if(TP != NULL){
	  if(!Lista_EstaVacia(TP->listaParticiones)){
	       TA = (Tarch*)Lista_RemoverPrimero(TP->listaParticiones);
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
	  Lista_Liberar(TP->listaParticiones);
	  free(TP->nombre);
	  free(TP);
     }
}


char* strcatint(const char* src, uint16_t num){
     #define STRCATINT_DIGITOS 5
     char contador[STRCATINT_DIGITOS+1];
     char* nuevoString = (char*)malloc(strlen(src)+STRCATINT_DIGITOS+1);
     sprintf(contador, "%05d", num);
     contador[STRCATINT_DIGITOS] = '\0';
     strcpy(nuevoString, src);
     strcat(nuevoString, contador);
     return nuevoString;
}
