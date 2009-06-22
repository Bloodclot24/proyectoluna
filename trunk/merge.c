#include "merge.h"

int compararDocumentos(const void* cadena1, const void* cadena2, const void* parametroExtra){
int doc1, doc2, resultado;
	doc1 = ((int*)cadena1)[0];
	doc2 = ((int*)cadena2)[0];

	if(doc1>doc2) resultado = 1;
	else if(doc1<doc2) resultado = -1;
	else resultado = 0;
	return resultado;
}

void* procRegistro(Lista* lista, const void* parametroExtra){
int frecuencia;
int documento;
int longTermino;
int longReg;
int posReg=0;
char* termino;
void* reg;
void* regFinal;
void* docFrec;
void* auxDocFrec;
int nada[2]= {0,0};
Lista* listaAux = Lista_Crear();
struct rb_table *miArbol = rb_create((rb_comparison_func*)compararDocumentos, NULL, NULL);

	reg = Lista_RemoverPrimero(lista);
	longTermino=((int*)reg)[0];
	longReg = ((int*)reg)[1];
	if(longTermino > 14) longTermino = 14;
	termino = (char*)malloc(sizeof(longTermino)+1);
	strncpy(termino,((char*)reg+8),longTermino);
	termino[longTermino]='\0';
	
	printf("Termino: %s \n", termino);
	printf("longitud del termino:%i \n",longTermino);
    longReg /=8;
    
    printf("longitud del reg:%i \n",longReg);
    
do{//printf("cantidad de elem en lista: %i \n",Lista_CantidadElem(lista) );
	//posReg es la posicion del primer par de Doc-Frec. 
	//longReg es la cantidad de par Par de Doc-Frec que tiene el registro.
	posReg = longTermino + 2*sizeof(int);
	while(longReg > 0){
		docFrec = (int*)malloc(sizeof(int)*2);
		memcpy(docFrec,reg+posReg,(sizeof(int)*2));
		posReg+=(2*sizeof(int));
		rb_insert(miArbol, docFrec);
		//free(docFrec); //preguntar si esta bien!!!
		longReg--;
	}
	//free(docFrec); //preguntar si esta bien!!!
	//free(reg);
	if(!(Lista_EstaVacia(lista))){
		reg = Lista_RemoverPrimero(lista);
		longTermino=((int*)reg)[0];
		longReg = ((int*)reg)[1];
	}
}
while(!(Lista_EstaVacia(lista)));
/*
 * guardarde todos los pares  Doc-Frec del mismo termino en el arbol.
 * 
 */	

//auxDocFrec = (int*)malloc(sizeof(int)*2);
int docAnterior=-1;

while((auxDocFrec = rb_remover_mayor_igual(miArbol,nada)) != NULL){
	documento = ((int*)auxDocFrec)[1];
	if(docAnterior == -1) docAnterior = documento;
	else if(docAnterior == documento) frecuencia += ((int*)auxDocFrec)[0];
	else{ //aca el documento es distinto.
		docFrec = (int*)malloc(sizeof(int)*2);
		memcpy(docFrec,&docAnterior,(sizeof(int)));
		memcpy(docFrec+sizeof(int),&frecuencia,(sizeof(int)));
		Lista_Insertar(listaAux,docFrec);
		docAnterior = -1;
		frecuencia = 0;
		}
	//if(auxDocFrec != NULL)free(auxDocFrec);
}
//el tamanio de la lista va a ser la cantidad de 
//par doc-frec que tengo que guardar.
int tamLista = Lista_CantidadElem(listaAux);
longReg = tamLista*sizeof(int)*2;

int tamRegistro = (longReg*sizeof(int)) + longTermino + 2*sizeof(int);
longReg *=8;
	
regFinal = malloc(tamRegistro);	
int posicion = 0;
void*elem;
//copio a regFinal la longitud del termino
memcpy(regFinal+posicion,&longTermino,sizeof(int));
posicion+=sizeof(int);

//copio a regFinal la longitud del registro
memcpy(regFinal+posicion,&longReg,sizeof(int));
posicion+=sizeof(int);

//copio el termino.
memcpy(regFinal+posicion,termino,longTermino);
posicion+=longTermino;

//copio todos los pares Doc-Frec que estan en la lista. 
while(!Lista_EstaVacia(listaAux)){
	elem = Lista_RemoverPrimero(listaAux);
	memcpy(regFinal+posicion,elem,(2*sizeof(int)));
	posicion+=(2*sizeof(int));
	free(elem);
}
Lista_Liberar(listaAux);
//free(listaAux);
//free(termino);
//free(docFrec);
return regFinal;
}


