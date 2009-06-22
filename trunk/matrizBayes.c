#include "matrizBayes.h"
#include "archivo.h"

#define QUERY_ELEM_UNIT 5

Matriz* armarMatriz(Tarch* archAuxiliar, Tarch** lexico, Tarch** punterosLexico){
     Matriz *X = (Matriz*)malloc(sizeof(Matriz));
     Tarch *arch1, *arch2, *arch3;
     
     int* linea;
     
     arch1 = Fopen("matriz1","w");
     arch2 = Fopen("matriz2","w");
     arch3 = Fopen("matriz3","w");
     *lexico = Fopen("Lexico", "w");
     *punterosLexico = Fopen("PunterosLexico", "w");
     
     uint64_t offset=0;
     char cero=0;
     
     
     int uno=1;
     int comienzo=0;

     uint32_t numFilas=0, numColumnas=0;
     
     while(!Feof(archAuxiliar)) {
	  FreadReg(archAuxiliar, (void**)&linea);
	  if(linea == NULL)
	       break;
	  
	  Fwrite(*lexico, (void*)RegGetWord(linea), RegGetWordLength(linea));
	  Fwrite(*lexico, &cero, sizeof(cero));

	  Fwrite(*punterosLexico, &offset, sizeof(offset));
	  
	  offset += RegGetWordLength(linea)+1;
	  
	  numFilas++;
	  int cantidad = RegGetNumPointers(linea); 
	  cantidad /= 2;

	  Fwrite(arch3, &comienzo, sizeof(int));
	  
	  int i;
	  int puntero=0;
	  for(i=0;i<cantidad;i++){
	       if(numColumnas< RegGetPointer(linea,puntero)) 
		    numColumnas = RegGetPointer(linea,puntero); 
	       comienzo++;
	       // por cada registro, escribo 1 si el termino esta en ese documento
	       Fwrite(arch1, &uno,sizeof(int));
	       // escribo en que posicion debe estar el numero (nº doc)
	       int posicion = RegGetPointer(linea,puntero);
	       Fwrite(arch2, &posicion, sizeof(int));
	       puntero += 2;
	  }
     }
     Fwrite(arch3, &comienzo, sizeof(int));

     Frewind(arch1);
     Frewind(arch2);
     Frewind(arch3);
     Frewind(*lexico);
     Frewind(*punterosLexico);

     X->elementos = arch1;
     X->columnas = arch2;
     X->inicioFila = arch3;
     X->numColumnas = numColumnas+1;
     X->numFilas = numFilas;

     return X;
}

HiperParametros* BSParam(Matriz *X, double dParam){
     double *fila = (double*)malloc(sizeof(double)*X->numColumnas);
     double *alpha, *beta;
     int i,j;
     int columnas, columnasAnterior;
     int posicion, valor;

     memset(fila,0,sizeof(double)*X->numColumnas);


     /*   m = full(sum(X,2))  */

     Fread(X->inicioFila, &columnas, sizeof(int));
     
     for(i=0;i<X->numFilas;i++){
	  columnasAnterior = columnas;
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
		    Fread(X->inicioFila, &elementosFila, sizeof(int));
		    elementosFila -= comienzoFila;
		    fila++;
	       }
	       repetida = 0;
	       Fseek(X->columnas, comienzoFila*sizeof(int), SEEK_SET);
	       Fseek(X->elementos, comienzoFila*sizeof(int), SEEK_SET);

	  }
	  else if((fila == q->query[i]) && (repetida)){
	       Fseek(X->columnas, comienzoFila*sizeof(int), SEEK_SET);
	       Fseek(X->elementos, comienzoFila*sizeof(int), SEEK_SET);
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
	       s[i] += w[columna]*elem;
	  }
     }

     free(w);
     return s;
}

Query* ArmarQuery(Query* query, const char* termino, Tarch* lexico, Tarch* punterosLexico){
     if(query == NULL){
	  query = (Query*)malloc(sizeof(Query));
	  query->query = NULL;
	  query->query = 0;
     }
     
     uint64_t inicio = 0;
     uint64_t fin = Fsize(punterosLexico)/sizeof(uint64_t);
     uint64_t posicionLexico=0;
     char* palabra;
     
     uint64_t medio;

     int encontrado = 0;

     while(inicio <= fin && !encontrado){
	  medio = (inicio+fin)/2;
	  Fseek(punterosLexico, medio*sizeof(uint64_t), SEEK_SET);
	  Fread(punterosLexico, &posicionLexico, sizeof(posicionLexico));
	  Fseek(lexico, posicionLexico, SEEK_SET);
	  palabra = (char*)FreadString(lexico);

	  int resultado = strcmp(termino,palabra);

	  if(resultado > 0){
	       inicio = medio+1;
	  }
	  else if(resultado < 0){
	       fin = medio-1;
	  }
	  else{
	       encontrado = 1;
	  }
     }

     if(encontrado){
	  if(query->elementos%QUERY_ELEM_UNIT == 0){
	       query->query = realloc(query->query, (query->elementos+QUERY_ELEM_UNIT)*sizeof(int));
	  }
	  query->query[(query->elementos)++] = medio;
     }

     return query;
} 
