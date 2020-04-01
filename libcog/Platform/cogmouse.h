#ifndef _COG_MOUSE_H_
#define _COG_MOUSE_H_

#include "CogVector2.h"

typedef enum {
  CogMouseButtonLeft = 0,
  CogMouseButtonRight,
  CogMouseButtonMiddle,
  CogMouseButtonX1,
  CogMouseButtonX2,
  CogMouseButtonScrollDown,
  CogMouseButtonScrollUp,
  CogMouseButtonScrollLeft,
  CogMouseButtonScrollRight,
  
  CogMouseButtonCount
} CogMouseButton;

void CogMouseSetRelativeMode(const Bool mode);
void CogMouseSetVisible(const Bool visible);
Bool CogMouseButtonDown(const CogMouseButton button);
Bool CogMouseButtonUp(const CogMouseButton button);
Bool CogMouseButtonPressed(const CogMouseButton button);
Bool CogMouseButtonReleased(const CogMouseButton button);
Bool CogMouseMoved(void);
CogVector2 CogMouseGetPosition(void);
CogVector2 CogMouseGetRelativePosition(void);

#endif