#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void print_error(int linenumber, const char* msg, ...) {
  va_list va;
  va_start(va, msg);
  fprintf(stderr,"[%d] Error: ", linenumber);
  vfprintf(stderr, msg, va);
  fprintf(stderr, "\n");
  va_end(va);
  exit(1);
}
