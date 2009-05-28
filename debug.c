#include "debug.h"

#ifdef DEBUG_FLAG

void Debug_Info(const char* string, ...){
     va_list args;
     va_start(args, string);
     vprintf(string,args);
     va_end(args);
}

void Debug_Warning(const char* string, ...){
     va_list args;
     va_start(args, string);
     vprintf(string,args);
     va_end(args);
}

#else

void Debug_Info(const char* string, ...){}

void Debug_Warning(const char* string, ...){}

#endif /* DEBUG_FLAG */

