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

void procesarPalabras(Controlador* controlador) {
	
	char* palabras= NULL;
	if(controlador->console)
		palabras= obtenerPalabrasShell(controlador->shell);		
	else
		palabras= obtenerPalabrasVista(controlador->vista);	
	
	printf("Palabras a procesar %s", palabras);
	
	Lista* lista= Lista_Crear();
    char* palabra;
    int bytes= 0;
    int inicioPalabra= 0;
    int lenght= strlen(palabras);
     
    while(bytes <= lenght){
     	if(palabras[bytes] == ' ' || bytes == lenght) {
     		palabra= (char*) malloc(bytes-inicioPalabra);
     		strncpy(palabra, palabras, bytes-inicioPalabra);
     		printf("Palabra: %s\n", palabra);
     		printf("Largo palabra: %d\n", bytes-inicioPalabra);
     		
     		inicioPalabra= bytes+1;
     		Lista_Insertar(lista, palabra);
     		palabras+= (bytes+1);
     	}
     	
     	printf("Caracter: %s\n", palabras);
     	printf("Bytes: %d\n", bytes);
     	bytes++;
     }
     
     while(!Lista_EstaVacia(lista)) {
     	char* palabra= (char*) Lista_PrimerElemento(lista);
     	Lista_RemoverPrimero(lista);
   		printf("Palabra Lista: %s\n", palabra);
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

