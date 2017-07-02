#include "termhelper.h"

struct EditorConfigs {
  struct termios orig_termios;

};

struct EditorConfigs editor_configs;

void termRawModeOff() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor_configs.orig_termios) == -1)
    handleErrorAndQuit("termRawModeOff: tcsetattr failed");
}

void termRawModeOn() {
  if (tcgetattr(STDIN_FILENO, &editor_configs.orig_termios) == -1)
    handleErrorAndQuit("termRawModeOn: tcgetattr failed");
  atexit(termRawModeOff);

  struct termios raw = editor_configs.orig_termios;

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

void termClear() {
  write(STDOUT_FILENO, ESC "[2J", 4);
}

void termCursorHome(int x = -1) {
  write(STDOUT_FILENO, ESC "[H", 3);
}

void pushEscSeq(const char* command) {
  
}