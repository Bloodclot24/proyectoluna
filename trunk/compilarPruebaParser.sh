#! /usr/bin/bash
gcc parser.c `pkg-config libxml-2.0 --cflags --libs` pruebaParser.c rb.c archivo.c particiones.c lista.c nodo.c pila.c debug.c -g -o pruebaParser