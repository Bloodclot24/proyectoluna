#include "Controlador/controlador.h"

int main(int argc, char **argv) {
   
	gtk_init(&argc, &argv);
   	
	Controlador* controlador= (Controlador*) malloc(sizeof(Controlador));
	
	/* Si el argumento es "consola", la aplicacion se corre por consola,
	 * cualquier otro caso se correra por la interfaz */
	crearControlador(controlador, argv[1]);
	correr(controlador);
	
	destruirControlador(controlador);
	free(controlador);

	return 0;
} 
