#include "error_handler.h"

void handleErrorAndQuit(const char *msg)
{
  perror(msg);
  exit(1);
}