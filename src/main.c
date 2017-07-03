#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "./error_handler/error_handler.h"
#include "./terminal/termhelper.h"

#define CTRL_KEY(k) ((k) & 0x1f)

void editorMapKeypress() {
  char input = termRead();

  switch(input) {
    case CTRL_KEY('q'):
      termClear();
      termCursorHome();
      exit(0);
      break;
  }

  if(!iscntrl(input)){
    //printf("%c\r\n", input);
  }
}

void editorDrawRows(){
  int i;

  int rows, cols;
  int res = termGetSize(&rows, &cols);
  if(res != 0)
    handleErrorAndQuit("editorDrawRows: termGetSize error");

  write(STDOUT_FILENO, "+\r\n", 3);
  for (i = 1; i < rows - 1; i++) {
    write(STDOUT_FILENO, "-\r\n", 3);
  }

  write(STDOUT_FILENO, "~", 1);
}

void editorRefresh(){
  termClear();
  termCursorHome();

  editorDrawRows();

  termCursorHome();
}

int main() {
  char* buffer = NULL;
  buffer = (char*) realloc(buffer, 10 * sizeof(char));

  buffer[0] = 'a';
  buffer[1] = 'b';
  buffer[2] = 'c';
  buffer[3] = 'd';
  buffer[4] = 'e';
  buffer[5] = 'f';
  buffer[6] = 'g';
  buffer[7] = 'h';
  buffer[8] = 'g';
  buffer[9] = '\0';

  printf("%s\n", buffer);

  free(buffer);
  return 0;
}

int main_1()
{
  termRawModeOn();

  while (1) 
  {
    editorRefresh();
    editorMapKeypress();
  }

  return 0;
}
