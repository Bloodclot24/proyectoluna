#include "rb.h"
#include <stdio.h>

int comparador(const void* a, const void* b, void* param){
     return strcmp((const char*)a,(const char*)b);
}

int main(int arg, char** argv){

     char* listaPalabras[] = {"Palabra1","Zorro","Casa","Avion","Plato","Notebook","Sort","Tecla","Repetida","Arbol","Datos","Repetida","Sarasa","Amarok","Frutilla","Saubidet","Silla","Mesa","Repetida","Merge","Wikipedia","\0\0"};

     struct rb_table *tabla1 = rb_create(comparador,NULL,NULL);
     int i=0;
     for(i=0;strcmp(listaPalabras[i], "\0")!=0;i++){

	  rb_insert(tabla1, listaPalabras[i]);
	  printf("Insertando Palabra: \"%s", listaPalabras[i]);
	  printf("\"\n");
     }

     struct rb_traverser traverser;

     printf("\nRecorriendo el arbol de forma ordenada:\n");
     rb_t_init(&traverser, tabla1);
     char* palabra;
     while((palabra=rb_t_next(&traverser))!= NULL){
	  printf("%s\n", palabra);
     }


     printf("\nRemoviendo nodos mayores o iguales a \"Malaria\":\n");
     while((palabra=rb_remover_mayor_igual(tabla1, "Malaria"))!=NULL){
	  printf("%s\n", palabra);
     }
     return 0;
}
