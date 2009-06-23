#include "archivo.h"
#include "matrizBayes.h"

int main(){
    Tarch * archivo = Fopen("auxiliar", "r");

    Tarch *lexico, *punterosLexico;

    Matriz *X;
    X=armarMatriz(archivo, &lexico, &punterosLexico);

    Query* q = (Query*)ArmarQuery(NULL,"casa",lexico, punterosLexico);
     ArmarQuery(q,"choza",lexico, punterosLexico);  
//      ArmarQuery(q,"tres",lexico, punterosLexico);
/*      ArmarQuery(q,"Palabra4",lexico, punterosLexico);   */
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
	 printf("%f \t %i\n ", resultado[i],i);
    }
//    printf("%f]\n", resultado[i]);
    return 1;
}
