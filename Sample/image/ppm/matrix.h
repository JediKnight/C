#ifndef _MATRIX_H
#define _MATRIX_H

#include <math.h>

#define toRadian(x) (M_PI * x / 180)
#define toDegree(x) (x * 180 / M_PI)

enum { X = 0, Y };

typedef struct {
  float px;
  float py;
  float cx;
  float cy;
  float mx;
  float my;
  float angle;
} CoordinateData;

int rotate(CoordinateData *, float *);

#endif	/* _MATRIX_H */
