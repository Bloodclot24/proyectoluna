#include "shell.h"

void crearShell(Shell* shell, Controlador* controlador) {
	
	shell->controlador= controlador;
}
 
void correrShell(Shell* shell) {
	
	int exit= 0;
	int length;
	char salir[6]= "salir";
	
	while(!exit) {
		printf("%s", PROMPT);
		fgets(shell->command, 300, stdin);

		length= strlen(shell->command);
		shell->command[length]= 0;
				
		if(strcmp(shell->command, salir)) {
			printf("no salir");
			procesarPalabras(shell->controlador);

		} else {
			printf("salir");
			exit= 1;
		}
	}
}

char* obtenerPalabrasShell(Shell* shell) {
	
	return shell->command;
}

void mostrarTituloResultado(Shell* shell) {

	printf("RESULTADOS:\n");	
}	

void mostrarPalabraShell(Shell* shell, char* palabra) {
	
	printf("%s\n", palabra);	
}	

void destruirShell(Shell* shell) { }
