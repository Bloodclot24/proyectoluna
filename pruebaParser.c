#include "parser.h"

int main(int argc, char** argv){

     Parser *p = Parser_Crear("Test1", 500, 10);

     Parser_Procesar(p);
     
     while(1){

     }

     return 0;
}
