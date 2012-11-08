#include <stdio.h>
#include <math.h>
#include "matrix.h"

int rotate(position *pos)
{
  double tmp_x1, tmp_y1, tmp_x2, tmp_y2, tmp_x3, tmp_y3;

  tmp_x1 = pos->x + pos->cx;
  tmp_y1 = pos->y + pos->cy;

  tmp_x2 = ((pos->x - pos->cx) * cos(pos->angle)) - ((pos->y - pos->cy) * sin(pos->angle)) + pos->cx;
  tmp_y2 = ((pos->x - pos->cx) * sin(pos->angle)) + ((pos->y - pos->cy) * cos(pos->angle)) + pos->cy;

  tmp_x3 = pos->x - pos->cx;
  tmp_y3 = pos->y - pos->cy;

  pos->x = tmp_x2; // * tmp_x2 * tmp_x3;
  pos->y = tmp_y2; // * tmp_y2 * tmp_y3;

  printf("tmp_x1 = %d, tmp_y1 = %d\n", tmp_x1, tmp_y1);
  printf("tmp_x2 = %d, tmp_y2 = %d\n", tmp_x2, tmp_y2);
  printf("tmp_x3 = %d, tmp_y3 = %d\n", tmp_x3, tmp_y3);
  puts("");
  return 0;
}

int scale()
{
  return 0;
}

int parallel()
{
  return 0;
}
