#include "cogplatform.h"

#include <SDL2/SDL.h>
#include <IL/il.h>
#include "coggl.h"

/* cog platform components */
  #include "cogscreen.h"
  #include "cogwindow.h"
  #include "cogtimer.h"
  #include "coglogger.h"
  #include "coggc.h"
  /* cog platform components*/ 
  static CogArray(CogScreen)  __cog_platform_screens__ = NULL;
  static CogScreen            __cog_platform_main_screen__ = NULL;
  static CogArray(CogWindow)  __cog_platform_windows__ = NULL;
  static CogWindow            __cog_platform_focused_window__ = NULL;
  static CogTimer             __cog_platform_timer__ = NULL;
  static CogLogger            __cog_platform_logger__ = NULL;
  static Bool                 __cog_platform_gl_extensions_loaded__ = False;
  static CogCleanupPool       __cog_platform_cleanup_pool__ = NULL;
  
/* cog managers */
  #include "cogresourcemanager.h"
  #include "cogscenemanager.h"
  #include "cognetworkmanager.h"
    
/* cog graphics */
  #include "coggraphics.h"
  /*static OpenGLVersion __cog_platform_open_gl_version__ = 0;*/
  
/* cog input */
  #include "cogkeyboard.h"
  #include "cogmouse.h"
  #include "cogcontroler.h"
  /* cog platform input */
  static struct { 
    Bool state[CogKeyCount];
    Bool old[CogKeyCount]; 
  } __cog_platform_keyboard__;
  
  static struct {
    Bool      state[CogMouseButtonCount];
    Bool      old[CogMouseButtonCount];
    Bool      relativeMode;
    // struct { int state, old; } verticalScrollAxis, horizontalScrollAxis;
    struct { int x, y; } position, oldPosition, relativePosition;
  } __cog_platform_mouse__ ;
  
  static struct {
    struct {
      struct { double left, right; } trigger;
      struct { Bool left, right; } bumper;
      struct { Bool start, back, commandButton; } controlButtons;
      struct { double verticalAxis, horizontalAxis; } leftThumbstick;
      struct { double verticalAxis, horizontalAxis; } rightThumbstick;
      struct { Bool up, down, left, right; } directionalPad;
      struct { Bool X, Y, A, B; } buttons;
    } state, old;
  } __cog_platform_controlers__[4];

/* cog threading */
  #include "cogthread.h"
  #include "cogmutex.h"

/* cog audio */
  // #include "cogaudio"
  
/* cog network */
  // #include "cognetwork.h"
  
Bool _initGraphics(void);
Bool _initInput(void);
Bool _initThreading(void);
Bool _initNetwork(void);
Bool _initAudio(void);

////////////////
//  Platform  //
////////////////
Bool PlatformInit(void /*OpenGLVersion      glversion*/) {
  /* init cog platform */
  // screens
  __cog_platform_cleanup_pool__ = CogCleanupPoolAlloc("Platform Autodesturct Pool");
  
  __cog_platform_logger__ = CogLoggerAlloc("Cog Logger", "cog.log");
  CogCleanupPoolRegisterForCleanup(__cog_platform_cleanup_pool__, __cog_platform_logger__, (auto_destruct)CogLoggerFree);
  
  __cog_platform_screens__ = CogArrayAlloc(CogScreen);
  /*__cog_platform_open_gl_version__ = glversion;*/
  
  ilInit();
  
  SDL_Init(SDL_INIT_VIDEO);
  int nvm, nvds = SDL_GetNumVideoDisplays();
  int i, j;
  SDL_DisplayMode mode;
  for(i = 0; i < nvds; ++i) {
    CogScreen screen = (CogScreen)malloc(sizeof(struct CogScreen));
    screen->name = CogStringAllocFromFormat("Screen (%d)", i);
    screen->id = i;
    screen->displayModes = CogArrayAlloc(CogDisplayMode);
    nvm = SDL_GetNumDisplayModes(i);
    for(j = 0; j < nvm; ++j) {
      SDL_GetDisplayMode(i, j, &mode);
      CogDisplayMode m;
      m.aspectRatio = (float)mode.w / mode.h;
      m.width = mode.w;
      m.height = mode.h;
      m.refreshRate = mode.refresh_rate;
      CogArrayPush(screen->displayModes, m);
    }
    int err = SDL_GetCurrentDisplayMode(i, &mode);
    if(err != 0) {
      CogLoggerError(__cog_platform_logger__, "Cant get current display mode (Screen %d)!", i);
    } else {
      screen->curentMode.aspectRatio = (float)mode.w / mode.h;
      screen->curentMode.width = mode.w;
      screen->curentMode.height = mode.h;
      screen->curentMode.refreshRate = mode.refresh_rate;;
    }
    CogArrayPush(__cog_platform_screens__, screen);
  }
  __cog_platform_main_screen__ = __cog_platform_screens__->data[0];
  
  // windows
  __cog_platform_windows__ = CogArrayAlloc(CogWindow);
  
  _initGraphics();
  _initInput();
  _initThreading();
  _initNetwork();
  _initAudio();
  
  
  return True;
}

