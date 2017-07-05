#include "termhelper.h"

struct termios orig_termios;

void termRawModeOff() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

int termRawModeOn() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    return -1;
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
    return -1;

  return 0;
}

int termRead(char* c) {
  int read_res = 0;
  char input;
  while(read_res != 1)
  {
    read_res = read(STDIN_FILENO, &input, 1);
    /*
     Cigwin returns -1 and sets errno to EAGAIN
     if read timeout occures
    */
    if(read_res == -1 && errno != EAGAIN)
      return -1;
  }

  *c = input;
  return 0;
}

int pushEscSeq(const char* command) {
  int length = strlen(command) + 2;
  char bytes[length + 1];
  bytes[0] = ESC;
  bytes[1] = '[';
  bytes[2] = '\0';
  strcat(bytes, command);

  int res = write(STDOUT_FILENO, bytes, length);
  if(res != length)
    return -1;
  
  return 0;
}

int termClear() {
  return pushEscSeq("2J");
  //return 0;
}

int termCursorHome() {
  return pushEscSeq("H");
}

int termCursorPos(int *rows, int *cols) {
  char pos_report[32];
  unsigned int i = 0;
  /**
    This escape sequence will return report
    in STDIN: <ESC>[22;22R (for example)
  */
  int res = pushEscSeq("6n");
  if(res != 0)
    return -1;

  while(i < sizeof(pos_report) - 1) {
    int readed = read(STDIN_FILENO, &pos_report[i], 1);
    if(readed != 1)
      break;

    if(pos_report[i] == 'R')
      break;
    
    ++i;
  }

  pos_report[i] = 0;

  if(pos_report[0] != ESC || pos_report[1] != '[')
    return -1;

  int parsed = sscanf(&pos_report[2], "%d;%d", rows, cols);
  if(parsed != 2)
    return -1;

  return 0;
}

int termGetSize(int *rows, int *cols) {
  struct winsize ws;

  int res = ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
  if(res == -1 || ws.ws_col == 0) {
    int written = pushEscSeq("999C") + pushEscSeq("999B");
    if(written != 12)
      return -1;

    return termCursorPos(rows, cols);
  } else {
    *rows = ws.ws_row;
    *cols = ws.ws_col;
    return 0;
  }
}