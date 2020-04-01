#ifndef _COG_CogArray_H_
#define _COG_CogArray_H_

#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* Should be called only once per type
 */
#define CogDefineArray(T) \
    struct CogArray_ ## T {\
        T*      data; \
        int     size; \
        int     storage; \
        T       __type_instance__; \
    }; \
    typedef struct CogArray_ ## T * CogArray_ ## T;

#define CogArray(T) CogArray_ ## T
#define CogArrayAlloc(T) ({ \
    struct CogArray_ ## T * __CogArray_ ## T ## __; \
    __CogArray_ ## T ## __ = (CogArray_ ## T)malloc(sizeof(struct CogArray_ ## T)); \
    __CogArray_ ## T ## __->size = 0; \
    __CogArray_ ## T ## __->storage = 0; \
    __CogArray_ ## T ## __->data = NULL; \
    memset(&__CogArray_ ## T ## __->__type_instance__, 0, sizeof(T)); \
    __CogArray_ ## T ## __; \
})

#define CogArrayFree(CogArray) free(CogArray->data); free(CogArray); CogArray = NULL;

#define CogArrayPush(CogArray, element) \
    if(CogArray->storage == 0) { \
        CogArray->storage = 16; \
        CogArray->data = (typeof(CogArray->__type_instance__) *)malloc(sizeof(typeof(CogArray->__type_instance__)) * CogArray->storage); \
    } \
    else if(CogArray->size >= CogArray->storage){ \
        CogArray->storage *= 2; \
        CogArray->data = (typeof(CogArray->__type_instance__) *)realloc(CogArray->data, sizeof(typeof(CogArray->__type_instance__)) * CogArray->storage); \
    } \
    CogArray->data[CogArray->size++] = element;

#define CogArrayInsertAt(CogArray, element, index) { \
    assert(index >= 0 && index < CogArray->size); \
    assert(CogArray->size > 0); \
    int __i__; \
    typeof(CogArray->__type_instance__) insert = element; \
    typeof(CogArray->__type_instance__) tmp; \
    for(__i__ = index; __i__ < CogArray->size; ++__i__) { \
        tmp = CogArray->data[__i__]; \
        CogArray->data[__i__] = insert; \
        insert = tmp; \
    } \
    CogArrayPush(CogArray, insert); \
}

#define CogArrayRemoveAt(CogArray, index) { \
    assert(index >= 0 && index < CogArray->size); \
    assert(CogArray->size > 0); \
    int __i__; \
    for(__i__ = index; __i__ < CogArray->size - 1; ++__i__) { \
        CogArray->data[__i__] = CogArray->data[__i__+1]; \
    } \
    CogArrayPop(CogArray); \
}

#define CogArraySort(CogArray, first, second, comparator) { \
    if(CogArray->size > 1) { \
        typeof(CogArray->__type_instance__) tmp; \
        typeof(CogArray->__type_instance__) first, second; \
        int __i__, __j__; \
        for(__i__ = 0; __i__ < CogArray->size - 1; ++__i__) { \
            for(__j__ = __i__ + 1; __j__ < CogArray->size; ++__j__) { \
                first = CogArray->data[__i__]; \
                second = CogArray->data[__j__]; \
                if( (comparator) ) { \
                    tmp = CogArray->data[__i__]; \
                    CogArray->data[__i__] = CogArray->data[__j__]; \
                    CogArray->data[__j__] = tmp; \
                } \
            } \
        } \
    } \
}

#define CogArrayPop(CogArray) ({ \
    assert(CogArray->size > 0); \
    typeof(CogArray->__type_instance__) elem = CogArray->data[--CogArray->size]; \
    if(CogArray->size <= CogArray->storage && CogArray->storage > 16) { \
        CogArray->storage /= 2; \
        CogArray->data = (typeof(CogArray->__type_instance__) *)realloc(CogArray->data, sizeof(typeof(CogArray->__type_instance__)) * CogArray->storage); \
    } \
    if(CogArray->size == 0) { \
        free(CogArray->data); \
        CogArray->storage = 0; \
    }; \
    elem; \
})

#define CogArrayAllocSlice(T, CogArray, start, end) ({ \
    assert(CogArray->size); \
    int s, e, step; \
    start < 0 ? (s = CogArray->size + start) : (s = start); \
    assert(s < CogArray->size && s >= 0); \
    end < 0 ? e = (CogArray->size + end) : (e = end); \
    assert(e < CogArray->size && e >= 0); \
    s <= e ? (step = 1) : (step = -1); \
    CogArray(T) __slice__ = CogArrayAlloc(T); \
    for(;;s += step) { \
        CogArrayPush(__slice__, CogArray->data[s]); \
        if(s == e) { break; } \
    } \
    __slice__; \
})

#define CogArrayAt(CogArray, index) ({ \
    int i; \
    if(index >= 0) { \
        assert(index < CogArray->size); \
        i = index; \
    } else { \
        assert((CogArray->size + index) >=0); \
        i = CogArray->size + index; \
    } \
    CogArray->data[i];\
})

#define CogArrayAllocCopy(T, CogArray) ({ \
    assert(CogArray != NULL); \
    CogArray(T) __copy__ = CogArrayAlloc(T); \
    int __i__; \
    for(__i__ = 0; __i__ < CogArray->size; ++__i__) { \
        CogArrayPush(__copy__, CogArrayAt(CogArray, __i__)); \
    } \
})

#define CogArrayForEach(CogArray, element, block) ({ \
    int __i__; \
    typeof(CogArray->__type_instance__) element; \
    for(__i__ = 0; __i__ < CogArray->size; ++__i__) { \
        element = CogArrayAt(CogArray, __i__); \
        (block); \
    } \
})

#define CogArrayReverse(CogArray) { \
    int l, r; \
    typeof(CogArray->__type_instance__) tmp; \
    for(l = 0, r = CogArray->size - 1; (l != r) && (l <= r); ++l, --r) { \
        tmp = CogArray->data[l]; \
        CogArray->data[l] = CogArray->data[r]; \
        CogArray->data[r] = tmp; \
    } \
}

#define CogArrayInspect(T, CogArray) { \
    printf("<CogArray (%d) @0x%lx [", CogArray->size, (unsigned long)CogArray); \
    int __k__ = 0; \
    CogArrayForEach(CogArray, __element__, { \
        T ## Inspect(__element__); \
        if(__k__ < CogArray->size - 1) { printf(", "); } \
        __k__++; \
    }); \
    printf("]>\n"); \
}

#endif