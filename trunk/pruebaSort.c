#include "archivo.h"
#include "particiones.h"
#include "ReplacementSelection.h"

int main(int argc, char** argv){

     Tarch *archivo = Fopen("pruebasort.lst","r");

     Tparticiones *TP = ReplacementSelection(archivo, 10);

     eliminarParticiones(TP);

     Fclose(archivo);


}
