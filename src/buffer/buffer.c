#ifndef _BUFFER_H_
#define _BUFFER_H_

struct Buffer {
  char* data;
  int length;
}

void bufferInit(Buffer buffer);
void bufferAppend(Buffer buffer, const char *bytes, int length);
void bufferFree(Buffer buffer);

#endif