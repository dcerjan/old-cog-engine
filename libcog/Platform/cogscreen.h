#ifndef _COG_SCREEN_H_
#define _COG_SCREEN_H_


#include "cogstring.h"
#include "cogtypes.h"

typedef struct CogDisplayMode {
  uint32                  width;
  uint32                  height;
  uint32                  refreshRate;
  float                   aspectRatio;
} CogDisplayMode;
CogDefineArray(CogDisplayMode);

void CogDisplayModeInspect(CogDisplayMode mode);

struct CogScreen {
  CogString                 name;
  int                       id;
  CogDisplayMode            nativeMode;
  CogDisplayMode            curentMode;
  CogArray(CogDisplayMode)  displayModes;
};
typedef struct CogScreen* CogScreen;
CogDefineArray(CogScreen);

void CogScreenSetMode(CogScreen self, const CogDisplayMode mode);
void CogScreenInspect(const CogScreen self);


#endif