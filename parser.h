#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#ifndef BASE
#define BASE "wiki"
#endif

#include "particiones.h"
#include "archivo.h"

#include "pila.h"
#include "debug.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libxml/parser.h>
#include <libxml/SAX.h>
#include <libxml/SAX2.h>
#include <libxml/xmlerror.h>

#ifndef NULL
#define NULL 0
#endif


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
     
     /* flag que indica si la palabra anterior esta incompleta o no */
	 int palabraIncompleta;	
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
Parser* Parser_Crear(const char* nombre, int tamanioPromedio, int particiones);

/* Guarda el lexico en registros de un formato dado */
void Parser_GuardarLexico(Parser* parser, char* texto, int len);

/* Comienza el proceso del parseo, registra el handler, y a partir de
 * ahi se iran llamando a los diferentes Callbacks segun
 * corresponda */
void Parser_Procesar(Parser* parser);

#endif /* PARSER_H_INCLUDED */
