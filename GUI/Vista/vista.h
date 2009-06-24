#ifndef VISTA_H_
#define VISTA_H_

#include <string.h>
#include <gtk/gtk.h> 
  
#define PATH_VISTA "GUI/Vista/vista.glade"

typedef struct Vista {
     GtkBuilder* builder;
     struct Controlador* controlador;
} Vista;

#include "../../Controlador/controlador.h"

/* Crea una vista */
void crearVista(Vista* vista, struct Controlador* controlador);

/* Mostrar ventana distraccion */
void mostrarDistraccion(Vista* vista);

/* Mostrar ventana de no encontrado */
void mostrarNoEncontrado(Vista* vista);

/* Corre la vista */
void correrVista(Vista* vista);

/* Devuelve las palabras ingresadas a buscar */
char* obtenerPalabrasVista(Vista* vista);

/* Devuelve las palabras ingresadas agregar */
char* obtenerPalabrasVistaAgregar(Vista* vista);

/* Devuelve las palabras ingresadas a eliminar */
char* obtenerPalabrasVistaEliminar(Vista* vista);

/* Muestra una palabra resultado */
void mostrarPalabraVista(Vista* vista, char* palabra);	

/* Muestra un mensaje de error */
void mostrarErrorVista(Vista* vista, char* mensaje);	

/* Limpia los entry texts*/
void limpiarEntrys(Vista* vista);	

/* Destruye la vista */
void destruirVista(Vista* vista);

#endif /*VISTA_H_*/
