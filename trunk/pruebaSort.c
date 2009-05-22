#include "archivo.h"
#include "ReplacementSelection.h"

int main(int argc, char** argv){

     Tarch *archivo = Fopen("pruebasort.lst","r");

     ReplacementSelection(archivo, 5);


}
