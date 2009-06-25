#include "matrizBayes.h"
#include "archivo.h"

#define QUERY_ELEM_UNIT 5

int preprocesarRegistro(void* registro){
     double promedio=0;
     double cantidadDoc=0;
     int i;
     
     for(i=0;i<RegGetNumPointers(registro);i++){
	  promedio += RegGetPointer(registro,++i);
	  cantidadDoc++;
     }

     promedio /= cantidadDoc;
     return (int) (promedio*1);
}

char* strcat2(char* a, char* b){
     char* resultado = malloc(strlen(a)+strlen(b)+1);
     resultado[0] = 0;
     strcat(resultado, a);
     strcat(resultado, b);
     return resultado;     
}

int armarMatriz(Tarch* archAuxiliar, char* prefijo){
     Tarch *arch1, *arch2, *arch3, *lexico, *punterosLexico;

     int* linea;

     char* nombre = strcat2(prefijo,MATRIZ1);
     arch1 = Fopen(nombre,"w");
     free(nombre);

     nombre = strcat2(prefijo,MATRIZ2);
     arch2 = Fopen(nombre,"w");
     free(nombre);

     nombre = strcat2(prefijo,MATRIZ3);
     arch3 = Fopen(nombre,"w");
     free(nombre);
     
     nombre = strcat2(prefijo,LEXICO);
     lexico = Fopen(nombre,"w");
     free(nombre);

     nombre = strcat2(prefijo,PLEXICO);
     punterosLexico = Fopen(nombre,"w");
     free(nombre);

     uint32_t offset=0;
     char cero=0;

     int uno=1;
     int comienzo=0;

     uint32_t numFilas=0, numColumnas=0;

     while(!Feof(archAuxiliar)) {
	  FreadReg(archAuxiliar, (void**)&linea);
	  if(linea == NULL)
	       break;

	  int threshold = preprocesarRegistro(linea);

	  Fwrite(lexico, (void*)RegGetWord(linea), RegGetWordLength(linea));
	  Fwrite(lexico, &cero, sizeof(cero));

	  Fwrite(punterosLexico, &offset, sizeof(offset));

	  offset += RegGetWordLength(linea)+1;

	  numFilas++;
	  int cantidad = RegGetNumPointers(linea);
	  cantidad /= 2;

	  Fwrite(arch3, &comienzo, sizeof(int));

	  int i;
	  uint32_t valor;
	  int puntero=0;
	  for(i=0;i<cantidad;i++){
	       if(numColumnas< RegGetPointer(linea,puntero))
		    numColumnas = RegGetPointer(linea,puntero);
	       comienzo++;
	       valor = RegGetPointer(linea,puntero+1);
	       if(valor >= threshold){
		    valor=1;
	       }
	       else valor = 0;

	       // por cada registro, escribo 1 si el termino esta en ese documento
	       Fwrite(arch1, &valor, sizeof(uint32_t));
	       // escribo en que posicion debe estar el numero (nº doc)
	       int posicion = RegGetPointer(linea,puntero);
	       Fwrite(arch2, &posicion, sizeof(int));

	       puntero += 2;
	  }
	  free(linea);
     }
     Fwrite(arch3, &comienzo, sizeof(int));

     Fclose(arch1);
     Fclose(arch2);
     Fclose(arch3);
     Fclose(lexico);
     Fclose(punterosLexico);

     printf("%i,%i",numColumnas+1,numFilas);

     return 1;
}

Matriz* cargarMatriz(char* prefijo){
     Matriz *X = (Matriz*)malloc(sizeof(Matriz));
     char* nombre = strcat2(prefijo,MATRIZ1);
     X->elementos = Fopen(nombre,"r");
     free(nombre);

     nombre = strcat2(prefijo,MATRIZ2);
     X->columnas = Fopen(nombre,"r");
     free(nombre);

     nombre = strcat2(prefijo,MATRIZ3);
     X->inicioFila = Fopen(nombre,"r");
     free(nombre);

     return X;
}

HiperParametros* cargarHParam(Matriz* X, char* prefijo){
     HiperParametros *H = (HiperParametros*)malloc(sizeof(HiperParametros));
     char* nombre = strcat2(prefijo,HIPERPARAM);
     Tarch *arch1 = Fopen(nombre,"r");
     free(nombre);

     H->alpha = malloc(X->numColumnas*sizeof(double));
     H->beta = malloc(X->numColumnas*sizeof(double));

     int i;
     for(i=0;!Feof(arch1);i++){
	  Fread(arch1, (H->alpha+i), sizeof(double));
	  Fread(arch1, (H->beta+i), sizeof(double));
     }
     
     Fclose(arch1);
     return H;
}

Tarch* cargarLexico(char* prefijo){
     char* nombre = strcat2(prefijo,LEXICO);
     Tarch *arch1 = Fopen(nombre,"r");
     free(nombre);
     return arch1;
}

Tarch* cargarPLexico(char* prefijo){
     char* nombre = strcat2(prefijo,PLEXICO);
     Tarch *arch1 = Fopen(nombre,"r");
     free(nombre);
     return arch1;
}

int compararInt(void* a, void* b){
     int aa=(int)a;
     int bb=(int)b;
     if(aa < bb)
	  return 1;
     if(aa > bb)
	  return -1;
     return 0;
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
	       //valor=1;
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

     double* c = (double*)malloc(sizeof(double)*X->numColumnas);

     qsort(q->query,q->elementos, sizeof(int),compararInt);
     qsort(q->agregar,q->cantidadAgregar, sizeof(int),compararInt);
     qsort(q->sacar,q->cantidadSacar, sizeof(int),compararInt);

     memset(c,0,sizeof(double)*X->numColumnas);

     Frewind(X->elementos);
     Frewind(X->columnas);
     Frewind(X->inicioFila);

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
//	       elem=1;
	       c[columna] += elem;
	  }
	  repetida = 1;	  
     }

     double* w = c;
     
     /* w = log(1 + c ./ alpha) - log(1 + (n - c) ./ beta); */

     for(i=0;i<X->numColumnas;i++)
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
//	       elem=1;
	       Fread(X->columnas, &columna, sizeof(int));
	       s[i] += w[columna]*elem;
	  }
     }

     free(w);
     return s;
}

Query* ArmarQuery(Query* query, const char* termino, Tarch* lexico, Tarch* punterosLexico){

     Frewind(lexico);
     Frewind(punterosLexico);
     
     
     if(query == NULL){
	  query = (Query*)malloc(sizeof(Query));
	  query->query = NULL;
	  query->query = 0;
	  query->elementos =0;
	  query->cantidadAgregar = 0;
	  query->cantidadSacar = 0;
	  query->agregar = NULL;
	  query->sacar = NULL;

     }
     
     uint32_t inicio = 0;
     uint32_t fin = Fsize(punterosLexico)/sizeof(uint32_t);
     uint32_t posicionLexico=0;
     char* palabra;
     
     uint32_t medio;

     int encontrado = 0;

     while(inicio <= fin && !encontrado){
	  medio = (inicio+fin)/2;
	  Fseek(punterosLexico, medio*sizeof(uint32_t), SEEK_SET);
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
	  free(palabra);
     }

     if(encontrado){
	  if(query->elementos%QUERY_ELEM_UNIT == 0){
	       query->query = realloc(query->query, (query->elementos+QUERY_ELEM_UNIT)*sizeof(int));
	  }
	  query->query[(query->elementos)++] = medio;
     }

     return query;
} 
