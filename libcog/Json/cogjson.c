#include "cogjson.h"

#include "cogtypes.h"
#include "cogstring.h"
#include "ctype.h"

static CogString _string_(const char* json, int* i);
static CogJsonObject _object_(const char* json, int* i);
static double _number_(const char* json, int* i);
static CogArray(CogJsonObject) _CogArray_(const char* json, int* i);

CogString _string_(const char* json, int* i) {
  char* s = malloc(sizeof(char) * 256);
  memset(s, '\0', sizeof(char) * 256);
  int size = 256;
  int taken = 0;
  char c;
  if(json[*i] == '"') {
    c = '"';
  } else {
    c = '\'';
  }
  (*i)++;
  for(; json[*i] != c; ++(*i)) {
    if(taken >= size - 1) {
      size *= 2;
      s = realloc(s, size);
    }
    strncat(s, &json[*i], 1);
    taken++;
  }
  ++(*i);
  if(json[*i] == ',') {
    ++(*i);
  }
  strcat(s, "\0");
  CogString ret = CogStringAlloc(s);
  free(s);
  return ret;
}

double _number_(const char* json, int* i) {
  char* s = malloc(sizeof(char) * 256);
  memset(s, '\0', sizeof(char) * 256);
  int size = 256;
  int taken = 0;
  for(; isdigit(json[*i]) || json[*i] == '.' || json[*i] == '-' || json[*i] == '+' || json[*i] == 'e' || json[*i] == 'E'; ++(*i)) {
    if(taken >= size - 1) {
      size *= 2;
      s = realloc(s, size);
    }
    strncat(s, &json[*i], 1);
    taken++;
  }
  if(json[*i] == ',') {
    ++(*i);
  }
  strcat(s, "\0");
  double ret = atof(s);
  free(s);
  return ret;
}

CogArray(CogJsonObject) _CogArray_(const char* json, int* i) {
  CogArray(CogJsonObject) self = CogArrayAlloc(CogJsonObject);
  
  while(json[*i] != ']' && json[*i] != '\0') {
    if(json[*i] == '{') {
      ++(*i);
      CogArrayPush(self, _object_(json, i));
    } else if(json[*i] == '[') {
      ++(*i);
      CogJsonObject a = CogJsonObjectAlloc();
      a->type = CogJsonObjectTypeCogArray;
      a->data =  _CogArray_(json, i);
      CogArrayPush(self, a);
    } else if(json[*i] == '"' || json[*i] == '\'') {
      CogString s = _string_(json, i);
      CogArrayPush(self, CogJsonObjectAllocString(s->data));
      CogStringFree(s);
    } else if(json[*i] == 'f' || json[*i] == 't' || json[*i] == 'n') {
      if( json[*i] == 'f' && json[(*i) + 1] == 'a' && json[(*i) + 2] == 'l' && json[(*i) + 3] == 's' && json[(*i) + 4] == 'e' ) {
        CogArrayPush(self, CogJsonObjectAllocBoolean(False));
        (*i) += 5;
      } else if(json[(*i)] == 't' && json[(*i) + 1] == 'r' && json[(*i) + 2] == 'u' && json[(*i) + 3] == 'e') {
        CogArrayPush(self, CogJsonObjectAllocBoolean(True));
        (*i) += 4;
      } else if(json[*i] == 'n' && json[(*i) + 1] == 'u' && json[(*i) + 2] == 'l' && json[(*i) + 3] == 'l') {
        CogArrayPush(self, CogJsonObjectAllocNull());
        (*i) += 4;
      } else {
        printf("Unexpected token while parsing JSON!\n");
        exit(-1);
      }
    } else if(isdigit(json[*i]) || json[*i] == '.' || json[*i] == '-') {
      CogArrayPush(self, CogJsonObjectAllocNumber(_number_(json, i)));
    } else {
      printf("Unexpected token while parsing JSON!\n");
      exit(-1);
    }
  }
  ++(*i);
  if(json[*i] == ',') {
    ++(*i);
  }
  
  return self;
}

