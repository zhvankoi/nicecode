#include "buffer.h"

void bufferInit(struct Buffer* buffer)
{
  buffer->data = NULL;
  buffer->length = 0;
}

void bufferAppend(struct Buffer* buffer, const char *bytes, int length)
{
  int new_length = buffer->length + length;
  char* new_buffer = (char*) realloc(buffer->data, new_length);
  memcpy(&new_buffer[buffer->length], bytes, length);

  buffer->data = new_buffer;
  buffer->length = new_length;
}

void bufferFree(struct Buffer* buffer)
{
  free(buffer->data);
  buffer->length = 0;
}