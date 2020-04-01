#ifndef _COG_TYPES_H_
#define _COG_TYPES_H_

#include <stdio.h>
#include <stdint.h>

#include "cogarray.h"
#include "coglist.h"
#include "coghash.h"
#include "cogmap.h"

//#ifndef __APPLE__
typedef enum Bool {
    True    = 1,
    False   = 0,
    Yes     = 1,
    No      = 0
} Bool;
//#endif

// some standard types as CogArrays
CogDefineArray(char);
CogDefineArray(short);
CogDefineArray(int);
CogDefineArray(long);
CogDefineArray(float);
CogDefineArray(double);

CogDefineList(char);
CogDefineList(short);
CogDefineList(int);
CogDefineList(long);
CogDefineList(float);
CogDefineList(double);

CogDefineHash(char);
CogDefineHash(short);
CogDefineHash(int);
CogDefineHash(long);
CogDefineHash(float);
CogDefineHash(double);

#define CogInherit(C) C super

/* Basic value types */
//typedef short           short;
typedef unsigned short  ushort;
//typedef int             int;
typedef unsigned int    uint;
typedef int32_t         int32;
typedef uint32_t        uint32;
typedef int64_t         int64;
typedef uint64_t        uint64;
//typedef char            byte;
typedef unsigned char   ubyte;
//typedef long            long;
typedef unsigned long   ulong;
//typedef float           float;
//typedef double          double;

/* builtin types inspection */
#define shortInspect(var) printf("%hd", var)
#define ushortInspect(var) printf("%hu", var)
#define intInspect(var) printf("%d", var)
#define uintInspect(var) printf("%u", var);
#define int32Inspect(var) printf("%d", var);
#define uint32Inspect(var) printf("%u", var);
#define int64Inspect(var) printf("%ld", var);
#define uint64Inspect(var) printf("%lu", var);
#define byteInspect(var) printf("%d", var);
#define charInspect(var) printf("%c", var);
#define ubyteInspect(var) printf("%hd", var;
#define longInspect(var) printf("%ld", var);
#define ulongInspect(var) printf("%lu", var);
#define floatInspect(var) printf("%f", var);
#define doubleInspect(var) printf("%g", var);

#endif