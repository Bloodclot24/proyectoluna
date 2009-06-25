#include "agua.h"

int comparar(const void* numero1, const void* numero2){
     const double const* const* n1 = numero1;
     const double const* const* n2 = numero2;

     if((**n1) > (**n2))
	  return -1;
     if((**n1) < (**n2))
	  return 1;
     return 0;
}

Agua* comenzar_agua(char* prefijo){
     
     Agua *agua = (Agua*)malloc(sizeof(Agua));
     
     agua->X = cargarMatriz(prefijo);

     if(agua->X!=NULL){
	  agua->H = cargarHParam(agua->X, prefijo);
	  if( agua->H!= NULL){
	       agua->lexico = cargarLexico(prefijo);
	       agua->punterosLexico = cargarPLexico(prefijo);

	       if(agua->lexico == NULL || agua->punterosLexico == NULL){
		    Fclose(agua->lexico);
		    Fclose(agua->punterosLexico);
		    free(agua->X);
		    agua->X=NULL;
		    free(agua->H->alpha);
		    free(agua->H->beta);
		    agua->H=NULL;
		    free(agua);
		    agua = NULL;
	       }
	  }
     }
     
     return agua;
}

int agua(Agua* agua, Datos* datos){
     Query* q=NULL;
     int i;
     while(!Lista_EstaVacia(datos->lista)){
	  char* termino= Lista_RemoverPrimero(datos->lista);
	  q = ArmarQuery(q,termino,agua->lexico, agua->punterosLexico);
     }

     buscarConsulta(q);
     guardarConsulta(q);
     
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
	  Lista_Insertar(datos->lista, termino);
     }

     free(ordenado);
     DestruirQuery(q);
     free(resultado);
     return 1;
}
