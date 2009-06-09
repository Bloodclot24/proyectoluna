#define _FILE_OFFSET_BITS 64

#include "rb.h"
#include "parser.h"

#define ARCHIVO_STOPWORDS "stopWords3.dat\0"

/* Arbol y parametro extra para procesar las stopWords */
static struct rb_table *stopWords=NULL;

/* Guarda el lexico en el archivo asociado al parser */
void Parser_GuardarLexico(Parser* parser, char* texto, int len){
     if(parser == NULL)
	  return ;

     int /*i,*/ bytes;
     char *inicio, *final;
	  
     bytes=0;
     inicio = texto;
     final = inicio+len;
     
     while(texto < final){
	  bytes = strcspn(texto, "%_~(){}[[]]<>| ,;.:\n\'\"*+-/=$#!?\0%\\");
	  if(bytes == 0) /* no encontro ningun separador, avanzo*/
	       texto++;
	  else if(texto+bytes < final){
	       /* Encontre una palabra, y no termin el buffer */
	       /* TODO: Guardo la palabra al archivo */
	       /* avanzo en el archivo */

	       char *palabra = (char*) malloc(bytes+1);
	       int i;
	       for (i = 0 ; i < bytes; i++)
		    palabra[i] = tolower(texto[i]);
	       	palabra[bytes] = '\0';
		   /* Si no es una stopWord, la escribimos */
	       if((rb_find(stopWords, palabra) == NULL) && (strcspn(palabra, "-0123456789@^_%~") != 0)){
//		    palabra[bytes]='\n';
		    int auxiliar = bytes;//+1;
		    Fwrite(parser->archivoActual, &auxiliar, sizeof(auxiliar));
		    printf("%d ",auxiliar);
		    auxiliar = sizeof(int)*2; //Tamao del par Doc,Freq
		    Fwrite(parser->archivoActual, &auxiliar, sizeof(auxiliar));
		    printf("%d ",auxiliar);
		    printf("%s ",palabra);
		    Fwrite(parser->archivoActual, palabra, auxiliar);
		    auxiliar=1; // frecuencia
		    Fwrite(parser->archivoActual, &(parser->documentoID), sizeof(parser->documentoID));
		    printf("%d ",parser->documentoID);
		    Fwrite(parser->archivoActual, &auxiliar, sizeof(auxiliar));
	       	printf("%d\n",auxiliar);
	       }
	       free(palabra);
	  }
	  else{
	       /* se nos termina el buffer y nos quedamos a mitad de
		* una palabra, guardamos el fragmento para la proxima
		* vez */
	       //parser->palabraAuxiliar = (char*)malloc(bytes);
	       //strncpy(parser->palabraAuxiliar, texto, bytes);
	       //parser->numAuxiliar = bytes;
	  }
	  texto += bytes;
     }
}

/* Callback a utilizar en el comienzo del archivo */
static void Parser_ComenzarDocumento(void *data){
     Debug_Info("Inicia el documento.\n");
}

/* Callback a utilizar en el final del archivo */
static void Parser_FinalizarDocumento(void *data){
     Parser *parser=(Parser*)data;
     Pila_LiberarTodo(parser->tagActual);
     Debug_Info("Termina el documento.\n");
}

/* Callback a utilizar para el comienzo de cada elemento del
 * XML. Basicamente registra el nombre del ultimo elemento abierto,
 * asi cuando nos encontramos en un elemento del tipo 'text', sabemos
 * que corresponde al texto del articulo. */
static void Parser_ComenzarElemento(void *data, const xmlChar *name, const xmlChar **attrs){
     Parser *parser=(Parser*)data;
     Debug_Info("<%s>\n", (char*)name);

     char *nombre = (char*)malloc(strlen((char*)name)+1);
     strcpy(nombre, (char*)name);
     Pila_Apilar(parser->tagActual, nombre);  /* pongo el nombre del tag que estoy procesando en la pila */

     if(strncmp(nombre, "title", strlen("title")) == 0){
	  /* cambiamos de documento */
	  /* TODO: podemos usarlo para registrar el nombre del
	   * articulo*/
	  parser->documentoID++;
     }
     else if(strncmp(nombre, "text", strlen("text")) == 0){
          /* Comienza el texto del documento */
	  /* TODO: alguna inicializacion? */
     }
}

