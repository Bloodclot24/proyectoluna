#include "merge.h"


Tarch* merge2(Tparticiones *Gparticion, int cantidad){
     if(cantidad <= 1)
	  cantidad = 2; /* No se puede hacer un mergecon una sola particion */

     char* nuevoNombre;
     char numero[6];
	
     nuevoNombre = malloc(strlen(Gparticion->nombre)+5);
     strcpy(nuevoNombre, Gparticion->nombre);
     strcat(nuevoNombre, "_bis");
     Tparticiones* nuevoGrupo = crearParticiones(nuevoNombre);
     free(nuevoNombre);
     int frec=1;
     
     int n=cantidad; // n = cantidad de archivos a abrir.
     int i;
     int posMenor=0;
     unsigned int nEof=0;  // cantidad de particiones que llegaron a EOF
     char* menor ="\0";   // Termino menor
     char* Eof = (char*) malloc(n*sizeof(char));
     char** palabraAComparar = (char**)malloc(n*sizeof(char*));
     Tarch **archivo = (Tarch**)malloc(n*sizeof(Tarch*));
	 char *auxmenor="\0";
     int contador=0;
	 char *frecchar = (char*)malloc(sizeof(int));
	 
	 	
     Tarch *archFinal;

     for(i=0;i<n;i++) palabraAComparar[i] = NULL;

     
     while((Gparticion->cantidad)>0 ){
//	  menor = NULL;
	  /* Abro el archivo destino  e incremento el contador */
	  nuevoNombre = strcatint(Gparticion->nombre, contador++);
	  archFinal = Fopen(nuevoNombre, "w");
	  free(nuevoNombre);
	  
	  if(Gparticion->cantidad < n) n=Gparticion->cantidad;
	  
	  /* Obtengo n particiones del grupo de particiones. */
	  for(i=0; i<n; i++) archivo[i] = obtenerParticion(Gparticion);  
	  
	  //Leo la primer palabra de cada particion de n cantidad de archivos.
	  //inicializo el vector Eof[] con flags para saber si el archivo termino.
	  for(i=0; i<n; i++) {
	       if(!Feof(archivo[i])){ 
		    FreadLn(archivo[i], (void**)&palabraAComparar[i]);
		    Eof[i]= '0';
	       }
	       else{ 
		    Eof[i]= '1';
		    Funlink(archivo[i]); /* llego a EOF, elarchivo no
					  * me sirve mas, lo borro */
		    nEof ++;
	       }
	  }
	  
	  
	  
	  while(nEof < n){
	  	if(Eof[posMenor] == '1'){
			posMenor = 0;
		   	while(Eof[posMenor]=='1') posMenor ++;
	    }		
	    
	    menor = palabraAComparar[posMenor];
	    for(i=0; i<n ; i++){
			if(Eof[i] != '1') {
				if(strcmp(menor, palabraAComparar[i]) > 0){
			    	menor = palabraAComparar[i];
			    	posMenor = i;
			 	}
		    }
	    }
       if(strcmp(menor, auxmenor)!=0 ){
			Fwrite(archFinal, menor, strlen(menor));
       }
      	auxmenor = menor;
        //free(menor);
	   
	   if(Feof(archivo[posMenor])){
	   		Eof[posMenor]= '1';
		    /* llego a EOF, el archivo no sirve mas, lo borro*/
		   	Funlink(archivo[posMenor]); 
		    nEof++;
	      }
	   else {
		    FreadLn(archivo[posMenor], (void**)&palabraAComparar[posMenor]);
	       }
	  }//fin del while de las n particiones.

	  /* Vuelvo el archivo destino al principio, para que quien lo
	   * quiera usar a continuacion no tenga problemas */
	  Frewind(archFinal);
	  agregarParticion(nuevoGrupo, archFinal);
	  nEof=0;
     }//fin del while de todo el grupo de particiones.

     
     free(Eof);
     free(archivo);
     free(palabraAComparar);

     eliminarParticiones(Gparticion);

     if(nuevoGrupo->cantidad > 1){
	  return merge2(nuevoGrupo, cantidad);
     }
     else{
	  Tarch* final = obtenerParticion(nuevoGrupo);
	  eliminarParticiones(nuevoGrupo);
	  return final;
     }
}
