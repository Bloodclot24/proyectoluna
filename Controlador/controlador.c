#include "controlador.h"

void crearControlador(Controlador* controlador, int argc, char** argv) {
     char vista[] = "-vista";

     controlador->agua = comenzar_agua("wiki");

     controlador->batch = NULL;
     if(argc < 2) {
	  controlador->shell= (Shell*) malloc(sizeof(Shell));
	  crearShell(controlador->shell, controlador);
	  controlador->console = 1;
     } else if(strcmp(argv[1], vista)==0){
	  controlador->vista= (Vista*) malloc(sizeof(Vista));
	  crearVista(controlador->vista, controlador);
	  controlador->console= 0;
     }
     else{
	  controlador->shell= (Shell*) malloc(sizeof(Shell));
	  crearShell(controlador->shell, controlador);
	  controlador->console = 1;
	  int i;
	  char *palabra=malloc(1);
	  palabra[0]=0;
	  for(i=1;i<argc;i++){
	       palabra = realloc(palabra, strlen(palabra)+strlen(argv[i])+2);
	       strcat(palabra,argv[i]);
	       strcat(palabra," ");
	  }
	  controlador->batch = palabra;
	  procesarPalabras(controlador);
	  free(palabra);
     }
     controlador->error = 1;
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

void* hacer_agua(void* punteros){
     Datos *datos = ((void**)punteros)[1];
     Controlador *controlador = ((void**)punteros)[0];

     agua(controlador->agua, datos);

}

void* distraccion(void* controladorV) {
	
     Controlador* controlador= (Controlador*) controladorV; 

     mostrarDistraccion(controlador->vista);
     printf("ESPERANDO\n");
/* pthread_mutex_lock(&controlador->mutex); */
/*      pthread_cond_wait(&controlador->condition, &controlador->mutex); */
/*      pthread_mutex_unlock(&controlador->mutex); */

     cerrarDistraccion(controlador->vista);
		
     return(NULL);
}

void procesarPalabras(Controlador* controlador) {

     pthread_t threads;
     pthread_mutex_init(&controlador->mutex, NULL);
     pthread_cond_init(&controlador->condition, NULL); 
	
     Datos* datos= (Datos*) malloc(sizeof(Datos));
     datos->cantMultiplicar= 1;
     datos->cantResultados = RESULTADOS_DEFECTO;
     datos->lista= Lista_Crear();
     datos->listaAgregar= Lista_Crear();
     datos->listaQuitar= Lista_Crear();
	
     controlador->error= 0;
		
     char* palabras= NULL;
     if(controlador->console){
	  if(!controlador->batch)
	       palabras= obtenerPalabrasShell(controlador->shell);
	  else
	       palabras = controlador->batch;
     }
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
			      agregar= 1;
			      eliminar= 0;
			      multiplicar= 0;
						
			 } else if(strcmp(palabra, "-d") == 0) {
			      agregar= 0;
			      eliminar= 1;
			      multiplicar= 0;
						
			 } else if(strcmp(palabra, "-m") == 0) {
			      agregar= 0;
			      eliminar= 0;
			      multiplicar= 1;
		 			
			 } else if(agregar) {
			      Lista_Insertar(datos->listaAgregar, palabra);
					
			 } else if(eliminar) {
			      Lista_Insertar(datos->listaQuitar, palabra);	
			 }
		    } else if(multiplicar) {
			 datos->cantMultiplicar= aInt;
			 multiplicar= 0; 			
		    } else {
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
	
     int salida;

     if(!controlador->console)
	  limpiarEntrys(controlador->vista);
	
     if(!controlador->error && controlador->agua != NULL){

	  salida = agua(controlador->agua, datos);

	  if(salida) {
	       while(!Lista_EstaVacia(datos->lista)){
		    char * palabra = Lista_RemoverPrimero(datos->lista);

		    if(controlador->console)
			 mostrarPalabraShell(controlador->shell, palabra);
		    else
			 mostrarPalabraVista(controlador->vista, palabra);

		    free(palabra);
	       }
	  } else
	       mostrarNoEncontrado(controlador->vista);
     }
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

