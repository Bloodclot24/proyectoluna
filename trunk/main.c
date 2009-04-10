#include "archivo.h"
#include "merge.h"
#include "particiones.h"

int main(){
  Tparticiones* particion;
  
  Tarch* archivo; 
  archivo= Fopen("part2.txt","r");
  
  particion= crearParticiones("nombrearchivo");
  agregarParticion(particion,archivo);
  
//  printf("Agrego\n");
  
//  Tarch* archivoVuelve; 
//  archivoVuelve= obtenerParticion(particion);
// 
//  printf("Obtener\n");	
//  
//  char* linea; 
//  FreadLn(archivoVuelve,(void**) &linea);
//  printf("%s", linea);
  
  
  archivo = Fopen("part1.txt","r");
  agregarParticion(particion,archivo);
  
  archivo = Fopen("part3.txt","r");
  agregarParticion(particion,archivo);
  
  archivo = Fopen("part4.txt","r");
  agregarParticion(particion,archivo);
  
  merge(particion);
  
  return(0);
  
}
