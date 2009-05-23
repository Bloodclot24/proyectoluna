#include "archivo.h"


Tarch* Fopen(const char* nombre, const char* modo){
     Tarch* archivo = NULL;

     FILE* fd = fopen(nombre, modo);
     if(fd != NULL){
	  archivo = (Tarch*)malloc(sizeof(Tarch));
	  archivo->nombre = (char*)malloc(strlen(nombre));
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

void Fwrite(Tarch *archivo, void* datos, size_t tam){
  if(archivo != NULL){
       if(archivo->fd != NULL)
	   	fwrite(datos,tam,1,archivo->fd);
  }
}     
void Fread(Tarch *archivo, void* datos, size_t tam){
  if(archivo != NULL){

    if(archivo->fd != NULL)
      fread(datos, tam, 1 ,archivo->fd );
         
  }
}

void FreadLn(Tarch *archivo, void** datos){
  char* linea=(char*)malloc(sizeof(char)*CANTIDAD_A_ALOCAR);
  char* lineaAux = linea;
  
  if(archivo != NULL){
	  if(fgets(lineaAux,CANTIDAD_A_ALOCAR,archivo->fd)!=NULL){
	    while((linea[strlen(linea)-1]!='\n')&&(lineaAux!=NULL)){
	      if((lineaAux=realloc(linea,strlen(linea)*2))!=NULL){
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
	  if(archivo->fd != NULL)
	       rewind(archivo->fd);
     }
}

void Funlink(Tarch* archivo){
  if(archivo != NULL){
    if(archivo->fd != NULL)
      remove(archivo->nombre);
  }
}
