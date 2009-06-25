#include "matrizBayes.h"
#include "archivo.h"

#include <errno.h>

int main(int argc, char** argv){
 
     if(argc < 3){
	  printf("Debe especificar el archivo que resulta del merge y un prefijo para\
los archivos resultantes.\n");
	  return 0;
     }
     
     Tarch * archivo = Fopen(argv[1], "r");
     if(archivo == NULL){
	  char* error =strerror(errno);
	  printf("Error con el arrchivo %s: %s.\n", argv[1], error);
	  return;
     }

     printf("Armando la matriz ...\n");

     Matriz *X;
     armarMatriz(archivo, argv[2]);
     X = cargarMatriz(argv[2]);

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
