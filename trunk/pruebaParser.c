#include "parser.h"

int main(int argc, char** argv){

     Parser *p = Parser_Crear("ParserTest1", 500, 10);

     Parser_Procesar(p);
     
     

     return 0;
}
