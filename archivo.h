#ifndef ARCHIVO_H_
#define ARCHIVO_H_

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#define  CANTIDAD_A_ALOCAR 20

typedef struct Tarch{

     FILE* fd;
     char* nombre;
     
}Tarch;


Tarch* Fopen(const char* nombre, const char* modo);

void Fclose(Tarch* archivo);
 
void Frewind(Tarch* archivo);

void Fwrite(Tarch *archivo, void* datos, size_t tam);

void Fread(Tarch *archivo, void* datos, size_t tam);

void FreadLn(Tarch *archivo, void** datos);

void Funlink(Tarch* archivo);


#endif /*ARCHIVO_H_*/
