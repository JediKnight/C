#ifndef _MATRIX_H
#define _MATRIX_H

#include <math.h>

#define torad(x) (M_PI * x / 180)
#define todeg(x) (x * 180 / M_PI)

typedef struct {
  double x;
  double y;
  double cx;
  double cy;
  double angle;
} position;

int rotate(position *);

#endif	/* _MATRIX_H */
