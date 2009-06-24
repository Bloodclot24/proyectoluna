#include "feedBack.h"

int guardarConsulta(Query* query){
	Tarch* archivo;
	int resultado = 0;

	if(query->cantidadAgregar == 0 && query->cantidadSacar == 0)
	     return 0;

	archivo = Fopen("feedBack.dat","ab");
	if(!archivo){
		resultado = 1;
		archivo = Fopen("feedBack.dat", "wb");
		if(!archivo) return -1;
	}
	int i = 0;
	int cantidadElementos = query->elementos;
	Fwrite(archivo,(void*)&cantidadElementos,sizeof(int));
	while(i < cantidadElementos){
		Fwrite(archivo,(void*)&(query->query[i]),sizeof(int));
		i++;
	}
	i = 0;
	int cantidadAltas = query->cantidadAgregar;
	Fwrite(archivo,(void*)&cantidadAltas,sizeof(int));
	while(i< cantidadAltas){
		Fwrite(archivo,(void*)&(query->agregar[i]),sizeof(int));
		i++;
	}
	i = 0;
	int cantidadBajas = query->cantidadSacar;
	Fwrite(archivo,(void*)&cantidadBajas,sizeof(int));
	while(i < cantidadBajas){
		Fwrite(archivo,(void*)&(query->sacar[i]),sizeof(int));
		i++;
	}

	Fclose(archivo);
	return resultado;
}


int buscarConsulta(Query* query){
	Tarch* archivo;
	archivo = Fopen("feedBack.dat","rb");
	if(!archivo) return -1;
	int cantidadTerminos = 0;
	int cantidadAltas = 0;
	int cantidadBajas = 0;
	int i = 0;
	int distintos = 0;
	int numTermino = 0;
	int encontrado = 0;
	while((!Feof(archivo))){// && (!encontrado)){
		/*leo la cantidad de terminos de la consulta*/
		Fread(archivo,&cantidadTerminos,sizeof(int));
		if(cantidadTerminos == query->elementos){
			while((i <= cantidadTerminos) && (!distintos)){
				Fread(archivo,&numTermino,sizeof(int));
				if(numTermino != query->query[i]) distintos = 1;
				i++;
			}
			if(!distintos){ 
				int terminoAgregar = 0;
				int terminoSacar = 0;
				encontrado = 1;
				Fread(archivo,&cantidadAltas,sizeof(int));
				i = 0;
				if(cantidadAltas != 0){
					while( i <= cantidadAltas){
						Fread(archivo,&terminoAgregar,sizeof(int));
						if(query->cantidadAgregar%QUERY_ELEM_UNIT == 0){
						     query->agregar = realloc(query->agregar, (query->cantidadAgregar+QUERY_ELEM_UNIT)*sizeof(int));
						}
						query->agregar[(query->cantidadAgregar)++] = terminoAgregar;
						i++;
					}
				}
				Fread(archivo,&cantidadBajas,sizeof(int));
				if(cantidadBajas != 0){	
					i = 0;
					while( i <= cantidadBajas){
						Fread(archivo,&terminoSacar,sizeof(int));
						if(query->cantidadSacar%QUERY_ELEM_UNIT == 0){
						     query->sacar = realloc(query->sacar, (query->cantidadSacar+QUERY_ELEM_UNIT)*sizeof(int));
						}
						query->sacar[(query->cantidadSacar)++] = terminoSacar;
						i++;
					}
				}
			}else Fseek(archivo,sizeof(int)*(cantidadTerminos - i) ,SEEK_CUR);	 
		}else{
			 Fseek(archivo,sizeof(int)*cantidadTerminos,SEEK_CUR);
			 Fread(archivo,&cantidadAltas,sizeof(int));
			 if(cantidadAltas != 0) Fseek(archivo, sizeof(int)*cantidadAltas,SEEK_CUR);
			 Fread(archivo,&cantidadBajas,sizeof(int));
			 if(cantidadBajas != 0) Fseek(archivo, sizeof(int)*cantidadBajas,SEEK_CUR);
		}
		i = 0;
	}
			
	Fclose(archivo);
	return 0;
}
