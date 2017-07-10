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
#define WELCOME_MSG "---==| NiCeCoDe EdItOr |==---\r\n"

struct EditorContext
{
  int cX;
  int cY;
  int rows;
  int cols;
};

struct EditorContext context;

void editorInitialize()
{
  context.cX = 1;
  context.cY = 1;
  context.rows = 0;
  context.cols = 0;
}

void editorMoveCursor(int key)
{
  switch (key)
  {
  case ARROW_UP:
    if (context.cY > 1)
      context.cY--;
    break;
  case ARROW_RIGHT:
    if (context.cX < context.cols)
      context.cX++;
    break;
  case ARROW_DOWN:
    if (context.cY < context.rows)
      context.cY++;
    break;
  case ARROW_LEFT:
    if (context.cX > 1)
      context.cX--;
    break;
  }
}

void editorMapKeypress()
{
  int input;
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
  case PAGE_UP:
  case PAGE_DOWN:
  {
    int times = context.rows;
    while (times--)
      editorMoveCursor(input == PAGE_UP ? ARROW_UP : ARROW_DOWN);
  }
  break;
  case ARROW_UP:
  case ARROW_RIGHT:
  case ARROW_DOWN:
  case ARROW_LEFT:
    editorMoveCursor(input);
    break;
  case HOME_KEY:
    context.cX = 0;
    break;
  case END_KEY:
    context.cX = context.cols - 1;
  }

  //if (!iscntrl(input))
  //{
  //printf("%d\r\n", input);
  //}
}

void editorDrawRows(Buffer *buffer)
{
  int w_len = strlen(WELCOME_MSG);

  //if welcome message too long, trunctate it
  if (w_len > context.cols)
    w_len = context.cols;

  //centering
  int padding = (context.cols - w_len) / 2;
  while (--padding)
    bufferAppend(buffer, " ", 1);

  //first line is welcome
  //bufferAppend(buffer, EL_ESC_SEQ, strlen(EL_ESC_SEQ));
  bufferAppend(buffer, WELCOME_MSG, w_len);

  //draw tidles
  int i;
  for (i = 1; i < context.rows - 1; i++)
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
  int res = termGetSize(&context.rows, &context.cols);
  if (res != 0)
    handleErrorAndQuit("editorRefresh: termGetSize error");

  Buffer buffer;
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
