#include <stdio.h>
#include <math.h>
#include "matrix.h"

int rotate(position *pos)
{
  double tmp_x, tmp_y;
  tmp_x = ((pos->x - pos->cx) * cos(pos->angle)) - ((pos->y - pos->cy) * sin(pos->angle)) + pos->cx;
  tmp_y = ((pos->x - pos->cx) * sin(pos->angle)) + ((pos->y - pos->cy) * cos(pos->angle)) + pos->cy;
  pos->x = tmp_x;
  pos->y = tmp_y;
  sleep(1);
  return 0;
}

int scale()
{
  return 0;
}
