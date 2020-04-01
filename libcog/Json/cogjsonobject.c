#include "CogJsonObject.h"


struct CogJsonPair {
  CogString           key;
  CogJsonObject          object;
};
typedef struct CogJsonPair* CogJsonPair;
CogDefineArray(CogJsonPair);

CogJsonPair
CogJsonPairAlloc(
  const char* key, 
  CogJsonObject value
) {
  CogJsonPair self = malloc(sizeof(struct CogJsonPair));
  self->key = CogStringAlloc(key);
  self->object = value;
  return self;
}


CogJsonObject 
CogJsonObjectAlloc(
  void
) {
  CogJsonObject self = malloc(sizeof(struct CogJsonObject));
  self->data = CogArrayAlloc(CogJsonPair);
  self->type = CogJsonObjectTypeObject;
  return self;
}

CogJsonObject
CogJsonObjectAllocNull(
  void
) {
  CogJsonObject self = malloc(sizeof(struct CogJsonObject));
  self->data = NULL;
  self->type = CogJsonObjectTypeNull;
  return self;
}

CogJsonObject 
CogJsonObjectAllocCogArray(
  void
) {
  CogJsonObject self = malloc(sizeof(struct CogJsonObject));
  self->data = CogArrayAlloc(CogJsonObject);
  self->type = CogJsonObjectTypeCogArray;
  return self;
}

CogJsonObject 
CogJsonObjectAllocBoolean(
  Bool value
) {
  CogJsonObject self = malloc(sizeof(struct CogJsonObject));
  self->data = malloc(sizeof(Bool));
  *(Bool*)self->data = value;
  self->type = CogJsonObjectTypeBoolean;
  return self;
}

CogJsonObject
CogJsonObjectAllocNumber(
  double value
) {
  CogJsonObject self = malloc(sizeof(struct CogJsonObject));
  self->data = malloc(sizeof(double));
  *(double*)self->data = value;
  self->type = CogJsonObjectTypeNumber;
  return self;
}

CogJsonObject 
CogJsonObjectAllocString(
  const char* value
) {
  CogJsonObject self = malloc(sizeof(struct CogJsonObject));
  self->data = CogStringAlloc(value);
  self->type = CogJsonObjectTypeString;
  return self;
}

void
CogJsonObjectFree(
  CogJsonObject object
) {
  assert(object != NULL);
  switch(object->type) {
    case CogJsonObjectTypeNull:
      free(object);
      break;
    case CogJsonObjectTypeBoolean:
      free((Bool*)object->data);
      free(object);
      break;
    case CogJsonObjectTypeNumber:
      free((double*)object->data);
      free(object);
      break;
    case CogJsonObjectTypeString:
      CogStringFree((CogString)object->data);
      free(object);
      break;
    case CogJsonObjectTypeCogArray:
      {
        CogArray(CogJsonObject) a = (CogArray(CogJsonObject))object->data;
        CogArrayForEach(a, v, {
          CogJsonObjectFree(v);
        });
        CogArrayFree(a);
      }
      free(object);
      break;
    case CogJsonObjectTypeObject:
      {
        CogArray(CogJsonPair) h = (CogArray(CogJsonPair))object->data;
        CogArrayForEach(h, v, {
          CogStringFree(v->key);
          CogJsonObjectFree(v->object);
          free(v);
        });
        CogArrayFree(h);
      }
      free(object);
      break;
    default:
      free(object);
      break;
  }
}

void _indent_(int depth) {
  int i;
  for(i = 0; i < depth * 2; ++i) { 
    printf(" "); 
  }
}

void _inspect_(CogJsonObject root, int depth) {
  if(!root) {
    printf("NULL\n");
    return;
  }
  int i;
  switch(root->type) {
    case CogJsonObjectTypeObject:
      printf("{\n");
      CogArray(CogJsonPair) h = (CogArray(CogJsonPair))root->data;
      i = 0;
      CogArrayForEach(h, pair, {
        i++;
        _indent_(depth + 1); printf("%s: ", pair->key->data);
        _inspect_(pair->object, depth + 1);
        if(i != h->size) {
          printf(",\n");
        } else {
          printf("\n");
        }
      });
      _indent_(depth);printf("}");
      break;
    case CogJsonObjectTypeCogArray:
      printf("[\n");
      CogArray(CogJsonObject) a = (CogArray(CogJsonObject))root->data;
      i = 0;
      CogArrayForEach(a, v, {
        i++;
        _indent_(depth + 1);
        _inspect_(v, depth + 1);
        if(i != a->size) {
          printf(",\n");
        } else {
          printf("\n");
        }
      });
      _indent_(depth);printf("]");
      break;
    case CogJsonObjectTypeString:
      printf("'%s'", c_str( ((CogString)root->data)));
      break;
    case CogJsonObjectTypeBoolean:
      if(True == *(Bool*)root->data) {
        printf("true");
      } else {
        printf("false");
      }
      break;
    case CogJsonObjectTypeNumber:
      printf("%lf", *(double*)root->data);
      break;
    default:
      printf("null");
      break;
      return;
  }
}

