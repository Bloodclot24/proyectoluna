#include "merge.h"

void* mergePunteros(uint32_t* pointers, uint32_t *numPointers, void* reg){
     int i=0;
     if(pointers == NULL){
	  pointers = malloc(RegGetPointersLength(reg)*sizeof(uint32_t));
	  for(i=0;i<RegGetNumPointers(reg);i++){
	       pointers[i] = RegGetPointer(reg,i);
	  }
	  *numPointers = i;
	  return pointers;
     }
     int finalizado = 0;
     int j=0;

     while(!finalizado){
	  if(pointers[i] < RegGetPointer(reg,j)){
	       i+=2;
	  }else if(pointers[i] > RegGetPointer(reg,j)){
	       void* newPointers=malloc((*numPointers+2)*sizeof(uint32_t));
	       memcpy(newPointers, pointers, i*sizeof(uint32_t));
	       memcpy(newPointers+(i+2)*sizeof(uint32_t), pointers+i, ((*numPointers)-i)*sizeof(uint32_t));
	       free(pointers);
	       pointers = newPointers;
	       
	       pointers[i++]=RegGetPointer(reg,j++);
	       (*numPointers)++;
	       pointers[i++]=RegGetPointer(reg,j++);
	       (*numPointers)++;
	  }else {//((uint32_t*)pointers[i] == RegGetPointer(reg,j))
	       i++;
	       j++;
	       pointers[i++] += RegGetPointer(reg,j++);
	  }

	  if(i+1 >= *numPointers || j+1 >= RegGetNumPointers(reg))
	       finalizado = 1;
     }

     if(j+1 < RegGetNumPointers(reg)){
	  pointers = realloc(pointers, *(numPointers)*sizeof(uint32_t)+(RegGetNumPointers(reg)-j)*sizeof(uint32_t));
	  while(j<RegGetNumPointers(reg)){
	       pointers[i++]=RegGetPointer(reg,j++);
	       (*numPointers)++;
	       pointers[i++]=RegGetPointer(reg,j++);
	       (*numPointers)++;
	  }
     }

     return pointers;
}
 
int compararTerminos(const void* cadena1, const void* cadena2, const void* parametroExtra){

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
     return 0;
}

Tarch* mergeLexico(Tparticiones *Gparticion, int cantidad){
     if(cantidad <= 1) cantidad = 2; /* No se puede hacer un merge con una sola particion */

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
     char* menor ="\0";    // Termino menor
     char* Eof = (char*) malloc(n*sizeof(char));
     char** palabraAComparar = (char**)malloc(n*sizeof(char*));
     Tarch **archivo = (Tarch**)malloc(n*sizeof(Tarch*));
     int contador=0;
     Tarch *archFinal;
     
     uint32_t* punteros=NULL;
     int cantidadPunteros=0;

     char* auxMenor = "\0";
     Lista* listaTerminos;

     for(i=0;i<n;i++) palabraAComparar[i] = NULL;

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
		    FreadReg(archivo[i], (void**)&palabraAComparar[i]);
		    Eof[i]= Feof(archivo[i]);
	       }
	       else{ 
		    Eof[i]= '1';
		    Funlink(archivo[i]); /* llego a EOF, elarchivo no me sirve mas, lo borro */
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
			 if(compararTerminos(menor, palabraAComparar[i],0) > 0){
			      menor = palabraAComparar[i];
			      posMenor = i;
			 }
		    }
	       }
	
	       //-------------------------------------------------------------------------
	       //AAABBBCC
	       if(strcmp(auxMenor, "\0")==0){
		    auxMenor = menor;
		    punteros=NULL;
		    cantidadPunteros = 0;
	       }



	       if(compararTerminos(menor, auxMenor,0)==0 ){
		    punteros = mergePunteros(punteros, &cantidadPunteros, menor);
		    if(menor!=auxMenor)
			 free(menor);
		    }
	       else{
		    auxMenor = RegSetPointers((void*)auxMenor, punteros, cantidadPunteros);
		    FwriteReg(archFinal, auxMenor);
		    free(auxMenor);
		    free(punteros);
		    auxMenor=menor;
		    punteros=NULL;
		    cantidadPunteros = 0;
		    punteros = mergePunteros(punteros, &cantidadPunteros, menor);
	       }
	       FreadReg(archivo[posMenor], (void**)&palabraAComparar[posMenor]);

	       if((Feof(archivo[posMenor]))||(palabraAComparar[posMenor]==NULL)){
		    Eof[posMenor]= '1';
		    Funlink(archivo[posMenor]);/* llego a EOF, el archivo no sirve mas, lo borro*/
		    nEof++;
	       }
	  }//fin del while de las n particiones.
	  /* Vuelvo el archivo destino al principio, para que quien lo
	   * quiera usar a continuacion no tenga problemas 
	   */
	   
	  Frewind(archFinal);
	  agregarParticion(nuevoGrupo, archFinal);
	  nEof=0;
     }//fin del while de todo el grupo de particiones.
//     free(menor);
//     free(Eof);
	
     //for(i=0; i<n;i++) free(archivo[i]);
     //free(archivo);
    
     for(i=0; i<n;i++) free(palabraAComparar[i]);
     //free(palabraAComparar);
    
     eliminarParticiones(Gparticion);
     if(nuevoGrupo->cantidad > 1){
	  return mergeLexico(nuevoGrupo, cantidad);
     }
     else{
	  Tarch* final = obtenerParticion(nuevoGrupo);
	  eliminarParticiones(nuevoGrupo);
	  Frewind(final);
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
