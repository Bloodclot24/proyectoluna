#ifndef REPLACEMENTSELECTION_H_
#define REPLACEMENTSELECTION_H_
#include "archivo.h"
#include "particiones.h"

typedef struct{
	char* termino;
	char flag;
}Tpalabra;

Tparticiones* ReplacementSelection(Tarch* archivo,int memoriaAutilizar);

#endif /*REPLACEMENTSELECTION_H_*/
