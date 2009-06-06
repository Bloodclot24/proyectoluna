#include "matrizBayes.h"

Tarch* armarMatriz(Tarch* archAuxiliar, Tarch* lexico, int cant_doc) {
	
	Tarch *archFinal;
	char* linea;
	int cant_terminos= 0;
	
	int* fila= (int*) malloc(cant_doc*sizeof(int));
	
	while(!Feof(lexico)) {
		FreadLn(lexico, (void**)&linea);
		
		//leer archAuxiliar MODIFICAR FORMA EN QUE GUARDA
		//buscar el termino en auxiliar
		//en fila guardar frecuencia
		//cant_terminos += frecuencia_docs
		//Normalizar frecuencia matriz
		//Guardar fila en matriz
	}
	
	
	//devuelve 3 archivos...
	return archFinal;
}