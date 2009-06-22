#include "shell.h"

void crearShell(Shell* shell, Controlador* controlador) {
	
	shell->controlador= controlador;
}

void correrShell(Shell* shell) {
	
	int exit= 0;
	
	while(!exit) {
		printf("%s", PROMPT);
		fgets(shell->command, 300, stdin);
		procesarPalabras(shell->controlador);
	}
}

char* obtenerPalabrasShell(Shell* shell) {
	
	return shell->command;
}

void destruirShell(Shell* shell) { }
