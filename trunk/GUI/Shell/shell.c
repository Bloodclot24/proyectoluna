#include "shell.h"

void crearShell(Shell* shell, Controlador* controlador) {
	
	shell->controlador= controlador;
}

void correrShell(Shell* shell) {
	
	int exit= 0;
	char salir[300] = "salir";
	
	while(!exit) {
		printf("%s", PROMPT);
		fgets(shell->command, 300, stdin);
		
		if(strcmp(shell->command, salir) != 0)
			procesarPalabras(shell->controlador);
		else
			exit= 1;
	}
}

char* obtenerPalabrasShell(Shell* shell) {
	
	return shell->command;
}

void destruirShell(Shell* shell) { }
