#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "./error_handler/error_handler.h"
#include "./terminal/termhelper.h"

#define CTRL_KEY(k) ((k) & 0x1f)

char editorRead() {
  int read_res = 0;
  char input;
  while(read_res != 1)
  {
    read_res = read(STDIN_FILENO, &input, 1);
    /*
     Cigwin returns -1 and sets errno to EAGAIN
     if read timeout occures
    */
    if(read_res == -1 && errno != EAGAIN)
    {
      handleErrorAndQuit("editorRead: read");
    }
  }

  return input;
}

void processKeypress() {
  char input = editorRead();

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
  for (i = 0; i < 24; i++) {
    write(STDOUT_FILENO, "~\r\n", 3);
  }
}

void editorRefresh(){
  termClear();
  termCursorHome();

  editorDrawRows();

  termCursorHome();
}

int main()
{
  termRawModeOn();

  while (1) 
  {
    editorRefresh();
    processKeypress();
  }

  return 0;
}
