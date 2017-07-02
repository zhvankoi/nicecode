#include "termhelper.h"

struct termios orig_termios;

void termRawModeOff() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    handleErrorAndQuit("termRawModeOff: tcsetattr failed");
}

void termRawModeOn() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    handleErrorAndQuit("termRawModeOn: tcgetattr failed");
  atexit(termRawModeOff);

  struct termios raw = orig_termios;

  tcgetattr(STDIN_FILENO, &raw);
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    handleErrorAndQuit("termRawModeOn: tcsetattr");
}

void pushEscSeq(const char* command) {
  int length = strlen(command) + 2;
  char bytes[length + 1];
  bytes[0] = 0;
  strcat(bytes, ESC);
  strcat(bytes, "[");
  strcat(bytes, command);

  write(STDOUT_FILENO, bytes, length);
}

void termClear() {
  pushEscSeq("2J");
}

void termCursorHome() {
  pushEscSeq("H");
}

int termGetSize(int *rows, int *cols) {
  struct winsize ws;

  int res = ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
  if(res == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *rows = ws.ws_col;
    *cols = ws.ws_row;
    return 0;
  }
}