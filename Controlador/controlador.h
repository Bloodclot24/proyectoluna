#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include <malloc.h>
#include "../GUI/Vista/vista.h"
#include "../GUI/Shell/shell.h"

typedef struct Controlador {
     int console;
     Vista* vista;
     Shell* shell;
} Controlador;

/* Crea un controlador */
void crearControlador(Controlador* controlador, char* argv);

/* Corre la aplicacion */
void correr(Controlador* controlador);

/* Procesa las palabras ingresadas en la vista */
void procesarPalabras(Controlador* controlador);

/* Destruye el contolador*/
void destruirControlador(Controlador* controlador);

#endif /*CONTROLADOR_H_*/
