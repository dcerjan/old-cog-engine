#ifndef _COG_WINDOW_H_
#define _COG_WINDOW_H_


#include "cogstring.h"
#include "cogtypes.h"
#include "cogrect.h"

#ifdef HAS_SDL2
  #include <SDL2/SDL.h>
#endif

struct CogScreen;
struct CogWindow {
  CogString                 name;
  CogRect                   rect;
  const struct CogScreen*   screen;
  Bool                      fullscreen;
  Bool                      fullscreenDesktop;
  Bool                      borderless;
#ifdef HAS_SDL2
  SDL_GLContext*            context;
  SDL_Window*               wnd;
#endif
};
typedef struct CogWindow* CogWindow;
CogDefineArray(CogWindow);

void CogWindowSetScreen(CogWindow self, const struct CogScreen* screen);
void CogWindowInspect(const CogWindow self);
void CogWindowSwap(const CogWindow self);


#endif