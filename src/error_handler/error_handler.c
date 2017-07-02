#include "error_handler.h"

void handleErrorAndQuit(const char *msg)
{
  clearTerminal();
  moveCursorToHome();

  perror(msg);
  exit(1);
}