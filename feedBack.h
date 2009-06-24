#ifndef FEEDBACK_H_
#define FEEDBACK_H_
#include "matrizBayes.h"
#include "archivo.h"


/* Guarda los numeros de terminos de la consulta realizada seguido de los numeros de terminos
 * de las palabras a agregar en los resultados de esta consulta y luego de los numeros de terminos
 * de las palabras a dar de baja en los resultados de esta consulta.
 * El formato en el que los guarda es el siguiente: cantTermConsulta numTerminos cantAltas altas cantBajas bajas.
 * Devuelve 0 en caso de que el proceso se haya realizado correctamente, 1 en caso de que no existiese el archivo
 * y lo haya creado y -1 en caso de no poder crear dicho archivo.*/
int guardarConsulta(Query* query);

/* Dada una consulta, busca en el archivo de feedback si en algun momento
 * el usuario realizo una modificacion, ya sea añadiendo o quitando terminos
 * sobre la misma, en caso de ser asi, devuelve dichas modificaciones y devuelve
 * 0 como resultado. Si no puede abrir el archivo, devuelve -1.
 * Siempre devuelve la primera aparicion de dicha busqueda que encuentra en el
 * archivo de feedback.*/
int buscarConsulta(Query* query);

#endif /*FEEDBACK_H_*/
