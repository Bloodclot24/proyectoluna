#include "debug.h"

#ifdef DEBUG_FLAG

void Debug_Info(char* string, ...){
     va_list args;
     va_start(args, string);
     vprintf(string,args);
     va_end(args);
}

void Debug_Warning(char* string, ...){
     va_list args;
     va_start(args, string);
     vprintf(string,args);
     va_end(args);
}

#else

void Debug_Info(char* string, ...){}

void Debug_Warning(char* string, ...){}

#endif /* DEBUG_FLAG */

