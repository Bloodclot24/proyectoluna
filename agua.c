#include "agua.h"

int comparar(const void* numero1, const void* numero2){
     const double** n1 = numero1;
     const double** n2 = numero2;

     if((**n1) > (**n2))
	  return -1;
     if((**n1) < (**n2))
	  return 1;
     return 0;
}

Agua* comenzar_agua(char* prefijo){
     
     Agua *agua = (Agua*)malloc(sizeof(Agua));
     
     agua->X = cargarMatriz(prefijo);
     agua->X->numFilas = 4224832;
     agua->X->numColumnas = 323415;

     agua->H = cargarHParam(agua->X, prefijo);

     agua->lexico = cargarLexico(prefijo);
     agua->punterosLexico = cargarPLexico(prefijo);
     
     return agua;
}

int agua(Agua* agua, Datos* datos){
     Query* q=NULL;
     int i;
     while(!Lista_EstaVacia(datos->lista)){
	  char* termino= Lista_RemoverPrimero(datos->lista);
	  q = ArmarQuery(q,termino,agua->lexico, agua->punterosLexico);
//	  free(termino);
     }

//     buscarConsulta(q);
//     guardarConsulta(q);
     
     double* resultado = BSets(agua->X,q,agua->H);
     
     double** ordenado = malloc(agua->X->numFilas*sizeof(double*));

     for(i=0;i<agua->X->numFilas;i++){
	  ordenado[i] = resultado+i;
     }
     
     qsort(ordenado, agua->X->numFilas, sizeof(double*), comparar);

     for(i=0;i<datos->cantResultados*datos->cantMultiplicar;i++){
	  Fseek(agua->punterosLexico, (int)(ordenado[i]-resultado)*sizeof(uint32_t), SEEK_SET);
	  uint32_t offset;
	  Fread(agua->punterosLexico, &offset, sizeof(uint32_t));
	  Fseek(agua->lexico, offset, SEEK_SET);
	  char* termino;
	  termino = FreadString(agua->lexico);
	  printf("---->%s\n",termino);
	  Lista_Insertar(datos->lista, termino);
     }

     free(ordenado);
     free(q);
     return 1;
}
