#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void logInfo(const char *msg);
void logError(const char *msg, const int errorNo);

#endif