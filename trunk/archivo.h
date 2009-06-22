#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED
#define  CANTIDAD_A_ALOCAR 20

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>

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
/* Escribe un registro de la forma (int)(int)(termino)(punteros) */
void FwriteReg(Tarch *archivo, void* datos);

/* Lee del archivo 'tam' bytes en el buffer 'datos' */
void Fread(Tarch *archivo, void* datos, size_t tam);

/* Lee del archivo un registro de la forma:
 * (int)(int)(termino)(punteros) */
void FreadReg(Tarch *archivo, void** datos);

/* Lee del archivo una linea completa en el buffer 'datos' */
void FreadLn(Tarch *archivo, void** datos);

/* Lee del archivo hasta encontrar el \0. Devuelve un char* con el
 * terminador incluido, que debe ser liberado por el usuario */
char* FreadString(Tarch* archivo);

int Fseek(Tarch* archivo, long offset, int whence);

/* Vuelve al principio del archivo */
void Frewind(Tarch* archivo);

/* Elimina el archivo del disco */
void Funlink(Tarch* archivo);

/* Posiciona el puntero de lectura/escritura en la posicion indicada */
int Fseek(Tarch* archivo, long offset, int whence);

/* indica la posicion en la que se encuentra el puntero de
 * lectura/escritura dentro del archivo */
uint64_t Ftell(Tarch* archivo);

/* Devuelve el tamaño del archivo */
uint64_t Fsize(Tarch* archivo);

/* Obtiene la cantidad de bytes que ocupan todos los punteros de un
 * registro */
uint32_t RegGetPointersLength(const void *reg);

/* Obtiene la longitud total de un registro */
uint32_t RegLength(const void *reg);

/* Obtiene la cantidad de punteros que guarda un registro */
uint32_t RegGetNumPointers(const void *reg);

/* Devuelve un puntero a la palabra que almacena un registro */
const char* RegGetWord(const void* reg);

/* Devuelve la longitud de la palabra almacenada en el registro */
uint32_t RegGetWordLength(const void *reg);

/* Obtiene elpuntero (num) almacenado en elregistro */
uint32_t RegGetPointer(const void* reg, int num);

/* Asigna una lista de punteros al registro, descartando los anteriores */
void* RegSetPointers(void* reg, const uint32_t* pointers, uint32_t numPointers);

#endif /* ARCHIVO_H_INCLUDED */
