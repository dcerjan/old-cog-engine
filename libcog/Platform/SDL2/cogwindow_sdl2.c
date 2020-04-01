#include "cogwindow.h"
#include "cogtypes.h"

void
CogWindowSetScreen(
  CogWindow                self,
  const struct CogScreen*  screen
)
{
  self->screen = screen;
}

void
CogWindowInspect(
  const CogWindow          self
)
{
  printf("<Window (%s) @0x%lx (%f, %f) (%f x %f)>", self->name->data, (unsigned long)self, self->rect.x, self->rect.y, self->rect.w, self->rect.h);
}

void
CogWindowSwap(
  const CogWindow         self
)
{
  SDL_GL_SwapWindow(self->wnd);
}