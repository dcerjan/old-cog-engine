#include "coglogger.h"
#include <stdarg.h>

CogLogger
CogLoggerAlloc(
  const char* name,
  const char* fileName
)
{
  CogLogger log = (CogLogger)malloc(sizeof(struct CogLogger));
  log->name = CogStringAllocFromFormat("Logger (%s - %s)", name, fileName);
  log->fileName = CogStringAlloc(fileName);
  log->fd = fopen(log->fileName->data, "w");
  return log;
}

void
CogLoggerFree(
  CogLogger   self
)
{
  CogStringFree(self->fileName);
  CogStringFree(self->name);
  free(self);
}

void
CogLoggerLog(
  CogLogger   self,
  const char* prefix,
  const char* format,
  ...
)
{
  va_list args;
  va_start(args, format);
  self->fd = fopen(self->fileName->data, "a");
  // to logfile
  fprintf(self->fd, "%s ", prefix);
  vfprintf(self->fd, format, args);
  // to console
  va_start(args, format);
  printf("%s ", prefix);
  vprintf(format, args);
  fclose(self->fd);
  va_end(args);
}

void
CogLoggerDebug(
  CogLogger     self,
  const char*   format,
  ...
)
{
  va_list args;
  va_start(args, format);
  self->fd = fopen(self->fileName->data, "a");
  // to logfile
  fprintf(self->fd, "%s ", COG_DEBUG);
  vfprintf(self->fd, format, args);
  // to console
  va_start(args, format);
  printf("%s ", COG_DEBUG);
  vprintf(format, args);
  fclose(self->fd);
  va_end(args);
}

void
CogLoggerError(
  CogLogger     self,
  const char*   format,
  ...
)
{
  va_list args;
  va_start(args, format);
  self->fd = fopen(self->fileName->data, "a");
  // to logfile
  fprintf(self->fd, "%s ", COG_ERROR);
  vfprintf(self->fd, format, args);
  // to console
  va_start(args, format);
  printf("%s ", COG_ERROR);
  vprintf(format, args);
  fclose(self->fd);
  va_end(args);
}

void
CogLoggerInfo(
  CogLogger     self,
  const char*   format,
  ...
)
{
  va_list args;
  va_start(args, format);
  self->fd = fopen(self->fileName->data, "a");
  // to logfile
  fprintf(self->fd, "%s ", COG_INFO);
  vfprintf(self->fd, format, args);
  // to console
  va_start(args, format);
  printf("%s ", COG_INFO);
  vprintf(format, args);
  fclose(self->fd);
  va_end(args);
}

void
CogLoggerWarning(
  CogLogger     self,
  const char*   format,
  ...
)
{
  va_list args;
  va_start(args, format);
  self->fd = fopen(self->fileName->data, "a");
  // to logfile
  fprintf(self->fd, "%s ", COG_WARNING);
  vfprintf(self->fd, format, args);
  // to console
  va_start(args, format);
  printf("%s ", COG_WARNING);
  vprintf(format, args);
  fclose(self->fd);
  va_end(args);
}