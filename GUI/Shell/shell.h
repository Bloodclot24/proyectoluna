#ifndef SHELL_H_
#define SHELL_H_

#define PROMPT "LUNA:~$ "

typedef struct Shell {
     Controlador* controlador;
} Shell;

#include "../Controlador/controlador.h"

/* Crea un shell */
void crearShell(Shell* shell, Controlador* controlador);

/* Corre el shell */
void correrShell(Shell* shell);

/* Devuelve las palabras ingresadas */
char* obtenerPalabras(Shell* shell);

/* Destruye el shell */
void destruirShell(Shell* shell);

#endif /*SHELL_H_*/
