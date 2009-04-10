
#include "archivo.h"
#include "merge.h"
#include "Lista.h"

void merge(Tparticiones *Gparticion){
  int cantidad = Gparticion ->cantidad;
  Tarch archivo[cantidad];
  int i;
  int posMenor = 0;
  char*  menor, actual;
  char ** palabraAComparar;
  Tarch archFinal;
  int n=3; // n = cantidad de archivos a abrir.
  archFinal = Fopen("archivoFinal", "w");
  
  for(i=0; i<n; i++){
    Fread(archivo[i],palabraAComparar, )
    Fread(fd,*palabraAComparar[i]) 
      }
  
  while(){ // agregar condicion de corte...
    strcpy(menor,palabraAComparar[posMenor]);
      for(i=0; i<n; i++){
	if(strcmp (menor, palabraAComparar[i])>0){//revisar comparacion
	  menor = palabraAComparar[i];
	  porMenor = i;
	}
      }
    
    Fwrite(archFinal->fd,menor);
    
  }
  