void _serialize_(CogJsonObject root, FILE* fp) {
  int i;
  switch(root->type) {
    case CogJsonObjectTypeObject:
      fprintf(fp, "{");
      CogArray(CogJsonPair) h = (CogArray(CogJsonPair))root->data;
      i = 0;
      CogArrayForEach(h, pair, {
        i++;
        fprintf(fp, "%s:", pair->key->data);
        _serialize_(pair->object, fp);
        if(i != h->size) {
          fprintf(fp, ",");
        }
      });
      fprintf(fp, "}");
      break;
    case CogJsonObjectTypeCogArray:
      fprintf(fp, "[");
      CogArray(CogJsonObject) a = (CogArray(CogJsonObject))root->data;
      i = 0;
      CogArrayForEach(a, v, {
        i++;
        _serialize_(v, fp);
        if(i != a->size) {
          fprintf(fp, ",");
        }
      });
      fprintf(fp, "]");
      break;
    case CogJsonObjectTypeString:
      fprintf(fp, "'%s'", c_str( ((CogString)root->data)));
      break;
    case CogJsonObjectTypeBoolean:
      if(True == *(Bool*)root->data) {
        fprintf(fp, "true");
      } else {
        fprintf(fp, "false");
      }
      break;
    case CogJsonObjectTypeNumber:
      fprintf(fp, "%lf", *(double*)root->data);
      break;
    default:
      fprintf(fp, "null");
      break;
      return;
  }
}

void _serialize_str_(CogJsonObject root, CogArray(char) string) {
  unsigned long i, j, l;
  
  char buffer[1024];
  memset(buffer, 0, sizeof(char) * 1024);
  
  switch(root->type) {
    case CogJsonObjectTypeObject:
      
      CogArrayPush(string, '{');
      CogArray(CogJsonPair) h = (CogArray(CogJsonPair))root->data;
      i = 0;
      CogArrayForEach(h, pair, {
        i++;
        sprintf(buffer, "\"%s\":", pair->key->data);
        l = strlen(buffer);
        for(j = 0; j < l; ++j) {
          CogArrayPush(string, buffer[j]);
        }
        _serialize_str_(pair->object, string);
        if(i != h->size) {
          CogArrayPush(string, ',');
        }
      });
      CogArrayPush(string, '}');
      break;
    case CogJsonObjectTypeCogArray:
      CogArrayPush(string, '[');
      CogArray(CogJsonObject) a = (CogArray(CogJsonObject))root->data;
      i = 0;
      CogArrayForEach(a, v, {
        i++;
        _serialize_str_(v, string);
        if(i != a->size) {
          CogArrayPush(string, ',');
        }
      });
      CogArrayPush(string, ']');
      break;
    case CogJsonObjectTypeString:
      sprintf(buffer, "\"%s\"", c_str( ((CogString)root->data)));
      l = strlen(buffer);
      for(j = 0; j < l; ++j) {
        CogArrayPush(string, buffer[j]);
      }
      break;
    case CogJsonObjectTypeBoolean:
      if(True == *(Bool*)root->data) {
        CogArrayPush(string, 't');
        CogArrayPush(string, 'r');
        CogArrayPush(string, 'u');
        CogArrayPush(string, 'e');
      } else {
        CogArrayPush(string, 'f');
        CogArrayPush(string, 'a');
        CogArrayPush(string, 'l');
        CogArrayPush(string, 's');
        CogArrayPush(string, 'e');
      }
      break;
    case CogJsonObjectTypeNumber:
      sprintf(buffer, "%lf", *(double*)root->data);
      l = strlen(buffer);
      for(j = 0; j < l; ++j) {
        CogArrayPush(string, buffer[j]);
      }
      break;
    default:
      CogArrayPush(string, 'n');
      CogArrayPush(string, 'u');
      CogArrayPush(string, 'l');
      CogArrayPush(string, 'l');
      break;
      return;
  }
}

void
CogJsonObjectInspect(
  CogJsonObject object
) {
  //assert(object->type == CogJsonObjectTypeObject);
  _inspect_(object, 0);
}

void
CogJsonObjectSerialze(
  CogJsonObject object, 
  const char* filename
) {
  assert(object->type == CogJsonObjectTypeObject);
  FILE* fp = fopen(filename, "w");
  assert(fp != NULL);
  _serialize_(object, fp);
  fclose(fp);
}

