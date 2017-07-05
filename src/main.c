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

struct EditorContext {
  int cX;
  int cY;
};

struct EditorContext context;

void editorInitialize() {
  context.cX = 1;
  context.cY = 1;
}

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
  case 'w':
    context.cY--;
    break;
  case 'd':
    context.cX++;
    break;
  case 's':
    context.cY++;
    break;
  case 'a':
    context.cX--;
    break;
  }

  if (!iscntrl(input))
  {
    //printf("%c\r\n", input);
  }
}

void editorDrawRows(struct Buffer *buffer)
{
  int rows, cols;
  int res = termGetSize(&rows, &cols);
  if (res != 0)
    handleErrorAndQuit("editorDrawRows: termGetSize error");

  char* welcome = "---==| NiCeCoDe EdItOr |==---\r\n";
  int w_len = strlen(welcome);

  //if welcome message too long, trunctate it
  if(w_len > cols)
    w_len = cols;

  //centering
  int padding = (cols - w_len) / 2;
  while(--padding)
    bufferAppend(buffer, " ", 1);

  //first line is welcome
  bufferAppend(buffer, EL_ESC_SEQ, strlen(EL_ESC_SEQ));
  bufferAppend(buffer, welcome, strlen(welcome));

  //draw tidles
  int i;
  for (i = 1; i < rows - 1; i++)
  {
    bufferAppend(buffer, EL_ESC_SEQ, strlen(EL_ESC_SEQ));
    bufferAppend(buffer, "~\r\n", 3);
  }

  //last tidle without \r\n
  bufferAppend(buffer, EL_ESC_SEQ, strlen(EL_ESC_SEQ));
  bufferAppend(buffer, "~", 1);
}

void editorRefresh()
{
  struct Buffer buffer;
  bufferInit(&buffer);
  bufferAppend(&buffer, CUR_HIDE_ESC_SEQ, strlen(CUR_HIDE_ESC_SEQ));
  bufferAppend(&buffer, CUR_HOME_ESC_SEQ, strlen(CUR_HOME_ESC_SEQ));

  editorDrawRows(&buffer);

  char buff[36];
  snprintf(buff, sizeof(buff), "\x1b[%d;%dH", context.cY, context.cX);
  
  bufferAppend(&buffer, buff, strlen(buff));
  bufferAppend(&buffer, CUR_SHOW_ESC_SEQ, strlen(CUR_SHOW_ESC_SEQ));

  write(STDOUT_FILENO, buffer.data, buffer.length);
  bufferFree(&buffer);
}

int main()
{
  editorInitialize();

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
