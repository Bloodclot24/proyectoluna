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
    X=armarMatriz(archivo, &lexico, &punterosLexico);

    //CD-DVD
    //lunes-martes

    Query* q = (Query*)ArmarQuery(NULL,"",lexico, punterosLexico);

    ArmarQuery(q,"",lexico, punterosLexico);
//    ArmarQuery(q,"collar",lexico, punterosLexico);



/*      ArmarQuery(q,"Palabra5",lexico, punterosLexico);  */
/*      ArmarQuery(q,"Palabra6",lexico, punterosLexico);  */
/*      ArmarQuery(q,"Palabra7",lexico, punterosLexico);  */
/*      ArmarQuery(q,"Palabra8",lexico, punterosLexico);  */
/*      ArmarQuery(q,"Palabra9",lexico, punterosLexico);  */

    int i;
    for(i=0;i<q->elementos;i++){
//	  printf("%i\n", q->query[i]);
    }

//    printf("Matriz de %i por %i\n", X->numFilas, X->numColumnas);

    HiperParametros *H = BSParam(X,2);
    double* resultado = BSets(X,q,H);

//    printf("Puntajes = ");
    for(i=0;i<X->numFilas;i++){
//	 printf("%f \t %i\n", resultado[i],i);
    }
//    printf("%f]\n", resultado[i]);

    double** ordenado = malloc(X->numFilas*sizeof(double*));
    for(i=0;i<X->numFilas;i++){
	 ordenado[i] = resultado+i;
    }

    qsort(ordenado, X->numFilas, sizeof(double*), comparar);

//    for(i=0;i<X->numFilas;i++){
//	 printf("%f \t %i\n", *ordenado[i],(int)(ordenado[i]-resultado));
//    }


    for(i=0;i<20;i++){
	 Fseek(punterosLexico, (int)(ordenado[i]-resultado)*8, SEEK_SET);
	 uint64_t offset;
	 Fread(punterosLexico, &offset, sizeof(uint64_t));
	 Fseek(lexico, offset, SEEK_SET);
	 char* termino;
	 termino = FreadString(lexico);
	 printf("%f, %s\n", *ordenado[i], termino);
    }

    return 1;
}
