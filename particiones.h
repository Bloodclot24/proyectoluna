#ifndef PARTICIONES_H_
#define PARTICIONES_H_

#include <stdio.h>
#include "lista.h"
#include "archivo.h"

typedef struct Tparticiones{
     Lista* listaParticiones;
     int cantidad;
     char* nombre;
} Tparticiones;


/* Crea un nuevo grupo de particiones con el nombre dado */ 
Tparticiones* crearParticiones(const char* nombre);

/* Agrega una particion al grupo de particiones */
void agregarParticion(Tparticiones* TP, Tarch* TA);

/* Obtiene la primera particion, la devuelve y la elimina de la lista. */
Tarch* obtenerParticion(Tparticiones* TP);

/* Elimina todo el grupo de particiones */
void eliminarParticiones(Tparticiones* TP);

#endif /*PARTICIONES_H_*/
