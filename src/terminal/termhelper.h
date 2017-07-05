#ifndef _TERM_HELPER_H_
#define _TERM_HELPER_H_

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

#define ESC '\x1b'

#define CLS_ESC_SEQ "\x1b[2J"

#define CUR_HOME_ESC_SEQ "\x1b[H"
#define CUR_FORWARD_ESC_SEQ(x) "\x1b[" #x "C"
#define CUR_DOWN_ESC_SEQ(x) "\xb[" #x "B"

int termRawModeOn();
void termRawModeOff();
int termRead(char *c);
int termGetSize(int *rows, int *cols);

#endif
