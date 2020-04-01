#ifndef _COG_CogList_H_
#define _COG_CogList_H_

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define CogDefineList(T) \
    typedef struct CogListCogNode_ ## T { \
        T                         data; \
        struct CogListCogNode_ ## T* prev; \
        struct CogListCogNode_ ## T* next; \
    } CogListCogNode_ ## T; \
    struct CogList_ ## T { \
        CogListCogNode_ ## T*        first; \
        CogListCogNode_ ## T*        last; \
        int                       size; \
        T                         __type_instance__; \
        CogListCogNode_ ## T         __type_instance_node__; \
    }; \
    typedef struct CogList_ ## T * CogList_ ## T;

#define CogList(T) CogList_ ## T
#define CogListAlloc(T) ({ \
    CogList(T) __CogList__ = (CogList_ ## T)malloc(sizeof(struct CogList_ ## T)); \
    __CogList__->first = __CogList__->last = NULL; \
    __CogList__->size = 0; \
    memset(&__CogList__->__type_instance__, 0, sizeof(__CogList__->__type_instance__)); \
    memset(&__CogList__->__type_instance_node__, 0, sizeof(__CogList__->__type_instance_node__)); \
    __CogList__; \
});

#define CogListFree(CogList) { \
    while(CogList->first != NULL) { \
        CogList->first = CogList->first->next; \
    } \
    free(CogList); \
    CogList = NULL; \
}

#define CogListPushBack(CogList, element) { \
    typeof(CogList->__type_instance_node__)* __node__ = (typeof(CogList->__type_instance_node__)*)malloc(sizeof(CogList->__type_instance_node__)); \
    __node__->data = element; \
    CogList->size++; \
    if(CogList->first == NULL) { \
        CogList->first = CogList->last = __node__; \
        __node__->prev = NULL; \
        __node__->next = NULL; \
    } else { \
        CogList->last->next = __node__; \
        __node__->next = NULL; \
        __node__->prev = CogList->last; \
        CogList->last = __node__; \
    } \
}

#define CogListPushFront(CogList, element) { \
    typeof(CogList->__type_instance_node__)* __node__ = (typeof(CogList->__type_instance_node__)*)malloc(sizeof(CogList->__type_instance_node__)); \
    __node__->data = element; \
    CogList->size++; \
    if(CogList->first == NULL) { \
        CogList->first = CogList->last = __node__; \
        __node__->prev = NULL; \
        __node__->next = NULL; \
    } else { \
        CogList->first->prev = __node__; \
        __node__->prev = NULL; \
        __node__->next = CogList->first; \
        CogList->first = __node__; \
    } \
}

#define CogListPopBack(CogList) ({ \
    assert(CogList->last != NULL); \
    typeof(CogList->__type_instance_node__)* __node__; \
    typeof(CogList->__type_instance__) __data__; \
    __node__ = CogList->last; \
    __data__ = __node__->data; \
    CogList->last = CogList->last->prev; \
    if(CogList->last != NULL) { CogList->last->next = NULL; } \
    free(__node__); \
    __node__ = NULL; \
    CogList->size--; \
    __data__; \
})

#define CogListPopFront(CogList) ({ \
    assert(CogList->first != NULL); \
    typeof(CogList->__type_instance_node__)* __node__; \
    typeof(CogList->__type_instance__) __data__; \
    __node__ = CogList->first; \
    __data__ = __node__->data; \
    CogList->first = CogList->first->next; \
    if(CogList->first != NULL) { CogList->first->prev = NULL; } \
    free(__node__); \
    __node__ = NULL; \
    CogList->size--; \
    __data__; \
})

