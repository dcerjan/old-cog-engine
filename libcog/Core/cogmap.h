#ifndef _COG_DICT_H_
#define _COG_DICT_H_

#include "cogpair.h"
#include <inttypes.h>

static uint64_t djb2(const char* key) {
  uint64_t hash = 5381;
  int c;
  while (c = *key++, c) { hash = ((hash << 5) + hash) + c; }
  return hash;
}

#define CogMap(K, V) struct CogMap_##K##_##V*
#define CogMapNode(K, V) struct CogMapNode_##K##_##V*
#define CogMapDefine(K, V) \
  \
  \
  struct CogMapNode_##K##_##V { \
    char                          color; \
    CogPair(K, V)                 pair; \
    struct CogMapNode_##K##_##V   *parent, *left, *right; \
  }; \
  typedef struct CogMapNode_##K##_##V* CogMapNode_##K##_##V; \
  \
  \
  struct CogMap_##K##_##V { \
    CogMapNode(K, V)              root; \
    int                           len; \
  }; \
  typedef struct CogMap##T* CogDict##T; \
  \
  \
  CogMap(K, V) CogMap_##K##_##V##_Alloc(void) { \
    CogMap(K, V) ret = malloc(sizeof(struct CogMap_##K##_##V)); \
    ret->root = NULL; \
    ret->len = 0; \
    return ret; \
  } \
  \
  \
  static void CogMapWalk_##K##_##V##_Free(CogMapNode(K, V) node) { \
    if(node != NULL) { \
      if(node->left) { \
        CogMapWalk_##K##_##V##_Free(node->left); \
        CogPairFree(K, V, node->left->pair); \
        free(node->left); \
      } \
      \
      if(node->right) { \
        CogMapWalk_##K##_##V##_Free(node->right); \
        CogPairFree(K, V, node->right->pair); \
        free(node->right); \
      } \
    } \
  } \
  void CogMap_##K##_##V##_Free(CogMap(K, V) self) { \
    CogMapWalk_##K##_##V##_Free(self->root); \
    free(self); \
  } \
  \
  \
  CogMapNode(K, V) CogMap_##K##_##V##_First(CogMap(K, V) map) { \
    CogMapNode(K,V) first = map->root; \
    while(first->left) { first = first->left; } \
    return first; \
  } \
  \
  \
  CogMapNode(K, V) CogMap_##K##_##V##_Last(CogMap(K, V) map) { \
    CogMapNode(K,V) last = map->root; \
    while(last->right) { last = last->right; } \
    return last; \
  } \
  \
  \
  CogMapNode(K, V) CogMap_##K##_##V##_Next(CogMapNode(K, V) node) { \
    if(node->left != NULL) { return node->left; } \
    if(node->right != NULL) { return node->right; } \
    while(node->parent != NULL) { \
      if(node == node->parent->left && node->parent->right != NULL) { \
        return node->parent->right; \
      } \
      node = node->parent; \
    } \
    return NULL; \
  } \
  \
  \
  static CogMapNode(K, V) CogMapNode_##K##_##V##_Alloc(char color, CogMapNode(K, V) parent, CogMapNode(K, V) left, CogMapNode(K, V) right, K key, V value) { \
    CogMapNode(K, V) ret = malloc(sizeof(CogMapNode_##K##_##V)); \
    ret->color = color; \
    ret->pair = CogPairAlloc(K, V, key, value); \
    ret->parent = NULL; \
    ret->left = NULL; \
    ret->right = NULL; \
    return ret; \
  } \
  \
  \
  void CogMap_##K##_##V##_Add(CogMap(K, V) map, K key, V value) { \
    if(!map->root) { \
      map->len++; \
      map->root = CogMapNode_##K##_##V##_Alloc('B', NULL, NULL, NULL, key, value); \
    } \
  } \
  \
  \
  void CogMap_##K##_##V##_Inspect(CogMap(K, V) map) { \
    printf("{"); \
    printf("\"len\": %d", map->len); \
    if(map->root) { \
      printf(", "); \
      printf("\"data\": ["); \
      CogMapNode(K, V) iter = CogMap_##K##_##V##_First(map); \
      int l = 0; \
      while(iter) { \
        CogPairInspect(K, V, iter->pair); \
        if(l < map->len - 1) { \
          printf(","); \
        } \
        l++; \
        iter = CogMap_##K##_##V##_Next(iter); \
      } \
    } \
    printf("]}"); \
  }
  

#define CogMapAlloc(K, V)                   CogMap_##K##_##V##_Alloc()
#define CogMapFree(K, V, map)               CogMap_##K##_##V##_Free(map)
#define CogMapClear(K, V, map)              CogMap_##K##_##V##_Clear(map)
#define CogMapAdd(K, V, map, key, value)    CogMap_##K##_##V##_Add(map, key, value)
#define CogMapGet(K, V, map, key)           CogMap_##K##_##V##_Get(map, key)
#define CogMapRemove(K, V, map, key)        CogMap_##K##_##V##_Remove(map, key)
#define CogMapInspect(K, V, map)            CogMap_##K##_##V##_Inspect(map)
#define CogMapFirst(K, V, map)              CogMap_##K##_##V##_First(map)
#define CogMapLast(K, V, map)               CogMap_##K##_##V##_Last(map)
#define CogMapNext(K, V, map)               CogMap_##K##_##V##_Next(node)
#define CogMapPrev(K, V, map)               CogMap_##K##_##V##_Prev(node)

#define CogMapForEach(K, V, map, iter, body) { \
    CogMapNode(K, V) iter; \
    for(iter = dict->root; iter; iter = CogMap_##K##_##V##_Next(iter)) { \
      (body); \
    } \
  }

#endif