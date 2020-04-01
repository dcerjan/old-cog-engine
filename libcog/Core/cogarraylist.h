#ifndef _COG_ARRAY_LIST_H_
#define _COG_ARRAY_LIST_H_

#define CogArrayList(T) CogArrayList ## T
#define CogArrayListAlloc(T) CogArrayList ## T ## Alloc()
#define CogArrayListNext(T, node) CogArrayList ## T ## Next(node)
#define CogDefineArrayList(T) \
  struct CogArrayList ## T; \
  struct CogArrayListNode ## T { \
    T* data; \
    int prev, next; \
    struct CogArrayList ## T * list; \
  }; \
  typedef struct CogArrayListNode ## T * CogArrayListNode ## T; \
  struct CogArrayList ## T { \
    struct CogArrayListNode ## T *data; \
    int size; \
    int storage; \
    int first, last; \
  }; \
  typedef struct CogArrayList ## T * CogArrayList ## T; \
  \
  CogArrayList ## T CogArrayList ## T ## Alloc(void) { \
    CogArrayList ## T self = malloc(sizeof(struct CogArrayList ## T)); \
    self->data = malloc(sizeof(struct CogArrayListNode ## T) * 16); \
    self->size = 0; \
    self->storage = 16; \
    self->first = 0; \
    self->last = 0; \
    memset(self->data, 0, sizeof(struct CogArrayListNode ## T *) * 16); \
    return self; \
  } \
  \
  struct CogArrayListNode ## T* CogArrayList ## T ## Next(struct CogArrayListNode ## T *node) { \
    assert(node->next >= 0 && node->prev < node->list->size); \
    if(node->next == -1) { \
      return NULL; \
    } else { \
      return &(node->list->data[node->next]); \
    } \
  } \
  \
  void CogArrayList ## T ## Push(struct CogArrayList ## T * self, T data) { \
    \
  }

#endif