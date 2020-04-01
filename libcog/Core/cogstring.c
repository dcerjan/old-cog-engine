#include "cogstring.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

CogString CogStringAlloc(const char* str) {
    CogString s = (CogString)malloc(sizeof(struct CogString));
    s->len = strlen(str) + 1;
    s->data = (char*)malloc(sizeof(char) * (s->len));
    strcpy(s->data, str);
    return s;
}

CogString CogStringAllocCopy(CogString str) {
    CogString s = (CogString)malloc(sizeof(struct CogString));
    s->len = str->len;
    s->data = (char*)malloc(sizeof(char) * str->len);
    strcpy(s->data, str->data);
    return s;
}

CogString CogStringAllocFromFormat(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int size = vsnprintf(NULL, 0, format, args);
    CogString s = (CogString)malloc(sizeof(struct CogString));
    s->len = size + 1;
    s->data = (char*)malloc(sizeof(char) * s->len);
    va_start(args, format);
    vsnprintf(s->data, s->len, format, args);
    va_end(args);
    return s;
}

void CogStringFree(CogString str) {
    free(str->data);
    free(str);
}

CogArray(CogString) CogStringAllocSplit(CogString str, const char* delimiter) {
    CogArray(CogString) split = CogArrayAlloc(CogString);
    unsigned long i;
    unsigned long l = strlen(delimiter);
    char* c = str->data;
    for(i = 0; i < str->len;) {
        if(!strncmp(str->data + i, delimiter, l)) {
            CogString tmp = (CogString)malloc(sizeof(struct CogString));
            tmp->len = (str->data + i) - c;
            tmp->data = (char*)malloc(sizeof(char) * (tmp->len + 1));
            strncpy(tmp->data, c, tmp->len);
            tmp->data[tmp->len] = '\0';
            CogArrayPush(split, tmp);
            i += l;
            c = str->data + i;
        } else {
            i++;
        }
    }
    CogString tmp = (CogString)malloc(sizeof(struct CogString));
    tmp->len = strlen(c);
    tmp->data = (char*)malloc(sizeof(char) * (tmp->len + 1));
    strncpy(tmp->data, c, tmp->len);
    tmp->data[tmp->len] = '\0';
    CogArrayPush(split, tmp);
    
    return split;
}

Bool CogStringContains(CogString str0, CogString str1) {
    unsigned long i;
    for(i = 0; i < str0->len; ++i) {
        if(!strcmp(str0->data + i, str1->data)) {
            return True;
        }
    }
    return False;
}

CogString CogStringCat(CogString str0, CogString str1) {
    CogString tmp = (CogString)malloc(sizeof(struct CogString));
    tmp->len = str0->len + str1->len;
    tmp->data = (char*)malloc(sizeof(char) * (tmp->len + 1));
    strcpy(tmp->data, str0->data);
    strcat(tmp->data, str1->data);
    return tmp;
}

CogString CogStringReplace(CogString str, const char* match, const char* replace) {
    CogString tmp = (CogString)malloc(sizeof(struct CogString));
    char* buffer = malloc(sizeof(char) * 256);
    memset(buffer, '\0', 256 * sizeof(char));
    int buflen = 256;
    unsigned long i = 0;
    unsigned long j = 0;
    unsigned long l = strlen(match);
    unsigned long k = strlen(replace);
    
    for(i = 0; i < str->len;) {
      if(!strncmp(str->data + i, match, l)) {
        while(j >= (buflen - k)) {
          buffer = realloc(buffer, sizeof(char) * (buflen) * 2);
          buflen *= 2;
          memset(buffer + buflen / 2, '\0', sizeof(char) * buflen / 2);
        }
        strncpy(buffer + j, replace, k);
        j += k;
        i += l;
      } else {
        while(j >= (buflen - k)) {
          buffer = realloc(buffer, sizeof(char) * (buflen) * 2);
          buflen *= 2;
          memset(buffer + buflen / 2, '\0', sizeof(char) * buflen / 2);
        }
        buffer[j++] = str->data[i++];
      }
    }
    tmp->data = malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(tmp->data, buffer);
    tmp->len = strlen(tmp->data);
    free(buffer);
    return tmp;
}
void CogStringInspect(CogString CogString) {
    printf("'%s'", CogString->data);
}
