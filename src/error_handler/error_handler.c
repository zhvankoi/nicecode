#include "error_handler.h"

void handleErrorAndQuit(const char *msg)
{
  termClear();
  termCursorHome();

  perror(msg);
  exit(1);
}