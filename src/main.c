#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "./error_handler/error_handler.h"
#include "./terminal/termhelper.h"

#define CTRL_KEY(k) ((k) & 0x1f)

void processKeypress() {
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
  if (termGetSize(&rows, &cols) == -1)
    handleErrorAndQuit("SIZE ERROR");
  printf("WINDOW SIZE: %d x %d", rows, cols);
  
  
  
  for (i = 0; i < 24; i++) {
    write(STDOUT_FILENO, "~\r\n", 3);
  }
}

void editorRefresh(){
  //termClear();
  //termCursorHome();

  editorDrawRows();

  //termCursorHome();
}

int main()
{
  //termRawModeOn();

  while (1) 
  {
    editorRefresh();
    processKeypress();
  }

  return 0;
}
