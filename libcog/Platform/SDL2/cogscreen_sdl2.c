#include "cogscreen.h"

void
CogDisplayModeInspect(
  CogDisplayMode   mode
)
{
  printf("<Display Mode (%ux%dpx @%dhz)>\n", mode.width, mode.height, mode.refreshRate);
}

void
CogScreenSetMode(
  CogScreen              self,
  const CogDisplayMode   mode
)
{
  Bool available = False;
  CogArrayForEach(self->displayModes, it, {
    if( (it.aspectRatio == mode.aspectRatio) &&
        (it.height == mode.height) &&
        (it.width == mode.width) &&
        (it.refreshRate == mode.refreshRate) ) {
      available = True;
      break;
    }
  });
  
  if(available) {
    self->curentMode = mode;
  }
}

void
CogScreenInspect(
  const CogScreen        self
)
{
  printf("<Screen (%s) Mode: %ux%upx @%uhz>\n", self->name->data, self->curentMode.width, self->curentMode.height, self->curentMode.refreshRate);
  CogArrayInspect(CogDisplayMode, self->displayModes);
}