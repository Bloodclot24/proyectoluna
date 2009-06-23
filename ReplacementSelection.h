#ifndef REPLACEMENTSELECTION_H_
#define REPLACEMENTSELECTION_H_
#include "archivo.h"
#include "particiones.h"

typedef struct{
	char* termino;
	char flag;
}Tpalabra;

/* Ordena el archivo de "lexico"(auxiliar) obteniendo particiones de igual tamaño 
 * de cantidad promedio de terminos en cada particion resultante igual a palabrasPromedio */
Tparticiones* ReplacementSelection(Tarch* archivo,int palabrasPromedio);

#endif /*REPLACEMENTSELECTION_H_*/
