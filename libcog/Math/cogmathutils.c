#include "cogmathutils.h"

void CogClampf(float *c, float min, float max) {
  if(*c < min) { *c = min; }
  if(*c > max) { *c = max; }
}

void CogClampd(double *c, double min, double max) {
  if(*c < min) { *c = min; }
  if(*c > max) { *c = max; }
}

void CogClampi(int *c, int min, int max) {
  if(*c < min) { *c = min; }
  if(*c > max) { *c = max; }
}

void CogClamps(short *c, short min, short max) {
  if(*c < min) { *c = min; }
  if(*c > max) { *c = max; }
}