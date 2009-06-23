#include "controlador.h"

void crearControlador(Controlador* controlador, char* argv) {
	 
	char consola[7] = "consola";
	
	if(!argv) {
		controlador->vista= (Vista*) malloc(sizeof(Vista));
		crearVista(controlador->vista, controlador);
		controlador->console= 0;	
	} else if(strcmp(argv, consola)) {
		controlador->shell= (Shell*) malloc(sizeof(Shell));
		crearShell(controlador->shell, controlador);
		controlador->console= 1;
	}
}

void correr(Controlador* controlador) {
	
	if(controlador->console)
		correrShell(controlador->shell);		
	else
		correrVista(controlador->vista);	
}

Datos* procesarPalabras(Controlador* controlador) {
	
	Datos* datos= (Datos*) malloc(sizeof(Datos));
	datos->modificar= nada;
		
	char* palabras= NULL;
	if(controlador->console)
		palabras= obtenerPalabrasShell(controlador->shell);		
	else
		palabras= obtenerPalabrasVista(controlador->vista);	
	
	char separadores[7]= " *\"\n\t";
	datos->lista= Lista_Crear();
	datos->listaModificar= Lista_Crear();
    char* palabra;
	int cantResultados;
	int modificar= 0;
	
  	palabra= strtok(palabras, separadores);
	Lista_Insertar(datos->lista, palabra);
 	
 	while((palabra= strtok(NULL, separadores)) != NULL) {
	   	
		if((cantResultados= atoi(palabra)) == 0) {
			if(strcmp(palabra, "-m") != 0 && strcmp(palabra, "-a") != 0 
			   && strcmp(palabra, "-d") != 0 && !modificar)
 				Lista_Insertar(datos->lista, palabra);
 				
			else if(strcmp(palabra, "-a") == 0) {
				datos->modificar= agregar;
				modificar= 1;
			} else if(strcmp(palabra, "-d") == 0) {
				datos->modificar= eliminar;
				modificar= 1;
			} else if(strcmp(palabra, "-m") == 0)
				datos->modificar= multiplicar;
			else
				Lista_Insertar(datos->listaModificar, palabra);	
		} else
			datos->cantResultados= cantResultados;
   	}
  	
//  	/*PRUEBAS*/
//  	while(!Lista_EstaVacia(datos->lista)) {
//     	char* palabra= (char*) Lista_PrimerElemento(datos->lista);
//    	Lista_RemoverPrimero(datos->lista);
//     	//printf("Palabra Lista: %s\n", palabra);
//     }
//
//	printf("Modificar: %d\n", datos->modificar);
//   	printf("Cant Resultados: %d\n", datos->cantResultados);
//   	
//   	while(!Lista_EstaVacia(datos->listaModificar)) {
//     	char* palabra= (char*) Lista_PrimerElemento(datos->listaModificar);
//    	Lista_RemoverPrimero(datos->listaModificar);
//     	mostrarPalabraVista(controlador->vista, palabra);	 
//     }
//     

	return(datos);
}

void mostrarPalabraResultados(Controlador* controlador, Lista* lista) {
	
	while(!Lista_EstaVacia(lista)) {
     	char* palabra= (char*) Lista_PrimerElemento(lista);

     	if(controlador->console) {
     		mostrarTituloResultado(controlador->shell);	
			mostrarPalabraShell(controlador->shell, palabra);		
     	} else
			mostrarPalabraVista(controlador->vista, palabra);
		
		Lista_RemoverPrimero(lista);
     }	
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

