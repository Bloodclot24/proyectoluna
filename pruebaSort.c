#include "archivo.h"
#include "particiones.h"
#include "merge.h"
#include "ReplacementSelection.h"

int main(int argc, char** argv){

     Tarch *archivo = Fopen("salida","r");

     Tparticiones *TP = ReplacementSelection(archivo, 10000);

     printf("Listo, merge:\n");

     merge(TP);

     printf("Listo\n");
     
     eliminarParticiones(TP);

     Fclose(archivo);


}
