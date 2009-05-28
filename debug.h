#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>

void Debug_Info(const char* string, ...);

void Debug_Warning(const char* string, ...);

#endif /* DEBUG_H_INCLUDED */
