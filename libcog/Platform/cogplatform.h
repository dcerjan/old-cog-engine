#ifndef _COG_PLATFORM_H_
#define _COG_PLATFORM_H_

#include "cogtypes.h"

#include "cogscreen.h"
#include "cogwindow.h"
#include "cogtimer.h"
#include "coglogger.h"

#include "cogkeyboard.h"
#include "cogmouse.h"
#include "cogcontroler.h"


Bool CogPlatformInit(void);
void CogPlatformCleanup(void);

// Logger
CogLogger CogPlatformGetLogger(void);

// Screens
CogScreen CogPlatformGetMainScreen(void);
CogArray(CogScreen) CogPlatformGetScreens(void);

// Windows
CogArray(CogWindow) CogPlatformGetWindows(void);
CogWindow CogPlatformGetFocusedWindow(void);
CogWindow CogPlatformCreateWindow(const char* name, const CogRect rect, const struct CogScreen* screen, const Bool fullscreen, const Bool fullscreenDesktop, const Bool borderless);

void CogPlatformDestroyWindow(CogWindow window);

// Keyboard

// Mouse

// Controlers
int CogPlatformGetNumPluggedControlers(void);

// Joystick
// Wheel
// Touches

#endif