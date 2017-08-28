#ifndef _ERROR_HANDLER_H_
#define _ERROR_HANDLER_H_

#include <stdio.h>
#include <stdlib.h>

#include "../terminal/termhelper.h"
#include "../logger/logger.h"

void handleError(const char *msg);
void handleErrorAndQuit(const char *msg);

#endif
