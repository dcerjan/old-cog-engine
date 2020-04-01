#ifndef _COG_PAIR_H_
#define _COG_PAIR_H_

#include <stdio.h>
#include <stdlib.h>

#define CogPairDefine(K, V) \
  struct CogPair_##K##_##V { \
    K   key; \
    V   value; \
  }; \
  typedef struct CogPair_##K##_##V* CogPair_##K##_##V; \
  \
  \
  CogPair_##K##_##V CogPair_##K##_##V##_Alloc(K kdata, V vdata) { \
    CogPair_##K##_##V ret = malloc(sizeof(struct CogPair_##K##_##V)); \
    ret->key = kdata; \
    ret->value = vdata; \
    return ret; \
  } \
  \
  \
  void CogPair_##K##_##V##_Free(struct CogPair_##K##_##V* pair) { \
    free(pair); \
  } \
  \
  \
  void CogPair_##K##_##V##_Inspect(struct CogPair_##K##_##V* pair) { \
    printf("{"); \
    printf("\"key\": "); \
    K##Inspect(pair->key); \
    printf(", \"value\":"); \
    V##Inspect(pair->value); \
    printf("}"); \
  } 
  
#define CogPair(K, V) struct CogPair_##K##_##V*
#define CogPairAlloc(K, V, key, value) CogPair_##K##_##V##_Alloc(key, value);
#define CogPairFree(K, V, pair) CogPair_##K##_##V##_Free(pair);
#define CogPairInspect(K, V, pair) CogPair_##K##_##V##_Inspect(pair);

#endif