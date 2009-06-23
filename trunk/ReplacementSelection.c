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
     int resultado = strncmp(RegGetWord(cadena1),			\
			     RegGetWord(cadena2),			\
			     (RegGetWordLength(cadena1))>(RegGetWordLength(cadena2))? \
			     (RegGetWordLength(cadena2)):(RegGetWordLength(cadena1)));
     if(resultado != 0)
	  return resultado;

     if(RegGetWordLength(cadena1) > RegGetWordLength(cadena2))
	  return 1;
     if(RegGetWordLength(cadena1) < RegGetWordLength(cadena2))
	  return -1;
     return strncmp(RegGetWord(cadena1),				\
		    RegGetWord(cadena2),				\
		    (RegLength(cadena1))>(RegLength(cadena2))?		\
		    (RegLength(cadena2)-8):(RegLength(cadena1)-8));
}

Tparticiones* ReplacementSelection(Tarch* archivo, int palabrasMaximas){

     /* Creo el arbol para almacenar los elementos */ 
     struct rb_table *miArbol = rb_create((rb_comparison_func*)compararLexico, NULL, NULL);
     
     int palabrasAlmacenadas = 0;
     /* Lleno el arbol hasta la cantidad dada */
     while(palabrasAlmacenadas <= palabrasMaximas && !Feof(archivo)){
	  char* linea;
	  FreadReg(archivo,(void**)&linea);
	  if(Feof(archivo))
	     break;
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
	  char* ultimaPalabra = "\0\0\0\0\0\0\0\0\0";
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