void CogPlatformCleanup(void) {
  CogCleanupPoolFree(__cog_platform_cleanup_pool__);
}

CogLogger CogPlatformGetLogger(void) {
  return __cog_platform_logger__;
} 

CogScreen CogPlatformGetMainScreen(void) {
  return __cog_platform_main_screen__;
}

CogArray(CogScreen) CogPlatformGetScreens(void) {
  return __cog_platform_screens__;
}

CogArray(CogWindow) CogPlatformGetWindows(void) {
  return __cog_platform_windows__;
}

CogWindow CogPlatformGetFocusedWindow(void) {
  return __cog_platform_focused_window__;
}

CogWindow CogPlatformCreateWindow(
  const char*             name,
  const CogRect           rect,
  const struct CogScreen* screen,
  const Bool              fullscreen,
  const Bool              fullscreenDesktop,
  const Bool              borderless
)
{
  if(!__cog_platform_windows__) {
    CogLoggerError(__cog_platform_logger__, "Cant create a window: Platform is not initialized!\n");
    exit(2);
  }
  CogWindow wnd = (CogWindow)malloc(sizeof(struct CogWindow));
  wnd->name = CogStringAlloc(name);
  wnd->fullscreen = fullscreen;
  wnd->borderless = borderless;
  wnd->fullscreenDesktop = fullscreenDesktop;
  wnd->rect = rect;
  wnd->screen = screen;
  /**/
  uint32 flags = SDL_WINDOW_OPENGL;
  if(fullscreen) flags |= SDL_WINDOW_FULLSCREEN;
  else if(fullscreenDesktop) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  if(borderless) flags |= SDL_WINDOW_BORDERLESS;
  
  wnd->wnd = SDL_CreateWindow(wnd->name->data,
                               SDL_WINDOWPOS_UNDEFINED_DISPLAY(screen->id), 
                               SDL_WINDOWPOS_UNDEFINED_DISPLAY(screen->id), 
                               rect.w, 
                               rect.h, 
                               flags);
                              
  /*
  if(__cog_platform_open_gl_version__ != OpenGLBest) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, __cog_platform_open_gl_version__ / 10);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, __cog_platform_open_gl_version__ % 10);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  }
  */
	
  wnd->context = SDL_GL_CreateContext(wnd->wnd);
  
#ifdef __APPLE__
  if(!__cog_platform_gl_extensions_loaded__) {
    __cog_platform_gl_extensions_loaded__ = True;
    LoggerInfo(__cog_platform_logger__, "GL context succesfully initialized for Window %s, Version: %s.\n", wnd->name->data, glGetString(GL_VERSION));
  } else {
    LoggerInfo(__cog_platform_logger__, "GL context succesfully initialized for Window %s, Version: %s.\n", wnd->name->data, glGetString(GL_VERSION));
  }
  
#else
  if(!__cog_platform_gl_extensions_loaded__) {
    GLenum err = glewInit();
    if(err != GLEW_OK) {
      CogLoggerError(__cog_platform_logger__, "GLEW failed to initialize!\n Error: %s\n", glewGetErrorString(err));
    } else {
      glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LEQUAL);
      __cog_platform_gl_extensions_loaded__ = True;
      CogLoggerInfo(__cog_platform_logger__, "GL context succesfully initialized for Window %s, Version: %s.\n", wnd->name->data, glGetString(GL_VERSION));
    } 
  } else {
    CogLoggerInfo(__cog_platform_logger__, "GL context succesfully initialized for Window %s, Version: %s.\n", wnd->name->data, glGetString(GL_VERSION));
  }
#endif
  
  /**/
  CogArrayPush(__cog_platform_windows__, wnd);
  if(__cog_platform_windows__->size == 1) {
    __cog_platform_focused_window__ = wnd;
  }
  
  CogCleanupPoolRegisterForCleanup(__cog_platform_cleanup_pool__, wnd, (auto_destruct)CogPlatformDestroyWindow);
  
  return wnd;
}

