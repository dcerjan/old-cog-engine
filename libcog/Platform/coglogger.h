#ifndef _COG_LOGGER_H_
#define _COG_LOGGER_H_


#include "cogstring.h"

#if !defined(__ANDROID__) && !defined(__IOS__)
  #include "stdio.h"
#endif

#define COG_DEBUG   "[Debug]"
#define COG_ERROR   "[Error]"
#define COG_INFO    "[Info]"
#define COG_WARNING "[Warning]"

struct CogLogger {
  CogString     name;
  CogString     fileName;
#if !defined(__ANDROID__) && !defined(__IOS__)
  FILE*         fd;
#endif
};
typedef struct CogLogger* CogLogger;

CogLogger CogLoggerAlloc(const char* name, const char* fileName);
void CogLoggerFree(CogLogger self);
void CogLoggerLog(CogLogger self, const char* prefix, const char* format, ...);
void CogLoggerDebug(CogLogger self, const char* format, ...);
void CogLoggerError(CogLogger self, const char* format, ...);
void CogLoggerInfo(CogLogger self, const char* format, ...);
void CogLoggerWarning(CogLogger self, const char* format, ...);


#endif