#include "Controlador/controlador.h"

int main(int argc, char **argv) {
   
	gtk_init(&argc, &argv);
   	
	Controlador* controlador= (Controlador*) malloc(sizeof(Controlador));
	
	/* Si el PRIMER argumento es "-vista", la aplicacion se corre
	 * por gtk, si no hay argumentos se corre un shell. En otro
	 * caso se corre en modo batch*/
	crearControlador(controlador, argc, argv);

	if(!controlador->batch){

	     correr(controlador);
	}
	destruirControlador(controlador);
	free(controlador);

	return 0;
} 