void CogPlatformDestroyWindow(
  CogWindow               window
)
{
  int i;
  for(i = 0; i < __cog_platform_windows__->size; ++i) {
    if(window == __cog_platform_windows__->data[i]) {
      if(window == __cog_platform_focused_window__) __cog_platform_focused_window__ = NULL;
      break;
    }
  } 
  CogArrayRemoveAt(__cog_platform_windows__, i);
  CogStringFree(window->name);
  /**/
  SDL_DestroyWindow(window->wnd);
  /**/
  free(window);
}


////////////////
//  Graphics  //
////////////////
Bool _initGraphics() { 
  return False; 
}


/////////////
//  Input  //
/////////////
Bool _initInput(void) { 
  int i;
  for(i = 0; i < CogKeyCount; ++i) {
    __cog_platform_keyboard__.state[i] = False;
    __cog_platform_keyboard__.old[i] = False;
  }
  
  for(i = 0; i < CogMouseButtonCount; ++i) {
    __cog_platform_mouse__.state[i] = False;
    __cog_platform_mouse__.old[i] = False;
  }
  
  SDL_GetMouseState(&__cog_platform_mouse__.position.x, &__cog_platform_mouse__.position.y);
  __cog_platform_mouse__.oldPosition.x = __cog_platform_mouse__.position.x;
  __cog_platform_mouse__.oldPosition.y = __cog_platform_mouse__.position.y;
  __cog_platform_mouse__.relativePosition.x = 0;
  __cog_platform_mouse__.relativePosition.y = 0;
  __cog_platform_mouse__.relativeMode = False;
  
  return True;
}

Bool CogKeyboardKeyDown(const CogKey key) {
  return __cog_platform_keyboard__.state[key];
}

Bool CogKeyboardKeyUp(const CogKey key) {
  return !__cog_platform_keyboard__.state[key];
}

Bool CogKeyboardKeyPressed(const CogKey key) {
  return !__cog_platform_keyboard__.old[key] && __cog_platform_keyboard__.state[key];
}

Bool CogKeyboardKeyReleased(const CogKey key) {
  return __cog_platform_keyboard__.old[key] && !__cog_platform_keyboard__.state[key];
}

Bool CogMouseButtonDown(const CogMouseButton button) {
  return __cog_platform_mouse__.state[button];
}

Bool CogMouseButtonUp(const CogMouseButton button) {
  return !__cog_platform_mouse__.state[button];
}

Bool CogMouseButtonPressed(const CogMouseButton button) {
  return !__cog_platform_mouse__.old[button] && __cog_platform_mouse__.state[button];
}

Bool CogMouseButtonReleased(const CogMouseButton button) {
  return __cog_platform_mouse__.old[button] && !__cog_platform_mouse__.state[button];
}

Bool CogMouseMoved(void) {
  return __cog_platform_mouse__.relativePosition.x || __cog_platform_mouse__.relativePosition.y;
}

CogVector2 CogMouseGetPosition(void) {
  return (CogVector2){.x = __cog_platform_mouse__.position.x, .y = __cog_platform_mouse__.position.y };
}

CogVector2 CogMouseGetRelativePosition(void) {
  return (CogVector2){(float)__cog_platform_mouse__.relativePosition.x, (float)__cog_platform_mouse__.relativePosition.y};
}

void CogMouseSetRelativeMode(const Bool mode) {
  __cog_platform_mouse__.relativeMode = mode;
}

void CogMouseSetVisible(const Bool visible) {
  SDL_ShowCursor(visible);
}

/////////////////
//  Threading  //
/////////////////
Bool _initThreading() { 
  return False; 
}


///////////////
//  Network  //
///////////////
Bool _initNetwork() { 
  return False; 
}


/////////////
//  Audio  //
/////////////
Bool _initAudio() { 
  return False; 
}


///////////////
//  Cog App  //
///////////////
void CogInit(void) {
  __cog_platform_timer__ = CogTimerAlloc("Platform Timer");
  CogTimerTick(__cog_platform_timer__);
  
  PlatformInit();
  CogLoggerInfo(CogPlatformGetLogger(), "Cog initializing..\n");
  CogResourceManagerInit();
  CogSceneManagerInit();
  CogLoggerInfo(CogPlatformGetLogger(), "..done.\n");
}

