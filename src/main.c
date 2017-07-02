#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "./error_handler/error_handler.h"
#include "./terminal/termhelper.h"

#define CTRL_KEY(k) ((k) & 0x1f)

int main()
{
  enableRawMode();

  while (1) {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) 
      handleErrorAndQuit("main: read failed");
    if (iscntrl(c) && c != 0) {
      printf("%d\r\n", c);
    } else if(c != 0) {
      printf("%d ('%c')\r\n", c, c);
    }
    if (c == 'q') break;
  }


  return 0;
}
