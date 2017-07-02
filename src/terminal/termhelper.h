#ifndef _TERM_HELPER_H_
#define _TERM_HELPER_H_

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include "../error_handler/error_handler.h"

#define ESC "\x1b"

void termRawModeOn();
void termRawModeOff();
void termClear();
void termCursorHome();
int termGetSize(int* rows, int* cols);

#endif
