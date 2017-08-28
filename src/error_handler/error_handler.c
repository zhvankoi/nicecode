#include "error_handler.h"

void handleError(const char *msg)
{
  logError(msg, 12);
}

void handleErrorAndQuit(const char *msg)
{
  termClear();
  termCursorHome();

  perror(msg);
  exit(1);
}
