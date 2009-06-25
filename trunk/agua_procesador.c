#include "parser.h"
#include "matrizBayes.h"
#include "archivo.h"
#include "particiones.h"
#include "ReplacementSelection.h"
#include "merge.h"

#include <errno.h>

int main(int argc, char** argv){
 
     if(argc < 3){
	  printf("Debe especificar el archivo xml de la wikipedia y un nombre para la\
base de datos.\n");
	  return 0;
     }

     printf("Comenzando el parseo...\n");

     Parser *p = Parser_Crear(argv[1], 5000000, 10);

     Parser_Procesar(p);

     Tarch *archivo = Fopen("Lexico.txt","r");

     printf("Ordenando el lexico...\n");

     Tparticiones *TP = ReplacementSelection(archivo, 500000);

     printf("Listo, merge de %d particiones con 20 particiones por pasada...\n", TP->cantidad);

     archivo = mergeLexico(TP, 20);

     if(archivo == NULL){
	  char* error =strerror(errno);
	  printf("Error con el arrchivo %s: %s.\n", argv[1], error);
	  return -1;
     }

     printf("Armando la matriz ...\n");

     Matriz *X;
     armarMatriz(archivo, argv[2]);
     X = cargarMatriz(argv[2]);
     X->numFilas = 4224832;
     X->numColumnas = 323415;

     printf("Calculando los hiperparametros ...\n");
     HiperParametros *H = BSParam(X,2);

     char *nombre = malloc(strlen(argv[2])+strlen(HIPERPARAM)+1);
     nombre[0]=0;
     strcat(nombre, argv[2]);
     strcat(nombre, HIPERPARAM);
     Tarch *hiper = Fopen(nombre, "w");
     free(nombre);

     int i;
     for(i=0;i<X->numColumnas;i++){
	  Fwrite(hiper, (H->alpha+i), sizeof(double));
	  Fwrite(hiper, (H->beta+i), sizeof(double));
     }

     Fclose(hiper);

     printf("Listo.\n");
     return 0;
}