CogJsonObject _object_(const char* json, int* i) {
  CogJsonObject self = CogJsonObjectAlloc();
  while(json[*i] != '}' && json[*i] != '\0') {
    if(json[*i] == ',') {
      (*i)++;
    }
    char* key = malloc(sizeof(char) * 256);
    memset(key, '\0', sizeof(char) * 256);
    int size = 256;
    int taken = 0;
    
    if(json[*i] == '"') {
      if(json[(*i) + 1] == '"') {
        fprintf(stderr, "Key can not be an empty CogString!\n");
        exit(4);
      }
      ++(*i);
      if(isalpha(json[*i]) || json[*i] == '_' ||json[*i] == '$') {
        for(; json[*i] != '"'; ++(*i)) {
          if(taken >= size -1) {
            size *= 2;
            key = realloc(key, sizeof(char) * size);
          }
          strncat(key, &json[*i], 1);
          taken++;
        }
        strcat(key, "\0");
        ++(*i);
        if(json[*i] != ':') {
          fprintf(stderr, "Expected ':' after '\"'\n");
          exit(4);
        }
        ++(*i);
      }
    }
    
    if(json[*i] == '{') {
      ++(*i);
      CogJsonObjectSetObject(self, key, _object_(json, i));
    } else if(json[*i] == '[') {
      ++(*i);
      CogJsonObjectSetCogArray(self, key, _CogArray_(json, i));
    } else if(json[*i] == '"' || json[*i] == '\'') {
      CogString s = _string_(json, i);
      CogJsonObjectSetString(self, key, s->data);
      CogStringFree(s);
    } else if(json[*i] == 'f' || json[*i] == 't' || json[*i] == 'n') {
      if( json[*i] == 'f' && json[(*i) + 1] == 'a' && json[(*i) + 2] == 'l' && json[(*i) + 3] == 's' && json[(*i) + 4] == 'e' ) {
        CogJsonObjectSetBoolean(self, key, False);
        (*i) += 5;
      } else if(json[(*i)] == 't' && json[(*i) + 1] == 'r' && json[(*i) + 2] == 'u' && json[(*i) + 3] == 'e') {
        CogJsonObjectSetBoolean(self, key, True);
        (*i) += 4;
      } else if(json[*i] == 'n' && json[(*i) + 1] == 'u' && json[(*i) + 2] == 'l' && json[(*i) + 3] == 'l') {
        CogJsonObjectSetObject(self, key, CogJsonObjectAllocNull());
        (*i) += 4;
      } else {
        printf("Unexpected token while parsing JSON!\n");
        exit(-1);
      }
    } else if(isdigit(json[*i]) || json[*i] == '.' || json[*i] == '-') {
      CogJsonObjectSetNumber(self, key, _number_(json, i));
    } else {
      printf("Unexpected token while parsing JSON!\n");
      exit(-1);
    }
    free(key);
  }
  
  ++(*i);
  if(json[*i] == ',') {
    ++(*i);
  }
  
  return self;
}

CogJsonObject _root_(const char* json) {
  assert(json[0] == '{');
  int i = 1;
  return _object_(json, &i);
}


Bool isWhite(char c) {
  if(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') {
    return True;
  }
  return False;
}

CogJsonObject _start_(const char* json) {
  // make another CogString without whitespace and comments
  char* string = malloc(sizeof(char) * 256);
  int size = 256;
  int taken = 0;
  
  int i;
  
  Bool singleQuote = False;
  Bool doubleQuote = False;
  Bool lineComment = False;
  Bool multilineComment = False;
  
  for(i = 0; json[i] != '\0'; ++i) {
    if(!singleQuote && !doubleQuote) {
      if((json[i] == '/' && json[i + 1] == '/')  && !lineComment && !multilineComment) { lineComment = True;} 
      else if((json[i] == '\n' || json[i] == '\r' || json[i] == '\f') && lineComment && !multilineComment) { i += 1; lineComment = False; }
      if( (json[i] == '/' && json[i + 1] == '*') && !lineComment && !multilineComment) { multilineComment = True; } 
      else if( (json[i] == '*' && json[i + 1] == '/') && !lineComment && multilineComment) { i += 2; multilineComment = False; }
    }
    
    if(!lineComment && !multilineComment) {
      if(isWhite(json[i]) && !(singleQuote || doubleQuote)) { 
        continue; 
      }
    }
    
    if(!lineComment && !multilineComment) {
      if(json[i] == '"' && !singleQuote && doubleQuote) { doubleQuote = False; } 
      else if(json[i] == '"' && !singleQuote && !doubleQuote) { doubleQuote = True; }
      
      if(json[i] == '\'' && singleQuote && !doubleQuote) { singleQuote = False; } 
      else if(json[i] == '\'' && !singleQuote && !doubleQuote) { singleQuote = True; }
    }
    
    if(taken == size - 1) {
      size *= 2;
      string = realloc(string, sizeof(char) * size);
    }
    
    if(!(lineComment || multilineComment)) {
      string[taken++] = json[i]; 
    }
  }
  string[taken] = '\0';
  
  CogJsonObject self = _root_(string);
  free(string);
  return self;
}


/////////////////////////////////

CogJsonParser JsonParserAlloc (void) {
  CogJsonParser self = malloc(sizeof(struct CogJsonParser));
  self->pool = CogCleanupPoolAlloc("JSONs");
  return self;
}
void CogJsonParserFree (CogJsonParser parser) {
  CogCleanupPoolFree(parser->pool);
  free(parser);
}

CogJsonObject CogJsonParserParseString (CogJsonParser parser, const char* json) {
  CogJsonObject self = _start_(json);
  if(parser != NULL) {
    CogCleanupPoolRegisterForCleanup(parser->pool, self, (auto_destruct)CogJsonObjectFree);
  }
  return self;
}

CogJsonObject CogJsonParserParseFromFile (CogJsonParser parser, const char* filename) {
  FILE* fp = fopen(filename, "r");
  if(!fp) {
    return NULL;
  }
  char c = EOF;
  char* buffer = malloc(sizeof(char) * 256);
  memset(buffer, '\0', sizeof(char) * 256);
  int size = 256;
  int taken = 0;
  
  while(c = fgetc(fp), c != EOF) {
    if(taken == size - 1) {
      size *= 2;
      buffer = realloc(buffer, sizeof(char) * size);
    }
    buffer[taken++] = c;
  }
  fclose(fp);
  buffer[taken] = '\0';
  
  CogJsonObject self = _start_((const char*)buffer);
  if(parser != NULL) {
    CogCleanupPoolRegisterForCleanup(parser->pool, self, (auto_destruct)CogJsonObjectFree);
  }
  
  free(buffer);
  
  return self;
}
