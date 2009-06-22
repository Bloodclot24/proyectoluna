#include "controlador.h"

void crearControlador(Controlador* controlador, char* argv) {
	
	if(!strcmp(argv,"console")) {
		controlador->vista= (Vista*) malloc(sizeof(Vista));
		crearVista(vista, controlador);
		controlador->console= 0;
	} else {
		controlador->shell= (Shell*) malloc(sizeof(Shell));
		crearShell(shell, controlador);
		controlador->console= 1;
	}
}

void correr(Controlador* controlador) {
	
	if(controlador->console)
		correrShell(controlador->shell);		
	else
		correrVista(controlador->vista);	
}

void procesarPalabras(Controlador* controlador) {
	
			
}

void destruirControlador(Controlador* controlador) {

	if(controlador->console) {
		destruirShell(controlador->shell);
		free(controlador->shell);
	} else {
		destruirVista(controlador->vista);
		free(controlador->vista);
	}	
}	
