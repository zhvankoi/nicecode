#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdlib.h>

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <string.h>

typedef struct
{
  char *data;
  int length;
} Buffer;

void bufferInit(Buffer *buffer);
void bufferAppend(Buffer *buffer, const char *bytes, int length);
void bufferFree(Buffer *buffer);

#endif