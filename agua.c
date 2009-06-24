#include "archivo.h"
#include "matrizBayes.h"
#include "feedBack.h"

int comparar(const void* numero1, const void* numero2){
     const double** n1 = numero1;
     const double** n2 = numero2;

     if((**n1) > (**n2))
	  return -1;
     if((**n1) < (**n2))
	  return 1;
     return 0;
}

int main(int argc, char** argv){

     if(argc < 3){
	  printf("Uso: %s [nombreBase] palabra1 <palabra2> ... <palabraN>\n", argv[0]);
	  return -1;
     }
     
     char* prefijo = argv[1];

     Matriz *X = cargarMatriz(prefijo);
     X->numFilas = 4224832;
     X->numColumnas = 323415;

     HiperParametros *H = cargarHParam(X, prefijo);

     Tarch* lexico = cargarLexico(prefijo);
     Tarch* punterosLexico = cargarPLexico(prefijo);
     
     Query* q=NULL;
     int i;
     for(i=2;i<argc;i++){
	  q = ArmarQuery(q,argv[i],lexico, punterosLexico);
     }

//     buscarConsulta(q);
//     guardarConsulta(q);
     
     double* resultado = BSets(X,q,H);
     
     double** ordenado = malloc(X->numFilas*sizeof(double*));

     for(i=0;i<X->numFilas;i++){
	  ordenado[i] = resultado+i;
     }
     
     qsort(ordenado, X->numFilas, sizeof(double*), comparar);
     
     for(i=0;i<20;i++){
	  Fseek(punterosLexico, (int)(ordenado[i]-resultado)*sizeof(uint32_t), SEEK_SET);
	  uint64_t offset;
	  Fread(punterosLexico, &offset, sizeof(uint32_t));
	  Fseek(lexico, offset, SEEK_SET);
	  char* termino;
	  termino = FreadString(lexico);
	  printf("%f, %s\n", *ordenado[i], termino);
     }
     
     return 1;
}
