#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

int returnsTheRotation(CoordinateData *cdata, float *xy)
{
  xy[X] = ((cdata->px - cdata->cx) * cos(cdata->angle)) + ((cdata->py - cdata->cy) * sin(cdata->angle)) + cdata->cx;
  xy[Y] = -((cdata->px - cdata->cx) * sin(cdata->angle)) + ((cdata->py - cdata->cy) * cos(cdata->angle)) + cdata->cy;
  return 0;
}

int magick(CoordinateData *cdata, float *xy)
{
  return 0;
}

int rotate(CoordinateData *cdata, float *xy)
{
  xy[X] = (cdata->px - cdata->cx) * cos(cdata->angle) - (cdata->py - cdata->cy) * sin(cdata->angle) + cdata->cx;
  xy[Y] = (cdata->px - cdata->cx) * sin(cdata->angle) + (cdata->py - cdata->cy) * cos(cdata->angle) + cdata->cy;
  return 0;
}

int parallel(CoordinateData *cdata, float *xy)
{
  xy[X] = cdata->px + cdata->mx;
  xy[Y] = cdata->py + cdata->my;
  return 0;
}

int scale(CoordinateData *cdata, float *xy)
{
  xy[X] = (cdata->px - cdata->cx) * cdata->sx + cdata->cx;
  xy[Y] = (cdata->py - cdata->cy) * cdata->sy + cdata->cy;
  return 0;
}
