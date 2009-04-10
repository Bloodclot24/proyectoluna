#include <stdio.h>
#include <stdlib.h>
#include <libxml/parser.h>
#include <libxml/SAX.h>
#include <libxml/SAX2.h>
#include <string.h>
#include <pila.h>
#include <debug.h>
#include <iconv.h>


#undef DEBUG_FLAG  /* eso mismo */


/*
 * Muy desprolija la cosa, pero es para probar.
 */

iconv_t traductor;

FILE* lexico;


void dumpLexico(char* texto, int len){
     int i, bytes;
     char *inicio, *final;
     char* palabra, *palabra2;

     bytes=0;
     inicio = texto;
     final = inicio+len;


     while(texto < final){
	  bytes = strcspn(texto, "(){}[]<>| ,;.:\n\'\"*+-/=$#!?¿¡\0");
	  if(bytes == 0)
	       texto++;
	  else if(texto+bytes < final){
	       palabra = (char*)malloc(bytes+1);
	       palabra2 = (char*)malloc(2*bytes+1);
	       strncpy(palabra, texto, bytes);
	       palabra[bytes]=0;

	       {
		    char *origen, *destino;
		    int caracteresOrigen=bytes;
		    int caracteresDestino=bytes*2;
		    origen = palabra;
		    destino = palabra2;
		    iconv(traductor, &origen, &caracteresOrigen, &destino,&caracteresDestino );
		    palabra2[bytes*2-caracteresDestino]=0;
	       }
	       if(strcmp(palabra, palabra2)!=0){
		    printf("%s -> %s \n", palabra, palabra2);
	       }
	       else printf("%s\n", palabra);

	       free(palabra);
	       free(palabra2);
	       texto+=bytes;
	  }
	  else{
	       texto += bytes; //estamos perdiendo letras, se puede arreglar
	  }
     }
     
}


struct datosAdministrativos{
     Pila *tagActual;
};

void startDocument1(void *data){
     struct datosAdministrativos *datos=data;
     Debug_Info("Inicia el documento.\n");
     datos->tagActual = Pila_Crear();
}

void endDocument1(void *data){
     struct datosAdministrativos *datos=data;
     Pila_LiberarTodo(datos->tagActual);
     Debug_Info("Termina el documento.\n");
}

void startElement1(void *data, const xmlChar *name, const xmlChar **attrs){
     struct datosAdministrativos *datos=data;
     Debug_Info("<%s>\n", (char*)name);
     char *nombre = malloc(strlen((char*)name)+1);
     strcpy(nombre, (char*)name);
     Pila_Apilar(datos->tagActual, nombre);  /* pongo el nombre del tag que estoy procesando en la pila */
     if(strncmp(nombre, "title", strlen("title")) == 0){
//	  printf("\nNombre del articulo: ");
     }
     else if(strncmp(nombre, "text", strlen("text")) == 0){
          printf("\n");
     }
}

void endElement1(void *data, const xmlChar *name, const xmlChar **attrs){
     struct datosAdministrativos *datos=data;
     Debug_Info("</%s>\n", (char*)name);
     char *nombre = Pila_Desapilar(datos->tagActual);
     free(nombre);
}

void characters1(void *data, const xmlChar* ch, int len){
     struct datosAdministrativos *datos=data;

     char *nombre = Pila_PrimerElemento(datos->tagActual);

     Debug_Info("Datos del elemento %s.\n", nombre);

     if(strncmp(nombre, "title", strlen("title")) == 0){
	/*   int i; */
/* 	  for(i=0;i<len;i++) */
/* 	       printf("%c", ch[i]); */
     }
     else if(strncmp(nombre, "text", strlen("text")) == 0){
	  //printf("texto"); /* Aca se procesa el articulo */
	  dumpLexico((char*)ch, len);
     }
     
}

xmlEntityPtr getEntity1(void *user_data, const xmlChar *name) {
     return xmlGetPredefinedEntity(name);
}


static xmlSAXHandler handler1 = {
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     getEntity1,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     NULL,
     startDocument1,
     endDocument1,
     startElement1,
     endElement1,
     NULL,
     characters1,
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


void parsear(const char *nombre){
     struct datosAdministrativos datos;
     Debug_Info("Iniciando el parseo.\n");
     if (xmlSAXUserParseFile(&handler1, &datos, nombre) >= 0) {
	  
     } else{
	  Debug_Warning("Algo salio mal.\n");
	  /* ERROR */
     }
}


int main(int argc, char** argv){
     if(argc < 2){
	  return -1;
     }

     lexico = fopen("lexico.dump", "w+");

     traductor = iconv_open("ASCII//TRANSLIT", "UTF-8");

     parsear(argv[1]);
     
     iconv_close(traductor);

     fclose(lexico);

     return 0;
}
