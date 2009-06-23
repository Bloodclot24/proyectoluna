#ifndef CONTROLADOR_H_
#define CONTROLADOR_H_

#include <malloc.h>
#include "../lista.h"
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

/* >Fiubasets "palabra" y/o "palabras" # 
 * # se puede sugerir de 1 a N elementos separados por espacio, y encerrados 
 * entre comillas si son palabras compuestas.
 */

/* >Fiubasets "palabras" -m * #
 * busca set relacionado a las palabras, pero con # más terminos.
 */
 
/* >Fiubasets "palabra" y/o "palabras" -a "palabras"
 * busca set relacionado a varios términos y agrega términos propuestos por el usuario, 
 * le indica al sistema que el set resultado debe tener despues del -a.
 */

/* >Fiubasets "palabra" y/o "palabras" -d "palabras"
 * busca set relacionado a varios términos y elimina términos propuestos por el usuario
 * despues del -d.
 */
void procesarPalabras(Controlador* controlador);

/* Destruye el contolador*/
void destruirControlador(Controlador* controlador);

#endif /*CONTROLADOR_H_*/
