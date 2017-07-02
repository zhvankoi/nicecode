#ifndef _TERM_HELPER_H_
#define _TERM_HELPER_H_

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../error_handler/error_handler.h"

void enableRawMode();
void disableRawMode();

#endif