void imprimirTermino(void*datos){
	int longTermino = ((int*)datos)[0];
	if(longTermino<15){
		char* termino = (char*)malloc(sizeof(longTermino)+1);
		//strncpy(termino,((char*)datos+8),longTermino);
		memcpy(termino,((char*)datos+8),longTermino);
		termino[longTermino]='\0';
		printf("Longitud: %i  ",longTermino);	
		printf("Termino: %s \n",termino);	
		//free(termino);
		termino=NULL;
		}
 }
 
int compararTerminos(const void* cadena1, const void* cadena2, const void* parametroExtra){
     return strncmp(cadena1+2*sizeof(int),				\
		    cadena2+2*sizeof(int),				\
		    ((int*)cadena1)[0]>((int*)cadena2)[0]?		\
		    ((int*)cadena1)[0]:((int*)cadena2)[0]);
}

Tarch* mergeLexico(Tparticiones *Gparticion, int cantidad){
	if(cantidad <= 1) cantidad = 2; /* No se puede hacer un mergecon una sola particion */

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
	int contador=0;
    Tarch *archFinal;
	
	void* registro;
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
		    	//FreadLn(archivo[i], (void**)&palabraAComparar[i]);
		    	FreadReg(archivo[i], (void**)&palabraAComparar[i]);
		    	Eof[i]= '0';
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
				//if(strcmp(menor, palabraAComparar[i]) > 0){
			    if(compararTerminos(menor, palabraAComparar[i],0) > 0){
			    	menor = palabraAComparar[i];
			    	posMenor = i;
			 	}
		    }
	    }
	
		  //-------------------------------------------------------------------------
    //AAABBBCC
    if(strcmp(auxMenor, "\0")==0){ 
    	listaTerminos = Lista_Crear();
   	 	auxMenor = menor;//strcpy(auxmenor,menor);
    }
    if(compararTerminos(menor, auxMenor,0)==0 ){
		Lista_Insertar(listaTerminos,menor); //fijarse si hay  que insertar menor o auxmenor.
	}
    else{
	//datos en el archivo (longTermino,longreg/doc-frec,termino,doc-frec,doc-frec...)
		registro = procRegistro(listaTerminos,0);
		//printf("---------termino de procesar el registro.------------\n");
		FwriteReg(archFinal, registro);
		Lista_Liberar(listaTerminos);
	  	free(registro);
	  	auxMenor=menor;
	  	listaTerminos = Lista_Crear();
		Lista_Insertar(listaTerminos,menor); //fijarse si hay  que insertar menor o auxmenor. 
    }  
    //-------------------------------------------------   
   		
		//Fwrite(archFinal, menor, strlen(menor));
	    //>FwriteReg(archFinal, menor);
	    //imprimirTermino(menor);
	    //FreadLn(archivo[posMenor], (void**)&palabraAComparar[posMenor]);
	    FreadReg(archivo[posMenor], (void**)&palabraAComparar[posMenor]);
	       	
	    if((Feof(archivo[posMenor]))||(palabraAComparar[posMenor]==NULL)){
		printf("termino el archivo: %i \n",posMenor);
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
	free(menor);
	free(Eof);
	
	//for(i=0; i<n;i++) free(archivo[i]);
    free(archivo);
    
    for(i=0; i<n;i++) free(palabraAComparar[i]);
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
