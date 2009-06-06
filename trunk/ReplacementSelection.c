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


Tparticiones* ReplacementSelection(Tarch* archivo, int palabrasMaximas){

     /* Creo el arbol para almacenar los elementos */ 
     struct rb_table *miArbol = rb_create(compararCadenas, NULL, NULL);
     
     int palabrasAlmacenadas = 0;
     /* Lleno el arbol hasta la cantidad dada */
     while(palabrasAlmacenadas <= palabrasMaximas){
	  char* linea;
	  FreadLn(archivo,(void**)&linea);
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
	       Fwrite(archivoDestino, palabra, strlen(palabra));
	       palabrasAlmacenadas--;

	       /* Obtengo otro termino y lo agrego al arbol */
	       if(!Feof(archivo)){

		    char* linea;
		    FreadLn(archivo,(void**)&linea);
		    rb_insert(miArbol, linea);
		    palabrasAlmacenadas++;
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