/* Callback a utilizar cuando termina un elemento del XML. */
static void Parser_FinalizarElemento(void *data, const xmlChar *name){ //, const xmlChar **attrs){
     Parser *parser=(Parser*)data;
     Debug_Info("</%s>\n", (char*)name);
     char *nombre = (char*)Pila_Desapilar(parser->tagActual);
     free(nombre);
}

/* Callback a utilizar cuando encuentro texto. Se fija si estamos en
 * la seccion correspondiente al texto del articulo, y si lo
 * estamos, va guardando cada termino del lexico. */
static void Parser_Caracteres(void *data, const xmlChar* ch, int len){
     Parser *parser=(Parser*)data;

     char *nombre = (char*)Pila_PrimerElemento(parser->tagActual);

     Debug_Info("Datos del elemento %s.\n", nombre);

     /* Si estamos en la seccion de texto, tenemos los terminos*/
     /* TODO: Hay formas de evitarnos la comparacion (hay que evitarlo) */
     if(strncmp(nombre, "text", strlen("text")) == 0){
	  Parser_GuardarLexico(parser, (char*)ch, len);
     }
}

static xmlEntityPtr Parser_ObtenerEntidad(void *user_data, const xmlChar *name) {
     return xmlGetPredefinedEntity(name);
}

/* Comienza el proceso del parseo, registra el handler, y a partir de
 * ahi se iran llamando a los diferentes Callbacks segun
 * corresponda */
void Parser_Procesar(Parser* parser){
     Debug_Info("Iniciando el parseo...\n");
     int error;
     if ((error=xmlSAXUserParseFile(parser->SAXhandler, parser, parser->nombreArchivo)) >= 0) {
	  Debug_Info("Parseo iniciado.\n");
	  printf("listo\n");
	  Fclose(parser->archivoActual);
     } else{
	  error = errno;
	  Debug_Warning("Algo salio mal.\n");
	  Debug_Warning("ERROR: %s\n", strerror(error));
	  /* ERROR */
     }
}

xmlSAXHandler SAXhandlerDefault = {
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     Parser_ObtenerEntidad,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     Parser_ComenzarDocumento,
     Parser_FinalizarDocumento,
     Parser_ComenzarElemento,
     Parser_FinalizarElemento,
     NULL,
     Parser_Caracteres,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     1,
     NULL,
     NULL,
     NULL,
     NULL
};

static int comparador(void* primero, void* segundo, void* datos){
     if(strlen(primero) == 0 || strlen(segundo) == 0)
	  return -1;
     else return strcmp(primero, segundo);
}

/* Crea un nuevo parser y abre el archivo. Mas que nada inicializa el
 * parser. Devuelve un nuvo parser o NULL si hubo algun error de
 * alocacion de memoria */
Parser* Parser_Crear(const char* nombre, int tamanioPromedio, int particiones){
     Pila *pila = Pila_Crear();
     Parser* parser=NULL;
     if(pila != NULL){
	  parser = (Parser*) malloc(sizeof(Parser));
	  parser->tagActual = pila;
	  parser->documentoID = 0;
 	  parser->nombreArchivo = (char*)malloc(strlen(nombre)+1); 
 	  strcpy(parser->nombreArchivo, nombre);
	  parser->tamanioPromedio = tamanioPromedio;
	  parser->tamanioActual = 0;
	  parser->particiones = particiones;
	  parser->SAXhandler = &SAXhandlerDefault;
	  /* Creo el archivo destino del lexico */
	  parser->archivoActual = Fopen("Lexico.txt", "w"); 
     }

     /* Si no existe el arbol de stopWords, lo creo */
     if(stopWords == NULL){
	  /* Creo un arbol RB */
	  stopWords = rb_create(comparador, NULL, NULL);
	  Tarch *archivo = Fopen(ARCHIVO_STOPWORDS, "r");
	  
	  /* Inserto los stopwords */
	  char *linea;
 	  while(!Feof(archivo)){ 
 	       FreadLn(archivo, (void**)&linea); 
 	       linea[strlen(linea)-1] = '\0'; 
 	       rb_insert(stopWords, linea); 
 	  } 
	  Fclose(archivo);
     }
     return parser;
}
