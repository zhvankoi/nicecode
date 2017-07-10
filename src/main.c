#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>

#include "./error_handler/error_handler.h"
#include "./terminal/termhelper.h"
#include "./buffer/buffer.h"

#define CTRL_KEY(k) ((k)&0x1f)
#define WELCOME_MSG "---==| NiCeCoDe EdItOr |==---\r\n"

typedef struct
{
  int length;
  char *content;
} EditorLine;

struct EditorContext
{
  int cX;
  int cY;
  int rows;
  int cols;
  int linesCount;
  EditorLine *lines;
};

struct EditorContext context;

void editorClose()
{
  termRawModeOff();
  for (int i = 0; i < context.linesCount; ++i)
  {
    free(context.lines[i].content);
  }

  free(context.lines);
}

void editorInitialize()
{
  context.cX = 1;
  context.cY = 1;
  context.rows = 0;
  context.cols = 0;
  context.linesCount = 0;
  context.lines = NULL;

  atexit(editorClose);
}

void editorAppendLine(char *line, size_t length)
{
  int new_lineCount = context.linesCount + 1;
  context.lines = realloc(context.lines, sizeof(EditorLine) * new_lineCount);
  EditorLine *newline_ref = &context.lines[context.linesCount];

  newline_ref->content = malloc(length + 1);
  memcpy(newline_ref->content, line, length);
  newline_ref->content[length] = '\0';
  newline_ref->length = length;

  context.linesCount = new_lineCount;
}

void editorOpen(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (!file)
    handleErrorAndQuit("editorOpen: failed to open file");

  char *line;
  size_t line_capacity;
  ssize_t line_length = 0;

  while (line_length != -1) // identical to !feof(file)
  {
    line = NULL;
    line_capacity = 0;
    line_length = getline(&line, &line_capacity, file);
    if (line_length != -1)
    {
      //this code works, but looks unsafe
      while (line[line_length - 1] == '\n' || line[line_length - 1] == '\r')
        line_length--;

      editorAppendLine(line, line_length);
    }

    free(line);
  }
  fclose(file);
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

  int i;
  //push text to screen
  int rows_count = context.linesCount > context.rows ? context.rows : context.linesCount;
  for (i = 0; i < rows_count; ++i)
  {
    bufferAppend(buffer, EL_ESC_SEQ, strlen(EL_ESC_SEQ));
    bufferAppend(buffer, context.lines[i].content, context.lines[i].length);
    bufferAppend(buffer, "\r\n", 2);
  }

  //draw tidles (+1 - including welcome msg)
  for (i = rows_count + 1; i < context.rows - 1; ++i)
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

int main(int argc, char *args[])
{
  editorInitialize();

  if (argc >= 2)
  {
    editorOpen(args[1]);
  }

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
