#ifndef SHELL_H_
#define SHELL_H_

#include <stdio.h>

#define PROMPT "> LUNA:~$ "
  
typedef struct Shell {
     struct Controlador* controlador;
     char command[300];
} Shell;

#include "../../Controlador/controlador.h"

/* Crea un shell */
void crearShell(Shell* shell, struct Controlador* controlador);

/* Corre el shell */
void correrShell(Shell* shell);

/* Devuelve las palabras ingresadas */
char* obtenerPalabrasShell(Shell* shell);

/* Muestra titulo resultado */
void mostrarTituloResultado(Shell* shell);	

/* Muestra una palabra resultado */
void mostrarPalabraShell(Shell* shell, char* palabra);	

/* Muestra un mensaje de error */
void mostrarErrorShell(Shell* shell, char* mensaje);	

/* Destruye el shell */
void destruirShell(Shell* shell);

#endif /*SHELL_H_*/
