#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED
#define  CANTIDAD_A_ALOCAR 20

#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* Estructura para almacenar los datos del archivo */
typedef struct Tarch{
     FILE* fd;
     char* nombre;
}Tarch;

/* Abre un archivo en el modo dado */
Tarch* Fopen(const char* nombre, const char* modo);

/* Cierra el archivo */
void Fclose(Tarch* archivo);

/* Indica si se llego o no a EOF */
int Feof(Tarch* archivo);

/* Escribe en el archivo 'tam' bytes del buffer 'datos' */
void Fwrite(Tarch *archivo, void* datos, size_t tam);

/* Lee del archivo 'tam' bytes en el buffer 'datos' */
void Fread(Tarch *archivo, void* datos, size_t tam);

/* Lee del archivo una linea completa en el buffer 'datos' */
void FreadLn(Tarch *archivo, void** datos);

/* Vuelve al principio del archivo */
void Frewind(Tarch* archivo);

/* Elimina el archivo del disco */
void Funlink(Tarch* archivo);

#endif /* ARCHIVO_H_INCLUDED */
