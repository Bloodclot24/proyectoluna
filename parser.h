#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include "particiones.h"
#include "archivo.h"

#include "pila.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/SAX.h>
#include <libxml/SAX2.h>


#ifndef NULL
#define NULL 0
#endif

/* Callback a utilizar en el comienzo del archivo */
static void Parser_ComenzarDocumento(void *data);

/* Callback a utilizar en el final del archivo */
static void Parser_FinalizarDocumento(void *data);

/* Callback a utilizar para el comienzo de cada elemento del
 * XML. Basicamente registra el nombre del ultimo elemento abierto,
 * asi cuando nos encontramos en un elemento del tipo 'text', sabemos
 * que corresponde al texto del articulo. */
static void Parser_ComenzarElemento(void *data, const xmlChar *name, const xmlChar **attrs);

/* Callback a utilizar cuando termina un elemento del XML. */
static void Parser_FinalizarElemento(void *data, const xmlChar *name, const xmlChar **attrs);

/* Callback a utilizar cuando encuentro texto. Se fija si estamos en
 * la seccion correspondiente al texto del articulo, y si lo
 * estamos, va guardando cada termino del lexico. */
static void Parser_Caracteres(void *data, const xmlChar* ch, int len);

static xmlEntityPtr Parser_ObtenerEntidad(void *user_data, const xmlChar *name);

static xmlSAXHandler SAXhandlerDefault = {
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


typedef struct Parser{
     /*
      * En esta pila se guardan los tags que se van abriendo en el archivo,
      * asi, a medida que se cierran, puedo verificar que el documento
      * este bien formado
      */
     Pila *tagActual;
     int documentoID; /* El numero del documento que estamos parseando */
     char* nombreArchivo;
     
     /* El numero de particion que estoy trabajando */
     int particionID;
     /* El numero de 'Tparticiones' que cree */
     int grupoParticionesID;

     /* para almacenar palabras a medias */
     char* palabraAuxiliar;
     /* cantidad de caracteres de la palabra a medias */
     int numAuxiliar;

     /*
      * Tamanio promedio de cada particion, tamanio de la particion actual
      * cantidad maxima de particiones.
      */
     int tamanioPromedio;
     int tamanioActual;
     int particiones;
     
     Tarch* archivoActual;
     Tparticiones* particion;

     xmlSAXHandler *SAXhandler;
     
}Parser;
 
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
	  parser->nombreArchivo = malloc(strlen(nombre));
	  strcpy(parser->nombreArchivo, nombre);
	  parser->tamanioPromedio = tamanioPromedio;
	  parser->tamanioActual = 0;
	  parser->particiones = particiones;
	  parser->SAXhandler = &SAXhandlerDefault;

	  parser->archivoActual = Fopen(parser->documentoID, "w");
     }
     return parser;
}

/* */
/*static Tpart* */ void Parser_GuardarLexico(Parser* parser, char* texto, int len){
     if(parser == NULL)
	  return ;

     int i, bytes;
     char *inicio, *final;
	  
     bytes=0;
     inicio = texto;
     final = inicio+len;
     
     while(texto < final){
	  bytes = strcspn(texto, "(){}[]<>| ,;.:\n\'\"*+-/=$#!?\0");
	  if(bytes == 0) /* no encontro ningun separador, avanzo*/
	       texto++;
	  else if(texto+bytes < final){
	       /* Encontre una palabra, y no terminó el buffer*/
	       /* TODO: Guardo la palabra al archivo */
	       /* avanzo en el archivo */
	       int i;
	       for(i=0;i<bytes;i++)
		    printf("%c", tolower(texto[i]));
	       printf("\n");
	       
	       texto+=bytes;
	  }
	  else{
	       /* se nos termina el buffer y nos quedamos a mitad de
		* una palabra, guardamos el fragmento para la proxima
		* vez */
	       parser->palabraAuxiliar = (char*)malloc(bytes);
	       strncpy(parser->palabraAuxiliar, texto, bytes);
	       parser->numAuxiliar = bytes;
 	       texto += bytes; 
	  }
     }
}

/* Callback a utilizar en el comienzo del archivo */
static void Parser_ComenzarDocumento(void *data){
     Debug_Info("Inicia el documento.\n");
}

/* Callback a utilizar en el final del archivo */
static void Parser_FinalizarDocumento(void *data){
     Parser *parser=data;
     Pila_LiberarTodo(parser->tagActual);
     Debug_Info("Termina el documento.\n");
}

/* Callback a utilizar para el comienzo de cada elemento del
 * XML. Basicamente registra el nombre del ultimo elemento abierto,
 * asi cuando nos encontramos en un elemento del tipo 'text', sabemos
 * que corresponde al texto del articulo. */
static void Parser_ComenzarElemento(void *data, const xmlChar *name, const xmlChar **attrs){
     Parser *parser=data;
     Debug_Info("<%s>\n", (char*)name);

     char *nombre = malloc(strlen((char*)name)+1);
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
	  /* TODO: ¿alguna inicializacion? */
     }
}

/* Callback a utilizar cuando termina un elemento del XML. */
static void Parser_FinalizarElemento(void *data, const xmlChar *name, const xmlChar **attrs){
     Parser *parser=data;
     Debug_Info("</%s>\n", (char*)name);
     char *nombre = Pila_Desapilar(parser->tagActual);
     free(nombre);
}

/* Callback a utilizar cuando encuentro texto. Se fija si estamos en
 * la seccion correspondiente al texto del articulo, y si lo
 * estamos, va guardando cada termino del lexico. */
static void Parser_Caracteres(void *data, const xmlChar* ch, int len){
     Parser *parser=data;

     char *nombre = Pila_PrimerElemento(parser->tagActual);

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
     Debug_Info("Iniciando el parseo.\n");
     if (xmlSAXUserParseFile(parser->SAXhandler, &parser, parser->nombreArchivo) >= 0) {
	  printf("Comenzando el parseo\n.");
     } else{
	  Debug_Warning("Algo salio mal.\n");
	  /* ERROR */
     }
}

#endif /* PARSER_H_INCLUDED */