#define CogListRemove(CogList, element) ({ \
    assert(CogList->size > 0); \
    typeof(CogList->__type_instance__) __target__ = NULL; \
    if(CogList->size == 1) { \
        assert(element == CogList->first->data); \
        __target__ = CogListPopFront(CogList); \
    } else if(CogList->first->data == element) { __target__ = CogListPopFront(CogList); \
    } else if(CogList->last->data == element) { __target__ = CogListPopBack(CogList); \
    } else { \
        typeof(CogList->__type_instance_node__)* __iter__, *__p__, *__n__; \
        __iter__ = CogList->first; \
            for(;__iter__ != NULL; __iter__ = __iter__->next) { \
            if(__iter__->data == element) { \
                __target__ = __iter__->data; \
                __p__ = __iter__->prev; \
                __n__ = __iter__->next; \
                __p__->next = __n__; \
                __n__->prev = __p__; \
                free(__iter__); \
                CogList->size--; \
                break; \
            } \
        } \
    } \
    __target__; \
})

#define CogListRemoveEach(CogList, e, block) { \
    assert(CogList->size > 0); \
    typeof(CogList->__type_instance_node__)* __node__, *__tmp__, *__p__, *__n__, *__fwd__; \
    typeof(CogList->__type_instance__) e; \
    __node__ = CogList->first; \
    while(__node__ != NULL) { \
        __fwd__ = __node__->next; \
        __tmp__ = __node__; \
        e = __node__->data; \
        if((block)) { \
            if(CogList->first == __node__) { CogList->first = CogList->first->next; } \
            if(CogList->last == __node__) {CogList->last = CogList->last->prev; } \
            __p__ = __tmp__->prev; \
            __n__ = __tmp__->next; \
            if(__p__ != NULL) { __p__->next = __n__; } \
            if(__n__ != NULL) { __n__->prev = __p__; } \
            free(__tmp__); \
            __tmp__ = NULL; \
            CogList->size--; \
        } \
        __node__ = __fwd__; \
    } \
}

#define CogListRemoveAt(CogList, index) { \
    int __i__; \
    if(index >= 0) { \
        assert(index < CogList->size); \
        __i__ = index; \
    } else { \
        assert(CogList->size + index > 0); \
        __i__ = CogList->size + index; \
    } \
    if(CogList->size == 1) { \
        CogListPopBack(CogList); \
    } else if(__i__ == 0) { CogListPopFront(CogList); \
    } else if(__i__ == (CogList->size - 1)) { CogListPopBack(CogList); \
    } else { \
        typeof(CogList->__type_instance_node__)* __iter__, *__p__, *__n__; \
        for(__iter__ = CogList->first; __i__ > 0; __iter__ = __iter__->next, --__i__); \
        __p__ = __iter__->prev; \
        __n__ = __iter__->next; \
        __p__->next = __n__; \
        __n__->prev = __p__; \
        free(__iter__); \
        __iter__ = NULL; \
        CogList->size--; \
    } \
}

#define CogListInsertAt(CogList, element, index) { \
    int i; \
    typeof(CogList->__type_instance_node__)* __iter__, *__prev__, *__new__; \
    if(index < 0) { \
        i = CogList->size + index; \
    } else { \
        i = index; \
    } \
    if(i >= CogList->size) { CogListPushBack(CogList, element); \
    } else if(i <= 0) { CogListPushFront(CogList, element); \
    } else { \
        for(__iter__ = CogList->first; i > 0; __iter__ = __iter__->next, --i); \
        __prev__ = __iter__->prev; \
        __new__ = (typeof(CogList->__type_instance_node__)*)malloc(sizeof(CogList->__type_instance_node__)); \
        __new__->data = element; \
        __prev__->next = __new__; \
        __new__->prev = __prev__; \
        __new__->next = __iter__; \
        __iter__->prev = __new__; \
        CogList->size++; \
    } \
}

#define CogListAt(CogList, index) ({ \
    int __i__; \
    if(index >= 0) { \
        assert(index < CogList->size); \
        __i__ = index; \
    } else { \
        assert(CogList->size + index > 0); \
        __i__ = CogList->size + index; \
    } \
    typeof(CogList->__type_instance_node__)* iter; \
    for(iter = CogList->first; __i__ > 0; iter = iter->next, --__i__); \
    iter->data; \
});

