#ifndef _COG_JSON_H_
#define _COG_JSON_H_

#include "CogJsonObject.h"
#include "coggc.h"

struct CogJsonParser {
  CogCleanupPool      pool;
};
typedef struct CogJsonParser* CogJsonParser;

CogJsonParser
CogJsonParserAlloc(
  void
);

CogJsonObject
CogJsonParserParseString(
  CogJsonParser       parser, 
  const char*         json
);

CogJsonObject
CogJsonParserParseFromFile(
  CogJsonParser       parser, 
  const char*         filename
);

void
CogJsonParserFree(
  CogJsonParser       parser
);

#endif