#ifndef _COG_CogHash_H_
#define _COG_CogHash_H_

#define CogDefineHash(T) \
    typedef struct __CogHashElement_ ## T { \
        char *                      key; \
        T                           value; \
    } __CogHashElement_ ## T; \
    \
    typedef struct __CogHashBucket_ ## T { \
        __CogHashElement_ ## T*     data; \
        int                         storage; \
        int                         size; \
    } __CogHashBucket_ ## T; \
    typedef __CogHashBucket_ ## T* CogHashBucket_ ## T; \
    \
    struct CogHash_ ## T { \
        CogHashBucket_ ## T         buckets; \
        unsigned int                numbuckets; \
        unsigned int                numPairs; \
        T                           __type_instance__; \
        __CogHashElement_ ## T      __type_instance_kv__; \
    }; \
    typedef struct CogHash_ ## T * CogHash_ ## T;

#define CogHash(T) CogHash_ ## T
#define CogHashAlloc(T) ({ \
    CogHash(T) __hash__ = malloc(sizeof(struct CogHash_ ## T)); \
    __hash__->numPairs = 0; \
    __hash__->numbuckets = 47; \
    __hash__->buckets = malloc(sizeof(__CogHashBucket_ ## T) * __hash__->numbuckets); \
    memset(&__hash__->__type_instance__, 0, sizeof(__hash__->__type_instance__)); \
    memset(&__hash__->__type_instance_kv__, 0, sizeof(__hash__->__type_instance_kv__)); \
    int __i__; \
    for(__i__ = 0; __i__ < __hash__->numbuckets; ++__i__) { \
        __hash__->buckets[__i__].data = NULL; \
        __hash__->buckets[__i__].storage = 0; \
        __hash__->buckets[__i__].size = 0; \
    } \
    __hash__; \
})

#define CogHashFree(hash) { \
    int __i__, __j__; \
    for(__i__ = 0; __i__ < hash->numbuckets; ++__i__) { \
        for(__j__ = 0; __j__ < hash->buckets[__i__].size; ++__j__) { \
            free(hash->buckets[__i__].data[__j__].key); \
        } \
        free(hash->buckets[__i__].data); \
    } \
    free(hash->buckets); \
    free(hash); \
    hash = NULL; \
}

#define CogHashForEach(hash, k, v, block) { \
    int __i__, __j__; \
    char* k; \
    typeof(hash->__type_instance__) v; \
    for(__i__ = 0; __i__ < hash->numbuckets; ++__i__) { \
        for(__j__ = 0; __j__ < hash->buckets[__i__].size; ++__j__) { \
            k = hash->buckets[__i__].data[__j__].key; \
            v = hash->buckets[__i__].data[__j__].value; \
            (block); \
        } \
    } \
}

#define CogHashContains(hash, k) ({ \
    int __i__; \
    Bool __found__ = False; \
    int __$__ = 0; \
    for(__i__ = 0; __i__ < strlen(k); ++__i__) { __$__+=k[__i__]; } \
    __$__ %= hash->numbuckets; \
    for(__i__ = 0; __i__ < hash->buckets[__$__].size; ++__i__) { \
        if(!strcmp(k, hash->buckets[__$__].data[__i__].key)) { \
            __found__ = True; \
            break; \
        } \
    } \
    __found__; \
})

#define CogHashSet(hash, k, e) { \
    int __i__; \
    int __$__ = 0; \
    for(__i__ = 0; __i__ < strlen(k); ++__i__) { __$__+=k[__i__]; } \
    __$__ %= hash->numbuckets; \
    if(hash->buckets[__$__].storage == 0) { \
        hash->buckets[__$__].storage = 16; \
        hash->buckets[__$__].size = 0; \
        hash->buckets[__$__].data = malloc(sizeof(typeof(hash->__type_instance_kv__)) * 16); \
        hash->buckets[__$__].data[hash->buckets[__$__].size].value = e; \
        hash->buckets[__$__].data[hash->buckets[__$__].size].key = malloc(strlen(k) + 1); \
        strcpy(hash->buckets[__$__].data[hash->buckets[__$__].size].key, k); \
        hash->buckets[__$__].size++; \
    } else { \
        if(hash->buckets[__$__].size == hash->buckets[__$__].storage) { \
            hash->buckets[__$__].storage *= 2; \
            hash->buckets[__$__].data = realloc(hash->buckets[__$__].data, sizeof(typeof(hash->__type_instance_kv__)) * hash->buckets[__$__].storage); \
        } \
        hash->buckets[__$__].data[hash->buckets[__$__].size].value = e; \
        hash->buckets[__$__].data[hash->buckets[__$__].size].key = malloc(strlen(k) + 1); \
        strcpy(hash->buckets[__$__].data[hash->buckets[__$__].size].key, k); \
        hash->buckets[__$__].size++; \
    } \
    hash->numPairs++; \
}

#define CogHashGet(hash, k) ({ \
    int __i__; \
    typeof(hash->__type_instance__) __value__ = 0; \
    int __$__ = 0; \
    for(__i__ = 0; __i__ < strlen(k); ++__i__) { __$__+=k[__i__]; } \
    __$__ %= hash->numbuckets; \
    for(__i__ = 0; __i__ < hash->buckets[__$__].size; ++__i__) { \
        if(!strcmp(k, hash->buckets[__$__].data[__i__].key)) { \
            __value__ = hash->buckets[__$__].data[__i__].value; \
        } \
    } \
    __value__; \
})

#define CogHashRemove(hash, k) { \
    assert(CogHashContains(hash, k)); \
    int __i__; \
    int __$__ = 0; \
    for(__i__ = 0; __i__ < strlen(k); ++__i__) { __$__+=k[__i__]; } \
    __$__ %= hash->numbuckets; \
    for(__i__ = 0; __i__ < hash->buckets[__$__].size; ++__i__) { \
        if(!strcmp(k, hash->buckets[__$__].data[__i__].key)) { \
            hash->numPairs--; \
            free(hash->buckets[__$__].data[__i__].key); \
            hash->buckets[__$__].size--; \
            if(hash->buckets[__$__].size == 0) { \
                hash->buckets[__$__].storage = 0; \
                free(hash->buckets[__$__].data); \
                break; \
            } \
            for(; __i__ < hash->buckets[__$__].size - 1; ++__i__) { \
                hash->buckets[__$__].data[__i__].value = hash->buckets[__$__].data[__i__ + 1].value; \
                hash->buckets[__$__].data[__i__].key = hash->buckets[__$__].data[__i__ + 1].key; \
            } \
            if(hash->buckets[__$__].size <= hash->buckets[__$__].storage) { \
                hash->buckets[__$__].storage /= 2; \
                hash->buckets[__$__].data = realloc(hash->buckets[__$__].data, sizeof(hash->__type_instance_kv__) * hash->buckets[__$__].storage); \
            } \
        } \
    } \
}

#define CogHashInspect(T, hash) ({ \
    printf("<CogHash (%d) @0x%lx { ", hash->numPairs, (unsigned long)hash); \
    unsigned int __k__ = 0; \
    CogHashForEach(hash, __key__, __value__, { \
        printf("\"%s\" => ", __key__); \
        T ## Inspect(__value__); \
        if(__k__ < hash->numPairs-1) { printf(", "); } \
        __k__++; \
    }); \
    printf("}>\n"); \
})


#endif