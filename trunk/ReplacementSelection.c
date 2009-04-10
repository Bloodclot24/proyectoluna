#include "ReplacementSelection.h"
#include "particiones.h"
#include <stdlib.h>
#include <string.h>

#define CANTIDAD_A_ALOCAR 20


int compararCadenas(void* cadena1,void* cadena2){
	return strcmp((char*)cadena1,(char*)cadena2);
}

int compararEnteros(void* n1,void* n2){
	if(n1==n2) return 0;
	if(n1>n2) return 1;
	else return -1;
}

Tparticiones* ReplacementSelection(Tarch* archivo,int memoriaAutilizar){
	/*Aloco un buffer de memoria dinamico, de acuerdo a la cantidad pasada*/
	/*Tpalabra* buffer = NULL;

	do{
		buffer = (Tpalabra*)malloc(sizeof(Tpalabra)*memoriaAutilizar);
		memoriaAutilizar--;
	}while (buffer==NULL);*/

	int tamanioBuffer = 0;
	/*Lleno el buffer del tamaño dado*/
	while(tamanioBuffer <= memoriaAutilizar){
	    char* linea;
	    Freadln(archivo,&linea);
	    //Obtengo el termino y lo agrego a la estructura de Tpalabra? o solo el termino?
	    //Inserto en la lista(inserta ordenado!!!)
        //ListaInsertar(&arbol,linea);
        tamanioBuffer++;
    }
    char* ultimaPalabra = NULL;
    char* palabra = NULL;
    Tparticiones* particiones = crearParticiones(strcat(archivo->nombre,"GParticiones"));
    while(retorno = Freadln()){//Ver condicion corte!!!!!
        //Ver como convertir un int a string para agregar lo que ahora esta como "000"
        //ver si si no existe el archivo lo crea
        Tarch arch = Fopen(strcat(archivo.nombre(),"000"),"w");
        //Obtengo el termino---> lo guardo en ul!!!
        while((palabra = ObtenerMayorIgualA(&arbol,ultimaPalabra)) != NULL){
            Fwrite(archivo,palabra,strlen(palabra));
            strcpy(ultimaPalabra,palabra);
        }
        agregarParticion(&particiones,&arch);
        //incrementar variable entero que va al nombre del archivo
    }
    return NULL;
}
