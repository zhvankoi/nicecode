#include "termhelper.h"

struct EditorConfigs {
  struct termios orig_termios;

};

struct EditorConfigs editor_configs;

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor_configs.orig_termios) == -1)
    handleErrorAndQuit("disableRawMode: tcsetattr failed");
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &editor_configs.orig_termios) == -1)
    handleErrorAndQuit("enableRawMode: tcgetattr failed");
  atexit(disableRawMode);

  struct termios raw = editor_configs.orig_termios;

  tcgetattr(STDIN_FILENO, &raw);
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    handleErrorAndQuit("enableRawMode: tcsetattr");
}

void clearTerminal() {
  write(STDOUT_FILENO, ESC "[2J", 4);
}

void moveCursorToHome() {
  write(STDOUT_FILENO, ESC "[H", 3);
}
