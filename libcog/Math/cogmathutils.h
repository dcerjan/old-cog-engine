#ifndef _COG_MATH_UTILS_H_
#define _COG_MATH_UTILS_H_

void CogClampf(float *c, float min, float max);
void CogClampd(double *c, double min, double max);
void CogClampi(int *c, int min, int max);
void CogClamps(short *c, short min, short max);

#endif