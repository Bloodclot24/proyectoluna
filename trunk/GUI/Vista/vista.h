#ifndef VISTA_H_
#define VISTA_H_

#include <stdio.h>
#include <gtk/gtk.h> 

#define PATH_VISTA "vista.glade"

typedef struct Vista {
     GtkBuilder* builder;
     Controlador* controlador;
} Vista;
#include "../Controlador/controlador.h"

/* Crea una vista */
void crearVista(Vista* vista, Controlador* controlador);

/* Corre la vista */
void correrVista(Vista* vista);

/* Devuelve las palabras ingresadas */
char* obtenerPalabras(Vista* vista);

/* Destruye la vista */
void destruirVista(Vista* vista);

#endif /*VISTA_H_*/