CogString
CogJsonObjectSerialzeToString(
  CogJsonObject object
) {
  assert(object->type == CogJsonObjectTypeObject);
  CogArray(char) string = CogArrayAlloc(char);
  _serialize_str_(object, string);
  CogArrayPush(string, '\0');
  CogString ret = CogStringAlloc(string->data);
  CogArrayFree(string);
  return ret;
}

/////////////
// Getters //
/////////////
double 
CogJsonObjectGetNumber(
  CogJsonObject object, 
  const char* key
) {
  assert(object->type == CogJsonObjectTypeObject);
  CogArrayForEach( ( (CogArray(CogJsonPair))object->data), v, {
    if(!strcmp(key, v->key->data)) {
      return *(double*)v->object->data;
    }
  });
  printf("Object does not contain a value for key %s.\n", key);
  return 0;
}

Bool
CogJsonObjectGetBoolean(
  CogJsonObject object, 
  const char* key
) {
  assert(object->type == CogJsonObjectTypeObject);
  CogArrayForEach( ( (CogArray(CogJsonPair))object->data), v, {
    if(!strcmp(key, v->key->data)) {
      return *(Bool*)v->object->data;
    }
  });
  printf("Object does not contain a value for key %s.\n", key);
  return 0;
}

CogString
CogJsonObjectGetString(
  CogJsonObject object, 
  const char* key
) {
  assert(object->type == CogJsonObjectTypeObject);
  CogArrayForEach( ( (CogArray(CogJsonPair))object->data), v, {
    if(!strcmp(key, v->key->data)) {
      return (CogString)v->object->data;
    }
  });
  printf("Object does not contain a value for key %s.\n", key);
  return NULL;
}

CogArray(CogJsonObject)
CogJsonObjectGetCogArray(
  CogJsonObject object, 
  const char* key
) {
  assert(object->type == CogJsonObjectTypeObject);
  CogArrayForEach( ( (CogArray(CogJsonPair))object->data), v, {
    if(!strcmp(key, v->key->data)) {
      return (CogArray(CogJsonObject))v->object->data;
    }
  });
  printf("Object does not contain a value for key %s.\n", key);
  return NULL;
}

CogJsonObject CogJsonObjectGetObject (CogJsonObject object, const char* key) {
  assert(object->type == CogJsonObjectTypeObject);
  CogArrayForEach( ( (CogArray(CogJsonPair))object->data), v, {
    if(!strcmp(key, v->key->data)) {
      return (CogJsonObject)v->object;
    }
  });
  printf("Object does not contain a value for key %s.\n", key);
  return NULL;
}

/////////////
// Setters //
/////////////
void 
CogJsonObjectSetNumber(
  CogJsonObject object, 
  const char* key, 
  double value
) {
  assert(object->type == CogJsonObjectTypeObject);
  CogJsonObject self = malloc(sizeof(struct CogJsonObject));
  self->data = malloc(sizeof(double));
  *(double*)self->data = value;
  self->type = CogJsonObjectTypeNumber;
  CogArrayPush( ((CogArray(CogJsonPair))object->data), CogJsonPairAlloc(key, self));
}


void
CogJsonObjectSetBoolean(
  CogJsonObject object, 
  const char* key, 
  Bool value
) {
  assert(object->type == CogJsonObjectTypeObject);
  CogJsonObject self = malloc(sizeof(struct CogJsonObject));
  self->data = malloc(sizeof(Bool));
  *(Bool*)self->data = value;
  self->type = CogJsonObjectTypeBoolean;
  CogArrayPush( ((CogArray(CogJsonPair))object->data), CogJsonPairAlloc(key, self));
}

void
CogJsonObjectSetString(
  CogJsonObject object, 
  const char* key, 
  const char* value
) {
  assert(object->type == CogJsonObjectTypeObject);
  CogJsonObject self = malloc(sizeof(struct CogJsonObject));
  self->data = CogStringAlloc(value);
  self->type = CogJsonObjectTypeString;
  CogArrayPush( ((CogArray(CogJsonPair))object->data), CogJsonPairAlloc(key, self));
}

void
CogJsonObjectSetCogArray(
  CogJsonObject object, 
  const char* key, 
  CogArray(CogJsonObject) CogArray
) {
  assert(object->type == CogJsonObjectTypeObject);
  CogJsonObject self = malloc(sizeof(struct CogJsonObject));
  self->data = CogArray;
  self->type = CogJsonObjectTypeCogArray;
  CogArrayPush( ((CogArray(CogJsonPair))object->data), CogJsonPairAlloc(key, self));
}

void
CogJsonObjectSetObject(
  CogJsonObject object, 
  const char* key, 
  CogJsonObject obj
) {
  assert(object->type == CogJsonObjectTypeObject);
  CogArrayPush( ((CogArray(CogJsonPair))object->data), CogJsonPairAlloc(key, obj));
}