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
     
     q = ArmarQuery(datos,agua->lexico, agua->punterosLexico);
         
     double* resultado = BSets(agua->X,q,agua->H);
     double** ordenado = malloc(agua->X->numFilas*sizeof(double*));

     for(i=0;i<agua->X->numFilas;i++){
	  ordenado[i] = resultado+i;
     }
     
     qsort(ordenado, agua->X->numFilas, sizeof(double*), comparar);

     buscarConsulta(q);
     guardarConsulta(q);

     int k=0,j=0;
     int f,w;
     int encontrado;
     for(i=0;i<datos->cantResultados*datos->cantMultiplicar;i++){
	  if((q->cantidadAgregar) > 0){
	       f=q->agregar[k++];
	       q->cantidadAgregar--;
	  }
	  else
	       f=(int)(ordenado[j++]-resultado);

	  for(w=0,encontrado=0;w<q->cantidadSacar && !encontrado;w++){
	       if(q->sacar[w] == f){
		    encontrado = 1;
	       }
	  }
	  if(!encontrado){
	       Fseek(agua->punterosLexico, f*sizeof(uint32_t), SEEK_SET);
	       uint32_t offset;
	       Fread(agua->punterosLexico, &offset, sizeof(uint32_t));
	       Fseek(agua->lexico, offset, SEEK_SET);
	       char* termino;
	       termino = FreadString(agua->lexico);
	       Lista_Insertar(datos->lista, termino);
	  }
	  else
	       i--;
     }

     free(ordenado);
     DestruirQuery(q);
     free(resultado);
     return 1;
}
