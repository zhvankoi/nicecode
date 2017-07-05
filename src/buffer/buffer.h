#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdlib.h>

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include <string.h>

struct Buffer {
  char* data;
  int length;
};

void bufferInit(struct Buffer* buffer);
void bufferAppend(struct Buffer* buffer, const char *bytes, int length);
void bufferFree(struct Buffer* buffer);

#endif