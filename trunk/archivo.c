#include "archivo.h"
#include <string.h>
#include <errno.h>

Tarch* Fopen(const char* nombre, const char* modo){
     Tarch* archivo = NULL;

     FILE* fd = fopen(nombre, modo);

     if(fd != NULL){
	  archivo = (Tarch*)malloc(sizeof(Tarch));
	  archivo->nombre = (char*)malloc(strlen(nombre)+1);
	  strcpy(archivo->nombre, nombre);
	  archivo->fd = fd;
     }

     return archivo;
}

void Fclose(Tarch* archivo){
     if(archivo != NULL){
	  if(archivo->fd != NULL)
	       fclose(archivo->fd);
	  if(archivo->nombre != NULL)
	       free(archivo->nombre);
	  free(archivo);
     }
}

int Feof(Tarch* archivo){
     if(archivo != NULL){
	  if(archivo->fd != NULL)
	       return feof(archivo->fd);
	  
     }     
}

int Fseek(Tarch* archivo, long offset, int whence){
     if(archivo != NULL){
	  if(archivo->fd != NULL)
	       return fseek(archivo->fd, offset, whence);
     }
}

uint64_t Ftell(Tarch* archivo){
     if(archivo == NULL)
	  return 0;

     if(archivo->fd == NULL)
	  return 0;

     return ftell(archivo->fd);
}

uint64_t Fsize(Tarch* archivo){
     if(archivo == NULL)
	  return 0;

     if(archivo->fd == NULL)
	  return 0;

     uint64_t posicion = Ftell(archivo);
     Fseek(archivo, 0, SEEK_END);
     uint64_t tamanio = Ftell(archivo);
     Fseek(archivo,posicion, SEEK_SET);
     
     return tamanio;
}

void Fwrite(Tarch *archivo, void* datos, size_t tam){
  if(archivo != NULL){
       if(archivo->fd != NULL)
	   	fwrite(datos,tam,1,archivo->fd);
  }
}     

void FwriteReg(Tarch *archivo, void* datos){
  if(archivo != NULL){
       if(archivo->fd != NULL)
	    fwrite(datos,						\
		   ((int*)datos)[0]+((int*)datos)[1]+2*sizeof(int),	\
		   1,							\
		   archivo->fd);
  }
}     

void Fread(Tarch *archivo, void* datos, size_t tam){
  if(archivo != NULL){

    if(archivo->fd != NULL)
      fread(datos, tam, 1 ,archivo->fd );
         
  }
}

void FreadReg(Tarch *archivo, void** datos){
     if(archivo != NULL){
	  if(archivo->fd != NULL){
	       int tamanio[2];
	       fread(tamanio, 2*sizeof(int), 1 ,archivo->fd);
	       if(Feof(archivo)){
		    printf("ERRORR: LEYENDO DESDE EOF.\n");
		    *datos = NULL;
		    return;
	       }
		    
	       *datos = malloc(tamanio[0]+tamanio[1]+2*sizeof(int));
	       (*(int**)datos)[0] = tamanio[0];
	       (*(int**)datos)[1] = tamanio[1];

	       fread(*datos+2*sizeof(int), tamanio[0]+tamanio[1], 1, archivo->fd);
	  }
     }  
}

void FreadLn(Tarch *archivo, void** datos){
  char* linea=(char*)malloc(sizeof(char)*CANTIDAD_A_ALOCAR);
  char* lineaAux = linea;
  
  if(archivo != NULL){
	  if(fgets(lineaAux,CANTIDAD_A_ALOCAR,archivo->fd)!=NULL){
	    while((linea[strlen(linea)-1]!='\n')&&(lineaAux!=NULL)){
		 if((lineaAux=(char*)realloc(linea,strlen(linea)*2))!=NULL){
	      	linea= lineaAux; 
	      	lineaAux+=strlen(lineaAux);
	      	lineaAux=fgets(lineaAux,CANTIDAD_A_ALOCAR,archivo->fd);
	      }
	    }
	  }
	  *datos = linea;
  }
} 

void Frewind(Tarch* archivo){
     if(archivo != NULL){
	  if(archivo->fd != NULL){
	       fclose(archivo->fd);
	       archivo->fd = fopen(archivo->nombre, "rw");
	       //rewind(archivo->fd);
	  }
     }
}

void Funlink(Tarch* archivo){
  if(archivo != NULL){
       if(archivo->fd != NULL){
	    fclose(archivo->fd);
	    remove(archivo->nombre);
       }
       if(archivo->nombre != NULL)
	    free(archivo->nombre);
       free(archivo);

  }
}

uint32_t RegLength(void *reg){
     if(reg == NULL)
	  return 0;

     return RegGetWordLength(reg)+RegGetPointersLength(reg)+2*sizeof(int);
}

uint32_t RegGetWordLength(void *reg){
     if(reg == NULL)
	  return 0;
     return ((int*)reg)[0];
}

uint32_t RegGetPointersLength(void *reg){
     if(reg == NULL)
	  return 0;
     return ((int*)reg)[1];
}

uint32_t RegGetNumPointers(void *reg){
     if(reg == NULL)
	  return 0;
     return ((int*)reg)[1]/(sizeof(int));
}

const char* RegGetWord(void* reg){
     if(reg == NULL)
	  return NULL;
     return (char*)reg+2*sizeof(int);
}

uint32_t RegGetPointer(void* reg, int num){
     if(reg != NULL || num >= RegGetNumPointers(reg))
	  return -1;

     uint32_t inicio = RegGetWordLength()+2*sizeof(int)+num*sizeof(int);
     return *((int*)(reg+inicio));
}

char* FreadString(Tarch* archivo){
     if(archivo == NULL)
	  return NULL;
     if(archivo->fd == NULL)
	  return NULL;
     
     int tamanioActual = 20;
     char* string = malloc(tamanioActual);
     int i=0;

     char c;
     do{
	  Fread(archivo, &c, sizeof(c));
	  string[i]=c;
	  i++;
	  if(i>= tamanioActual){
	       tamanioActual+=10;
	       string = realloc(string, tamanioActual);
	  }
     }while(c!=0);

     return string;
}
