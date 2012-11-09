#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

int returnsTheRotation(CoordinateData *cdata, float *xy)
{
  /*
   * |  cos(A) sin(A) 0 | |x|
   * | -sin(A) cos(A) 0 | |y|
   * |      0      0  1 | |1|
   */
  xy[X] = ((cdata->px - cdata->cx) * cos(-cdata->angle)) + ((cdata->py - cdata->cy) * sin(-cdata->angle)) + cdata->cx;
  xy[Y] = -((cdata->px - cdata->cx) * sin(-cdata->angle)) + ((cdata->py - cdata->cy) * cos(-cdata->angle)) + cdata->cy;
  return 0;
}

int rotate(CoordinateData *cdata, float *xy)
{
  /*
   * | cos(A) -sin(A) 0 | |x|
   * | sin(A)  cos(A) 0 | |y|
   * |     0       0  1 | |1|
   */
  xy[X] = ((cdata->px - cdata->cx) * cos(cdata->angle)) - ((cdata->py - cdata->cy) * sin(cdata->angle)) + cdata->cx;
  xy[Y] = ((cdata->px - cdata->cx) * sin(cdata->angle)) + ((cdata->py - cdata->cy) * cos(cdata->angle)) + cdata->cy;
  return 0;
}

int scale()
{
  return 0;
}

int parallel(CoordinateData *cdata)
{
  int tmp_x, tmp_y;
  tmp_x = cdata->px + cdata->mx;
  tmp_y = cdata->py + cdata->my;
  cdata->px = tmp_x;
  cdata->py = tmp_y;
  return 0;
}
