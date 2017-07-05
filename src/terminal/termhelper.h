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

//Erase In Display
#define CLS_ESC_SEQ "\x1b[2J"
//Erase In Line.
//http://vt100.net/docs/vt100-ug/chapter3.html#EL
#define EL_ESC_SEQ "\x1b[K"

#define CUR_HOME_ESC_SEQ "\x1b[H"
//#define CUR_SET_POS_ESC_SEQ(x, y) "\x1b[" #x #y "H"
#define CUR_FORWARD_ESC_SEQ(x) "\x1b[" #x "C"
#define CUR_DOWN_ESC_SEQ(x) "\xb[" #x "B"
#define CUR_HIDE_ESC_SEQ "\x1b[?25l"
#define CUR_SHOW_ESC_SEQ "\x1b[?25h"

int termRawModeOn();
void termRawModeOff();
int termRead(char *c);
int termGetSize(int *rows, int *cols);
int termClear();
int termCursorHome();

#endif
