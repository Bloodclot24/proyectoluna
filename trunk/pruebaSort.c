#include "archivo.h"
#include "particiones.h"
#include "ReplacementSelection.h"

int main(int argc, char** argv){

     Tarch *archivo = Fopen("salida","r");

     Tparticiones *TP = ReplacementSelection(archivo, 1000);

     eliminarParticiones(TP);

     Fclose(archivo);


}
