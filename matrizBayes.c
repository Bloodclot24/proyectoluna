#include "matrizBayes.h"
#include <stdint.h>
#include <math.h>

Matriz* armarMatriz(Tarch* archAuxiliar){
     Matriz *X = (Matriz*)malloc(sizeof(Matriz));
     Tarch *arch1, *arch2, *arch3;
     
     int* linea;
     int cant_terminos= 0;
     
     arch1 = Fopen("matriz1","w");
     arch2 = Fopen("matriz2","w");
     arch3 = Fopen("matriz3","w");
     
     int uno=1;
     int comienzo=0;

     uint32_t numFilas=0, numColumnas=0;
     
     while(!Feof(archAuxiliar)) {
	  FreadReg(archAuxiliar, (void**)&linea);
	  if(linea == NULL)
	       break;
	  numFilas++;
	  int cantidad = ((int*)linea)[1];
	  int* inicio = linea+((int*)linea)[0]+2*sizeof(int);
	  //Aca empieza la fila
	  Fwrite(arch3, &comienzo, sizeof(int));
	  cantidad /= sizeof(int)*2; // cantidad de pares documento frecuencia
	  int i;
	  for(i=0;i<cantidad;i++){
	       if(numColumnas<inicio[0])
		    numColumnas = inicio[0];
	       comienzo++;
	       // por cada registro, escribo 1 si el termino esta en ese documento
	       Fwrite(arch1, &uno,sizeof(int));
	       // escribo en que posicion debe estar el numero (n� doc)
	       Fwrite(arch2, inicio,sizeof(int));
	       inicio +=2;
	       numFilas++;
	  }
     }

     Frewind(arch1);
     Frewind(arch2);
     Frewind(arch3);

     X->elementos = arch1;
     X->columnas = arch2;
     X->inicioFila = arch3;
     X->numColumnas = numColumnas+1;
     X->numFilas = numFilas;

     return X;
}

HiperParametros* BSParam(Matriz *X, double dParam){
     int size = X->numFilas;
     double *fila = (double*)malloc(sizeof(double)*X->numColumnas);
     double *alpha, *beta;
     int i,j;
     int columnas, columnasAnterior;
     int posicion, valor;

     memset(fila,0,sizeof(double)*X->numColumnas);


     /*   m = full(sum(X,2))  */

     Fread(X->inicioFila, &columnasAnterior, sizeof(int));

     for(i=0;i<X->numFilas;i++){
	  Fread(X->inicioFila, &columnas, sizeof(int));
	  for(j=0;j<columnas-columnasAnterior;j++){
	       Fread(X->columnas,&posicion,sizeof(int));
	       Fread(X->elementos,&valor,sizeof(int));
	       fila[posicion] += valor;
	  }
     }

     alpha = (double*)malloc(sizeof(double)*X->numColumnas);
     beta = fila;


     for(i=0;i<X->numColumnas;i++){
	  /* m = m/size(X,2); */
	  fila[i] /= X->numFilas;
	  /* alpha = c * m; */
	  alpha[i] = dParam * fila[i];
	  /* beta = c * (1 - m); */
	  beta[i] = dParam * (1-fila[i]);
     }

     HiperParametros *par = malloc(sizeof(HiperParametros));
     par->alpha = alpha;
     par->beta = beta;

     Frewind(X->elementos);
     Frewind(X->columnas);
     Frewind(X->inicioFila);
     
     return par;
}

double* BSets(Matriz *X, Query* q, HiperParametros *param){
     int n=q->elementos;

     double* c = (double*)malloc(sizeof(double)*X->numFilas);

     memset(c,0,sizeof(double)*X->numFilas);

     int i,j;
     int fila=0;
     int comienzoFila=0;
     int elementosFila=0;
     int repetida =0;
     int columna=0;
     int elem;

     /* c = sum(X(:,q),2); */

     Fread(X->inicioFila, &comienzoFila, sizeof(int));
     Fread(X->inicioFila, &elementosFila, sizeof(int));

     for(i=0;i<q->elementos;i++){
	  if(fila < q->query[i]){
	       while(fila < q->query[i]){
		    comienzoFila += elementosFila;
		    Fseek(X->columnas, elementosFila, SEEK_CUR);
		    Fseek(X->elementos, elementosFila, SEEK_CUR);
		    Fread(X->inicioFila, &elementosFila, sizeof(int));
		    elementosFila -= comienzoFila;
		    fila++;
	       }
	       repetida = 0;
	  }
	  else if((fila == q->query[i]) && (repetida)){
	      
	  }
	  
	  for(j=0;j<elementosFila;j++){
	       Fread(X->columnas, &columna, sizeof(int));
	       Fread(X->elementos, &elem, sizeof(int));
	       c[columna] += elem;
	  }
	  repetida = 1;	  
     }

     double* w = c;
     
     /* w = log(1 + c ./ alpha) - log(1 + (n - c) ./ beta); */

     for(i=0;i<X->numFilas;i++)
	  w[i] = log(1+c[i] / param->alpha[i]) - log(1+(n-c[i]) / param->beta[i]);

     Frewind(X->elementos);
     Frewind(X->columnas);
     Frewind(X->inicioFila);

     /* s = w' * X; */
     double* s = (double*)malloc(sizeof(double)*X->numFilas);
     memset(s,0,sizeof(double)*X->numFilas);

     Fread(X->inicioFila, &comienzoFila, sizeof(int));
     elementosFila = 0;

     for(i=0;i<X->numFilas;i++){

	  comienzoFila += elementosFila;
	  Fread(X->inicioFila, &elementosFila, sizeof(int));
	  elementosFila -= comienzoFila;

	  for(j=0;j<elementosFila;j++){
	       Fread(X->elementos, &elem, sizeof(int));
	       Fread(X->columnas, &columna, sizeof(int));
	       s[columna] += w[columna]*elem;
	  }
     }

     free(w);
     return s;
}
