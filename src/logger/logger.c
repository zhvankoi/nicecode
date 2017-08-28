#include "logger.h"

const FILE *getLogFile()
{
  FILE *file = fopen("log.txt", "aw");
  return file;
}

const char *getTimestamp(const int ltime)
{
  time_t currTime = 0;
  time(&currTime);
  struct tm *timeStruct = NULL;
  if (ltime == 0)
  {
    timeStruct = gmtime(&currTime);
  }
  else
  {
    timeStruct = localtime(&currTime);
  }

  char *timestamp = calloc(2 + 1 + 2 + 1 + 4 + 1 + 2 + 1 + 2 + 1 + 2 + 1, sizeof(char));

  sprintf(timestamp, "%02d.%02d.%4d %02d:%02d:%02d",
          timeStruct->tm_mday, timeStruct->tm_mon + 1, timeStruct->tm_year + 1900,
          timeStruct->tm_hour, timeStruct->tm_min, timeStruct->tm_sec);
  return timestamp;
}

void logWrite(const char *level, const char *msg, const int errorNo)
{
  FILE *file = fopen("log.txt", "aw");
  const char *timestamp = getTimestamp(0);

  if (errorNo == 0)
    fprintf(file, "[%s] %s: %s\n", timestamp, level, msg);
  else
    fprintf(file, "[%s] %s: %s. Errno: %d\n", timestamp, level, msg, errorNo);

  fclose(file);
}

void logInfo(const char *msg)
{
  const char *INFO_LEVEL = "INFO";
  logWrite(INFO_LEVEL, msg, 0);
}

void logError(const char *msg, const int errorNo)
{
  const char *ERROR_LEVEL = "ERROR";
  logWrite(ERROR_LEVEL, msg, errorNo);
}