#ifndef MERGE_H_
#define MERGE_H_

#include "archivo.h"
#include "particiones.h"
#include "lista.h"
#include "rb.h"
#include <string.h>
#include <stdio.h>

/* Realiza un optimal merge entre las particiones dadas */
Tarch* mergeLexico(Tparticiones *Gparticion, int cantidad);

#endif /*MERGE_H_*/
