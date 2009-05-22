#include "merge.h"

void merge(Tparticiones *Gparticion){
  int n=3; // n = cantidad de archivos a abrir.
  int i;
  int posMenor=0;
  unsigned int nEof=0;
  char* menor;
  char* Eof;
  char** palabraAComparar;
  Tarch *archivo[n];
  
  Eof = (char*) malloc(sizeof(char)*n);
  palabraAComparar = (char**)malloc(n*sizeof(char*));
  Tarch *archFinal= Fopen("archivoFinal", "w");
  
  while((Gparticion->cantidad)>0 ){
  if((Gparticion->cantidad) < n) n=Gparticion->cantidad;
  
  //Obtengo n particiones del grupo de particiones. 
  for(i=0; i<n; i++) archivo[i] = obtenerParticion(Gparticion);  
		
  //Leo la primer palabra de cada particion de n cantidad de archivos.
  //inicializo el vector Eof[] con flags para saber si el archivo termino.
  for(i=0; i<n; i++) {
  	if(!feof(archivo[i]->fd)){ 
  		FreadLn(archivo[i], (void**)&palabraAComparar[i]);
   		Eof[i]= '0';
  	}
  	else{ 
  		Eof[i]= '1';
  		nEof ++;
  	}
  }
  	
  while(nEof < n) {
 	if(Eof[posMenor] == '1'){
   		posMenor = 0;
   		while(Eof[posMenor]=='1') posMenor ++;
 	}		
 	menor = palabraAComparar[posMenor];
    for(i=0; i<n ; i++){
    	if(Eof[i] != '1') {
			if(strcmp (menor, palabraAComparar[i]) > 0){
	  			menor = palabraAComparar[i];
	  			posMenor = i;
			}
     	 }
    }
  	Fwrite(archFinal, menor, strlen(menor));
   
    if(feof(archivo[posMenor]->fd)) {
    	Eof[posMenor]= '1';
      	nEof++;
    }
    else {
    	FreadLn(archivo[posMenor], (void**)&palabraAComparar[posMenor]);	  
    }
  }//fin del while de las n particiones.
  nEof=0;
  }//fin del while de todo el grupo de particiones.
  
}
