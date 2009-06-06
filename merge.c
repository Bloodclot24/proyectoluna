#include "merge.h"

Tarch* mergeLexico(Tparticiones *Gparticion, int cantidad){
     if(cantidad <= 1)
	  cantidad = 2; /* No se puede hacer un mergecon una sola particion */

     char* nuevoNombre;
  
     nuevoNombre = malloc(strlen(Gparticion->nombre)+5);
     strcpy(nuevoNombre, Gparticion->nombre);
     strcat(nuevoNombre, "_bis");
     Tparticiones* nuevoGrupo = crearParticiones(nuevoNombre);
     free(nuevoNombre);
     
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
	  return mergeLexico(nuevoGrupo, cantidad);
     }
     else{
	  Tarch* final = obtenerParticion(nuevoGrupo);
	  eliminarParticiones(nuevoGrupo);
	  return final;
     }
}

int comparador(void* elem1, void* elem2, void* datos) {
	
	if(*(int*)elem1 > *(int*)elem2)	
		return 1;
	else if(*(int*)elem1 < *(int*)elem2)
		return -1;
	else
		return 0;
}

void parsearRegistro(Tarch* archivo, int* longitud, int* num_termino, int* frecuencia,
                     int* documentos) {

	Fread(archivo, longitud, sizeof(int));
	Fread(archivo, num_termino, sizeof(int));
	Fread(archivo, frecuencia, sizeof(int));
	documentos= (int*) malloc(*longitud-8);
	fread(documentos, *longitud-8, 8, archivo->fd);
}

Tarch* mergeAuxiliar(Tparticiones *Gparticion, int cantidad){

     if(cantidad <= 1)
	  cantidad = 2; /* No se puede hacer un mergecon una sola particion */

     char* nuevoNombre;
  
     nuevoNombre = malloc(strlen(Gparticion->nombre)+5);
     strcpy(nuevoNombre, Gparticion->nombre);
     strcat(nuevoNombre, "_bis");
     Tparticiones* nuevoGrupo = crearParticiones(nuevoNombre);
     free(nuevoNombre);
     
     int n=cantidad; // n = cantidad de archivos a abrir.
     int i;
     int posMenor=0;
     unsigned int nEof=0;  // cantidad de particiones que llegaron a EOF
     int menor;   // Termino menor
     char* Eof = (char*) malloc(n*sizeof(char));
     Tarch **archivo = (Tarch**)malloc(n*sizeof(Tarch*));
	 int auxmenor= 0;
     int contador=0;
	 int* docum_frec;
     Tarch *archFinal;

	//funcion parser
	int* longitud= (int*) malloc(sizeof(int));
	int* num_termino= (int*) malloc(sizeof(int));
	int* frecuencia= (int*) malloc(sizeof(int));
	int* documentos;
  
  	//rb
  	struct rb_table *arbol= rb_create((rb_comparison_func*)comparador,NULL,NULL);
  	int cero=0;
  	
    while((Gparticion->cantidad)>0 ){

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
		    parsearRegistro(archivo[i], longitud, num_termino, frecuencia, documentos);
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
	    
	    menor= *num_termino;
	    for(i=0; i<n ; i++){
			if(Eof[i] != '1') {
				if(menor > *num_termino){
			    	menor= *num_termino;
			    	posMenor = i;
			 	}
		    }
	    }
       
       if(auxmenor == 0)
       		auxmenor= menor;
       
       if(menor == auxmenor)
       		//rb_insert_unique_auxs(arbol, documentos);
       		rb_insert(arbol, documentos);
       else {
			//tam_arbol= rb_size(arbol);
			//Fwrite(archFinal, &tam_arbol, sizeof(tam_arbol));
			Fwrite(archFinal, &auxmenor, sizeof(auxmenor));
									
			//while((docum_frec= (int*) rb_remover_mayor_igual(arbol, &cero)) != NULL) {
				//Fwrite(archFinal, &docum_frec, sizeof(docum_frec)*2);
			//}
			
			
			//TODO: se debe insertar el primer distinto al anterior
			//rb_insert(arbol, documentos);
       }
              
      auxmenor = menor;
	   
	   if(Feof(archivo[posMenor])){
	   		Eof[posMenor]= '1';
		    /* llego a EOF, el archivo no sirve mas, lo borro*/
		   	Funlink(archivo[posMenor]); 
		    nEof++;
	      }
	   else {
		    parsearRegistro(archivo[posMenor], longitud, num_termino, frecuencia, documentos);
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

     eliminarParticiones(Gparticion);

     if(nuevoGrupo->cantidad > 1){
	  return mergeLexico(nuevoGrupo, cantidad);
     }
     else{
	  Tarch* final = obtenerParticion(nuevoGrupo);
	  eliminarParticiones(nuevoGrupo);
	  return final;
     }
}