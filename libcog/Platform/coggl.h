#ifndef _COG_GL_H_
#define _COG_GL_H_

#if defined(__APPLE__)
  #include <OpenGL/gl3.h>
#elif defined(__IOS__)
#elif defined(__ANDROID__)
#else
  #include <GL/glew.h>
#endif

#endif
