#include "ReplacementSelection.h"
#include "particiones.h"
#include "rb.h"
#include <stdlib.h>
#include <string.h>


#define CANTIDAD_A_ALOCAR 20


int compararCadenas(const void* cadena1, const void* cadena2, const void* parametroExtra){
     return strcmp((char*)cadena1,(char*)cadena2);
}

int compararRegistros(const void* cadena1, const void* cadena2, const void* parametroExtra){
     return strncmp((char*)cadena1,(char*)cadena2,16);
}

int compararLexico(const void* cadena1, const void* cadena2, const void* parametroExtra){
/* 	char* cad1 = (char*) cadena1; */
/* 	char* cad2 = (char*) cadena2; */
/* 	char* longitudString1 = (char*) malloc(sizeof(int)); */
/* 	strncpy(longitudString1,cad1,4); */
/* 	char* longitudString2 = (char*) malloc(sizeof(int));; */
/* 	strncpy(longitudString2,cad2,4); */
/* 	int longitud1 = atoi(longitudString1); */
/* 	int longitud2 = atoi(longitudString2); */
/* 	char* termino1 = (char*) malloc(sizeof(char)*longitud1); */
/* 	char* termino2 = (char*) malloc(sizeof(char)*longitud2); */
/* 	cadena1+=7; */
/* 	strncpy(termino1,cad1,longitud1); */
/* 	cadena2+=7; */
/* 	strncpy(termino2,cad2,longitud2); */
/* 	free(longitudString1); */
/* 	free(longitudString2); */
/* 	int resultado = strcmp(termino1,termino2); */
/* 	free(termino1); */
/* 	free(termino2); */
/* 	return resultado; */

     return strncmp(cadena1+2*sizeof(int),				\
		    cadena2+2*sizeof(int),				\
		    ((int*)cadena1)[0]>((int*)cadena2)[0]?		\
		    ((int*)cadena1)[0]:((int*)cadena2)[0]);
}

Tparticiones* ReplacementSelection(Tarch* archivo, int palabrasMaximas){

     /* Creo el arbol para almacenar los elementos */ 
     struct rb_table *miArbol = rb_create((rb_comparison_func*)compararLexico, NULL, NULL);
     
     int palabrasAlmacenadas = 0;
     /* Lleno el arbol hasta la cantidad dada */
     while(palabrasAlmacenadas <= palabrasMaximas){
	  char* linea;
	  FreadReg(archivo,(void**)&linea);
	  /* Obtengo el termino y lo agrego al arbol */
	  rb_insert(miArbol, linea);
	  palabrasAlmacenadas++;
     }
     

     char *nombreGParticion = malloc(strlen(archivo->nombre)+strlen("GParticiones")+1);
     strcpy(nombreGParticion, archivo->nombre);
     strcat(nombreGParticion,"GParticiones");
     Tparticiones* particiones = crearParticiones(nombreGParticion);
     
     free(nombreGParticion);

     int contadorArchivo=0;
     char* nombreArchivo; /* El nombre del archivo destino */
     
     int continuar = 1;
     
     while(!Feof(archivo) || continuar){
	  continuar=!Feof(archivo); /* Para saber si es la ultima
				       vuelta. Si entramos al loop y
				       el archivo da EOF, quiere decir
				       que no vamos a leer mas
				       terminos, entonces solo debemos
				       vaciar el arbol */
	  int primero = 1;
	  char* ultimaPalabra = "\0";
	  char* palabra = NULL;
	  
	  /* creo el nombre del archivo destino */
	  nombreArchivo = strcatint(archivo->nombre, contadorArchivo);

	  /* Incremento el contador, asi el proximo archivo tiene un
	   * nombre diferente */
	  contadorArchivo++;

	  /* Creo el archivo */
	  Tarch *archivoDestino = Fopen(nombreArchivo,"w");
	  
	  /* Obtengo el termino---> lo guardo en la particion destino */
	  while((palabra = rb_remover_mayor_igual(miArbol,ultimaPalabra)) != NULL){
	       FwriteReg(archivoDestino, palabra);
	       palabrasAlmacenadas--;

	       /* Obtengo otro termino y lo agrego al arbol */
	       if(!Feof(archivo)){

		    char* linea;
		    FreadReg(archivo,(void**)&linea);
		    if(linea != NULL){
			 rb_insert(miArbol, linea);
			 palabrasAlmacenadas++;
		    }
	       }
	       
	       char *auxiliar = ultimaPalabra;
	       ultimaPalabra = palabra;
	       if(palabra != auxiliar && !primero){
	       	    free(auxiliar);
	       }
	       primero = 0;
	       
	  }
	  if(!primero)
	       free(ultimaPalabra);
	  Frewind(archivoDestino);
	  agregarParticion(particiones,archivoDestino);
     }

     rb_destroy(miArbol, NULL);
     
     return particiones;
}
