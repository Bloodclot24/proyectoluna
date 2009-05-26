#include "archivo.h"
#include "particiones.h"
#include "merge.h"
#include "ReplacementSelection.h"

int main(int argc, char** argv){

     Tarch *archivo = Fopen("salida","r");

     Tparticiones *TP = ReplacementSelection(archivo, 10000);

     printf("Listo, merge de %d particiones con 20 particiones por pasada:\n", TP->cantidad);

     merge2(TP, 20);

     printf("Listo\n");
}
