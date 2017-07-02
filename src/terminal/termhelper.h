#ifndef _TERM_HELPER_H_
#define _TERM_HELPER_H_

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../error_handler/error_handler.h"

#define ESC "\x1b"

void enableRawMode();
void disableRawMode();
void clearTerminal();
void moveCursorToHome();

#endif
