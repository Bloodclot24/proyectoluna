#ifndef PARTICIONES_H_
#define PARTICIONES_H_

#include <stdio.h>
#include "Lista.h"
#include "archivo.h"

typedef struct Tparticiones{
     TListaSimple particiones;
     int cantidad;
     char* nombre;
} Tparticiones;

 
Tparticiones* crearParticiones(const char* nombre);

void agregarParticion(Tparticiones* TP, Tarch* TA);

/*
 * Obtiene la primera particion y la elimina de la lista.
 */
Tarch* obtenerParticion(Tparticiones* TP);

void eliminarParticiones(Tparticiones* TP);

#endif /*PARTICIONES_H_*/