void CogStart(void) {
  CogTimerTick(__cog_platform_timer__);
  Bool done = False;
  SDL_Event evt;
  int i;
  double fps = 0.0f;
  double second = 0.0f;
  while(!done) {
    CogTimerTick(__cog_platform_timer__);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // update the keyboard    
    for(i = 0; i < CogKeyCount; ++i) {
      __cog_platform_keyboard__.old[i] = __cog_platform_keyboard__.state[i];
    }
    
    // update the mouse
    CogWindow wnd = CogPlatformGetFocusedWindow();
    if(__cog_platform_mouse__.relativeMode) {
      SDL_PumpEvents();
      __cog_platform_mouse__.oldPosition.x = __cog_platform_mouse__.position.x;
      __cog_platform_mouse__.oldPosition.y = __cog_platform_mouse__.position.y;
      SDL_GetMouseState(&__cog_platform_mouse__.position.x, &__cog_platform_mouse__.position.y);
      int w, h;
      w = wnd->rect.w / 2;
      h = wnd->rect.h / 2;
      //printf("(%d, %d) - (%d, %d)\n", w, h, __cog_platform_mouse__.relativePosition.x, __cog_platform_mouse__.relativePosition.y);
      __cog_platform_mouse__.relativePosition.x = (__cog_platform_mouse__.position.x - w);
      __cog_platform_mouse__.relativePosition.y = (__cog_platform_mouse__.position.y - h);
      SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
      SDL_WarpMouseInWindow(wnd->wnd, w, h);
      SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
      SDL_PumpEvents();
    } else {
      __cog_platform_mouse__.oldPosition.x = __cog_platform_mouse__.position.x;
      __cog_platform_mouse__.oldPosition.y = __cog_platform_mouse__.position.y;
      SDL_GetMouseState(&__cog_platform_mouse__.position.x, &__cog_platform_mouse__.position.y);
      __cog_platform_mouse__.relativePosition.x = __cog_platform_mouse__.position.x - __cog_platform_mouse__.oldPosition.x;
      __cog_platform_mouse__.relativePosition.y = __cog_platform_mouse__.position.y - __cog_platform_mouse__.oldPosition.y;
    }
    
    for(i = 0; i < CogMouseButtonCount; ++i) {
      __cog_platform_mouse__.old[i] = __cog_platform_mouse__.state[i];
    }
    
    // update the controllers
    // TODO
    
    while(SDL_PollEvent(&evt)) {
      if(evt.type == SDL_KEYDOWN) {
        __cog_platform_keyboard__.state[evt.key.keysym.scancode] = True;
      } else if(evt.type == SDL_KEYUP) {
        __cog_platform_keyboard__.state[evt.key.keysym.scancode] = False;
      } else if(evt.type == SDL_MOUSEBUTTONDOWN) {
        if(evt.button.button == SDL_BUTTON_LEFT) { __cog_platform_mouse__.state[CogMouseButtonLeft] = True; }
        else if(evt.button.button == SDL_BUTTON_RIGHT) { __cog_platform_mouse__.state[CogMouseButtonRight] = True; }
        else if(evt.button.button == SDL_BUTTON_MIDDLE) { __cog_platform_mouse__.state[CogMouseButtonMiddle] = True; }
      } else if(evt.type == SDL_MOUSEBUTTONUP) {
        if(evt.button.button == SDL_BUTTON_LEFT) { __cog_platform_mouse__.state[CogMouseButtonLeft] = False; }
        else if(evt.button.button == SDL_BUTTON_RIGHT) { __cog_platform_mouse__.state[CogMouseButtonRight] = False; }
        else if(evt.button.button == SDL_BUTTON_MIDDLE) { __cog_platform_mouse__.state[CogMouseButtonMiddle] = False; }
      }
    }
      
    if(CogSceneManagerUpdate(__cog_platform_timer__->delta)) {
      CogSceneManagerDraw();
      CogWindowSwap(CogPlatformGetFocusedWindow());
    } else {
      done = True;
    }
    
    fps += 1.0f;
    second += __cog_platform_timer__->delta;
    if(second >= 5.0f) {
      second -= 5.0f;
      printf("FPS: %f\n", fps * 0.2f);
      fps = 0.0f;
    }
  }
  
}

void CogCleanUp(void) {
  CogLoggerInfo(CogPlatformGetLogger(), "Cog cleaning up..\n");
  CogSceneManagerCleanUp();
  CogResourceManagerCleanup();
  CogLoggerInfo(CogPlatformGetLogger(), "..done.\n");
  CogPlatformCleanup();
}