#define CogListForEach(CogList, e, block) { \
    typeof(CogList->__type_instance_node__)* __iter__; \
    typeof(CogList->__type_instance__) e; \
    for(__iter__ = CogList->first; __iter__ != NULL; __iter__ = __iter__->next) { \
        e = __iter__->data; \
        (block); \
    } \
}

#define CogListReverse(CogList) { \
    typeof(CogList->__type_instance_node__)* __l__, *__r__; \
    typeof(CogList->__type_instance__) __d__; \
    for(__l__ = CogList->first, __r__ = CogList->last; __l__ != __r__ && __l__->prev != __r__; __l__ = __l__->next, __r__ = __r__->prev) { \
        __d__ = __l__->data; \
        __l__->data = __r__->data; \
        __r__->data = __d__; \
    } \
}

#define CogListSort(CogList, first, second, compare) { \
    typeof(CogList->__type_instance_node__) *__f__, *__s__; \
    typeof(CogList->__type_instance__) __tmp__, first, second; \
    for(__f__ = CogList->first; __f__ != NULL; __f__ = __f__->next) { \
        for(__s__ = __f__; __s__ != NULL; __s__ = __s__->next) { \
            first = __f__->data; \
            second = __s__->data; \
            if((compare)) { \
                __tmp__ = __f__->data; \
                __f__->data = __s__->data; \
                __s__->data = __tmp__; \
            } \
        } \
    } \
}

#define CogListAllocCopy(T, CogList) ({ \
    CogList(T) __CogList__ = CogListAlloc(T); \
    typeof(CogList->__type_instance_node__) *__iter__; \
    for(__iter__ = CogList->first; __iter__ != NULL; __iter__ = __iter__->next) { \
        CogListPushBack(__CogList__, __iter__->data); \
    } \
    __CogList__; \
})

#define CogListAllocSlice(T, CogList, start, end) ({ \
    int __l__, __r__, __counter__; \
    typeof(CogList->__type_instance_node__)* __iter__; \
    CogList(T) __slice__ = CogListAlloc(T); \
    if(CogList->size > 0) { \
        if(start < 0) { __l__ = CogList->size + start; } \
        else { __l__ = start; } \
        if(end < 0) { __r__ = CogList->size + end; } \
        else { __r__ = end; } \
        if(__l__ <= 0) { __l__ = 0; } \
        else if(__l__ > CogList->size) { __l__ = CogList->size; } \
        if(__r__ <= 0) { __r__ = 0; } \
        else if(__r__ > CogList->size) { __r__ = CogList->size; } \
        if(__l__ != __r__) { \
            if(__l__ < __r__) { \
                __counter__ = 0; \
                for(__iter__ = CogList->first; (__counter__ < __l__) && (__iter__ != NULL); __iter__ = __iter__->next, ++__counter__); \
                for(; (__counter__ <= __r__) && (__iter__ != NULL); __iter__=__iter__->next, ++__counter__) { CogListPushBack(__slice__, __iter__->data); } \
            } else { \
                __counter__ = CogList->size - 1; \
                for(__iter__ = CogList->last; (__counter__ > __l__) && (__iter__ != NULL); __iter__ = __iter__->prev, --__counter__); \
                for(; (__counter__ >= __r__) && (__iter__ != NULL); __iter__=__iter__->prev, --__counter__) { CogListPushBack(__slice__, __iter__->data); } \
            } \
        } \
    } \
    __slice__; \
})

#define CogListInspect(T, CogList) { \
    typeof(CogList->__type_instance_node__)* __iter__; \
    printf("<CogList (%d) @0x%lx [", CogList->size, (unsigned long)CogList); \
    for(__iter__ = CogList->first; __iter__ != NULL; __iter__ = __iter__->next) { \
        T ## Inspect(__iter__->data); \
        if(__iter__->next) { printf(", "); } \
    } \
    printf("]>\n"); \
}

#endif