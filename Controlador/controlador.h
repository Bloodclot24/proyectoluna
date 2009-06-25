#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_
 
#include <malloc.h>
#include <stdlib.h>
#include <pthread.h>
#include "../lista.h"
#include "../GUI/Vista/vista.h"
#include "../GUI/Shell/shell.h"


#ifndef BASE
#define BASE "wiki"
#endif

#define RESULTADOS_DEFECTO 20

typedef struct Datos {
	int cantResultados;
	int cantMultiplicar;
	Lista* lista;
	Lista* listaAgregar;
	Lista* listaQuitar;
} Datos;

#include "../agua.h"

typedef struct Controlador {
     int console;
     Vista* vista;
     Shell* shell;
     int error;
     Agua* agua;
     int procesando;
     char* batch;
     pthread_mutex_t mutex;
	 pthread_cond_t condition;
} Controlador;

/* Crea un controlador */
void crearControlador(Controlador* controlador, int argc, char** argv);

/* Corre la aplicacion */
void correr(Controlador* controlador);

/* Procesa las palabras ingresadas en la vista */
/* Devuelve un puntero a un estructura de Datos donde:
 * - modificar indica si se trata de:
 * >Fiubasets "palabras" -m * #
 * busca set relacionado a las palabras, pero con # más terminos.
 * >Fiubasets "palabra" y/o "palabras" -a "palabras"
 * busca set relacionado a varios términos y agrega términos propuestos por el usuario, 
 * le indica al sistema que el set resultado debe tener despues del -a.
 * >Fiubasets "palabra" y/o "palabras" -d "palabras"
 * busca set relacionado a varios términos y elimina términos propuestos por el usuario
 * despues del -d.
 * - cantResultados: cantidad de resultados a devolver
 * - lista: lista de las palabras insertadas.
 * - listaModificar: lista de las palabras agregar o eliminar en la busqueda.
 * En caso de error, devuelve NULL.
 */
void procesarPalabras(Controlador* controlador);

/* Muestra el set de palabras resultado */
void mostrarPalabraResultados(Controlador* controlador, Lista* lista);

/* Destruye el contolador*/
void destruirControlador(Controlador* controlador);

#endif /*CONTROLADOR_H_*/
