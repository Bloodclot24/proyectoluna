#include "shell.h"

void crearShell(Shell* shell, Controlador* controlador) {
	
	shell->controlador= controlador;
}

void correrShell(Shell* shell) {
	
	int exit= 0;
	
	while(!exit) {
		printf(PROMPT);
		
			
	}
	
}

char* obtenerPalabras(Shell* shell) {
	
}

void destruirShell(Shell* shell) {
	
}
