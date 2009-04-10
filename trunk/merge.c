#include "merge.h"

void merge(Tparticiones *Gparticion){
  int cantidad = Gparticion ->cantidad;
  Tarch *archivo[cantidad];
  int i;
  int posMenor =0;
  char*  menor;
  char** palabraAComparar;
  int n=4; // n = cantidad de archivos a abrir.
  char* Eof= (char*) malloc(sizeof(char)*n);
  unsigned int nEof= 0;
  Tarch *archFinal= Fopen("archivoFinal", "w");
  
  for(i=0; i<cantidad; i++)
  		archivo[i]=  obtenerParticion(Gparticion);
  
  for(i=0; i<n; i++) {
  	FreadLn(archivo[i], (void**)&palabraAComparar[i]);
  	Eof[i]= '0';
  }
  
  /*for(i=0; i<n; i++) {
  	printf("%s\n", palabraAComparar[i]);
  }
  
  printf("Pos menor %d\n", posMenor );
  */
  while(nEof <= n+1) {
	  menor= palabraAComparar[posMenor];
	  printf("El menor: %s\n", menor);
	  for(i=0; i<n ; i++){
		if(Eof[i] != '1') {
			if(strcmp (menor, palabraAComparar[i]) > 0){
		    	menor = palabraAComparar[i];
		    	posMenor = i;
		    	printf("El menor nuevo: %s\n", menor);
		    }
		}
	  }
		Fwrite(archFinal, menor, strlen(menor));
		FreadLn(archivo[posMenor], (void**)&palabraAComparar[posMenor]);	  
		
		if(feof(archivo[posMenor]->fd)) {
		 	Eof[posMenor]= '1';
		  	nEof++;
		  	printf("termino %d\n", posMenor);
		  	
		  	//posMenor= 1;
		  }
		 	
	 
		  
	  }
 
  

    
}


