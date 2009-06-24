#include "archivo.h"
#include "matrizBayes.h"

int comparar(const void* numero1, const void* numero2){
     const double** n1 = numero1;
     const double** n2 = numero2;

     if((**n1) > (**n2))
	  return -1;
     if((**n1) < (**n2))
	  return 1;
     return 0;
}

int main(){
    Tarch * archivo = Fopen("auxiliar", "r");

    Tarch *lexico, *punterosLexico;
    
    Matriz *X;
//    X=armarMatriz(archivo, &lexico, &punterosLexico);
/*     return 0; */
     X=malloc(sizeof(Matriz)); 
     X->elementos = Fopen("wiki_matriz1","r"); 
     X->columnas = Fopen("wiki_matriz2","r"); 
     X->inicioFila = Fopen("wiki_matriz3","r"); 
     lexico = Fopen("wiki_lexico", "r"); 
     punterosLexico = Fopen("wiki_punterosLexico", "r"); 
     X->numFilas = 4224832; 
     X->numColumnas = 323415; 
	 

    //CD-DVD
    //lunes-martes

    HiperParametros *H = BSParam(X,2);

    Query* q = (Query*)ArmarQuery(NULL,"gato",lexico, punterosLexico);
    ArmarQuery(q,"perro",lexico, punterosLexico);
    ArmarQuery(q,"zorro",lexico, punterosLexico);


    double* resultado = BSets(X,q,H);

    double** ordenado = malloc(X->numFilas*sizeof(double*));
    int i;
    for(i=0;i<X->numFilas;i++){
	 ordenado[i] = resultado+i;
    }

    qsort(ordenado, X->numFilas, sizeof(double*), comparar);

    for(i=0;i<20;i++){
	 Fseek(punterosLexico, (int)(ordenado[i]-resultado)*4, SEEK_SET);
	 uint64_t offset;
	 Fread(punterosLexico, &offset, sizeof(uint32_t));
	 Fseek(lexico, offset, SEEK_SET);
	 char* termino;
	 termino = FreadString(lexico);
	 printf("%f, %s\n", *ordenado[i], termino);
    }

    return 1;
}
