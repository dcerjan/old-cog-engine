#ifndef _COG_STRING_H_
#define _COG_STRING_H_

#include "cogtypes.h"
#include "CogArray.h"

#define c_str(str) (*(char**)(str))

struct CogString {
  char*         data;
  unsigned long len;
};
typedef struct CogString* CogString;
CogDefineArray(CogString);

CogString CogStringAlloc(const char* str);
CogString CogStringAllocCopy(CogString str);
CogString CogStringAllocFromFormat(const char* format, ...);
void CogStringFree(CogString str);
CogArray(CogString) CogStringAllocSplit(CogString str, const char* delimiter);

Bool CogStringContains(CogString str0, CogString str1);

CogString CogStringCat(CogString str0, CogString str1);
CogString CogStringReplace(CogString str, const char* match, const char* replace);

void CogStringInspect(CogString CogString);

#endif
