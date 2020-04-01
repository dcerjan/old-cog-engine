#ifndef _COG_JSON_OBJECT_H_
#define _COG_JSON_OBJECT_H_


#include "CogList.h"
#include "cogtypes.h"
#include "cogstring.h"

typedef enum CogJsonObjectType {
  CogJsonObjectTypeNull     = 0,
  CogJsonObjectTypeString   = 1,
  CogJsonObjectTypeCogArray    = 2,
  CogJsonObjectTypeNumber   = 3,
  CogJsonObjectTypeObject   = 4,
  CogJsonObjectTypeBoolean  = 5
} CogJsonObjectType;

struct CogArray_CogJsonObject;
struct CogJsonObject {
  CogJsonObjectType      type;
  void*               data;
};
typedef struct CogJsonObject* CogJsonObject;
CogDefineArray(CogJsonObject);
CogDefineList(CogJsonObject);

CogJsonObject
CogJsonObjectAlloc(
  void
);

CogJsonObject
CogJsonObjectAllocNull(
  void
);

CogJsonObject
CogJsonObjectAllocCogArray(
  void
);

CogJsonObject
CogJsonObjectAllocBoolean(
  Bool value
);

CogJsonObject
CogJsonObjectAllocNumber(
  double value
);

CogJsonObject
CogJsonObjectAllocString(
  const char* value
);

void
CogJsonObjectFree(
  CogJsonObject object
);

void
CogJsonObjectInspect(
  CogJsonObject object
);

void
CogJsonObjectSerialze(
  CogJsonObject object, 
  const char* filename
);

CogString
CogJsonObjectSerialzeToString(
  CogJsonObject object
);

double
CogJsonObjectGetNumber(
  CogJsonObject object, 
  const char* key
);

CogString
CogJsonObjectGetString(
  CogJsonObject object, 
  const char* key
);

Bool
CogJsonObjectGetBoolean(
  CogJsonObject object, 
  const char* key
);

CogArray(CogJsonObject)
CogJsonObjectGetCogArray(
  CogJsonObject object, 
  const char* key
);

CogJsonObject
CogJsonObjectGetObject(
  CogJsonObject object, 
  const char* key
);

void
CogJsonObjectSetNumber(
  CogJsonObject object, 
  const char* key, 
  double value
);

void
CogJsonObjectSetBoolean(
  CogJsonObject object, 
  const char* key, 
  Bool value
);

void
CogJsonObjectSetString(
  CogJsonObject object, 
  const char* key, 
  const char* value
);

void
CogJsonObjectSetCogArray(
  CogJsonObject object, 
  const char* key, 
  CogArray(CogJsonObject) CogArray
);

void
CogJsonObjectSetObject(
  CogJsonObject object, 
  const char* key, 
  CogJsonObject obj
);


#endif