#include "archivo.h"
#include "particiones.h"
#include "merge.h"
#include "ReplacementSelection.h"

int main(int argc, char** argv){

    Tarch *archivo = Fopen("Lexico.txt","r");

    Tparticiones *TP = ReplacementSelection(archivo, 500000);

    printf("Listo, merge de %d particiones con 20 particiones por pasada:\n", TP->cantidad);

    mergeLexico(TP, 20);

    printf("Listo\n");
}
