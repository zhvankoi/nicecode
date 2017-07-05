#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "./error_handler/error_handler.h"
#include "./terminal/termhelper.h"
#include "./buffer/buffer.h"

#define CTRL_KEY(k) ((k)&0x1f)

void editorMapKeypress()
{
  char input;
  int res = termRead(&input);
  if (res != 0)
    handleErrorAndQuit("editorMapKeyPress: termRead");

  switch (input)
  {
  case CTRL_KEY('q'):
    //Clear and move cursor to home position.
    write(STDOUT_FILENO, CLS_ESC_SEQ CUR_HOME_ESC_SEQ, 4 + 3);
    exit(0);
    break;
  }

  if (!iscntrl(input))
  {
    //printf("%c\r\n", input);
  }
}

void editorDrawRows(struct Buffer *buffer)
{
  int i;
  int rows, cols;
  int res = termGetSize(&rows, &cols);
  if (res != 0)
    handleErrorAndQuit("editorDrawRows: termGetSize error");

  for (i = 0; i < rows - 1; i++)
  {
    bufferAppend(buffer, "~\r\n", 3);
  }

  bufferAppend(buffer, "~", 1);
}

void editorRefresh()
{
  struct Buffer buffer;
  bufferInit(&buffer);
  bufferAppend(&buffer, "\x1b[?25l", 6);
  bufferAppend(&buffer, CLS_ESC_SEQ, 4);
  bufferAppend(&buffer, CUR_HOME_ESC_SEQ, 3);

  editorDrawRows(&buffer);

  bufferAppend(&buffer, CUR_HOME_ESC_SEQ, 3);
  bufferAppend(&buffer, "\x1b[?25h", 6);

  write(STDOUT_FILENO, buffer.data, buffer.length);
  bufferFree(&buffer);
}

int main()
{
  int res = 0;
  res = termRawModeOn();
  if (res != 0)
    handleErrorAndQuit("main: termRawModeOn");

  while (1)
  {
    editorRefresh();
    editorMapKeypress();
  }

  return 0;
}
