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

void parsearPalabra(char* palabras, Lista* lista, Controlador* controlador) {
	
	char* palabra;
	char separadores[7]= " \"\n\t";
	
	palabra= strtok(palabras, separadores);
	
	if(strcmp(palabra, "-m") != 0 && strcmp(palabra, "-a") != 0 &&
	   strcmp(palabra, "-d") != 0) {
	
		Lista_Insertar(lista, palabra);
 	
 		while((palabra= strtok(NULL, separadores)) != NULL)
			Lista_Insertar(lista, palabra);
	} else {
		mostrarErrorVista(controlador->vista, "Ingreso Inválido");
		controlador->error= 1;
	}	
}

void mostrar(Controlador* controlador, char* mensaje) {
	
	controlador->error= 1;
	
	if(controlador->console) 
		mostrarErrorShell(controlador->shell, mensaje);
	else
		mostrarErrorVista(controlador->vista, mensaje);	 
}

void* distraccion(void* controlador) {
	
	
	
	return(NULL);
}

void procesarPalabras(Controlador* controlador) {
	
	//pthread_t threads;
	//pthread_create(&threads, NULL, distraccion, (void*) controlador);
	
	Datos* datos= (Datos*) malloc(sizeof(Datos));
	datos->cantMultiplicar= 0;
	datos->lista= Lista_Crear();
	datos->listaAgregar= Lista_Crear();
	datos->listaQuitar= Lista_Crear();
	
	mostrar(controlador, "");
		
	controlador->error= 0;
		
	char* palabras= NULL;
	if(controlador->console)
		palabras= obtenerPalabrasShell(controlador->shell);		
	else
		palabras= obtenerPalabrasVista(controlador->vista);	
	
	if(strcmp(palabras, "") != 0) {
	
		char separadores[7]= " \"\n\t";
		char* palabra;
		int aInt;
		int agregar= 0;
		int eliminar= 0;
		int multiplicar= 0;
		
	  	palabra= strtok(palabras, separadores);
		
		if(strcmp(palabra, "-m") != 0 && strcmp(palabra, "-a") != 0 &&
		   strcmp(palabra, "-d") != 0) {
			Lista_Insertar(datos->lista, palabra);
		 	
		 	while((palabra= strtok(NULL, separadores)) != NULL) {
			   	
				if((aInt= atoi(palabra)) == 0) {
					
					if(strcmp(palabra, "-m") != 0 && strcmp(palabra, "-a") != 0 
					   && strcmp(palabra, "-d") != 0 && !agregar && !eliminar && !multiplicar) {
		 				Lista_Insertar(datos->lista, palabra);
		 				 				
					} else if(strcmp(palabra, "-a") == 0) {
						printf("AGREGAR -a\n");
						agregar= 1;
						eliminar= 0;
						multiplicar= 0;
						
					} else if(strcmp(palabra, "-d") == 0) {
						printf("QUITAR -d\n");
						agregar= 0;
						eliminar= 1;
						multiplicar= 0;
						
					} else if(strcmp(palabra, "-m") == 0) {
						printf("MULTIPLICAR -m\n");
						agregar= 0;
						eliminar= 0;
						multiplicar= 1;
		 			
		 			} else if(agregar) {
		 				printf("AGREGAR\n");
						Lista_Insertar(datos->listaAgregar, palabra);
					
					} else if(eliminar) {
		 				printf("QUITAR\n");
						Lista_Insertar(datos->listaQuitar, palabra);	
					}
				} else if(multiplicar) {
					printf("MULTIPLICAR %d\n", multiplicar);
		 			datos->cantMultiplicar= aInt;
					multiplicar= 0; 			
				} else {
					printf("RESTO\n");
		 			datos->cantResultados= aInt;
				}		
		   	}
		   	
		   	if(!controlador->console) {
		   		palabras= obtenerPalabrasVistaAgregar(controlador->vista);
				
				printf("PALABRA AGREGRAR %s\n", palabras);
				
				if(strcmp(palabras, "") != 0)
					parsearPalabra(palabras, datos->listaAgregar, controlador);
				
				palabras= obtenerPalabrasVistaEliminar(controlador->vista);
				
				printf("PALABRA QUITAR %s\n", palabras);
				
				if(strcmp(palabras, "") != 0)
					parsearPalabra(palabras, datos->listaQuitar, controlador);	
		   	}
		
		} else
			mostrar(controlador, "Ingreso Inválido");
	} else
		mostrar(controlador, "Ingreso Inválido");
	
//  	/*PRUEBAS*/
//   	printf("-- LISTA --\n");
//  	while(!Lista_EstaVacia(datos->lista)) {
//     	char* palabra= (char*) Lista_PrimerElemento(datos->lista);
////     	mostrarPalabraVista(controlador->vista, palabra);
//     	printf("Palabra Lista: %s\n", palabra);
//	   	Lista_RemoverPrimero(datos->lista);
//     }
//
//   	printf("Cant Resultados: %d\n", datos->cantResultados);
//   	printf("Cant Multiplicar: %d\n", datos->cantMultiplicar);
//   
//   	printf("LISTA AGREGAR\n");
//   	while(!Lista_EstaVacia(datos->listaAgregar)) {
//     	char* palabra= (char*) Lista_PrimerElemento(datos->listaAgregar);
// //    	mostrarPalabraVista(controlador->vista, palabra);
//   		printf("%s\n", palabra);	 
//       	Lista_RemoverPrimero(datos->listaAgregar);
//    }
//
//   	printf("LISTA QUITAR\n");
//    while(!Lista_EstaVacia(datos->listaQuitar)) {
//     	char* palabra= (char*) Lista_PrimerElemento(datos->listaQuitar);
//     	//mostrarPalabraVista(controlador->vista, palabra);
//     	printf("%s\n", palabra);	 	 
//    	Lista_RemoverPrimero(datos->listaQuitar);
//    }
     
    if(!controlador->error) {
		//LLAMAR AL MODELOreturn datos;
    }
    
    //mostrarDistraccion(controlador->vista);
    //mostrarNoEncontrado(controlador->vista);
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

